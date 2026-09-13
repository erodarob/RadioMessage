#ifndef WRAPPER_H
#define WRAPPER_H

#if defined(ARDUINO)
#include <Arduino.h>
#elif defined(_WIN32) || defined(_WIN64) || defined(__unix__) || defined(__APPLE__) // Windows, Linux, or OSX
#include <cstdint>
#include <cstring>
#include <cstdio>
#include <cmath>
using namespace std;
#endif

class Wrapper
{
public:
    // whether this wrapper is enabled, generally managed by a Message object
    bool enabled = true;

    virtual ~Wrapper() {}; // Virtual destructor. Very important

    // wrap the Message using data from this Wrapper, with the prepended data starting at ```prependPos``` and the appeneded data starting at ```appendPos```
    virtual int wrap(uint8_t *prependPos, uint8_t *appendPos) = 0;
    // unwrap the Message putting data into this Wrapper, with the prepended data starting at ```prependPos``` and the appeneded data starting at ```appendPos```
    virtual int unwrap(uint8_t *prependPos, uint8_t *appendPos) = 0;

    // get the length of the data this Wrapper prepends to the Message
    virtual uint16_t prependLen() = 0;
    // get the length of the data this Wrapper appends to the Message
    virtual uint16_t appendLen() = 0;
};

#endif