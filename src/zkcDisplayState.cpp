
#include "zkcDisplayState.h"

ZkcDisplayState::ZkcDisplayState(String killCountLabel) {
    String defaultMessage = ZKC_DEFAULT_DISPLAY_STR;

    killCount = 0;
    state = stateKillCount;
    this->killCountLabel = killCountLabel;

    display.setup();

    R = (ZKC_BLANKING_INTERVALS * log10(2))/(log10(255));
    lastBlankingChange = 0;
    blankingStep = 0;

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
    resetBlanking();
    lastDisplayStateChange = millis();
}

void ZkcDisplayState::displayMessage(String *msg) {
    changeToState(stateShowMessage, msg);
}

void ZkcDisplayState::displayMessage(char *msg) {
    String sMsg = String(msg);
    changeToState(stateShowMessage, &sMsg);
}

void ZkcDisplayState::recordKills(uint8_t kills) {
    killCount += kills;
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

void ZkcDisplayState::changeBlanking() {
    uint8_t brightness = pow(2, (blankingStep / R)) - 1;
    display.setBlanking(brightness);
    blankingStep--;
    lastBlankingChange = millis();
}

void ZkcDisplayState::resetBlanking() {
    blankingStep = ZKC_BLANKING_INTERVALS;
    changeBlanking();
}

void ZkcDisplayState::maybeChangeBlanking() {
    if (millis() - lastBlankingChange <=  (displayDelayMs/ZKC_BLANKING_INTERVALS))
        return; // not time to change the blanking yet

    changeBlanking();
}

void ZkcDisplayState::tick() {
    maybeChangeState();
    maybeChangeBlanking();
}