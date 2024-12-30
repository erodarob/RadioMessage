#ifndef APRSTEXT_H
#define APRSTEXT_H

#include "APRSData.h"

class APRSText : public APRSData
{
public:
    // the maximum length of the message portion
    static const int maxMsgLen = 67;
    // the maximum length of the addressee portion
    static const int maxAddrLen = 9;
    // APRSText type
    static const uint8_t TYPE = 0x01;
    // the message buffer, 67 message length + 1 for \0
    char msg[maxMsgLen + 1] = {0};
    // the addressee buffer, 9 addressee length + 1 for \0
    char addressee[maxAddrLen + 1] = {0};
    // the length of the message buffer
    int msgLen = 0;
    // the length of the addressee buffer
    int addrLen = 0;

    APRSText() : APRSData() {};

    // APRSText constructor
    // - config : the APRS config to use
    APRSText(APRSConfig config);
    // APRSText constructor
    // - config : the APRS config to use
    // - msg : the message portion, expected to be a valid c string
    // - addressee : the addressee portion (callsign), expected to be a valid c string
    APRSText(APRSConfig config, char msg[67], char addressee[9]);

    // encode the data stored in the ```Data``` object and place the result in ```data```
    uint16_t encode(uint8_t *data, uint16_t sz) override;
    // decode the data stored in ```data``` and place it in the ```Data``` object
    uint16_t decode(uint8_t *data, uint16_t sz) override;

    // place the data in the ```Data``` object in the ```json``` string, ```sz``` is the max size of the string, ```deviceId``` can be set based on hardware
    uint16_t toJSON(char *json, uint16_t sz, int deviceId) override;
    // place the data in the ```json``` string in the ```Data``` object, ```sz``` is the max size of the string, ```deviceId``` can be set based on hardware
    uint16_t fromJSON(char *json, uint16_t sz, int &deviceId) override;
};

#endif