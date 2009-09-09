// This code turns a led on/off through a debounced switch


#include <Debounce.h>
#define SWITCH 5
#define LED 13

// Instantiate a Debounce object with a 20 millisecond debounce time
Debounce debouncer = Debounce( 20 , SWITCH ); 

void setup() {
  pinMode(SWITCH,INPUT);
  pinMode(LED,OUTPUT);
}

void loop() {
 // Update the debouncer
  debouncer.update ( );
 
 // Get the update value
   digitalWrite(LED, debouncer.read() );
 
}

