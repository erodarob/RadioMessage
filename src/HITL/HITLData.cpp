//Created by Divyansh Srivastava on 1/11/2026
//This is the implementation file for the HITLData class defined within HITLData.h
//This class will be for HITLData, used for polymorphic data transmission through RadioMessage

#include <stdexcept>
#include "HITLData.h"


 HITLData:: HITLData() : timestamp_s(0.0f), ax(0.0f), ay(0.0f), az(0.0f),
                  gx(0.0f), gy(0.0f), gz(0.0f),
                  mx(0.0f), my(0.0f), mz(0.0f),
                  pressure_hPa(0.0f), temp_C(0.0f),
                  lat_deg(0.0), lon_deg(0.0), alt_m(0.0f),
                  fixqual(0), heading_deg(0.0f)
            {} //complete default contructor, everything intialized to zero

HITLData::HITLData(float _timestamp_s, 
                     float _ax, float _ay, float _az,
                     float _gx, float _gy, float _gz,
                     float _mx, float _my, float _mz,
                     float _pressure_hPa, float _temp_C,
                     double _lat_deg, double _lon_deg, float _alt_m,
                  uint8_t _fixqual, float _heading_deg)
                : timestamp_s(_timestamp_s), ax(_ax), ay(_ay), az(_az),
                  gx(_gx), gy(_gy), gz(_gz),
                  mx(_mx), my(_my), mz(_mz),
                  pressure_hPa(_pressure_hPa), temp_C(_temp_C),
                  lat_deg(_lat_deg), lon_deg(_lon_deg), alt_m(_alt_m),
                  fixqual(_fixqual), heading_deg(_heading_deg)
            {} //parameterized constructor



HITLData HITLData::parser(std::FILE *file)
{
    if (file == nullptr) {
        throw std::invalid_argument("HITLData::parser: file is null");
    }

    HITLData out;
    char line[512];

    while (std::fgets(line, sizeof(line), file) != nullptr) {
        // Skip empty/comment lines
        if (line[0] == '\n' || line[0] == '\r' || line[0] == '\0' || line[0] == '#') {
            continue;
        }

        unsigned int fixqual_int = 0; // parse uint8 safely via wider type

        int itemsParsed = std::sscanf(
            line,
            "%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%lf,%lf,%f,%u,%f",
            &out.timestamp_s,
            &out.ax, &out.ay, &out.az,
            &out.gx, &out.gy, &out.gz,
            &out.mx, &out.my, &out.mz,
            &out.pressure_hPa,
            &out.temp_C,
            &out.lat_deg,
            &out.lon_deg,
            &out.alt_m,
            &fixqual_int,
            &out.heading_deg
        );

        if (itemsParsed != 17) {
            throw std::runtime_error("HITLData::parser: CSV parse failed (expected 17 fields)");
        }

        if (fixqual_int > 255) {
            throw std::runtime_error("HITLData::parser: fixqual out of uint8_t range");
        }
        out.fixqual = static_cast<uint8_t>(fixqual_int);

        return out; // return one parsed row
    }

    throw std::runtime_error("HITLData::parser: EOF (no more data)");
}


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



