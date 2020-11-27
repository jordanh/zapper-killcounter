
#include "zkcDisplayState.h"

ZkcDisplayState::ZkcDisplayState(String killCountLabel) {
    String defaultMessage = ZKC_DEFAULT_DISPLAY_STR;

    killCount = 0;
    state = stateKillCount;
    this->killCountLabel = killCountLabel;

    display.setup();

    changeToState(stateShowMessage, &defaultMessage);
}

void ZkcDisplayState::changeToState(zkcDisplayStatesEnum newState) {
    changeToState(newState, NULL);
}

void ZkcDisplayState::changeToState(zkcDisplayStatesEnum newState, String *msg) {
    switch (newState) {
        case stateShowMessage:
            displayDelayMs = ZKC_MSG_DELAY_MS;
            if (msg)
                display.write(*msg);
            break;
        case stateKillCount:
            displayDelayMs = ZKC_KILLCOUNT_DELAY_MS;
            display.write(String(killCount));
            break;
        case stateKillCountLabel:
        default:
            newState = stateKillCountLabel;     // assert valid state            
            displayDelayMs = ZKC_KILLCOUNT_DELAY_MS;
            display.write(killCountLabel);
            break;
    }
    state = newState;
    lastDisplayStateChange = millis();
}

void ZkcDisplayState::displayMessage(String *msg) {
    changeToState(stateShowMessage, msg);
}

void ZkcDisplayState::recordKill() {
    killCount++;
    changeToState(stateKillCount);
}

void ZkcDisplayState::maybeChangeState() {
    if (millis() - lastDisplayStateChange <= displayDelayMs)
        return; // not time to change the display yet

    switch (state) {
        case stateShowMessage:
            // fall through
        case stateKillCount:
            changeToState(stateKillCountLabel);
            break;
        case stateKillCountLabel:
        default:
            changeToState(stateKillCount);
            break;
    }
}
