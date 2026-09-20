// This class will be for creating a HITLData type by inheriting from the Data class
#ifndef HITL_DATA_H
#define HITL_DATA_H

#include "Data.h"

class HITLData : public Data
{
public:
    /** HITLData type */
    static const uint8_t type = 0x07;
    /** HITLData error ID */
    static const int ERR_ID = -type * 100;
    //  ---- Time ----
    /** simulation time (sec) */
    float timestamp_s;

    // ---- IMU ----
    /** x acceleration (m/s^2) */
    float ax;
    /** y acceleration (m/s^2) */
    float ay;
    /** z acceleration (m/s^2) */
    float az;

    /** x angular velocity (rad/s) */
    float gx;
    /** y angular velocity (rad/s) */
    float gy;
    /** z angular velocity (rad/s) */
    float gz;

    /** x magnetic field (uT) */
    float mx;
    /** y magnetic field (uT) */
    float my;
    /** z magnetic field (uT) */
    float mz;

    // ---- Environmental ----
    /** pressure (hPa) */
    float pressure_hPa;
    /** temperature (deg C) */
    float temp_C;

    // ---- GPS ----
    /** latitude (decimal degrees) */
    float lat_deg;
    /** longitude (decimal degrees) */
    float lon_deg;
    /** altitude above MSL (meters) */
    float alt_m;

    /** GPS fix quality (number of satellites) */
    uint8_t fixqual;

    /** GPS heading (degrees) */
    float heading_deg;

    /** size of serialized HITLData */
    static constexpr uint16_t PAYLOAD_SIZE =
        sizeof(float) +     // timestamp_s
        3 * sizeof(float) + // ax, ay, az
        3 * sizeof(float) + // gx, gy, gz
        3 * sizeof(float) + // mx, my, mz
        sizeof(float) +     // pressure_hPa
        sizeof(float) +     // temp_C
        2 * sizeof(float) + // lat_deg, lon_deg
        sizeof(float) +     // alt_m
        sizeof(uint8_t) +   // fixqual
        sizeof(float);      // heading_deg

    /**
     * HITLData default contructor, everything intialized to zero
     */
    HITLData();

    /**
     * HITLData constructor
     * @param _timestamp_s the current timestamp in seconds
     * @param _ax the current x acceleratin in m/s^2
     * @param _ay the current y acceleratin in m/s^2
     * @param _az the current z acceleratin in m/s^2
     * @param _gx the current x angular velocity in rad/s
     * @param _gy the current y angular velocity in rad/s
     * @param _gz the current z angular velocity in rad/s
     * @param _mx the current x magnetic field in uT (micro Telsa)
     * @param _my the current y magnetic field in uT
     * @param _mz the current z magnetic field in uT
     * @param _pressure_hPa the current pressure in hPa
     * @param _temp_C the current temperature in degrees C
     * @param _lat_deg the current latitude in decimal degrees
     * @param _lon_deg the current longitude in decimal degrees
     * @param _alt_m the current altitude in meters
     * @param _fixqual the current number of connected GPS satellites
     * @param _heading_deg the current azimuthal heading in degrees
     */
    HITLData(float _timestamp_s,
             float _ax, float _ay, float _az,
             float _gx, float _gy, float _gz,
             float _mx, float _my, float _mz,
             float _pressure_hPa, float _temp_C,
             float _lat_deg, float _lon_deg, float _alt_m,
             uint8_t _fixqual, float _heading_deg); // parameterized constructor

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