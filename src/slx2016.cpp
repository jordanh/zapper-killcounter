#include "slx2016.h"

uint16_t SLX2016::addr_map[SLX2016_NUM_A_LINES] = {SLX2016_A0, SLX2016_A1};
uint16_t SLX2016::data_map[SLX2016_NUM_D_LINES] = {
        SLX2016_D0,
        SLX2016_D1,
        SLX2016_D2,
        SLX2016_D3,
        SLX2016_D4,
        SLX2016_D5,
        SLX2016_D6,
};

void SLX2016::setup() {
    pinMode(SLX2016_NWR, OUTPUT);
    pinMode(SLX2016_A0, OUTPUT);
    pinMode(SLX2016_A1, OUTPUT);
    pinMode(SLX2016_BLK, OUTPUT);
    pinMode(SLX2016_NCLR, OUTPUT);
    pinMode(SLX2016_D0, OUTPUT);
    pinMode(SLX2016_D1, OUTPUT);
    pinMode(SLX2016_D2, OUTPUT);
    pinMode(SLX2016_D3, OUTPUT);
    pinMode(SLX2016_D4, OUTPUT);
    pinMode(SLX2016_D5, OUTPUT);
    pinMode(SLX2016_D6, OUTPUT);

    digitalWrite(SLX2016_NWR, HIGH);

    this->clear();    
}

void SLX2016::clear() {
    digitalWrite(SLX2016_NCLR, LOW);
    delayMicroseconds(10);
    digitalWrite(SLX2016_NCLR, HIGH);
}

void SLX2016::setAddr(uint8_t addr) {
    for (uint8_t i = 0; i < SLX2016_NUM_A_LINES; i++) {
        digitalWrite(SLX2016::addr_map[i], (addr & 0x1) ? HIGH : LOW);
        addr >>= 1;
    }
}

void SLX2016::setData(uint8_t data) {
    for (uint8_t i = 0; i < SLX2016_NUM_D_LINES; i++) {
        digitalWrite(SLX2016::data_map[i], (data & 0x1) ? HIGH : LOW);
        data >>= 1;
    }
}

void SLX2016::setBlanking(uint8_t brightness) {
    analogWrite(SLX2016_BLK, brightness);
}

void SLX2016::write(String data) {
    uint8_t addr = 0;
    uint8_t bound = min(data.length(), SLX2016_CHARS);
    clear();
    for (unsigned int i = 0; i < data.length(); i++) {
        addr = (bound-1) - (i % bound);
        this->setAddr(addr);
        this->setData(data.charAt(i));
        delayMicroseconds(10);
        digitalWrite(SLX2016_NWR, LOW);
        delayMicroseconds(10);
        digitalWrite(SLX2016_NWR, HIGH);
    }
}
