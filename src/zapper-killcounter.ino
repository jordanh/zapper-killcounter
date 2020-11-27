/*
 * Project zapper-killcounter
 * Description:
 * Author:
 * Date:
 */

#include "slx2016.h"

#define BLINK_PIN               D7
#define BLINK_RATE              500

#define DEFAULT_DISPLAY_STR     "RDY!"

#define DS_STR                  0x0
#define DS_KILL_COUNT           0x1

#define DS_CHANGE_RATE_MS       5000

#define KILL_DEBOUNCE_MS        250
#define KILLS_UNIT_STR          "kilz"
#define KILLS_UNIT_DELAY_MS     500

SLX2016 display;

uint16_t killCount = 0;
unsigned long lastKillTime = 0;

String displayString = "";
uint8_t displayState = DS_STR;
unsigned long lastDisplayStateChange = 0;
int blinkState = 0;

int setDisplay(String str);
void counterInterrupt();

void setup() {
    String str = DEFAULT_DISPLAY_STR;

    display.setup();
    display.write(str);

    //tell the device we want to write to this pin
    pinMode(BLINK_PIN, OUTPUT);

    Particle.function("setDisplay", setDisplay);

    pinMode(A6, INPUT_PULLUP);
    attachInterrupt(A6, counterInterrupt, FALLING);
}

void loop() {
    if (millis() - lastDisplayStateChange > DS_CHANGE_RATE_MS) {
        lastDisplayStateChange = millis();
        switch (displayState) {
            case DS_STR:
                displayState = DS_KILL_COUNT;
                break;
            case DS_KILL_COUNT:
            default:
                displayState = DS_STR;
        }
    }

    switch (displayState) {
        case DS_STR:
            display.write(displayString);
            break;
        case DS_KILL_COUNT:
        default:
            display.write(String(killCount));
    }

    //alternate the PIN between high and low
    digitalWrite(BLINK_PIN, (blinkState) ? HIGH : LOW);

    //invert the state
    blinkState = !blinkState;

    //wait half a second
    delay(BLINK_RATE);
}

int setDisplay(String str) {
    displayString = str;
    display.write(displayString);

    return 1;
}

void counterInterrupt() {
    unsigned long killTime = millis();
    if (killTime - lastKillTime < KILL_DEBOUNCE_MS)
        return;
    lastKillTime = killTime;
    killCount++;
    display.write(String(killCount));
}
