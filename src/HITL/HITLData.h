//Created by Divyansh Srivastava on 1/11/2026
//This class will be for creating a HITLData type by inheritign from the Data class defined within RadioMessage
//through the repository: https://github.com/erodarob/RadioMessage.git
#ifndef HITL_DATA_H
#define HITL_DATA_H

#include "Data.h"

    class HITLData : public Data {
        public: 


            HITLData();//complete default contructor, everything intialized to zero

            HITLData(float _timestamp_s, 
                     float _ax, float _ay, float _az,
                     float _gx, float _gy, float _gz,
                     float _mx, float _my, float _mz,
                     float _pressure_hPa, float _temp_C,
                     float _lat_deg, float _lon_deg, float _alt_m,
                    uint8_t _fixqual, float _heading_deg);  //parameterized constructor




            uint16_t encode(uint8_t *data, uint16_t sz) override;
            uint16_t decode(uint8_t *data, uint16_t sz) override;
            uint16_t toJSON(char *json, uint16_t sz, int deviceId) override;
            uint16_t fromJSON(char *json, uint16_t sz, int &deviceId) override;


            //these are the data memebers that we need to run the Avionics algorithms on 
            // ---- Time ----
            float timestamp_s;          // simulation time (seconds)

            // ---- IMU ----
            float ax;                   // acceleration X (m/s^2)
            float ay;                   // acceleration Y (m/s^2)
            float az;                   // acceleration Z (m/s^2)

            float gx;                   // angular velocity X (rad/s)
            float gy;                   // angular velocity Y (rad/s)
            float gz;                   // angular velocity Z (rad/s)

            float mx;                   // magnetic field X (uT)
            float my;                   // magnetic field Y (uT)
            float mz;                   // magnetic field Z (uT)

            // ---- Environmental ----
            float pressure_hPa;         // pressure (hPa / mbar)
            float temp_C;               // temperature (C)

            // ---- GPS ----
            float lat_deg;             // latitude (decimal degrees)
            float lon_deg;             // longitude (decimal degrees)
            float  alt_m;               // altitude MSL (meters)

            uint8_t fixqual;            // GPS fix quality (number of satellites)

            float heading_deg;          // GPS heading (degrees)


           static const uint8_t type = 0x09; //default stream type for HITLData

           static constexpr uint16_t PAYLOAD_SIZE = //size of serialized HITLData
            sizeof(float) +            // timestamp_s
            3*sizeof(float) +          // ax, ay, az
            3*sizeof(float) +          // gx, gy, gz
            3*sizeof(float) +          // mx, my, mz
            sizeof(float) +            // pressure_hPa
            sizeof(float) +            // temp_C
            2*sizeof(float) +         // lat_deg, lon_deg
            sizeof(float) +            // alt_m
            sizeof(uint8_t) +          // fixqual
            sizeof(float);             // heading_deg


    };





#endif // HITL_DATA_H