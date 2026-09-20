#ifndef METRICS_H
#define METRICS_H

#include "../Data.h"
#include "../Types/PackedNum.h"

class Metrics : public Data
{
public:
    /** Metrics type */
    static const uint8_t type = 0x02;
    /** Metrics error ID */
    static const int ERR_ID = -type * 100;
    /** the total length in bytes of the encoded data */
    static const uint8_t dataLen = 1 + 4 + 2;

    /** the id of the hardware the Metrics belong to (max 0xF) */
    uint8_t deviceId = 0;
    /** the current bitrate of the hardware (bits/s) */
    uint32_t bitrate = 0; // bits/sec
    /** the current rssi (assuming the hardware is a radio) */
    short rssi = 0;

    /** the time when setInitialTime() was called in ms */
    uint64_t initialTime = 0;
    /** the last time bitrate was calculated in ms */
    uint64_t lastTime = 0;
    /** bits saved in case update is called faster than timer resolution */
    int savedBits = 0;
    /** an average of the bitrate since setInitialTime() was called in bits/s */
    uint32_t averageBitrate = 0;
    /** the last few data points for bits received in bits */
    uint8_t lastBits[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    /** the last few data points for change in time in ms */
    uint8_t lastdeltaT[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    /** pointer to the data point to replace next in the lastBits and lastdeltaT arrays */
    uint8_t lastPointer = 0;

    /**
     * Metrics default constructor
     */
    Metrics() {};

    /**
     * Metrics constructor
     * @param deviceId the id for the hardware
     */
    Metrics(int deviceId);
    /**
     * Metrics constructor
     * @param deviceId the id for the hardware
     * @param bitrate the current bitrate in bits/s
     * @param rssi the current RSSI
     */
    Metrics(int deviceId, int bitrate, int rssi);

    /**
     * Set the initial time for calculating bitrate, must be set for first calculation to be valid
     * @param time the time to use
     */
    void setInitialTime(uint64_t time);
    /**
     * Update the internal bitrate taking into account elapsed time
     * @param bits the number of bits received since last time updateBitrate() was called
     * @param currentTime the current time in ms
     */
    void updateBitrate(uint32_t bits, uint64_t currentTime);
    /**
     * Update the internal metrics all in one function call
     * @param bits the number of bits received since last time updateBitrate() was called
     * @param currentTime the current time in ms
     * @param rssi the current RSSI of the receiver
     */
    void update(uint32_t bits, uint64_t currentTime, short rssi);

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