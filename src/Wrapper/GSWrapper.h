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
    // size of the header
    static const uint8_t headerLen = 3;
    // the PackedNum encoding used by the header
    static constexpr uint8_t headerEncoding[] = {4, 4, 16};
    // size of header encoding
    static const uint8_t headerEncodingLength = sizeof(headerEncoding);

    // type of message, first byte of header, must be given by Data subclass, max 0xF
    uint8_t dataType = 0x00;
    // the multiplexing id of the message, max 0xF
    uint8_t id = 0x00;
    // size of message, second and third bytes of header
    uint16_t msgSize = 0x0000; // length of the message (10000 btyes should be enough)
    // stores the header for this message in the order dataType, id, deviceId, size (first 4), size (last 8)
    PackedNum header = {headerEncoding, headerEncodingLength};

    GSWrapper() {};
    GSWrapper(uint8_t streamType, uint8_t streamId) : dataType(streamType), id(streamId) {};

    // set the metadata for encoding from ```streamType``` and ```streamId```
    void setMetadata(uint8_t streamType, uint8_t streamId);
    // retrieve metadata and place into ```streamType``` and ```streamId```
    void getMetadata(uint8_t &streamType, uint8_t &streamId);

    int wrap(uint8_t *prependPos, uint8_t *appendPos) override;
    int unwrap(uint8_t *prependPos, uint8_t *appendPos) override;

    uint16_t prependLen() override { return 3; };
    uint16_t appendLen() override { return 0; };

    bool messageComplete(Message *m);

private:
    bool unwrapped = false;
};

#endif