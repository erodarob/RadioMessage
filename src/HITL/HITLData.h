//Created by Divyansh Srivastava on 1/11/2026
//This class will be for creating a HITLData type by inheritign from the Data class defined within RadioMessage
//through the repository: https://github.com/erodarob/RadioMessage.git
#ifndef HITL_DATA_H
#define HITL_DATA_H

#include "Data.h"

    class HITLData : public Data {
        public: 
            HITLData() 
                : timestamp_s(0.0f), ax(0.0f), ay(0.0f), az(0.0f),
                  gx(0.0f), gy(0.0f), gz(0.0f),
                  mx(0.0f), my(0.0f), mz(0.0f),
                  pressure_hPa(0.0f), temp_C(0.0f),
                  lat_deg(0.0), lon_deg(0.0), alt_m(0.0f),
                  fix(0), fixqual(0), heading_deg(0.0f)
            {} //complete default contructor, everything intialized to zero

            HITLData(float _timestamp_s, 
                     float _ax, float _ay, float _az,
                     float _gx, float _gy, float _gz,
                     float _mx, float _my, float _mz,
                     float _pressure_hPa, float _temp_C,
                     double _lat_deg, double _lon_deg, float _alt_m,
                     uint8_t _fix, uint8_t _fixqual, float _heading_deg)
                : timestamp_s(_timestamp_s), ax(_ax), ay(_ay), az(_az),
                  gx(_gx), gy(_gy), gz(_gz),
                  mx(_mx), my(_my), mz(_mz),
                  pressure_hPa(_pressure_hPa), temp_C(_temp_C),
                  lat_deg(_lat_deg), lon_deg(_lon_deg), alt_m(_alt_m),
                  fix(_fix), fixqual(_fixqual), heading_deg(_heading_deg)
            {} //parameterized constructor




            uint16_t encode(uint8_t *data, uint16_t sz) override;
            uint16_t decode(uint8_t *data, uint16_t sz) override;
            uint16_t toJSON(char *json, uint16_t sz, int deviceId) override;
            uint16_t fromJSON(char *json, uint16_t sz, int &deviceId) override;

            HITLData parser(std::FILE *file); //this parser is going to be used to scan a file and place every value in its 
                                              //respective field 

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
            double lat_deg;             // latitude (decimal degrees)
            double lon_deg;             // longitude (decimal degrees)
            float  alt_m;               // altitude MSL (meters)

            uint8_t fix;                // GPS fix status (0 or 1)
            uint8_t fixqual;            // GPS fix quality (number of satellites)

            float heading_deg;          // GPS heading (degrees)


           static constexpr uint8_t type = 0x09; //default stream type for HITLData

           static constexpr uint16_t PAYLOAD_SIZE = //size of serialized HITLData
            sizeof(float) +            // timestamp_s
            3*sizeof(float) +          // ax, ay, az
            3*sizeof(float) +          // gx, gy, gz
            3*sizeof(float) +          // mx, my, mz
            sizeof(float) +            // pressure_hPa
            sizeof(float) +            // temp_C
            2*sizeof(double) +         // lat_deg, lon_deg
            sizeof(float) +            // alt_m
            sizeof(uint8_t) +          // fix
            sizeof(uint8_t) +          // fixqual
            sizeof(float);             // heading_deg


    };





#endif // HITL_DATA_H