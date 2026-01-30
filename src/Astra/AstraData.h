//Created by Divyansh Srivastava on 1/13/2026
//This is the header file for the ASTRAData class defined within ASTRAData.cpp
//This class is tied wiht the entire HITL system and will be used to hold filtered sensor outputs after 
//the state algorithm has run
#ifndef ASTRA_DATA_H
#define ASTRA_DATA_H

#include "Data.h"


    
    class ASTRAData : public Data {
        public: 

            ASTRAData();//complete default contructor, everything intialized to zero

            ASTRAData(float _timestamp_s,
                      float _px, float _py, float _pz,
                      float _vx, float _vy, float _vz,
                      float _ax_e, float _ay_e, float _az_e,
                      float _qw, float _qx, float _qy, float _qz);  //parameterized constructor

            uint16_t encode(uint8_t *data, uint16_t sz);
            uint16_t decode(uint8_t *data, uint16_t sz);
            uint16_t toJSON(char *json, uint16_t sz, int deviceId);
            uint16_t fromJSON(char *json, uint16_t sz, int &deviceId);
    
            //these are the data memebers that we need to run the Avionics algorithms on 
            static const uint8_t type = 0xA;

        // ---- fused outputs ----
        float timestamp_s;

        // KF state (relative frame)
        float px, py, pz;     // meters
        float vx, vy, vz;     // m/s

        // Derived control input actually used for KF propagation (earth frame)
        float ax_e, ay_e, az_e; // m/s^2

        // Orientation (Mahony)
        float qw, qx, qy, qz;

        // Status
        uint8_t mahony_mode;  // AstraMahonyMode as uint8_t
        uint8_t flags;        // validity / readiness bits

           // Total payload size in bytes (for encode/decode buffer checks)
        static constexpr uint16_t PAYLOAD_SIZE =
            sizeof(timestamp_s) +
            3 * sizeof(px) +     // px,py,pz
            3 * sizeof(vx) +     // vx,vy,vz
            3 * sizeof(ax_e) +   // ax_e,ay_e,az_e
            4 * sizeof(qw);    // qw,qx,qy,qz


    };

#endif // ASTRA_DATA_H