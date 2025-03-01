#ifndef APRSCMD_H
#define APRSCMD_H

#include "APRSData.h"
#include "../Types/PackedNum.h"

// Technically this is not a valid APRS message as in it shouldn't be picked up by APRS repeaters,
// but we can still use the APRS header structure since we need to transmit our callsign anyway
class APRSCmd : public APRSData
{
public:
    // APRSCmd type
    static const uint8_t type = 0x03;

    // the command to send, 0x00 should always be NOP
    uint8_t cmd = 0x00;
    // args for cmd
    PackedNum args = (uint16_t)0x0000;

    APRSCmd() : APRSData() {};

    // APRSCmd constructor
    // - config : the APRS config to use
    APRSCmd(APRSConfig config);
    // APRSCmd constructor
    // - config : the APRS config to use
    // - cmd : the command the send
    // - args : the arguments for ```cmd```
    APRSCmd(APRSConfig config, uint8_t cmd, uint16_t args);
    APRSCmd(APRSConfig config, uint8_t cmd, uint8_t *args, uint8_t *encoding, uint8_t length);

    bool getArgs(uint8_t *args);

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