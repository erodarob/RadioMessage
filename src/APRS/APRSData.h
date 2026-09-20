#ifndef APRSDATA_H
#define APRSDATA_H

#include "../Data.h"

/**
 * APRS configuration for:
 * - Callsign
 * - Tocall
 * - Path
 * - Type
 * - Symbol
 * - Overlay
 */
struct APRSConfig
{
    /** the callsign of the person sending the message */
    char callsign[8];
    /** the callsign of the person the message is being sent to */
    char tocall[8];
    /** indicates how the message should be propagated through the APRS network */
    char path[10];
    /** the type of the message, see APRSMessageType (in APRSData.h) */
    char type;
    /** indicates where the message is coming from */
    char symbol;
    /** provides greater specificity about where the message is coming from */
    char overlay;
};

/**
 * APRS message types
 * Characters used to represent the type of APRS message
 * For more details see the APRS documentation https://www.aprs.org/doc/APRS101.PDF
 */
enum APRSMessageType : char
{
    PositionWithoutTimestampWithAPRS = '=',
    PositionWithoutTimestampWithoutAPRS = '!',
    PositionWithTimestampWithAPRS = '@',
    PositionWithTimestampWithoutAPRS = '/',
    Status = '>',
    Query = '?',
    TextMessage = ':',
    Weather = '_',
    Telemetry = 'T',
    CurrentMicEData = '`',
};

class APRSData : public Data
{
public:
    // Note: Since APRSData is an abstract class, it doesn't get it's own ERR_ID
    // Instead, everything that inherits from APRSData should offset its own errors
    // by 10 (i.e. starting at -X10) to allow errors 1-9 to be used by APRSData.
    // APRSData function that return error codes will return the the local error
    // code, which should be added to the child's ERR_ID to get the global error code.
    // static const int ERR_ID = 0;

    /**
     * The configuration for this message.
     * Note: modified when message is decoded
     */
    APRSConfig config;

    /**
     * APRSData default constructor
     */
    APRSData() : config((APRSConfig){"",
                                     "",
                                     "",
                                     '\0',
                                     '\0'}) {};
    // For some reason intellisense says there's an error above, but it compiles so ¯\_(ツ)_/¯

    /**
     * APRSData constructor
     * @param cfg the APRS config to use
     */
    APRSData(APRSConfig cfg) : config(cfg) {};

    /**
     * Encodes the APRS header into the given buffer
     * @param data a pointer the buffer to encode the header to
     * @param sz the maximum length of the buffer
     * @param pos a reference to the current position in the data array, will be incremented to the end of the encoded header
     * @return the position in the buffer where the header ends if successful, otherwise and error code less than 0
     */
    int encodeHeader(uint8_t *data, uint16_t sz, uint16_t &pos);
    /**
     * Decodes the APRS header from the given buffer
     * @param data a pointer the buffer to decode the header from
     * @param sz the maximum length of the buffer
     * @param pos a reference to the current position in the data array, will be incremented to the end of the decoded header
     * @return the position in the buffer where the header ends if successful, otherwise and error code less than 0
     */
    int decodeHeader(uint8_t *data, uint16_t sz, uint16_t &pos);

    /**
     * Convert a number to a base 91 number represented by ascii characters
     * @param str the string to add the number to, must be long enough to hold ```precision``` more characters
     * @param pos the position to start adding the number to, will be incremented to the end of the encoded number
     * @param val the value of the number to add
     * @param precision the number of base91 digits to use
     */
    static void numtoBase91(uint8_t *str, uint16_t &pos, uint32_t val, int precision);
    /**
     * Convert a base 91 number represented by ascii characters to a number
     * @param str the string to take the number from, must contain at least ```precision``` more characters
     * @param pos the position to start taking the number from, will be incremented to the end of the decoded number
     * @param val will be set to the value of the number
     * @param precision the number of base91 digits to use
     */
    static void base91toNum(uint8_t *str, uint16_t &pos, uint32_t &val, int precision);
};

#endif