// Created by Divyansh Srivastava on 1/13/2026
// This is the header file for the ASTRAData class defined within ASTRAData.cpp
// This class is tied wiht the entire HITL system and will be used to hold filtered sensor outputs after
// the state algorithm has run
#ifndef ASTRA_DATA_H
#define ASTRA_DATA_H

#include "Data.h"

class AstraData : public Data
{
public:
    // AstraData type
    static const uint8_t type = 0x06;
    // type error ID
    static const int ERR_ID = -type * 100;

    AstraData(); // complete default contructor, everything intialized to zero

    AstraData(float _timestamp_s,
              float _px, float _py, float _pz,
              float _vx, float _vy, float _vz,
              float _ax_e, float _ay_e, float _az_e,
              float _qw, float _qx, float _qy, float _qz); // parameterized constructor

    // encode the data stored in the ```Data``` object and place the result in ```data```, ```sz``` is the max size of ```data```
    int encode(uint8_t *data, uint16_t sz) override;
    // decode the data stored in ```data``` and place it in the ```Data``` object, ```sz``` is the number of bytes from ```data``` to decode
    int decode(uint8_t *data, uint16_t sz) override;

    // place the data in the ```Data``` object in the ```json``` string, ```sz``` is the max size of the string, ```deviceId``` can be set based on hardware
    int toJSON(char *json, uint16_t sz, int deviceId) override;
    // place the data in the ```json``` string in the ```Data``` object, ```sz``` is the max size of the string, ```deviceId``` can be set based on hardware
    int fromJSON(char *json, uint16_t sz, int &deviceId) override;

    // these are the data memebers that we need to run the Avionics algorithms on
    static const uint8_t type = 0xA;

    // ---- fused outputs ----
    float timestamp_s;

    // KF state (relative frame)
    float px, py, pz; // meters
    float vx, vy, vz; // m/s

    // Derived control input actually used for KF propagation (earth frame)
    float ax_e, ay_e, az_e; // m/s^2

    // Orientation (Mahony)
    float qw, qx, qy, qz;

    // Status
    uint8_t mahony_mode; // AstraMahonyMode as uint8_t
    uint8_t flags;       // validity / readiness bits

    // Total payload size in bytes (for encode/decode buffer checks)
    static constexpr uint16_t PAYLOAD_SIZE =
        sizeof(timestamp_s) +
        3 * sizeof(px) +   // px,py,pz
        3 * sizeof(vx) +   // vx,vy,vz
        3 * sizeof(ax_e) + // ax_e,ay_e,az_e
        4 * sizeof(qw);    // qw,qx,qy,qz
};

#endif // ASTRA_DATA_H