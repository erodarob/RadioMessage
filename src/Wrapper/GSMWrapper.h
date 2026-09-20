#ifndef GSMWRAPPER_H
#define GSMWRAPPER_H

#include "../Wrapper.h"
#include "../Types/PackedNum.h"

class GSMWrapper : public Wrapper
{
public:
    /** the header for Ground Station Muxer (gsm) files, format: (version)GroundStationMuxer(bitrate)\n */
    static constexpr char staticGSMHeader[] = "\1GroundStationMuxer";
    /** the total size of the gsm header */
    static const int gsmHeaderSize = sizeof(staticGSMHeader) - 1 + sizeof(uint32_t); // -1 for null terminator

    /** the average bitrate of the data in the file, in bit/s */
    uint32_t bitrate = 0;

    /**
     * GSMWrapper default constructor
     */
    GSMWrapper() {};

    /**
     * GSMWrapper constructor
     * @param bitrate the average bitrate of the data in the file bit/s
     */
    GSMWrapper(uint32_t bitrate) : bitrate(bitrate) {};

    /**
     * Wrap the Message using data from this Wrapper
     * @param prependPos the start of the prepended data for this Wrapper in the Message buffer
     * @param appendPos the end of the appended data for this Wrapper in the Message buffer
     * @return the result of the wrapping operation, usually the length of bytes added, always greater than 0 if successful
     */
    int wrap(uint8_t *prependPos, uint8_t *appendPos) override;
    /**
     * Unwrap the Message putting data into this Wrapper
     * @param prependPos the start of the prepended data for this Wrapper in the Message buffer
     * @param appendPos the end of the appended data for this Wrapper in the Message buffer
     * @return the result of the unwrapping operation, usually the length of bytes added, always greater than 0 if successful
     */
    int unwrap(uint8_t *prependPos, uint8_t *appendPos) override;

    /**
     * Get the length of the data this Wrapper prepends to the Message
     * @return the length of the prepended data
     */
    uint16_t prependLen() override { return GSMWrapper::gsmHeaderSize; };
    /**
     * Get the length of the data this Wrapper appends to the Message
     * @return the length of the appended data
     */
    uint16_t appendLen() override { return 0; };
};

#endif