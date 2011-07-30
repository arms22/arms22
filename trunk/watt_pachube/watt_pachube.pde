#include <SPI.h>
#include <Ethernet.h>
#include <EthernetDHCP.h>
#include <Time.h>
#include "settings.h"

// Pin definition
const int arefPin = 0;
const int ct1Pin = 1;
const int vt1Pin = 2;
const int ct2Pin = 3;
const int vt2Pin = 4;

// The IP address of api.pachube.com
byte serverIpAddress[] = { 
  173, 203, 98, 29 };

// The TCP client
Client client(serverIpAddress, 80);

// The last connection time to disconnect from the server
// after uploaded feeds
unsigned long lastConnectionTime = 0;

// The last update time
int lastUpdateTime = 0;
int lastDay;

float Vrms;
float Irms;
float Watt;

float watt_hour;

float vrms_sum;
float irms_sum;
float watt_sum;

float vrms_sum2;
float irms_sum2;
float watt_sum2;

int   watt_samples;

String postData;

void setup()
{
  Serial.begin(115200);
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

  setTime(9,00,0, 28,6,2011); // 2011/6/28 9:55:0
  ntpInit();

  watt_hour     = 0;
  vrms_sum      = 0;
  irms_sum      = 0;
  watt_sum      = 0;
  vrms_sum2     = 0;
  irms_sum2     = 0;
  watt_sum2     = 0;
  watt_samples	= 0;

  lastUpdateTime = minute();
}

void loop()
{
  // Periodically call this method to maintain your DHCP lease
  EthernetDHCP.maintain();

  // Time sync
  ntpPolling();

  // Echo received strings to a host PC
  while (client.available() > 0) {
    char c = client.read();
    Serial.print(c);
  }

  if ((millis() - lastConnectionTime) > 10000) {
    if (client.connected()) {
      Serial.println("Disconnecting.");
      client.stop();
    }
  }

  // Calculate the power1
  calcWatt(ct1Pin, vt1Pin);

  vrms_sum += Vrms;
  irms_sum += Irms;
  watt_sum += Watt;

  // Calculate the power2
  calcWatt(ct2Pin, vt2Pin);

  vrms_sum2 += Vrms;
  irms_sum2 += Irms;
  watt_sum2 += Watt;

  watt_samples++;

  if (abs(minute() - lastUpdateTime) >= updateIntervalInMinutes) {
    lastUpdateTime = minute();

    // Calculate the mean
    vrms_sum  /= watt_samples;
    irms_sum  /= watt_samples;
    watt_sum  /= watt_samples;

    Serial.print("1 ");
    Serial.print(vrms_sum);
    Serial.print(" V, ");

    Serial.print(irms_sum);
    Serial.print(" A, ");

    Serial.print(vrms_sum * irms_sum);
    Serial.print(" VA, ");

    Serial.print(watt_sum);
    Serial.print(" W, ");

    Serial.print((watt_sum * 100) / (vrms_sum * irms_sum));
    Serial.println(" %, ");

    vrms_sum2 /= watt_samples;
    irms_sum2 /= watt_samples;
    watt_sum2 /= watt_samples;

    Serial.print("2 ");
    Serial.print(vrms_sum2);
    Serial.print(" V, ");

    Serial.print(irms_sum2);
    Serial.print(" A, ");

    Serial.print(vrms_sum2 * irms_sum2);
    Serial.print(" VA, ");

    Serial.print(watt_sum2);
    Serial.print(" W, ");

    Serial.print((watt_sum2 * 100) / (vrms_sum2 * irms_sum2));
    Serial.println(" %, ");

    float w = (watt_sum + watt_sum2) / (60 / updateIntervalInMinutes);
    if( lastDay != day() ){
      lastDay = day();
      watt_hour = w;
    }
    else{
      watt_hour += w;
    }

    Serial.print(watt_hour);
    Serial.println(" Wh");

    updateDataStream();

    vrms_sum     = 0;
    irms_sum     = 0;
    watt_sum     = 0;
    vrms_sum2    = 0;
    irms_sum2    = 0;
    watt_sum2    = 0;
    watt_samples = 0;
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
    Serial.println("ok");
    lastConnectionTime = millis();
  }
  else {
    Serial.println("NG");
    return;
  }

  postData = "";

  // 0	Watt		Watt (W)
  postData += "0,";
  appendFloatValueAsString(postData, watt_sum);
  postData += "\n";

  // 1	Watt		Watt (W)
  postData += "1,";
  appendFloatValueAsString(postData, watt_sum2);
  postData += "\n";

  // 2	Watt-total	Watt (W)
  postData += "2,";
  appendFloatValueAsString(postData, watt_sum + watt_sum2);
  postData += "\n";

  // 3	Watt-hour	watt-hour (Wh)
  postData += "3,";
  appendFloatValueAsString(postData, watt_hour);
  postData += "\n";

  Serial.println("Post Data:");
  Serial.println(postData);
  Serial.println();

  client.print("PUT /v2/feeds/");
  client.print(environmentId);
  client.println(" HTTP/1.1");
  client.println("User-Agent: Arduino");
  client.println("Host: api.pachube.com");
  client.print("X-PachubeApiKey: ");
  client.println(apiKey);
  client.print("Content-Length: ");
  client.println(postData.length());
  client.println("Content-Type: text/csv");
  client.println();
  client.println(postData);
}




