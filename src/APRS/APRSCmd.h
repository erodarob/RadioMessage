#ifndef APRSCMD_H
#define APRSCMD_H

#include "APRSData.h"
#include "../Types/PackedNum.h"

// Technically this is not a valid APRS message as in it shouldn't be picked up by APRS repeaters,
// but we can still use the APRS header structure since we need to transmit our callsign anyway
class APRSCmd : public APRSData
{
public:
    /** APRSCmd type */
    static const uint8_t type = 0x03;
    /** APRSCmd error ID */
    static const int ERR_ID = -type * 100;

    /** the command to send, 0x00 should always be NOP */
    uint8_t cmd = 0x00;
    /** args for cmd */
    PackedNum args = (uint16_t)0x0000;

    /**
     * APRSCmd default constructor
     */
    APRSCmd() : APRSData() {};

    /**
     * APRSCmd constructor
     * @param config the APRS config to use
     */
    APRSCmd(APRSConfig config);
    /**
     * APRSCmd constructor
     * @param config the APRS config to use
     * @param cmd the command to send
     * @param args the arguments for the command
     */
    APRSCmd(APRSConfig config, uint8_t cmd, uint16_t args);
    /**
     * APRSCmd constructor
     * @param config the APRS config to use
     * @param cmd the command to send
     * @param args the arguments for the command
     * @param encoding the encoding for the args PackedNum
     * @param length the length of the encoding
     */
    APRSCmd(APRSConfig config, uint8_t cmd, uint64_t *args, uint8_t *encoding, uint8_t length);

    /**
     * Extract the individual args from the args PackedNum
     * @param args the array to extract the args into, must be as long as the encoding
     * @return whether the args were successfully extracted
     */
    bool getArgs(uint64_t *args);

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