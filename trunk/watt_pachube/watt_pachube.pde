#include <SPI.h>
#include <Ethernet.h>
#include <EthernetDHCP.h>
#include "settings.h"

// The IP address of api.pachube.com
byte serverIpAddress[] = { 
  173, 203, 98, 29 };

// The TCP client
Client client(serverIpAddress, 80);

// Sampling interval (e.g. 60,000ms = 1min)
unsigned long updateIntervalInMillis = 0;

// The next time to feed
unsigned long nextExecuteMillis = 0;

// The last connection time to disconnect from the server
// after uploaded feeds
long lastConnectionTime = 0;

float Vrms;
float Irms;
float Watt;

float watt_hour;
float vrms_sum;
float irms_sum;
float watt_sum;
int   watt_samples;

String csvData = "";

void setup()
{
  Serial.begin(57600);
  delay(3000);

  Serial.println("Attempting to obtain a DHCP lease...");
  EthernetDHCP.begin(macAddress);

  // We now have a DHCP lease, so we print out some information
  const byte* ipAddr = EthernetDHCP.ipAddress();
  const byte* gatewayAddr = EthernetDHCP.gatewayIpAddress();
  const byte* dnsAddr = EthernetDHCP.dnsIpAddress();

  Serial.println("A DHCP lease has been obtained.");

  Serial.print("My IP address is ");
  Serial.println(ip_to_str(ipAddr));

  Serial.print("Gateway IP address is ");
  Serial.println(ip_to_str(gatewayAddr));

  Serial.print("DNS IP address is ");
  Serial.println(ip_to_str(dnsAddr));

  updateIntervalInMillis = (updateIntervalInMinutes * 60000) - 1;
  nextExecuteMillis = millis() + updateIntervalInMillis;

  watt_hour     = 0;
  vrms_sum      = 0;
  irms_sum      = 0;
  watt_sum      = 0;
  watt_samples	= 0;
}

void loop()
{
  // Periodically call this method to maintain your DHCP lease
  EthernetDHCP.maintain();

  // Echo received strings to a host PC
  while (client.available() > 0) {
    char c = client.read();
    Serial.print(c);
  }

  if ((millis() - lastConnectionTime) > 5000) {
    if (client.connected()) {
      Serial.println("Disconnecting.");
      client.stop();
    }
  }

  // Calculate the power
  calcWatt();

  // Adding the results
  vrms_sum += Vrms;
  irms_sum += Irms;
  watt_sum += Watt;
  watt_samples++;

  if (millis() > nextExecuteMillis) {
    Serial.println();
    Serial.println("Updating...");

    // Calculate the mean
    vrms_sum  /= watt_samples;
    irms_sum  /= watt_samples;
    watt_sum  /= watt_samples;

    // Calculate the watt-hour
    watt_hour += watt_sum / (3600000 / updateIntervalInMillis);

    updateDataStream();
    nextExecuteMillis = millis() + updateIntervalInMillis;

    watt_samples = 0;
    vrms_sum = irms_sum = watt_sum = 0;
  }
}

void updateDataStream(void)
{
  if (client.connected()) {
    Serial.println();
    Serial.println("Disconnecting.");
    client.stop();
  }

  // Try to connect to the server
  Serial.println();
  Serial.print("Connecting to Pachube...");
  if (client.connect()) {
    Serial.println("connected");
    lastConnectionTime = millis();
  }
  else {
    Serial.println("failed");
    return;
  }

  csvData = "";

  // 0	Watt		Watt (W)
  csvData += "0,";
  appendFloatValueAsString(csvData, watt_sum);
  csvData += "\n";

  // 1	Vrms		volts root mean square (Vrms)
  csvData += "1,";
  appendFloatValueAsString(csvData, vrms_sum);
  csvData += "\n";

  // 2	Irms		ampere root mean square (Irms)
  csvData += "2,";
  appendFloatValueAsString(csvData, irms_sum);
  csvData += "\n";

  // 3	VA		volt-ampere (VA)
  csvData += "3,";
  appendFloatValueAsString(csvData, (vrms_sum * irms_sum));
  csvData += "\n";

  // 4	Power ratio		percent (%)
  csvData += "4,";
  appendFloatValueAsString(csvData, (watt_sum * 100) / (vrms_sum * irms_sum));
  csvData += "\n";

  // 5	Watt-hour		watt-hour (Wh)
  csvData += "5,";
  appendFloatValueAsString(csvData, watt_hour);
  csvData += "\n";

  Serial.println(csvData);

  client.print("PUT /v2/feeds/");
  client.print(environmentId);
  client.println(" HTTP/1.1");
  client.println("User-Agent: Arduino");
  client.println("Host: api.pachube.com");
  client.print("X-PachubeApiKey: ");
  client.println(apiKey);
  client.print("Content-Length: ");
  client.println(csvData.length());
  client.println("Content-Type: text/csv");
  client.println();
  client.println(csvData);
}



