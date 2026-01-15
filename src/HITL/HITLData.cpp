//Created by Divyansh Srivastava on 1/11/2026
//This is the implementation file for the HITLData class defined within HITLData.h
//This class will be for HITLData, used for polymorphic data transmission through RadioMessage

#include "HITLData.h"


 HITLData:: HITLData(): timestamp_s(0.0f), ax(0.0f), ay(0.0f), az(0.0f),
                  gx(0.0f), gy(0.0f), gz(0.0f),
                  mx(0.0f), my(0.0f), mz(0.0f),
                  pressure_hPa(0.0f), temp_C(0.0f),
                  lat_deg(0.0), lon_deg(0.0), alt_m(0.0f),
                  fix(0), fixqual(0), heading_deg(0.0f)
            {} //complete default contructor, everything intialized to zero

HITLData::HITLData(float _timestamp_s, 
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

// HITLData parser(std::FILE *file){
//     if (file == NULL){
//                 __throw_invalid_argument; //file not found, need better error logging system 
//             }

//             // Get buffer instance
//             HITLData buffer;

//             // Parse CSV data
//             // Format: timestamp,ax,ay,az,gx,gy,gz,mx,my,mz,pressure,temp,lat,lon,alt,fix,fixqual,heading

//             while(stdin != out){ //do while loop until the end of the file 

    
//             int itemsParsed = sscanf(file, "%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%d,%d,%lf",
//                                      &buffer.timestamp_s,
//                                      &buffer.ax,
//                                      &buffer.ay,
//                                      &buffer.az,
//                                      &buffer.data.gyro.x(),
//                                      &buffer.data.gyro.y(),
//                                      &buffer.data.gyro.z(),
//                                      &buffer.data.mag.x(),
//                                      &buffer.data.mag.y(),
//                                      &buffer.data.mag.z(),
//                                      &buffer.data.pressure,
//                                      &buffer.data.temperature,
//                                      &buffer.data.gps_lat,
//                                      &buffer.data.gps_lon,
//                                      &buffer.data.gps_alt,
//                                      (int *)&buffer.data.gps_fix,
//                                      &buffer.data.gps_fix_quality,
//                                      &buffer.data.gps_heading);
//             }

//             if (itemsParsed != 18)
//             {
//                 LOGE("HITL: Parse error, got %d items (expected 18)", itemsParsed);
//                 __throw_invalid_argument;
//             }


//             // Return timestamp
//             timestamp = buffer.timestamp_s;

//             return buffer;

// }

// uint16_t HITLData::encode(uint8_t *data, uint16_t sz){

//     uint16_t pos = 0;

//     if(sz < this->PAYLOAD_SIZE){
//         return 0; //error, size is too small 
//     }

//     if(this->type != 0x09){
//         return 0; //type is not HITLData
//     }

//     data[pos++] = this->type;
//     data[pos++] = (uint8_t)this->timestamp_s;
//     data[pos++] = (uint8_t)this->ax;
//     data[pos++] = (uint8_t)this->ay;
//     data[pos++] = (uint8_t)this->az; //this is defintely wrong, will get a lot of stuff "forgotten" because i'm casting a float 
//                                      // which is around 4 bytes into an 8 bit unsigned int (1 byte unsigned int)




// }



