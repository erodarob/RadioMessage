#ifndef VIDEODATA_H
#define VIDEODATA_H

#include "../Data.h"
#include "GenericData.h"

/**
 * Indicates different video encoding types
 */
enum VideoEncoding : uint8_t
{
    AV1,
    H264,
    H265,
    RAW,
    NONE
};

class VideoData : public GenericData
{
public:
    /** VideoData type */
    static const uint8_t type = 0x09;
    /** VideoData error ID */
    static const int ERR_ID = -type * 100;
    /** the maximum size of the data, subtract 1 byte for encoding */
    static const uint16_t maxSize = 0x1FFF - 1;

    /** encoding of the video */
    VideoEncoding encoding = NONE;

    /**
     * VideoData default constructor
     */
    VideoData() : GenericData() {};

    /**
     * VideoData constructor
     * @param encoding the encoding of the video
     */
    VideoData(VideoEncoding encoding);
    /**
     * VideoData constructor
     * @param data the data to be encoded, assumed to be ```VideoData::maxSize```
     * @param encoding the encoding of the video
     */
    VideoData(uint8_t *data, VideoEncoding encoding);
    /**
     * VideoData constructor
     * @param data the data to be encoded
     * @param sz the size of the data
     * @param encoding the encoding of the video
     */
    VideoData(uint8_t *data, uint16_t sz, VideoEncoding encoding);

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

private:
    /**
     * Get a string representing the video encoding, based on the VideoEncoding enum
     * @param str the string to put the encoding type into, not guaranteed to be null terminated
     * @param sz the size of the string, must be >6
     */
    void getEncodingStr(char *str, int sz);
    /**
     * Set the encoding based on a string representation of the VideoEncoding enum
     * @param str the string to use, must be null terminated
     */
    void setEncoding(char *str);
};

#endif