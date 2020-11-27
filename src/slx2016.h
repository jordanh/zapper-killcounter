#ifndef SLX2016_H
#define SLAX2016_H

#include <Arduino.h>
#include <string.h>

#define SLX2016_NWR     A2  // pin 1
#define SLX2016_A0      A0  // pin 2
#define SLX2016_A1      A1  // pin 3
#define SLX2016_D0      D0  // pin 5
#define SLX2016_D1      D1  // pin 6
#define SLX2016_D2      D2  // pin 7
#define SLX2016_D3      D3  // pin 8
#define SLX2016_D4      D4  // pin 9
#define SLX2016_D5      D5  // pin 10
#define SLX2016_D6      D6  // pin 11
#define SLX2016_NCLR    A3  // pin 13

#define SLX2016_NUM_A_LINES 2
#define SLX2016_NUM_D_LINES 7

#define SLX2016_CHARS       4
#define SLX2016_DELAY_US    1

class SLX2016 {
    static uint16_t addr_map[SLX2016_NUM_A_LINES];
    static uint16_t data_map[SLX2016_NUM_D_LINES];
    public:
        void setup();
        void clear();
        void setAddr(uint8_t addr);
        void setData(uint8_t data);
        void write(String data);
};

#endif