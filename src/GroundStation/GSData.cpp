#include "GSData.h"

GSData::GSData(uint8_t streamType, uint8_t streamId, uint8_t deviceId) : dataType(streamType), id(streamId), deviceId(deviceId) {}

GSData::GSData(uint8_t streamType, uint8_t streamId, uint8_t deviceId, uint8_t *buf, uint16_t size) : dataType(streamType), id(streamId), deviceId(deviceId)
{
    this->fill(buf, size);
}

bool GSData::decodeGSMHeader(char *header, int length, uint32_t &bitrate)
{
    if (length < (int)sizeof(staticGSMHeader) - 1)
        return false; // Error: header data not big enough
    int findHeader = memcmp(header, GSData::staticGSMHeader, sizeof(staticGSMHeader) - 1);
    if (findHeader == 0)
    {
        if (length - (sizeof(staticGSMHeader) - 1) >= sizeof(uint32_t))
        {
            uint8_t bitrateData[4] = {0, 0, 0, 0};
            memcpy(bitrateData, header + sizeof(staticGSMHeader) - 1, sizeof(uint32_t));
            bitrate += bitrateData[0] << 24;
            bitrate += bitrateData[1] << 16;
            bitrate += bitrateData[2] << 8;
            bitrate += bitrateData[3] << 0;
            return true;
        } // else header is corrupted (too few bytes given)
    }
    return false;
}

bool GSData::encodeGSMHeader(char *header, int length, uint32_t bitrate)
{
    if (length < GSData::gsmHeaderSize)
        return false;
    memcpy(header, staticGSMHeader, sizeof(staticGSMHeader) - 1);
    uint8_t bitrateData[4] = {0, 0, 0, 0};
    bitrateData[0] = (bitrate & 0xff000000) >> 24;
    bitrateData[1] = (bitrate & 0x00ff0000) >> 16;
    bitrateData[2] = (bitrate & 0x0000ff00) >> 8;
    bitrateData[3] = (bitrate & 0x000000ff) >> 0;
    memcpy(header + sizeof(staticGSMHeader) - 1, bitrateData, sizeof(bitrateData));
    return true;
}

bool GSData::decodeHeader(uint32_t header, uint8_t &streamType, uint8_t &streamId, uint8_t &deviceId, uint16_t &size)
{
    // header
    // TIDSss
    // T = type
    // I = id
    // D = deviceId
    // S = size first 4
    // ss = size last 8
    // create a PackedNum to decode the header
    PackedNum packedHeader(GSData::headerEncoding, GSData::headerEncodingLength);
    packedHeader.set(header);
    // unpack the data
    uint8_t headerData[GSData::headerEncodingLength] = {0};
    bool success = packedHeader.unpack(headerData);

    if (success)
    {
        // set all variables according to where data is stored
        streamType = headerData[0];
        streamId = headerData[1];
        deviceId = headerData[2];
        size = headerData[3] << 8;
        size += headerData[4];
    }
    return success;
}

bool GSData::decodeHeader(uint8_t *header, uint8_t &streamType, uint8_t &streamId, uint8_t &deviceId, uint16_t &size)
{
    // header
    // TIDSss
    // T = type
    // I = id
    // D = deviceId
    // S = size first 4
    // ss = size last 8
    // create a PackedNum to decode the header
    PackedNum packedHeader(GSData::headerEncoding, GSData::headerEncodingLength);
    packedHeader.set(header);
    // unpack the data
    uint8_t headerData[GSData::headerEncodingLength] = {0};
    bool success = packedHeader.unpack(headerData);

    if (success)
    {
        // set all variables according to where data is stored
        streamType = headerData[0];
        streamId = headerData[1];
        deviceId = headerData[2];
        size = headerData[3] << 8;
        size += headerData[4];
    }
    return true;
}

bool GSData::decodeHeader(uint32_t header)
{
    // header
    // TIDSss
    // T = type
    // I = id
    // D = deviceId
    // S = size first 4
    // ss = size last 8
    this->header.set(header);
    // unpack the data
    uint8_t headerData[GSData::headerEncodingLength] = {0};
    bool success = this->header.unpack(headerData);

    if (success)
    {
        // set all variables according to where data is stored
        this->dataType = headerData[0];
        this->id = headerData[1];
        this->deviceId = headerData[2];
        this->size = headerData[3] << 8;
        this->size += headerData[4];
    }
    return success;
}

bool GSData::decodeHeader(uint8_t *header)
{
    // header
    // TIDSss
    // T = type
    // I = id
    // D = deviceId
    // S = size first 4
    // ss = size last 8
    this->header.set(header);
    // unpack the data
    uint8_t headerData[GSData::headerEncodingLength] = {0};
    bool success = this->header.unpack(headerData);

    if (success)
    {
        // set all variables according to where data is stored
        this->dataType = headerData[0];
        this->id = headerData[1];
        this->deviceId = headerData[2];
        this->size = headerData[3] << 8;
        this->size += headerData[4];
    }
    return success;
}

uint16_t GSData::encode(uint8_t *data, uint16_t sz)
{
    uint16_t pos = 0;
    if (sz < pos + GSData::headerLen)
        return 0; // error data too small for header

    // header
    // TIDSss
    // T = type
    // I = id
    // D = deviceId
    // S = size first 4
    // ss = size last 8
    uint8_t headerData[] = {this->dataType, this->id, this->deviceId, (uint8_t)(this->size >> 8), (uint8_t)(this->size & 0xFF)};
    header.pack(headerData);
    // place header in data
    header.get(data);

    pos += GSData::headerLen;

    if (sz < pos + this->size)
        return 0; // not enough room for data

    // body
    memcpy(data + pos, this->buf, this->size);

    return pos + this->size;
}

uint16_t GSData::decode(uint8_t *data, uint16_t sz)
{
    uint16_t pos = 0;
    if (sz > GSData::maxSize)
        return 0; // error data too big
    if (sz < pos + GSData::headerLen)
        return 0; // error data too small for header

    // header
    // TIDSss
    // T = type
    // I = id
    // D = deviceId
    // S = size first 4
    // ss = size last 8
    this->decodeHeader(data);

    pos += GSData::headerLen;

    if (sz < pos + 1)
        return 0; // no data available

    // body
    memcpy(this->buf, data + pos, this->size);
    return pos + this->size;
}

GSData *GSData::fill(uint8_t *buf, uint16_t size)
{
    // check if size is too big
    if (size <= GSData::maxSize)
    {
        // if it is fine fill the buffer
        this->size = size;
        memcpy(this->buf, buf, size);
    }
    else
    {
        // otherwise only copy maxSize bytes
        this->size = GSData::maxSize;
        memcpy(this->buf, buf, GSData::maxSize);
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