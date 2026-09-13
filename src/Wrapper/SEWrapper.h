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

    // SEWrapper default constructor
    SEWrapper() {};

    // SEWrapper constructor
    // - start : the start character
    // - end : the end character
    SEWrapper(uint8_t start, uint8_t end) : start(start), end(end) {};

    // wrap the Message using data from this Wrapper, with the prepended data starting at ```prependPos``` and the appeneded data starting at ```appendPos```
    int wrap(uint8_t *prependPos, uint8_t *appendPos) override;
    // unwrap the Message putting data into this Wrapper, with the prepended data starting at ```prependPos``` and the appeneded data starting at ```appendPos```
    int unwrap(uint8_t *prependPos, uint8_t *appendPos) override;

    // get the length of the data this Wrapper prepends to the Message
    uint16_t prependLen() override { return 1; };
    // get the length of the data this Wrapper appends to the Message
    uint16_t appendLen() override { return 1; };

    // returns whether the Message ```m``` is complete according to data unwrapped from ```m``` into this wrapper
    bool messageComplete(Message *m);

private:
    // indicates whether unwrapping was successful (i.e. the Message is complete), reset by ```messageComplete()```
    bool success = false;
};

#endif