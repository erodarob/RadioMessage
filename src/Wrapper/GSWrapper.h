#ifndef GSWRAPPER_H
#define GSWRAPPER_H

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

class GSWrapper : public Wrapper
{
public:
    /** size of the header */
    static const uint8_t headerLen = 3;
    /** the PackedNum encoding used by the header */
    static constexpr uint8_t headerEncoding[] = {4, 4, 16};
    /** size of header encoding */
    static const uint8_t headerEncodingLength = sizeof(headerEncoding);

    /** type of message, first byte of header, must be given by Data subclass, max 0xF */
    uint8_t dataType = 0x00;
    /** the multiplexing id of the message, max 0xF */
    uint8_t id = 0x00;
    /** size of message, second and third bytes of header */
    uint16_t msgSize = 0x0000; // length of the message (10000 btyes should be enough)
    /** stores the header for this message in the order dataType, id, deviceId, size (first 4), size (last 8) */
    PackedNum header = {headerEncoding, headerEncodingLength};

    /**
     * GSWrapper default constructor
     */
    GSWrapper() {};

    /**
     * GSWrapper constructor
     * @param streamType the Data subclass type of the stream (e.g. APRSTelem::type)
     * @param streamId the unique id identifying this stream
     */
    GSWrapper(uint8_t streamType, uint8_t streamId) : dataType(streamType), id(streamId) {};

    /**
     * Set the metadata for encoding
     * @param streamType the Data subclass type of the stream (e.g. APRSTelem::type)
     * @param streamId the unique id identifying this stream
     */
    void setMetadata(uint8_t streamType, uint8_t streamId);
    //  and place into ```streamType``` and ```streamId```
    /**
     * Retrieve metadata and place into the given variables
     * @param streamType will be set to the Data subclass type of the stream (e.g. APRSTelem::type)
     * @param streamId will be set to the unique id identifying this stream
     */
    void getMetadata(uint8_t &streamType, uint8_t &streamId);

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
    uint16_t prependLen() override { return GSWrapper::headerLen; };
    /**
     * Get the length of the data this Wrapper appends to the Message
     * @return the length of the appended data
     */
    uint16_t appendLen() override { return 0; };

    /**
     * Check whether the given Message is complete according to data unwrapped from the Message into this Wrapper
     * @param m a pointer to the Message to check
     * @return whether the Message is complete
     */
    bool messageComplete(Message *m);
};

#endif