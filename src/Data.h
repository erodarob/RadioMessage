#ifndef DATA_H
#define DATA_H

#if defined(ARDUINO)
#include <Arduino.h>
#elif defined(_WIN32) || defined(_WIN64) || defined(__unix__) || defined(__APPLE__) // Windows, Linux, or OSX
#include <cstdint>
#include <cstring>
#include <cstdio>
#include <cmath>
using namespace std;
#endif

// TODO: deviceId probably not needed in JSON

// basically just a container to place decoded data into
class Data
{
public:
    static const uint8_t type;
    static const int ERR_ID;

    virtual ~Data() {}; // Virtual descructor. Very important
    // encode the data stored in the ```Data``` object and place the result in ```data```, ```sz``` is the max size of ```data```
    virtual int encode(uint8_t *data, uint16_t sz) = 0;
    // decode the data stored in ```data``` and place it in the ```Data``` object, ```sz``` is the number of bytes from ```data``` to decode
    virtual int decode(uint8_t *data, uint16_t sz) = 0;

    // place the data in the ```Data``` object in the ```json``` string, ```sz``` is the max size of the string, ```deviceId``` can be set based on hardware
    virtual int toJSON(char *json, uint16_t sz, int deviceId) = 0;
    // place the data in the ```json``` string in the ```Data``` object, ```sz``` is the max size of the string, ```deviceId``` can be set based on hardware
    virtual int fromJSON(char *json, uint16_t sz, int &deviceId) = 0;

    // extract a substing from a string
    // - src: the string to take the substring from
    // - szSource: the size of the ```src``` string
    // - lookFor: the substring to look for to start the substring
    // - stopCond: the char to look for to stop the substring
    // - dest: the string to place the substring into
    // - szDest: the size of the ```dest``` string
    static bool extractStr(char *src, int szSource, const char *lookFor, char stopCond, char *dest, int szDest = -1)
    {
        // find to position of the start of the lookFor string
        char *strPos = strstr(src, lookFor);
        // check that the substring was found
        if (!strPos)
            return false;
        // find the position of the start of the substring
        int pos = int(strPos - src) + strlen(lookFor);
        int counter = 0;
        // write to the dest string until the stop charater is found, making sure we don't overrun any buffers
        while (src[pos] != stopCond && pos < szSource && (szDest == -1 || counter < szDest - 1))
        {
            dest[counter++] = src[pos++];
        }
        return true;
    }
};

#endif