#ifndef APRSTELEM_H
#define APRSTELEM_H

#include "APRSData.h"
#include "../Types/PackedNum.h"

class APRSTelem : public APRSData
{
public:
    // APRSTelem type
    static const uint8_t type = 0x04;
    // type error ID
    static const int ERR_ID = -type * 100;
    // Scale factors for encoding/decoding ignoring lat/long
    const double ALT_SCALE = (pow(91, 3) / 36000.0);       // (91^3/36000) scale to fit in 3 base91 characters
    const double SPD_SCALE = (pow(91, 2) / 1000.0);        // (91^2/1000) scale to fit in 2 base91 characters
    const double HDG_SCALE = (pow(91, 2) / 360.0);         // (91^2/360) scale to fit in 2 base91 characters
    const double ORIENTATION_SCALE = (pow(91, 2) / 360.0); // same as heading

    const int ALT_OFFSET = +1000; // range of -1000 to 35000 ft.

    double lat = 0.0;                            // decimal latitude
    double lng = 0.0;                            // decimal longitude
    double alt = 0.0;                            // ft
    double spd = 0.0;                            // knots
    double hdg = 0.0;                            // degrees
    double orient[3] = {0.0, 0.0, 0.0};          // euler angles in degrees
    PackedNum stateFlags = (uint32_t)0x00000000; // flight computer specific state flags (max 32 bits)
                                                 // maybe a bit much, but we can fit a lot of info here

    APRSTelem() : APRSData() {};

    // APRSTelem constructor
    // - config : the APRS config to use
    APRSTelem(APRSConfig config);
    // APRSTelem constructor
    // - config : the APRS config to use
    // - lat : the current decimal latitude (degrees)
    // - lng : the current decimal longitude (degrees)
    // - alt : the current altitude (ft)
    // - spd : the current speed (knots)
    // - hdg : the current heading (degrees)
    // - orient[3] : the current XYZ orientation (degrees)
    // - stateFlags : various data representing the state of the rocket
    APRSTelem(APRSConfig config, double lat, double lng, double alt, double spd, double hdg, double orient[3], uint32_t stateFlags);
    APRSTelem(APRSConfig config, double lat, double lng, double alt, double spd, double hdg, double orient[3], uint8_t *stateFlags, uint8_t *encoding, uint8_t length);

    bool getStateFlags(uint8_t *flags);

    // encode the data stored in the ```Data``` object and place the result in ```data```
    int encode(uint8_t *data, uint16_t sz) override;
    // decode the data stored in ```data``` and place it in the ```Data``` object
    int decode(uint8_t *data, uint16_t sz) override;

    // place the data in the ```Data``` object in the ```json``` string, ```sz``` is the max size of the string, ```deviceId``` can be set based on hardware
    int toJSON(char *json, uint16_t sz, int deviceId) override;
    // place the data in the ```json``` string in the ```Data``` object, ```sz``` is the max size of the string, ```deviceId``` can be set based on hardware
    int fromJSON(char *json, uint16_t sz, int &deviceId) override;
};

#endif