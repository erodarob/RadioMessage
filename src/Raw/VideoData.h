#ifndef VIDEODATA_H
#define VIDEODATA_H

#include "../Data.h"
#include "GenericData.h"

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
    // the maximum size of the data
    // subtract 1 byte for encoding
    static const uint16_t maxSize = 0x1FFF - 1;

    // VideoData type
    static const uint8_t type = 0x08;
    // type error ID
    static const int ERR_ID = -type * 100;

    // encoding of the video
    VideoEncoding encoding = NONE;

    // VideoData default constructor
    VideoData() : GenericData() {};

    // VideoData constructor
    // - encoding : the encoding of the video
    VideoData(VideoEncoding encoding);

    // VideoData constructor
    // - data : the data to be encoded, assumed to be ```maxSize```
    // - encoding : the encoding of the video
    VideoData(uint8_t *data, VideoEncoding encoding);

    // VideoData constructor
    // - data : the data to be encoded
    // - sz : the size of the data
    // - encoding : the encoding of the video
    VideoData(uint8_t *data, uint16_t sz, VideoEncoding encoding);

    // encode the data stored in the ```Data``` object and place the result in ```data```, ```sz``` is the max size of ```data```
    int encode(uint8_t *data, uint16_t sz) override;
    // decode the data stored in ```data``` and place it in the ```Data``` object, ```sz``` is the number of bytes from ```data``` to decode
    int decode(uint8_t *data, uint16_t sz) override;

    // place the data in the ```Data``` object in the ```json``` string, ```sz``` is the max size of the string, ```deviceId``` can be set based on hardware
    int toJSON(char *json, uint16_t sz, int deviceId) override;
    // place the data in the ```json``` string in the ```Data``` object, ```sz``` is the max size of the string, ```deviceId``` can be set based on hardware
    int fromJSON(char *json, uint16_t sz, int &deviceId) override;

private:
    void getEncodingStr(char *str, int sz);
    void setEncoding(char *str, int sz);
};

#endif