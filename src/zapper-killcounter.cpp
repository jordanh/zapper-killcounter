/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "/Users/jrhusney/Source/particle-iot/zapper-killcounter/src/zapper-killcounter.ino"
/*
 * Project zapper-killcounter
 * Description:
 * Author:
 * Date:
 */

void setup();
void loop();
#line 8 "/Users/jrhusney/Source/particle-iot/zapper-killcounter/src/zapper-killcounter.ino"
#define PIN D6
int state = 0;

void setup() {
    //tell the device we want to write to this pin
    pinMode(PIN, OUTPUT);
}
void loop() {
    //alternate the PIN between high and low
    digitalWrite(PIN, (state) ? HIGH : LOW);

    //invert the state
    state = !state;

    //wait half a second
    delay(500);
}
