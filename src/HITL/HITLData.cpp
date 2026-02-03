//Created by Divyansh Srivastava on 1/11/2026
//This is the implementation file for the HITLData class defined within HITLData.h
//This class will be for HITLData, used for polymorphic data transmission through RadioMessage

#include <stdexcept>
#include "HITLData.h"


 HITLData:: HITLData() : timestamp_s(0.0f), ax(0.0f), ay(0.0f), az(0.0f),
                  gx(0.0f), gy(0.0f), gz(0.0f),
                  mx(0.0f), my(0.0f), mz(0.0f),
                  pressure_hPa(0.0f), temp_C(0.0f),
                  lat_deg(0.0f), lon_deg(0.0f), alt_m(0.0f),
                  fixqual(0), heading_deg(0.0f)
            {} //complete default contructor, everything intialized to zero

HITLData::HITLData(float _timestamp_s, 
                     float _ax, float _ay, float _az,
                     float _gx, float _gy, float _gz,
                     float _mx, float _my, float _mz,
                     float _pressure_hPa, float _temp_C,
                     float _lat_deg, float _lon_deg, float _alt_m,
                  uint8_t _fixqual, float _heading_deg)
                : timestamp_s(_timestamp_s), ax(_ax), ay(_ay), az(_az),
                  gx(_gx), gy(_gy), gz(_gz),
                  mx(_mx), my(_my), mz(_mz),
                  pressure_hPa(_pressure_hPa), temp_C(_temp_C),
                  lat_deg(_lat_deg), lon_deg(_lon_deg), alt_m(_alt_m),
                  fixqual(_fixqual), heading_deg(_heading_deg)
            {} //parameterized constructor


//encode sends string data instead of binary data becuase of worries of 
//little-endian vs big-endian issues with binary data across different hardware, 
//and also because JSON is human readable and easier to debug

uint16_t HITLData::encode(uint8_t *data, uint16_t sz)
{
    if (data == nullptr) return 0;
    if (sz < 2) return 0;

    // Write JSON into the outgoing buffer as text
    int deviceId = 0; // need to fix
    uint16_t n = toJSON(reinterpret_cast<char*>(data), (uint16_t)(sz), deviceId); 
    if (n == 0) return 0;

    return n;         // number of bytes of JSON (not counting '\0')
}


//decode reads string data instead of binary data becuase of worries of
//little-endian vs big-endian issues with binary data across different hardware,
//and also because JSON is human readable and easier to debug
 uint16_t HITLData::decode(uint8_t *data, uint16_t sz)
{
    if (data == nullptr) return 0;

    int deviceId = 0; //need to fix
    // Interpret incoming bytes as a JSON string and parse it
    uint16_t ok = fromJSON(reinterpret_cast<char*>(data), sz, deviceId);
    return ok ? 1 : 0;
}

