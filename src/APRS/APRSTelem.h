#ifndef APRSTELEM_H
#define APRSTELEM_H

#include "APRSData.h"
#include "../Types/PackedNum.h"

class APRSTelem : public APRSData
{
public:
    /** APRSTelem type */
    static const uint8_t type = 0x04;
    /** APRSTelem error ID */
    static const int ERR_ID = -type * 100;
    // Scale factors for encoding/decoding ignoring lat/long
    /** altitude (91^3/36000) scale to fit in 3 base91 characters */
    const double ALT_SCALE = (pow(91, 3) / 36000.0); //
    /** speed (91^2/1500) scale to fit in 2 base91 characters */
    const double SPD_SCALE = (pow(91, 2) / 1500.0);
    /** heading (91^2/360) scale to fit in 2 base91 characters */
    const double HDG_SCALE = (pow(91, 2) / 360.0);
    /** orientation (91^2/360) scale to fit in 2 base91 characters */
    const double ORIENTATION_SCALE = (pow(91, 2) / 360.0);

    /** offset to yield altitude range of -1000 to 35000 ft. */
    const int ALT_OFFSET = +1000; //

    /** the minimum encodable altitude in ft */
    const double MIN_ALT = 0.0 - ALT_OFFSET;
    /** the maximum encodable altitude in ft */
    const double MAX_ALT = 36000.0 - ALT_OFFSET;
    /** the maximum encodable speed in knots */
    const double MAX_SPD = 1500.0;
    /** the maximum encodable heading in degrees */
    const double MAX_HDG = 360.0;

    /** decimal latitude in degrees */
    double lat = 0.0;
    /** decimal longitude in degrees */
    double lng = 0.0;
    /** altitude in ft */
    double alt = 0.0;
    /** speed in knots */
    double spd = 0.0;
    /** azimuthal heading in degrees */
    double hdg = 0.0;
    /** orientation euler angles in degrees */
    double orient[3] = {0.0, 0.0, 0.0};
    /** flight computer specific state flags (max 32 bits) */
    PackedNum stateFlags = (uint32_t)0x00000000; // maybe a bit much, but we can fit a lot of info here

    /**
     * APRSTelem default constructor
     */
    APRSTelem() : APRSData() {};

    /**
     * APRSTelem constructor
     * @param config the APRS config to use
     */
    APRSTelem(APRSConfig config);
    /**
     * APRSTelem constructor
     * @param config the APRS config to use
     * @param lat the current decimal latitude (degrees)
     * @param lng the current decimal longitude (degrees)
     * @param alt the current altitude (ft)
     * @param spd the current speed (knots)
     * @param hdg the current heading (degrees)
     * @param orient length 3 array with the current XYZ orientation (degrees)
     * @param stateFlags various data representing the state of the rocket
     */
    APRSTelem(APRSConfig config, double lat, double lng, double alt, double spd, double hdg, double orient[3], uint32_t stateFlags);
    /**
     * APRSTelem constructor
     * @param config the APRS config to use
     * @param lat the current decimal latitude (degrees)
     * @param lng the current decimal longitude (degrees)
     * @param alt the current altitude (ft)
     * @param spd the current speed (knots)
     * @param hdg the current heading (degrees)
     * @param orient length 3 array with the current XYZ orientation (degrees)
     * @param stateFlags an array with various data representing the state of the rocket, length is the same as the encoding
     * @param encoding the PackedNum encoding of the state flags
     * @param length the length of the encoding
     */
    APRSTelem(APRSConfig config, double lat, double lng, double alt, double spd, double hdg, double orient[3], uint64_t *stateFlags, uint8_t *encoding, uint8_t length);

    /**
     * Get the state flags from the state flags PackedNum
     * @param flags the array to place the state flags in
     * @return whether the state flags were decoded successfully
     */
    bool getStateFlags(uint64_t *flags);

    /**
     * Encode the data stored in the Data object and place it in the provided buffer
     * @param data a pointer to the array to place the data in
     * @param sz the maximum length of the data array
     * @return the result of the encoding, the length of bytes added if successful, otherwise an error code less than 0
     */
    int encode(uint8_t *data, uint16_t sz) override;
    /**
     * Decode the data stored in the the provided buffer and place it in the Data object
     * @param data a pointer to the array to extract data from
     * @param sz the length of the data array
     * @return the result of the decoding, the length of bytes decoded if successful, otherwise an error code less than 0
     */
    int decode(uint8_t *data, uint16_t sz) override;

    /**
     * Encode the data in the Data object into a JSON format
     * @param json the string to place the JSON into
     * @param sz the maximum size of the json string
     * @param deviceId a number indicating the hardware the data came from
     * @return the result of the encoding, the length of bytes added if successful, otherwise an error code less than 0
     */
    int toJSON(char *json, uint16_t sz, int deviceId) override;
    /**
     * Decode the data from a JSON format into the Data object
     * @param json the JSON string to extract data from
     * @param sz the size of the json string
     * @param deviceId a number indicating the hardware the data came from, decoded separately from the JSON
     * @return the result of the decoding, greater than 0 if successful, otherwise an error code less than 0
     */
    int fromJSON(char *json, uint16_t sz, int &deviceId) override;
};

#endif