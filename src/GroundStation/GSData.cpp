#include "GSData.h"

GSData::GSData(uint8_t streamType, uint8_t streamId) : dataType(streamType), id(streamId) {}

GSData::GSData(uint8_t streamType, uint8_t streamId, uint8_t *buf, uint16_t size) : dataType(streamType), id(streamId)
{
    this->fill(buf, size);
}

bool GSData::decodeHeader(uint32_t header, uint8_t &streamType, uint8_t &streamIndex, uint16_t &size)
{
    // header will be TISSss so >> 20 to get 0x0T
    streamType = header >> 20;
    // >> 16 to get 0xTI and & 0xF to get 0x0I
    streamIndex = (header >> 16) & 0x0F;
    // then & 0xFFFF to get SSss
    size = header & 0xFFFF;
    return true;
}

bool GSData::decodeHeader(uint8_t *header, uint8_t &streamType, uint8_t &streamIndex, uint16_t &size)
{
    // header will be TISSss so 0 is TI, so >> 4 to get 0x0T
    streamType = header[0] >> 4;
    // and & 0x0F to get 0x0I
    streamIndex = header[0] & 0x0F;
    // then SSss is in 1 and 2, so need to << 8 header[1] to make it 0xSS00, then add header[2]
    size = (header[1] << 8) + header[2];
    return true;
}

uint16_t GSData::encode(uint8_t *data, uint16_t size)
{
    uint16_t pos = 0;
    if (size < pos + 3)
        return 0; // error data too small for header

    // header
    // TISSss
    // T = type
    // I = index
    // Ss = size
    // type will be 0x0T so << 4 to make it 0xT0
    // index will be 0x0I so just add
    // then add to get 0xLN
    data[pos++] = (this->dataType << 4) + (this->id);
    // size will be 0xSSss so >> 8 to get 0x00SS
    data[pos++] = this->size >> 8;
    // size will be 0xSSss so & 0x00FF to get 0x00ss
    data[pos++] = this->size & 0x00FF;

    if (size < pos + this->size)
        return 0; // not enough room for data

    // body
    memcpy(data + pos, this->buf, this->size);

    return pos + this->size;
}

uint16_t GSData::decode(uint8_t *data, uint16_t size)
{
    uint16_t pos = 0;
    if (size > this->maxSize)
        return 0; // error data too big
    if (size < pos + 3)
        return 0; // error data too small for header

    // header
    // TISSss
    // T = type
    // I = index
    // Ss = size
    // 0 is 0xTI, so >> 4 to get 0x0T
    this->dataType = data[pos] >> 4;
    // and & 0x0F to get 0x0I
    this->id = data[pos++] & 0x0F;
    // then SSss is in 1 and 2, so need to << 8 header[1] to make it 0xSS00, then add header[2]
    this->size = (data[pos++] << 8);
    this->size += data[pos];

    if (size < pos + 1)
        return 0; // no data available

    pos++;

    // body
    memcpy(this->buf, data + pos, size - pos);
    return size;
}

GSData *GSData::fill(uint8_t *buf, uint16_t size)
{
    // check if size is too big
    if (size <= this->maxDataSize)
    {
        // if it is fine fill the buffer
        this->size = size;
        memcpy(this->buf, buf, size);
    }
    else
    {
        // otherwise only copy dataMaxSize bytes
        this->size = this->maxDataSize;
        memcpy(this->buf, buf, this->maxDataSize);
    }
    return this;
}

uint16_t GSData::toJSON(char *json, uint16_t sz, int deviceId)
{
    uint16_t result = (uint16_t)snprintf(json, sz, "{\"type\":\"GSData\",\"deviceId\":%d,\"data\":{\"id\":%d,\"buf\":[", deviceId, this->id);
    if (result >= sz)
    {
        // output too large
        return 0;
    }

    // result should be the index of the \0

    // need to dynamically adjust the number of bytes added based on size
    for (int i = 0; i < this->size; i++)
    {
        if (result + 4 < sz)
        {
            int added = sprintf(json + result, "%d,", this->buf[i]);
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

uint16_t GSData::fromJSON(char *json, uint16_t sz, int &deviceId)
{
    // strings to store data in
    char deviceIdStr[5] = {0};
    char idTxt[10] = {0};

    // extract each string
    if (!extractStr(json, sz, "\"deviceId\":", ',', deviceIdStr))
        return 0;
    if (!extractStr(json, sz, "\"index\":", ',', idTxt, 3))
        return 0;

    // convert to correct data type
    deviceId = atoi(deviceIdStr);
    this->id = atoi(idTxt);

    // need to manually extract buf (instead of using extractStr) since it is an array
    char *dataStrPos = strstr(json, "\"buf\":[");
    int current = int(dataStrPos - json) + strlen("\"buf\":[");
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
        this->buf[this->size] = atoi(dataByte);
        this->size++;
        current++;
    }
    return this->size;
}