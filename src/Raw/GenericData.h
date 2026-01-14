#ifndef GENERICDATA_H
#define GENERICDATA_H

#include "../Data.h"

class GenericData : public Data
{
public:
    // the maximum size of the data
    static const uint16_t maxSize = 0x1FFF;
    // GenericData type
    static const uint8_t type = 0x07;
    // type error ID
    static const int ERR_ID = -type * 100;

    // the size of the data
    uint16_t size = 0;
    // buffer to store the data
    uint8_t data[maxSize] = {0};

    // GenericData default constructor
    GenericData() {};

    // GenericData constructor
    // - data : the data to be encoded, assumed to be ```maxSize```
    GenericData(uint8_t *data);

    // GenericData constructor
    // - data : the data to be encoded
    // - sz : the size of the data
    GenericData(uint8_t *data, uint16_t sz);

    // copy ```sz``` bytes from ```data``` into the Message buffer, ```sz``` must be less than ```Message.maxSize```
    bool fill(uint8_t *data, uint16_t sz);

    // encode the data stored in the ```Data``` object and place the result in ```data```, ```sz``` is the max size of ```data```
    int encode(uint8_t *data, uint16_t sz) override;
    // decode the data stored in ```data``` and place it in the ```Data``` object, ```sz``` is the number of bytes from ```data``` to decode
    int decode(uint8_t *data, uint16_t sz) override;

    // place the data in the ```Data``` object in the ```json``` string, ```sz``` is the max size of the string, ```deviceId``` can be set based on hardware
    int toJSON(char *json, uint16_t sz, int deviceId) override;
    // place the data in the ```json``` string in the ```Data``` object, ```sz``` is the max size of the string, ```deviceId``` can be set based on hardware
    int fromJSON(char *json, uint16_t sz, int &deviceId) override;
};

#endif