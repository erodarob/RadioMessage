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
    /** whether this wrapper is enabled, generally managed by a Message object */
    bool enabled = true;

    /**
     * Virtual destructor. Very important
     */
    virtual ~Wrapper() {};

    /**
     * Wrap the Message using data from this Wrapper
     * @param prependPos the start of the prepended data for this Wrapper in the Message buffer
     * @param appendPos the end of the appended data for this Wrapper in the Message buffer
     * @return the result of the wrapping operation, usually the length of bytes added, always greater than 0 if successful
     */
    virtual int wrap(uint8_t *prependPos, uint8_t *appendPos) = 0;
    /**
     * Unwrap the Message putting data into this Wrapper
     * @param prependPos the start of the prepended data for this Wrapper in the Message buffer
     * @param appendPos the end of the appended data for this Wrapper in the Message buffer
     * @return the result of the unwrapping operation, usually the length of bytes added, always greater than 0 if successful
     */
    virtual int unwrap(uint8_t *prependPos, uint8_t *appendPos) = 0;

    /**
     * Get the length of the data this Wrapper prepends to the Message
     * @return the length of the prepended data
     */
    virtual uint16_t prependLen() = 0;
    /**
     * Get the length of the data this Wrapper appends to the Message
     * @return the length of the appended data
     */
    virtual uint16_t appendLen() = 0;
};

#endif