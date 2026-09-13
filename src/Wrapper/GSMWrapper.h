#ifndef GSMWRAPPER_H
#define GSMWRAPPER_H

#include "../Wrapper.h"
#include "../Types/PackedNum.h"

// TODO: test
class GSMWrapper : public Wrapper
{
public:
    // the header for Ground Station Muxer (gsm) files, format: (version)GroundStationMuxer(bitrate)\n
    static constexpr char staticGSMHeader[] = "\1GroundStationMuxer";
    // the total size of the gsm header
    static const int gsmHeaderSize = sizeof(staticGSMHeader) - 1 + sizeof(uint32_t); // -1 for null terminator

    // the average bitrate of the data in the file, in bit/s
    uint32_t bitrate = 0;

    // GSMHeader default constructor
    GSMWrapper() {};
    // GSMHeader constructor
    // - bitrate: the average bitrate of the data in the file bit/s
    GSMWrapper(uint32_t bitrate) : bitrate(bitrate) {};

    // encoded GSM header into ```header``` with max length ```length```, with data bitrate  ```bitrate```
    // returns whether encoding was successful
    int wrap(uint8_t *prependPos, uint8_t *appendPos) override;
    // decode Ground Station Multiplexer (GSM) header from ```header``` with length ```length```, with data bitrate placed into ```bitrate```
    // returns whether decoding was successful
    int unwrap(uint8_t *prependPos, uint8_t *appendPos) override;

    uint16_t prependLen() { return GSMWrapper::gsmHeaderSize; };
    uint16_t appendLen() { return 0; };
};

#endif