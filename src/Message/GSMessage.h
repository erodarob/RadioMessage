#ifndef GSMESSAGE_H
#define GSMESSAGE_H

#if defined(ARDUINO)
#include <Arduino.h>
#elif defined(_WIN32) || defined(_WIN64) || defined(__unix__) || defined(__APPLE__) // Windows, Linux, or OSX
#include <cstdint>
#include <cstring>
#include <cstdio>
#include <cmath>
using namespace std;
#endif

#include "../Data.h"
#include "../Message.h"
#include "../Types/PackedNum.h"

class GSMessage : public Message
{
public:
    // size of the header
    static const uint8_t headerLen = 3;
    // the PackedNum encoding used by the header
    static constexpr uint8_t headerEncoding[] = {4, 4, 8, 8};
    // size of header encoding
    static const uint8_t headerEncodingLength = sizeof(headerEncoding);
    // the header for Ground Station Muxer (gsm) files, format: (version)GroundStationMuxer(bitrate)\n
    static constexpr char staticGSMHeader[] = "\1GroundStationMuxer";
    // the total size of the gsm header
    static const int gsmHeaderSize = sizeof(staticGSMHeader) - 1 + sizeof(uint32_t); // -1 for null terminator

    // type of message, first byte of header, must be given by Data subclass, max 0xF
    uint8_t dataType = 0x00;
    // the multiplexing id of the message, max 0xF
    uint8_t id = 0x00;
    // size of message, second and third bytes of header
    uint16_t msgSize = 0x0000; // length of the message (10000 btyes should be enough)
    // stores the header for this message in the order dataType, id, deviceId, size (first 4), size (last 8)
    PackedNum header = {headerEncoding, headerEncodingLength};

    // GSMessage default constructor
    GSMessage() : Message() {};

    // GSMessage constructor
    // - type : the type of the message
    // - id : the multiplexing id of the message
    // - deviceId: the id for the hardware
    GSMessage(uint8_t streamType, uint8_t streamId);

    // GSMessage constructor
    // - type : the type of the message
    // - id : the multiplexing id of the message
    // - deviceId: the id for the hardware
    // - buf : the data for the message
    // - size : the size of the data
    GSMessage(uint8_t streamType, uint8_t streamId, uint8_t *rawData, uint16_t size);

    // GSMessage constructor
    // - type : the type of the message
    // - id : the multiplexing id of the message
    // - deviceId: the id for the hardware
    // - buf : the data for the message
    // - size : the size of the data
    GSMessage(uint8_t streamType, uint8_t streamId, Data *data);

    // decode Ground Station Multiplexer (GSM) header from ```header``` with length ```length```, with data bitrate placed into ```bitrate```
    // returns whether decoding was successful
    static bool decodeGSMHeader(char *header, int length, uint32_t &bitrate);
    // encoded GSM header into ```header``` with max length ```length```, with data bitrate  ```bitrate```
    // returns whether encoding was successful
    static bool encodeGSMHeader(char *header, int length, uint32_t bitrate);

    // decode GSMessage header ```header```, and place the results into ```streamType```, ```streamId```, and ```size```
    // returns whether decoding was successful
    static bool decodeHeader(uint32_t header, uint8_t &streamType, uint8_t &streamId, uint16_t &size);
    // decode GSMessage header ```header```, and place the results into ```streamType```, ```streamId```, and ```size```
    // header assumed to contain at least GSMessage::headerLen bytes
    // returns whether decoding was successful
    static bool decodeHeader(uint8_t *header, uint8_t &streamType, uint8_t &streamId, uint16_t &size);

    // decode GSMessage header ```header```, and place the results into this object
    // returns whether decoding was successful
    bool decodeHeader(uint32_t header);
    // decode GSMessage header ```header```, and place the results into this object
    // header assumed to contain at least GSMessage::headerLen bytes
    // returns whether decoding was successful
    bool decodeHeader(uint8_t *header);
    // decode the header stored in the first 3 bytes of this object's buffer, and place the results into this object
    // fails if buffer contains less than GSMessage::headerLen bytes
    // returns whether decoding was successful
    bool decodeHeader();

    // set the metadata for encoding from ```streamType``` and ```streamId```
    void setMetadata(uint8_t streamType, uint8_t streamId);
    // retrieve metadata and place into ```streamType``` and ```streamId```
    void getMetadata(uint8_t &streamType, uint8_t &streamId);

    // use return type Message* so we can stack operators e.g., ```Message()->fill()->encode()```

    // encodes ```data``` and places the output in the Message buffer
    Message *encode(uint8_t *data, uint16_t sz);
    // encodes ```data``` and places the output in the Message buffer
    Message *encode(Data *data) override;
    // decodes data from the Message buffer, ```data``` is populated with the decoded information
    Message *decode(Data *data) override;
};

#endif