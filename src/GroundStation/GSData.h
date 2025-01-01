#ifndef GSDATA_H
#define GSDATA_H

#include "../Data.h"

class GSData : public Data
{
public:
    // maximum total message size
    static const uint16_t maxSize = 0xFFFF;
    // size of the header
    static const uint8_t headerLen = 3;
    // maximum size of the data
    static const uint16_t maxDataSize = maxSize - headerLen;
    // GSData type
    static const uint8_t type = 0x00;

    // type of message, first byte of header, must be given by Data subclass, max 0xF
    uint8_t dataType = 0x00;
    // the multiplexing id of the message, max 0xF
    uint8_t id = 0x00;
    // size of message, second and third bytes of header
    uint16_t size = 0x0000; // length of the message (65535 btyes should be enough :)
    // buffer to store message data (not including header)
    uint8_t buf[maxDataSize] = {0}; // leave space for header

    // GSData default constructor
    GSData() {};

    static bool decodeHeader(uint32_t header, uint8_t &streamType, uint8_t &streamIndex, uint16_t &size);
    // header assumed to contain at least GSData::headerLen bytes
    static bool decodeHeader(uint8_t *header, uint8_t &streamType, uint8_t &streamIndex, uint16_t &size);

    // GSData constructor
    // - type : the type of the message
    // - id : the multiplexing id of the message
    GSData(uint8_t streamType, uint8_t streamId);

    // GSData constructor
    // - type : the type of the message
    // - id : the multiplexing id of the message
    // - buf : the data for the message
    // - size : the size of the data
    GSData(uint8_t streamType, uint8_t streamId, uint8_t *buf, uint16_t size);

    // encode the data stored in the ```Data``` object and place the result in ```data```
    uint16_t encode(uint8_t *data, uint16_t sz) override;
    // decode the data stored in ```data``` and place it in the ```Data``` object
    uint16_t decode(uint8_t *data, uint16_t sz) override;

    // fill internal buffer with ```size``` bytes using the data in ```buf```
    GSData *fill(uint8_t *buf, uint16_t size);

    // place the data in the ```Data``` object in the ```json``` string, ```sz``` is the max size of the string, ```deviceId``` can be set based on hardware
    uint16_t toJSON(char *json, uint16_t sz, int deviceId) override;
    // place the data in the ```json``` string in the ```Data``` object, ```sz``` is the max size of the string, ```deviceId``` can be set based on hardware
    uint16_t fromJSON(char *json, uint16_t sz, int &deviceId) override;
};

#endif