/*
 * Project zapper-killcounter
 * Description:
 * Author:
 * Date:
 */

#include "slx2016.h"
#include "zkcDisplayState.h"

// From lib/ArduinoStatsd
#include <Statsd.h>

#define APP_VERSION_STR         "v001"
#define DOGSTATSD_HOST              192,168,2,115
#define DOGSTATSD_PORT              8125
#define DOGSTATSD_GLOBAL_TAGS       "host=zapper,location=2900_Shorb"
#define DOGSTATSD_KILLCOUNTER_NAME  "test_counter"

#define KILL_DEBOUNCE_MS        250
#define KILLS_UNIT_STR          "kilz"

UDP udp;
Statsd statsd(
    udp,
    IPAddress(DOGSTATSD_HOST),
    DOGSTATSD_PORT,
    DOGSTATSD_GLOBAL_TAGS
);
bool statsdBegan = false;

ZkcDisplayState *displayState;

volatile unsigned long lastKillTime = 0;
uint8_t freshKills = 0;

int setDisplay(String str);
void counterInterrupt();

void setup() {
    displayState = new ZkcDisplayState( String(KILLS_UNIT_STR) );

    Particle.function("setDisplay", setDisplay);

    pinMode(A6, INPUT_PULLUP);
    attachInterrupt(A6, counterInterrupt, FALLING);

    statsd.setTagStyle(TAG_STYLE_DATADOG);
    displayState->displayMessage(APP_VERSION_STR);
}

void loop() {
    displayState->tick();

    if (!WiFi.ready())
        statsdBegan = false;

    if (!statsdBegan && WiFi.ready()) {
        statsd.begin();
        statsdBegan = true;
    }

    if (!freshKills)
        return;

    displayState->recordKills(freshKills);    

    if (statsdBegan) {
        statsd.count("test_counter", freshKills);
        // NOTE: hack, see: https://github.com/particle-iot/device-os/issues/240#issuecomment-147222560
        delay(10);
        statsd.stop();
        statsdBegan = false;
    }

    freshKills = 0;
}

int setDisplay(String str) {
    displayState->displayMessage(&str);
    return 1;
}

void counterInterrupt() {
    volatile unsigned long killTime = millis();
    if (killTime - lastKillTime < KILL_DEBOUNCE_MS)
        return;
    lastKillTime = killTime;
    freshKills++;
}
