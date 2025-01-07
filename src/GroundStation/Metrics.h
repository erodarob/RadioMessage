#ifndef METRICS_H
#define METRICS_H

#include "../Data.h"
#include "../Types/PackedNum.h"

class Metrics : public Data
{
public:
    // Metrics type
    static const uint8_t type = 0x05;
    // the total length in bytes of the encoded data
    static const uint8_t dataLen = 1 + 4 + 2;

    // the id of the hardware the Metrics belong to
    uint8_t deviceId = 0; // max 0xF;
    // the current bitrate of the hardware
    uint32_t bitrate = 0; // bits/sec
    // the current rssi (assuming the hardware is a radio)
    short rssi = 0;

    // the time when setInitialTime() was called
    uint64_t initialTime = 0; // ms
    // the last time bitrate was calculated
    uint64_t lastTime = 0; // ms
    // bits saved in case update is called faster than timer resolution
    int savedBits = 0;
    // an average of the bitrate since setInitialTime() was called
    uint32_t averageBitrate = 0; // bits/sec
    // the last few data points for bits received
    uint8_t lastBits[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    // the last few data points for change in time
    uint8_t lastdeltaT[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    // pointer to the data point to replace next
    uint8_t lastPointer = 0;

    // Metrics default constructor
    Metrics() {};

    // Metrics constructor
    // - deviceId: the id for the hardware
    Metrics(int deviceId);

    // Metrics constructor
    // - deviceId: the id for the hardware
    // - bitrate: the current bitrate
    // - rssi: the current rssi
    Metrics(int deviceId, int bitrate, int rssi);

    // set the initial time for calculating bitrate, must be set for first calculation to be valid
    // - time: the time to use
    void setInitialTime(uint64_t time);
    // update the internal bitrate taking into account elapsed time
    // - bits: the number of bits received since last time updateBitrate() was called
    // - currentTime: the current time
    void updateBitrate(uint32_t bits, uint64_t currentTime);
    // update the internal metrics all in one function call
    // - bits: the number of bits received since last time updateBitrate() was called
    // - currentTime: the current time
    // - rssi: the current rssi of the receiver
    void update(uint32_t bits, uint64_t currentTime, short rssi);

    // encode the data stored in the ```Data``` object and place the result in ```data```
    uint16_t encode(uint8_t *data, uint16_t sz) override;
    // decode the data stored in ```data``` and place it in the ```Data``` object
    uint16_t decode(uint8_t *data, uint16_t sz) override;

    // place the data in the ```Data``` object in the ```json``` string, ```sz``` is the max size of the string, ```deviceId``` can be set based on hardware
    uint16_t toJSON(char *json, uint16_t sz, int deviceId) override;
    // place the data in the ```json``` string in the ```Data``` object, ```sz``` is the max size of the string, ```deviceId``` can be set based on hardware
    uint16_t fromJSON(char *json, uint16_t sz, int &deviceId) override;
};

#endif