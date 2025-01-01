#include "VideoData.h"

VideoData::VideoData(uint8_t *data)
{
    // copy maxSize bytes from data into internal buffer
    memcpy(this->data, data, this->maxSize);
    this->size = this->maxSize;
}

VideoData::VideoData(uint8_t *data, uint16_t sz)
{
    // make sure size is not too big
    if (sz > this->maxSize)
        this->size = this->maxSize;
    else
        this->size = sz;
    // copy size bytes into internal buffer
    memcpy(this->data, data, this->size);
}

uint16_t VideoData::encode(uint8_t *data, uint16_t sz)
{
    // make sure the provided array size is long enough
    if (sz >= this->maxSize)
    {
        // copy the data
        memcpy(data, this->data, this->maxSize);
        return this->maxSize;
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
    // copy data into internal buffer
    memcpy(this->data, data, sz);
    return sz;
}

uint16_t VideoData::toJSON(char *json, uint16_t sz, int deviceId)
{
    uint16_t result = (uint16_t)snprintf(json, sz, "{\"type\":\"VideoData\",\"deviceId\":%d,\"data\":{\"data\":[", deviceId);

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

    // extract each string
    if (!extractStr(json, sz, "\"deviceId\":", ',', deviceIdStr))
        return 0;

    // convert to correct data type
    deviceId = atoi(deviceIdStr);

    // need to manually extract data (instead of using extractStr) since it is an array
    char *dataStrPos = strstr(json, "{\"data\":[");
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