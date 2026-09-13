#include "GSMWrapper.h"

int GSMWrapper::wrap(uint8_t *prependPos, uint8_t *appendPos)
{

    // add static header
    memcpy(prependPos, GSMWrapper::staticGSMHeader, sizeof(GSMWrapper::staticGSMHeader) - 1);

    // convenient way of putting raw bytes in data for bitrate
    PackedNum p(this->bitrate);
    p.get(prependPos + sizeof(GSMWrapper::staticGSMHeader) - 1);

    return GSMWrapper::gsmHeaderSize;
}

int GSMWrapper::unwrap(uint8_t *prependPos, uint8_t *appendPos)
{
    // check if static header string matches
    if (memcmp(prependPos, GSMWrapper::staticGSMHeader, sizeof(GSMWrapper::staticGSMHeader) - 1) == 0)
    {

        // convenient way of extracting raw bytes into uint32
        PackedNum p(prependPos + sizeof(GSMWrapper::staticGSMHeader) - 1, sizeof(uint32_t));
        this->bitrate = p.get();

        return 1;
    }
    return 0; // Error: header does not match
}