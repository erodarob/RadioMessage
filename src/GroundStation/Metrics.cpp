#include "Metrics.h"

Metrics::Metrics(int deviceId) : deviceId(deviceId) {}

Metrics::Metrics(int deviceId, int bitrate, int rssi) : deviceId(deviceId), bitrate(bitrate), rssi(rssi) {}

void Metrics::setInitialTime(uint64_t time)
{
    this->initialTime = time;
    this->lastTime = time;
    this->averageBitrate = 0;
}

void Metrics::updateBitrate(uint32_t bits, uint64_t currentTime)
{
    uint64_t deltaT = currentTime - this->lastTime;

    // avoid division by 0
    if (deltaT > 0)
    {
        this->bitrate = (bits + this->savedBits) * 1000 / deltaT; // need to convert deltaT to seconds
        this->savedBits = 0;

        // running average
        this->averageBitrate = ((uint64_t)this->averageBitrate * ((double)(this->lastTime - this->initialTime) / 1000) + (uint64_t)this->bitrate * ((double)deltaT / 1000)) / ((double)(currentTime - this->initialTime) / 1000);

        this->lastTime = currentTime;
    }
    else
    {
        // updated is being called faster than clock resolution
        this->savedBits += bits;
    }
}

void Metrics::update(uint32_t bits, uint64_t currentTime, short rssi)
{
    this->updateBitrate(bits, currentTime);
    this->rssi = rssi;
}

uint16_t Metrics::encode(uint8_t *data, uint16_t sz)
{
    uint16_t pos = 0;
    if (sz < pos + Metrics::dataLen)
        return 0; // error data too small for data

    // pretend rssi is an unsigned int to make sure we can work with it nicely
    // TODO: is this really necessary
    uint16_t rssiUnsigned = reinterpret_cast<uint16_t &>(this->rssi);

    data[pos++] = deviceId;
    data[pos++] = (this->bitrate & 0xff000000) >> 24;
    data[pos++] = (this->bitrate & 0x00ff0000) >> 16;
    data[pos++] = (this->bitrate & 0x0000ff00) >> 8;
    data[pos++] = (this->bitrate & 0x000000ff) >> 0;
    data[pos++] = (rssiUnsigned & 0xff00) >> 8;
    data[pos++] = (rssiUnsigned & 0x00ff) >> 0;

    return pos;
}

uint16_t Metrics::decode(uint8_t *data, uint16_t sz)
{
    uint16_t pos = 0;
    if (sz < pos + Metrics::dataLen)
        return 0; // error data too small for data

    // pretend rssi is an unsigned int to make sure we can work with it nicely using +=
    uint16_t rssiUnsigned = 0;

    this->deviceId = data[pos++];
    this->bitrate += data[pos++] << 24;
    this->bitrate += data[pos++] << 16;
    this->bitrate += data[pos++] << 8;
    this->bitrate += data[pos++] << 0;
    rssiUnsigned += data[pos++] << 8;
    rssiUnsigned += data[pos++] << 0;

    // get the actual signed values
    this->rssi = reinterpret_cast<short &>(rssiUnsigned);

    return pos;
}

uint16_t Metrics::toJSON(char *json, uint16_t sz, int deviceId)
{
    uint16_t result = (uint16_t)snprintf(json, sz, "{\"type\":\"Metrics\",\"deviceId\":%d,\"data\":{\"deviceId\":%d,\"bitrate\":%d,\"rssi\":%d}}", deviceId, this->deviceId, this->bitrate, this->rssi);

    if (result < sz)
    {
        // ran properly
        return result;
    }
    // output too large
    return 0;
}

uint16_t Metrics::fromJSON(char *json, uint16_t sz, int &deviceId)
{
    // strings to store data in
    char deviceIdStr1[5] = {0};
    char deviceIdStr2[5] = {0};
    char bitrateStr[12] = {0};
    char rssiStr[7] = {0};
    // extract each string
    if (!extractStr(json, sz, "\"deviceId\":", ',', deviceIdStr1, sizeof(deviceIdStr1)))
        return 0;
    if (!extractStr(json, sz, "\"data\":{\"deviceId\":", ',', deviceIdStr2, sizeof(deviceIdStr2)))
        return 0;
    if (!extractStr(json, sz, "\"bitrate\":", ',', bitrateStr, sizeof(bitrateStr)))
        return 0;
    if (!extractStr(json, sz, "\"rssi\":", ',', rssiStr, sizeof(rssiStr)))
        return 0;

    // convert to correct data type
    deviceId = atoi(deviceIdStr1);
    this->deviceId = atoi(deviceIdStr2) & 0xF;
    this->bitrate = atoi(bitrateStr);
    this->rssi = atoi(rssiStr);

    return 1;
}