#include "VideoData.h"

VideoData::VideoData(VideoEncoding encoding) : encoding(encoding)
{
}

VideoData::VideoData(uint8_t *data, VideoEncoding encoding) : encoding(encoding), GenericData(data)
{
}

VideoData::VideoData(uint8_t *data, uint16_t sz, VideoEncoding encoding) : encoding(encoding), GenericData(data, sz)
{
}

uint16_t VideoData::encode(uint8_t *data, uint16_t sz)
{
    // make sure the provided array size is long enough
    if (sz >= this->size + 1)
    {
        // copy the encoding
        data[0] = encoding;
        // copy the data
        memcpy(data + 1, this->data, this->size);
        return this->size + 1;
    }
    return 0;
}

uint16_t VideoData::decode(uint8_t *data, uint16_t sz)
{
    // make sure size is not too big
    if (sz > this->maxSize)
        this->size = this->maxSize;
    else
        this->size = sz;
    // get encoding
    // assume NONE is the last value in the VideoEncoding enum
    if (data[0] > NONE)
        // there is not an enum for this, so set to none
        encoding = NONE;
    else
        // get the encoding normally
        encoding = (VideoEncoding)data[0];
    // copy data into internal buffer
    memcpy(this->data, data + 1, sz - 1);
    return sz;
}

uint16_t VideoData::toJSON(char *json, uint16_t sz, int deviceId)
{
    char encodingStr[10] = {0};
    this->getEncodingStr(encodingStr, sizeof(encodingStr));
    uint16_t result = (uint16_t)snprintf(json, sz, "{\"type\":\"VideoData\",\"deviceId\":%d,\"data\":{\"encoding\":\"%s\", \"data\":[", deviceId, encodingStr);

    if (result >= sz)
    {
        // output too large
        return 0;
    }

    // result should be the index of the [

    // need to dynamically adjust the number of bytes added based on size
    for (int i = 0; i < this->size; i++)
    {
        if (result + 4 < sz)
        {
            int added = sprintf(json + result, "%d,", this->data[i]);
            if (added > 0 && result + added < sz)
                result += added;
            else
                return 0; // output too large
        }
        else
            return 0; // output too large
    }

    // result should be the index of \0
    // unless size is 0
    if (this->size == 0)
        result++;

    // add closing braces
    // need to overwrite the last trailing comma
    if (result + 3 < sz)
    {
        json[result - 1] = ']';
        json[result] = '}';
        json[result + 1] = '}';
        json[result + 2] = '\0';

        // ran properly
        return result;
    }

    // output too large
    return 0;
}

uint16_t VideoData::fromJSON(char *json, uint16_t sz, int &deviceId)
{
    // strings to store data in
    char deviceIdStr[5] = {0};
    char encodingStr[10] = {0};

    // extract each string
    if (!extractStr(json, sz, "\"deviceId\":", ',', deviceIdStr))
        return 0;
    if (!extractStr(json, sz, "\"encoding\":", ',', encodingStr))
        return 0;

    // set encoding
    this->setEncoding(encodingStr, sizeof(encodingStr));

    // convert to correct data type
    deviceId = atoi(deviceIdStr);

    // need to manually extract data (instead of using extractStr) since it is an array
    char *dataStrPos = strstr(json, "\"data\":[");
    if (dataStrPos == nullptr)
        return 0;
    int current = int(dataStrPos - json) + 10; // add 10 to move to the "["
    this->size = 0;

    while (json[current] != ']' && current < sz)
    {
        char dataByte[4] = {0};
        int index = 0;
        while (json[current] != ',' && current < sz)
        {
            if (index < 3)
            {
                dataByte[index] = json[current];
                index++;
            }
            current++;
        }
        this->data[this->size] = atoi(dataByte);
        this->size++;
        current++;
    }

    return this->size;
}

void VideoData::getEncodingStr(char *str, int sz)
{
    if (sz < 6)
    {
        sz = 0;
        return;
    }
    switch (this->encoding)
    {
    case AV1:
        memcpy(str, "AV1", sizeof("AV1"));
        sz = sizeof("AV1");
        break;
    case H264:
        memcpy(str, "H.264", sizeof("H.264"));
        sz = sizeof("H.264");
        break;
    case H265:
        memcpy(str, "H.265", sizeof("H.265"));
        sz = sizeof("H.265");
        break;
    case RAW:
        memcpy(str, "RAW", sizeof("RAW"));
        sz = sizeof("RAW");
        break;
    default:
        memcpy(str, "NONE", sizeof("NONE"));
        sz = sizeof("NONE");
        break;
    }
}

void VideoData::setEncoding(char *str, int sz)
{
    if (strcmp(str, "AV1") == 0)
        this->encoding = AV1;
    else if (strcmp(str, "H.264") == 0)
        this->encoding = H264;
    else if (strcmp(str, "H.265") == 0)
        this->encoding = H265;
    else if (strcmp(str, "RAW") == 0)
        this->encoding = RAW;
    else
        this->encoding = NONE;
}