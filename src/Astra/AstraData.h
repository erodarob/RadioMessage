#ifndef ASTRADATA_H
#define ASTRADATA_H

#include "../Data.h"

class AstraData : public Data
{
public:
    // AstraData type
    static const uint8_t type = 0x06;

    AstraData() {}

    // encode the data stored in the ```Data``` object and place the result in ```data```, ```sz``` is the max size of ```data```
    uint16_t encode(uint8_t *data, uint16_t sz) override;
    // decode the data stored in ```data``` and place it in the ```Data``` object, ```sz``` is the number of bytes from ```data``` to decode
    uint16_t decode(uint8_t *data, uint16_t sz) override;

    // place the data in the ```Data``` object in the ```json``` string, ```sz``` is the max size of the string, ```deviceId``` can be set based on hardware
    uint16_t toJSON(char *json, uint16_t sz, int deviceId) override;
    // place the data in the ```json``` string in the ```Data``` object, ```sz``` is the max size of the string, ```deviceId``` can be set based on hardware
    uint16_t fromJSON(char *json, uint16_t sz, int &deviceId) override;
};

#endif