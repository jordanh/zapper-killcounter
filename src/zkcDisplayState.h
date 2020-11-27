#ifndef ZKC_STATE_H
#define ZKC_STATE_H

#include <Arduino.h>
#include <string.h>

#include "slx2016.h"

#define ZKC_DEFAULT_DISPLAY_STR     "RDY!"

#define ZKC_MSG_DELAY_MS            5000
#define ZKC_KILLCOUNT_DELAY_MS      1000

enum zkcDisplayStatesEnum { stateKillCount, stateKillCountLabel, stateShowMessage };

class ZkcDisplayState {
    SLX2016 display;
    zkcDisplayStatesEnum state;
    unsigned long lastDisplayStateChange, displayDelayMs;
    uint16_t killCount;
    String killCountLabel;
    public:
        void changeToState(zkcDisplayStatesEnum newState);
        void changeToState(zkcDisplayStatesEnum newState, String *msg);
        void maybeChangeState();
        void displayMessage(String *msg);
        void recordKill();
        ZkcDisplayState(String killCountLabel);
};

#endif