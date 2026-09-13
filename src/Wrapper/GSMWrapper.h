#ifndef GSMWRAPPER_H
#define GSMWRAPPER_H

#include "../Wrapper.h"
#include "../Types/PackedNum.h"

class GSMWrapper : public Wrapper
{
public:
    // the header for Ground Station Muxer (gsm) files, format: (version)GroundStationMuxer(bitrate)\n
    static constexpr char staticGSMHeader[] = "\1GroundStationMuxer";
    // the total size of the gsm header
    static const int gsmHeaderSize = sizeof(staticGSMHeader) - 1 + sizeof(uint32_t); // -1 for null terminator

    // the average bitrate of the data in the file, in bit/s
    uint32_t bitrate = 0;

    // GSMWrapper default constructor
    GSMWrapper() {};
    // GSMWrapper constructor
    // - bitrate: the average bitrate of the data in the file bit/s
    GSMWrapper(uint32_t bitrate) : bitrate(bitrate) {};

    // wrap the Message using data from this Wrapper, with the prepended data starting at ```prependPos``` and the appeneded data starting at ```appendPos```
    int wrap(uint8_t *prependPos, uint8_t *appendPos) override;
    // unwrap the Message putting data into this Wrapper, with the prepended data starting at ```prependPos``` and the appeneded data starting at ```appendPos```
    int unwrap(uint8_t *prependPos, uint8_t *appendPos) override;

    // get the length of the data this Wrapper prepends to the Message
    uint16_t prependLen() override { return GSMWrapper::gsmHeaderSize; };
    // get the length of the data this Wrapper appends to the Message
    uint16_t appendLen() override { return 0; };
};

#endif