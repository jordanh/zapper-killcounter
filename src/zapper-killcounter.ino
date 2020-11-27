/*
 * Project zapper-killcounter
 * Description:
 * Author:
 * Date:
 */

#include "slx2016.h"
#include "zkcDisplayState.h"

#define BLINK_PIN               D7
#define BLINK_RATE              500

#define KILL_DEBOUNCE_MS        250
#define KILLS_UNIT_STR          "kilz"

ZkcDisplayState *displayState;

unsigned long lastKillTime = 0;
int blinkState = 0;

int setDisplay(String str);
void counterInterrupt();

void setup() {
    displayState = new ZkcDisplayState( String(KILLS_UNIT_STR) );

    //tell the device we want to write to this pin
    pinMode(BLINK_PIN, OUTPUT);

    Particle.function("setDisplay", setDisplay);

    pinMode(A6, INPUT_PULLUP);
    attachInterrupt(A6, counterInterrupt, FALLING);
}

void loop() {
    displayState->tick();

    //alternate the PIN between high and low
    //digitalWrite(BLINK_PIN, (blinkState) ? HIGH : LOW);

    //invert the state
    //blinkState = !blinkState;

    //wait half a second
    //delay(BLINK_RATE);
}

int setDisplay(String str) {
    displayState->displayMessage(&str);
    return 1;
}

void counterInterrupt() {
    unsigned long killTime = millis();
    if (killTime - lastKillTime < KILL_DEBOUNCE_MS)
        return;
    lastKillTime = killTime;
    displayState->recordKill();
}
