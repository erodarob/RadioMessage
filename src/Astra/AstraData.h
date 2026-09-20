// This is the header file for the ASTRAData class defined within ASTRAData.cpp
// This class is tied wiht the entire HITL system and will be used to hold filtered sensor outputs after
// the state algorithm has run

#ifndef ASTRA_DATA_H
#define ASTRA_DATA_H

#include "Data.h"

class AstraData : public Data
{
public:
    /** AstraData type */
    static const uint8_t type = 0x06;
    /** AstraData error ID */
    static const int ERR_ID = -type * 100;

    // ---- fused outputs ----
    /** the timestamp of the data (units?) */
    float timestamp_s;

    // KF state (relative frame)
    /** the current x position in meters */
    float px;
    /** the current y position in meters */
    float py;
    /** the current z position in meters */
    float pz;
    /** the current x velocity in m/s */
    float vx;
    /** the current y velocity in m/s */
    float vy;
    /** the current z velocity in m/s */
    float vz;

    // Derived control input actually used for KF propagation (earth frame)
    // TODO: what Earth frame?
    /** the x acceleration in the Earth frame (m/s^2) */
    float ax_e;
    /** the y acceleration in the Earth frame (m/s^2) */
    float ay_e;
    /** the z acceleration in the Earth frame (m/s^2) */
    float az_e;

    // Orientation (Mahony)
    /** quaternion orientation w component (units?) */
    float qw;
    /** quaternion orientation x component (units?) */
    float qx;
    /** quaternion orientation y component (units?) */
    float qy;
    /** quaternion orientation z component (units?) */
    float qz;

    // Status
    /** AstraMahonyMode as uint8_t */
    uint8_t mahony_mode;
    /** validity / readiness bits */
    uint8_t flags;

    /** Total payload size in bytes (for encode/decode buffer checks) */
    static constexpr uint16_t PAYLOAD_SIZE =
        sizeof(timestamp_s) +
        3 * sizeof(px) +   // px,py,pz
        3 * sizeof(vx) +   // vx,vy,vz
        3 * sizeof(ax_e) + // ax_e,ay_e,az_e
        4 * sizeof(qw);    // qw,qx,qy,qz

    /**
     * AstraData default constructor, everything intialized to zero
     */
    AstraData();

    /**
     * AstraData constructor
     * @param _timestamp_s the current timestamp in units?
     * @param _px the current x position in meters
     * @param _py the current y position in meters
     * @param _pz the current z position in meters
     * @param _vx the current x velocity in m/s
     * @param _vy the current y velocity in m/s
     * @param _vz the current z velocity in m/s
     * @param _ax_e the current x acceleration in m/s^2
     * @param _ay_e the current y acceleration in m/s^2
     * @param _az_e the current z acceleration in m/s^2
     * @param _qw the current orientation quaternion w component in units?
     * @param _qx the current orientation quaternion w component in units?
     * @param _qy the current orientation quaternion w component in units?
     * @param _qz the current orientation quaternion w component in units?
     */
    AstraData(float _timestamp_s,
              float _px, float _py, float _pz,
              float _vx, float _vy, float _vz,
              float _ax_e, float _ay_e, float _az_e,
              float _qw, float _qx, float _qy, float _qz); // parameterized constructor

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