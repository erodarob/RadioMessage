#ifndef APRSTEXT_H
#define APRSTEXT_H

#include "APRSData.h"

class APRSText : public APRSData
{
public:
    /** APRSText type */
    static const uint8_t type = 0x05;
    /** APRSText error ID */
    static const int ERR_ID = -type * 100;
    /** the maximum length of the message portion */
    static const int maxMsgLen = 67;
    /** the maximum length of the addressee portion */
    static const int maxAddrLen = 9;

    /** the message buffer, 67 message length + 1 for \0 */
    char msg[maxMsgLen + 1] = {0};
    /** the addressee buffer, 9 addressee length + 1 for \0 */
    char addressee[maxAddrLen + 1] = {0};
    /** the length of the message buffer */
    int msgLen = 0;
    /** the length of the addressee buffer */
    int addrLen = 0;

    /**
     * APRSText default constructor
     */
    APRSText() : APRSData() {};

    /**
     * APRSText constructor
     * @param config the APRS config to use
     */
    APRSText(APRSConfig config);
    /**
     * APRSText constructor
     * @param config the APRS config to use
     * @param msg the message portion, expected to be a valid c string, must be of length 67
     * @param addressee the addressee portion (callsign), expected to be a valid c string, must be of length 9
     */
    APRSText(APRSConfig config, char msg[67], char addressee[9]);

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