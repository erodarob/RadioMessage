#ifndef SEWRAPPER_H
#define SEWRAPPER_H

#if defined(ARDUINO)
#include <Arduino.h>
#elif defined(_WIN32) || defined(_WIN64) || defined(__unix__) || defined(__APPLE__) // Windows, Linux, or OSX
#include <cstdint>
#include <cstring>
#include <cstdio>
#include <cmath>
using namespace std;
#endif

#include "../Message.h"
#include "../Wrapper.h"
#include "../Types/PackedNum.h"

class SEWrapper : public Wrapper
{
public:
    uint8_t start = 0xff;
    uint8_t end = 0x01;

    SEWrapper() {};
    SEWrapper(uint8_t start, uint8_t end) : start(start), end(end) {};

    int wrap(uint8_t *prependPos, uint8_t *appendPos) override;
    int unwrap(uint8_t *prependPos, uint8_t *appendPos) override;

    uint16_t prependLen() override { return 1; };
    uint16_t appendLen() override { return 1; };

    bool messageComplete(Message *m);

private:
    bool success = false;
};

#endif