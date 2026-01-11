#include "GenericData.h"

GenericData::GenericData(uint8_t *data)
{
    // copy maxSize bytes from data into internal buffer
    memcpy(this->data, data, this->maxSize);
    this->size = this->maxSize;
}

GenericData::GenericData(uint8_t *data, uint16_t sz)
{
    // make sure size is not too big
    if (sz > this->maxSize)
        this->size = this->maxSize;
    else
        this->size = sz;
    // copy size bytes into internal buffer
    memcpy(this->data, data, this->size);
}

int GenericData::encode(uint8_t *data, uint16_t sz)
{
    // make sure the provided array size is long enough
    if (sz >= this->size)
    {
        // copy the data
        memcpy(data, this->data, this->size);
        return this->size;
    }
    return GenericData::ERR_ID - 1;
}

int GenericData::decode(uint8_t *data, uint16_t sz)
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

int GenericData::toJSON(char *json, uint16_t sz, int deviceId)
{
    uint16_t result = (uint16_t)snprintf(json, sz, "{\"type\":\"GenericData\",\"deviceId\":%d,\"data\":{\"data\":[", deviceId);

    if (result >= sz)
    {
        // output too large
        return GenericData::ERR_ID - 2;
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
                return GenericData::ERR_ID - 3; // output too large
        }
        else
            return GenericData::ERR_ID - 4; // output too large
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
    return GenericData::ERR_ID - 5;
}

int GenericData::fromJSON(char *json, uint16_t sz, int &deviceId)
{
    // strings to store data in
    char deviceIdStr[5] = {0};

    // extract each string
    if (!extractStr(json, sz, "\"deviceId\":", ',', deviceIdStr))
        return GenericData::ERR_ID - 6;

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
    return sz;
}