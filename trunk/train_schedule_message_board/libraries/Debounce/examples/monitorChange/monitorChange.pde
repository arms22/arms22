#include <Debounce.h>
#define SWITCH 5

Debounce debouncer = Debounce( 20 , SWITCH ); 

void setup() {
  pinMode(SWITCH,INPUT);
     Serial.begin(115200);
}

void loop() {

   if ( debouncer.update (  ) ) {
      Serial.println( debouncer.read( ) );
   }
}