uint16_t HITLData::toJSON(char *json, uint16_t sz, int deviceId){
    
    uint16_t result = (uint16_t) snprintf(
            json,
            sz,
            "{"
            "\"type\":\"HITLData\","
            "\"deviceId\":%d,"
            "\"data\":{"
                "\"t\":%.7f,"
                "\"ax\":%.7f,\"ay\":%.7f,\"az\":%.7f,"
                "\"gx\":%.7f,\"gy\":%.7f,\"gz\":%.7f,"
                "\"mx\":%.7f,\"my\":%.7f,\"mz\":%.7f,"
                "\"pressure_hPa\":%.7f,"
                "\"temp_C\":%.7f,"
                "\"lat\":%.7f,"
                "\"lon\":%.7f,"
                "\"alt_m\":%.7f,"
                "\"fixqual\":%u,"
                "\"heading_deg\":%.7f"
            "}"
            "}",
            deviceId,
            timestamp_s,
            ax, ay, az,
            gx, gy, gz,
            mx, my, mz,
            pressure_hPa,
            temp_C,
            lat_deg,
            lon_deg,
            alt_m,
            fixqual,
            heading_deg
        );

    if (result < sz)
    {
        // ran properly
        return result;
    }
    // output too large
    return 0;
    
 }

 uint16_t HITLData::fromJSON(char *json, uint16_t sz, int &deviceId){
        // ---- buffers for extracted strings ----
    char deviceIdStr[8] = {0};
    char tStr[18] = {0};

    char axStr[18] = {0};
    char ayStr[18] = {0};
    char azStr[18] = {0};

    char gxStr[18] = {0};
    char gyStr[18] = {0};
    char gzStr[18] = {0};

    char mxStr[18] = {0};
    char myStr[18] = {0};
    char mzStr[18] = {0};

    char pressureStr[18] = {0};
    char tempStr[18] = {0};

    char latStr[18] = {0};
    char lonStr[18] = {0};

    char altStr[18] = {0};
    char fixqualStr[8] = {0};
    char headingStr[18] = {0};

    // ---- extract fields ----
    if (!extractStr(json, sz, "\"deviceId\":", ',', deviceIdStr, sizeof(deviceIdStr)))
        return 0;

    if (!extractStr(json, sz, "\"t\":", ',', tStr, sizeof(tStr)))
        return 0;

    if (!extractStr(json, sz, "\"ax\":", ',', axStr, sizeof(axStr)))
        return 0;
    if (!extractStr(json, sz, "\"ay\":", ',', ayStr, sizeof(ayStr)))
        return 0;
    if (!extractStr(json, sz, "\"az\":", ',', azStr, sizeof(azStr)))
        return 0;

    if (!extractStr(json, sz, "\"gx\":", ',', gxStr, sizeof(gxStr)))
        return 0;
    if (!extractStr(json, sz, "\"gy\":", ',', gyStr, sizeof(gyStr)))
        return 0;
    if (!extractStr(json, sz, "\"gz\":", ',', gzStr, sizeof(gzStr)))
        return 0;

    if (!extractStr(json, sz, "\"mx\":", ',', mxStr, sizeof(mxStr)))
        return 0;
    if (!extractStr(json, sz, "\"my\":", ',', myStr, sizeof(myStr)))
        return 0;
    if (!extractStr(json, sz, "\"mz\":", ',', mzStr, sizeof(mzStr)))
        return 0;

    if (!extractStr(json, sz, "\"pressure_hPa\":", ',', pressureStr, sizeof(pressureStr)))
        return 0;
    if (!extractStr(json, sz, "\"temp_C\":", ',', tempStr, sizeof(tempStr)))
        return 0;

    if (!extractStr(json, sz, "\"lat\":", ',', latStr, sizeof(latStr)))
        return 0;
    if (!extractStr(json, sz, "\"lon\":", ',', lonStr, sizeof(lonStr)))
        return 0;

    if (!extractStr(json, sz, "\"alt_m\":", ',', altStr, sizeof(altStr)))
        return 0;

    if (!extractStr(json, sz, "\"fixqual\":", ',', fixqualStr, sizeof(fixqualStr)))
        return 0;

    // last field ends with '}'
    if (!extractStr(json, sz, "\"heading_deg\":", '}', headingStr, sizeof(headingStr)))
        return 0;

    // ---- convert to numeric types ----
    deviceId = atoi(deviceIdStr);

    timestamp_s = (float)atof(tStr);

    ax = (float)atof(axStr);
    ay = (float)atof(ayStr);
    az = (float)atof(azStr);

    gx = (float)atof(gxStr);
    gy = (float)atof(gyStr);
    gz = (float)atof(gzStr);

    mx = (float)atof(mxStr);
    my = (float)atof(myStr);
    mz = (float)atof(mzStr);

    pressure_hPa = (float)atof(pressureStr);
    temp_C = (float)atof(tempStr);

    lat_deg = (float)atof(latStr);
    lon_deg = (float)atof(lonStr);

    alt_m = (float)atof(altStr);

    fixqual = (uint8_t)atoi(fixqualStr);
    heading_deg = (float)atof(headingStr);

    return 1;

 }



