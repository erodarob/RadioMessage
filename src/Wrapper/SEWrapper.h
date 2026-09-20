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
    /** the start character */
    uint8_t start = 0xff;
    /** the end character */
    uint8_t end = 0x01;

    /**
     * SEWrapper default constructor
     */
    SEWrapper() {};

    /**
     * SEWrapper constructor
     * @param start the start character
     * @param end the end character
     */
    SEWrapper(uint8_t start, uint8_t end) : start(start), end(end) {};

    /**
     * Wrap the Message using data from this Wrapper
     * @param prependPos the start of the prepended data for this Wrapper in the Message buffer
     * @param appendPos the end of the appended data for this Wrapper in the Message buffer
     * @return the result of the wrapping operation, usually the length of bytes added, always greater than 0 if successful
     */
    int wrap(uint8_t *prependPos, uint8_t *appendPos) override;
    /**
     * Unwrap the Message putting data into this Wrapper
     * @param prependPos the start of the prepended data for this Wrapper in the Message buffer
     * @param appendPos the end of the appended data for this Wrapper in the Message buffer
     * @return the result of the unwrapping operation, usually the length of bytes added, always greater than 0 if successful
     */
    int unwrap(uint8_t *prependPos, uint8_t *appendPos) override;

    /**
     * Get the length of the data this Wrapper prepends to the Message
     * @return the length of the prepended data
     */
    uint16_t prependLen() override { return 1; };
    /**
     * Get the length of the data this Wrapper appends to the Message
     * @return the length of the appended data
     */
    uint16_t appendLen() override { return 1; };

    /**
     * Check whether the given Message is complete according to data unwrapped from the Message into this Wrapper
     * @param m a pointer to the Message to check
     * @return whether the Message is complete
     */
    bool messageComplete(Message *m);

private:
    /** internally indicates whether unwrapping was successful (i.e. the Message is complete), reset by messageComplete() */
    bool success = false;
};

#endif