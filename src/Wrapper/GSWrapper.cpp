#include "GSWrapper.h"

void GSWrapper::setMetadata(uint8_t streamType, uint8_t streamId)
{
    this->dataType = streamType;
    this->id = streamId;
}

void GSWrapper::getMetadata(uint8_t &streamType, uint8_t &streamId)
{
    streamType = this->dataType;
    streamId = this->id;
}

int GSWrapper::wrap(uint8_t *prependPos, uint8_t *appendPos)
{
    // pointers point to the outer edges of this wrapper, so subtract the length of this wrapper
    this->msgSize = appendPos - prependPos;
    // header
    // TISSss
    // T = type (4 bits)
    // I = id (4 bits)
    // SS = size (first 8 bits)
    // ss = size (last 8 bites)
    uint64_t headerData[] = {this->dataType, this->id, this->msgSize};
    header.pack(headerData);
    // place header in data
    // start at the beginning of the message, which means we need to remove the headerLen
    header.get(prependPos);

    return 1;
}

int GSWrapper::unwrap(uint8_t *prependPos, uint8_t *appendPos)
{

    // header
    // TISSss
    // T = type (4 bits)
    // I = id (4 bits)
    // SS = size (first 8 bits)
    // ss = size (last 8 bites)
    this->header.set(prependPos);
    // unpack the data
    uint64_t headerData[GSWrapper::headerEncodingLength] = {0};
    bool success = this->header.unpack(headerData);

    if (success)
    {
        // set all variables according to where data is stored
        this->dataType = headerData[0];
        this->id = headerData[1];
        this->msgSize = headerData[2];
        return 1;
    }
    // decodeHeader() sets: this->dataType, this->id, this->size

    return 0;
}

bool GSWrapper::messageComplete(Message *m)
{
    return this->msgSize > 0 && m->size == this->msgSize;
}