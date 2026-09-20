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
    /** the type of the Data subclass, unique to each */
    static const uint8_t type;
    /** the error ID of the Data subclass, used to identify errors encountered during encoding/decoding */
    static const int ERR_ID;

    /**
     * Virtual destructor. Very important
     */
    virtual ~Data() {};
    /**
     * Encode the data stored in the Data object and place it in the provided buffer
     * @param data a pointer to the array to place the data in
     * @param sz the maximum length of the data array
     * @return the result of the encoding, the length of bytes added if successful, otherwise an error code less than 0
     */
    virtual int encode(uint8_t *data, uint16_t sz) = 0;
    /**
     * Decode the data stored in the the provided buffer and place it in the Data object
     * @param data a pointer to the array to extract data from
     * @param sz the length of the data array
     * @return the result of the decoding, the length of bytes decoded if successful, otherwise an error code less than 0
     */
    virtual int decode(uint8_t *data, uint16_t sz) = 0;

    /**
     * Encode the data in the Data object into a JSON format
     * @param json the string to place the JSON into
     * @param sz the maximum size of the json string
     * @param deviceId a number indicating the hardware the data came from
     * @return the result of the encoding, the length of bytes added if successful, otherwise an error code less than 0
     */
    virtual int toJSON(char *json, uint16_t sz, int deviceId) = 0;
    /**
     * Decode the data from a JSON format into the Data object
     * @param json the JSON string to extract data from
     * @param sz the size of the json string
     * @param deviceId a number indicating the hardware the data came from, decoded separately from the JSON
     * @return the result of the decoding, greater than 0 if successful, otherwise an error code less than 0
     */
    virtual int fromJSON(char *json, uint16_t sz, int &deviceId) = 0;

    /**
     * Extract a substing from a string
     * @param src the string to take the substring from
     * @param szSource the size of the source string
     * @param lookFor the substring to look for to start the substring
     * @param stopCond the char to look for to stop the substring
     * @param dest the string to place the substring into
     * @param szDest the size of the destination string
     * @return whether the substring was located
     */
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
        // ensure null terminated
        dest[counter] = 0;
        return true;
    }
};

#endif