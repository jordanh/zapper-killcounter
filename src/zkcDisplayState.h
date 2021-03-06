#ifndef ZKC_STATE_H
#define ZKC_STATE_H

#include <Arduino.h>
#include <string.h>

#include "slx2016.h"

#define ZKC_DEFAULT_DISPLAY_STR     "RDY!"

#define ZKC_MSG_DELAY_MS            5000
#define ZKC_KILLCOUNT_DELAY_MS      1000

#define ZKC_BLANKING_INTERVALS      30

enum zkcDisplayStatesEnum { stateKillCount, stateKillCountLabel, stateShowMessage };

class ZkcDisplayState {
    SLX2016 display;
    zkcDisplayStatesEnum state;
    unsigned long lastDisplayStateChange, displayDelayMs;
    uint16_t killCount;
    String killCountLabel;

    float R;
    unsigned long lastBlankingChange;
    uint8_t blankingStep;

    public:
        void changeToState(zkcDisplayStatesEnum newState);
        void changeToState(zkcDisplayStatesEnum newState, String *msg);
        void changeBlanking();
        void resetBlanking();
        void maybeChangeState();
        void maybeChangeBlanking();
        void displayMessage(String *msg);
        void displayMessage(char *msg);
        void recordKills(uint8_t kills);
        void tick();
        ZkcDisplayState(String killCountLabel);
};

#endif