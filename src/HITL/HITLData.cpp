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


uint16_t HITLData::encode(uint8_t *data, uint16_t sz){

    uint16_t pos = 0;

    const uint16_t totalSize = static_cast<uint16_t>(PAYLOAD_SIZE);
    if (sz < totalSize)
        this->error = Error::TooSmall; //error: provided buffer too small
        return 0;

      std::memcpy(&data[pos], &timestamp_s, sizeof(timestamp_s));
    pos += sizeof(timestamp_s);

    std::memcpy(&data[pos], &ax, sizeof(ax));
    pos += sizeof(ax);
    std::memcpy(&data[pos], &ay, sizeof(ay));
    pos += sizeof(ay);
    std::memcpy(&data[pos], &az, sizeof(az));
    pos += sizeof(az);

    std::memcpy(&data[pos], &gx, sizeof(gx));
    pos += sizeof(gx);
    std::memcpy(&data[pos], &gy, sizeof(gy));
    pos += sizeof(gy);
    std::memcpy(&data[pos], &gz, sizeof(gz));
    pos += sizeof(gz);

    std::memcpy(&data[pos], &mx, sizeof(mx));
    pos += sizeof(mx);
    std::memcpy(&data[pos], &my, sizeof(my));
    pos += sizeof(my);
    std::memcpy(&data[pos], &mz, sizeof(mz));
    pos += sizeof(mz);

    std::memcpy(&data[pos], &pressure_hPa, sizeof(pressure_hPa));
    pos += sizeof(pressure_hPa);

    std::memcpy(&data[pos], &temp_C, sizeof(temp_C));
    pos += sizeof(temp_C);

    std::memcpy(&data[pos], &lat_deg, sizeof(lat_deg));
    pos += sizeof(lat_deg);
    std::memcpy(&data[pos], &lon_deg, sizeof(lon_deg));
    pos += sizeof(lon_deg);

    std::memcpy(&data[pos], &alt_m, sizeof(alt_m));
    pos += sizeof(alt_m);

    std::memcpy(&data[pos], &fixqual, sizeof(fixqual));
    pos += sizeof(fixqual);

    std::memcpy(&data[pos], &heading_deg, sizeof(heading_deg));
    pos += sizeof(heading_deg);

    // ---- sanity check ----
//     if (pos != totalSize) {
//             this->error = Error::EncodeError; //error: encoding error
//             return 0;
//     }
    return pos;

}

 uint16_t HITLData::decode(uint8_t *data, uint16_t sz){

      if(data != nullptr){
       
            uint16_t pos = 0;

            if(sz < static_cast<uint16_t>(1 + PAYLOAD_SIZE)){
            this->error = Error::TooSmall; //error: provided buffer too small
            return 0;
            }

            std::memcpy(&timestamp_s, &data[pos], sizeof(timestamp_s));
            pos += sizeof(timestamp_s);

            std::memcpy(&ax, &data[pos], sizeof(ax));
            pos += sizeof(ax);

            std::memcpy(&ay, &data[pos], sizeof(ay));
            pos += sizeof(ay);

            std::memcpy(&az, &data[pos], sizeof(az));
            pos += sizeof(az);

            std::memcpy(&gx, &data[pos], sizeof(gx));
            pos += sizeof(gx);
            
            std::memcpy(&gy, &data[pos], sizeof(gy));
            pos += sizeof(gy);

            std::memcpy(&gz, &data[pos], sizeof(gz));
            pos += sizeof(gz);

            std::memcpy(&mx, &data[pos], sizeof(mx));
            pos += sizeof(mx);

            std::memcpy(&my, &data[pos], sizeof(my));
            pos += sizeof(my);

            std::memcpy(&mz, &data[pos], sizeof(mz));
            pos += sizeof(mz);

            std::memcpy(&pressure_hPa, &data[pos], sizeof(pressure_hPa));
            pos += sizeof(pressure_hPa);

            std::memcpy(&temp_C, &data[pos], sizeof(temp_C));
            pos += sizeof(temp_C);

            std::memcpy(&lat_deg, &data[pos], sizeof(lat_deg));
            pos += sizeof(lat_deg);

            std::memcpy(&lon_deg, &data[pos], sizeof(lon_deg));
            pos += sizeof(lon_deg);

            std::memcpy(&alt_m, &data[pos], sizeof(alt_m));
            pos += sizeof(alt_m);

            std::memcpy(&fixqual, &data[pos], sizeof(fixqual));
            pos += sizeof(fixqual);

            std::memcpy(&heading_deg, &data[pos], sizeof(heading_deg));
            pos += sizeof(heading_deg);

            // ---- sanity check ----
            // if (pos != static_cast<uint16_t>(1 + PAYLOAD_SIZE)) {
            // this->error = Error::DecodeError; //error: decoding error
            // return 0;
            // }
            return pos;

      }

}



