#ifndef GENERICDATA_H
#define GENERICDATA_H

#include "../Data.h"

class GenericData : public Data
{
public:
    /** GenericData type */
    static const uint8_t type = 0x08;
    /** GenericData error ID */
    static const int ERR_ID = -type * 100;
    /** the maximum size of the data */
    static const uint16_t maxSize = 0x1FFF;

    /** the size of the data */
    uint16_t size = 0;
    /** buffer to store the data */
    uint8_t data[maxSize] = {0};

    /**
     * GenericData default constructor
     */
    GenericData() {};

    /**
     * GenericData constructor
     * @param data the data to be encoded, length assumed to be ```GenericData::maxSize```
     */
    GenericData(uint8_t *data);
    /**
     * GenericData constructor
     * @param data the data to be encoded
     * @param sz the size of the data
     */
    GenericData(uint8_t *data, uint16_t sz);

    /**
     * Copies from the given array into the internal buffer
     * @param data a pointer to the array to copy data from
     * @param sz the number of bytes to be copied, must be less than ```GenericData::maxSize```
     * @return whether the requested number of bytes could be copied
     */
    bool fill(uint8_t *data, uint16_t sz);

    /**
     * Encode the data stored in the Data object and place it in the provided buffer
     * @param data a pointer to the array to place the data in
     * @param sz the maximum length of the data array
     * @return the result of the encoding, the length of bytes added if successful, otherwise an error code less than 0
     */
    int encode(uint8_t *data, uint16_t sz) override;
    /**
     * Decode the data stored in the the provided buffer and place it in the Data object
     * @param data a pointer to the array to extract data from
     * @param sz the length of the data array
     * @return the result of the decoding, the length of bytes decoded if successful, otherwise an error code less than 0
     */
    int decode(uint8_t *data, uint16_t sz) override;

    /**
     * Encode the data in the Data object into a JSON format
     * @param json the string to place the JSON into
     * @param sz the maximum size of the json string
     * @param deviceId a number indicating the hardware the data came from
     * @return the result of the encoding, the length of bytes added if successful, otherwise an error code less than 0
     */
    int toJSON(char *json, uint16_t sz, int deviceId) override;
    /**
     * Decode the data from a JSON format into the Data object
     * @param json the JSON string to extract data from
     * @param sz the size of the json string
     * @param deviceId a number indicating the hardware the data came from, decoded separately from the JSON
     * @return the result of the decoding, greater than 0 if successful, otherwise an error code less than 0
     */
    int fromJSON(char *json, uint16_t sz, int &deviceId) override;
};

#endif