#include "GSMessage.h"

constexpr uint8_t GSMessage::headerEncoding[];
constexpr char GSMessage::staticGSMHeader[];

GSMessage::GSMessage(uint8_t streamType, uint8_t streamId) : Message(), dataType(streamType), id(streamId) {}

GSMessage::GSMessage(uint8_t streamType, uint8_t streamId, uint8_t *rawData, uint16_t size)
    : Message(rawData, size), dataType(streamType), id(streamId) {}

GSMessage::GSMessage(uint8_t streamType, uint8_t streamId, Data *data)
    : Message(), dataType(streamType), id(streamId)
{
    this->encode(data);
}

bool GSMessage::decodeGSMHeader(char *header, int length, uint32_t &bitrate)
{
    if (length < (int)sizeof(GSMessage::staticGSMHeader) - 1)
        return false; // Error: header data not big enough
    int findHeader = memcmp(header, GSMessage::staticGSMHeader, sizeof(GSMessage::staticGSMHeader) - 1);
    if (findHeader == 0)
    {
        if (length - (sizeof(GSMessage::staticGSMHeader) - 1) >= sizeof(uint32_t))
        {
            uint8_t bitrateData[4] = {0, 0, 0, 0};
            memcpy(bitrateData, header + sizeof(GSMessage::staticGSMHeader) - 1, sizeof(uint32_t));
            bitrate = bitrateData[0] << 24;
            bitrate += bitrateData[1] << 16;
            bitrate += bitrateData[2] << 8;
            bitrate += bitrateData[3] << 0;
            return true;
        } // else header is corrupted (too few bytes given)
    }
    return false;
}

bool GSMessage::encodeGSMHeader(char *header, int length, uint32_t bitrate)
{
    // TODO: comment this
    if (length < GSMessage::gsmHeaderSize)
        return false;
    memcpy(header, GSMessage::staticGSMHeader, sizeof(GSMessage::staticGSMHeader) - 1);
    uint8_t bitrateData[4] = {0, 0, 0, 0};
    bitrateData[0] = (bitrate & 0xff000000) >> 24;
    bitrateData[1] = (bitrate & 0x00ff0000) >> 16;
    bitrateData[2] = (bitrate & 0x0000ff00) >> 8;
    bitrateData[3] = (bitrate & 0x000000ff) >> 0;
    memcpy(header + sizeof(GSMessage::staticGSMHeader) - 1, bitrateData, sizeof(bitrateData));
    return true;
}

bool GSMessage::decodeHeader(uint32_t header, uint8_t &streamType, uint8_t &streamId, uint16_t &size)
{
    // header
    // TISSss
    // T = type (4 bits)
    // I = id (4 bits)
    // SS = size (first 8 bits)
    // ss = size (last 8 bites)
    // create a PackedNum to decode the header
    PackedNum packedHeader(GSMessage::headerEncoding, GSMessage::headerEncodingLength);
    packedHeader.set(header);
    // unpack the data
    uint8_t headerData[GSMessage::headerEncodingLength] = {0};
    bool success = packedHeader.unpack(headerData);

    if (success)
    {
        // set all variables according to where data is stored
        streamType = headerData[0];
        streamId = headerData[1];
        size = headerData[2] << 8;
        size += headerData[3];
    }
    return success;
}

bool GSMessage::decodeHeader(uint8_t *header, uint8_t &streamType, uint8_t &streamId, uint16_t &size)
{
    // header
    // TISSss
    // T = type (4 bits)
    // I = id (4 bits)
    // SS = size (first 8 bits)
    // ss = size (last 8 bites)
    // create a PackedNum to decode the header
    PackedNum packedHeader(GSMessage::headerEncoding, GSMessage::headerEncodingLength);
    packedHeader.set(header);
    // unpack the data
    uint8_t headerData[GSMessage::headerEncodingLength] = {0};
    bool success = packedHeader.unpack(headerData);

    if (success)
    {
        // set all variables according to where data is stored
        streamType = headerData[0];
        streamId = headerData[1];
        size = headerData[2] << 8;
        size += headerData[3];
    }
    return true;
}

bool GSMessage::decodeHeader(uint32_t header)
{
    // header
    // TISSss
    // T = type (4 bits)
    // I = id (4 bits)
    // SS = size (first 8 bits)
    // ss = size (last 8 bites)
    this->header.set(header);
    // unpack the data
    uint8_t headerData[GSMessage::headerEncodingLength] = {0};
    bool success = this->header.unpack(headerData);

    if (success)
    {
        // set all variables according to where data is stored
        this->dataType = headerData[0];
        this->id = headerData[1];
        this->msgSize = headerData[2] << 8;
        this->msgSize += headerData[3];
    }
    return success;
}

bool GSMessage::decodeHeader(uint8_t *header)
{
    // header
    // TISSss
    // T = type (4 bits)
    // I = id (4 bits)
    // SS = size (first 8 bits)
    // ss = size (last 8 bites)
    this->header.set(header);
    // unpack the data
    uint8_t headerData[GSMessage::headerEncodingLength] = {0};
    bool success = this->header.unpack(headerData);

    if (success)
    {
        // set all variables according to where data is stored
        this->dataType = headerData[0];
        this->id = headerData[1];
        this->msgSize = headerData[2] << 8;
        this->msgSize += headerData[3];
    }
    return success;
}

bool GSMessage::decodeHeader()
{
    if (this->size >= GSMessage::headerLen)
    {
        // header
        // TISSss
        // T = type (4 bits)
        // I = id (4 bits)
        // SS = size (first 8 bits)
        // ss = size (last 8 bites)
        this->header.set(this->buf);
        // unpack the data
        uint8_t headerData[GSMessage::headerEncodingLength] = {0};
        bool success = this->header.unpack(headerData);

        if (success)
        {
            // set all variables according to where data is stored
            this->dataType = headerData[0];
            this->id = headerData[1];
            this->msgSize = headerData[2] << 8;
            this->msgSize += headerData[3];
        }
        return success;
    }
    return false;
}

void GSMessage::setMetadata(uint8_t streamType, uint8_t streamId)
{
    this->dataType = streamType;
    this->id = streamId;
}

void GSMessage::getMetadata(uint8_t &streamType, uint8_t &streamId)
{
    streamType = this->dataType;
    streamId = this->id;
}

Message *GSMessage::encode(uint8_t *data, uint16_t sz)
{
    // check if start + sz is larger than this->maxSize
    if (sz > this->maxSize - GSMessage::headerLen)
    {
        // copy only this->maxSize bytes
        memcpy(this->buf + GSMessage::headerLen, data, this->maxSize);
        this->msgSize = this->maxSize - GSMessage::headerLen;
        this->size = this->maxSize;

        // error because the whole message won't be transmitted
        this->error(GSMessage::ERR_ID - 1);
    }
    // if start + sz is smaller than this->maxSize
    else
    {
        // copy sz bytes
        memcpy(this->buf + GSMessage::headerLen, data, sz);
        this->msgSize = sz;
        this->size = sz + GSMessage::headerLen;
    }

    // header
    // TISSss
    // T = type (4 bits)
    // I = id (4 bits)
    // SS = size (first 8 bits)
    // ss = size (last 8 bites)
    uint8_t headerData[] = {this->dataType, this->id, (uint8_t)(this->msgSize >> 8), (uint8_t)(this->msgSize & 0xFF)};
    header.pack(headerData);
    // place header in data
    // start at the beginning of the message, which means we need to remove the headerLen
    header.get(this->buf);

    return this;
}

Message *GSMessage::encode(Data *data)
{
    // figure out where to start the new message
    uint16_t pos = 0;

    // skip the header and encode the message first to get the message size
    pos += GSMessage::headerLen;

    // encode the message
    int status = data->encode(this->buf + pos, GSMessage::maxSize - pos);
    if (status > 0)
    {
        // set msgSize to the size of the internal encoded message
        this->msgSize = status;
        // set size to the size of the overall buffer
        this->size = this->msgSize + pos;
        // null terminate
        this->buf[this->size] = 0;
    }
    else
    {
        this->error(status); // data encoding error
        return this;
    }

    if (this->size > this->maxSize)
    {
        this->error(GSMessage::ERR_ID - 1); // message too large
        this->size = this->maxSize;
        return this;
    }

    // header
    // TISSss
    // T = type (4 bits)
    // I = id (4 bits)
    // SS = size (first 8 bits)
    // ss = size (last 8 bites)
    uint8_t headerData[] = {this->dataType, this->id, (uint8_t)(this->msgSize >> 8), (uint8_t)(this->msgSize & 0xFF)};
    header.pack(headerData);
    // place header in data
    // start at the beginning of the message, which means we need to remove the headerLen
    header.get(this->buf + pos - GSMessage::headerLen);

    return this;
}

Message *GSMessage::decode(Data *data)
{
    uint16_t pos = 0;
    if (this->size < GSMessage::headerLen)
    {
        this->error(GSMessage::ERR_ID - 2); // error data too small for header
        return this;
    }

    // header
    // TISSss
    // T = type (4 bits)
    // I = id (4 bits)
    // SS = size (first 8 bits)
    // ss = size (last 8 bites)
    if (!this->decodeHeader(this->buf))
    {
        this->error(GSMessage::ERR_ID - 3); // header decoding error
        return this;
    }
    // decodeHeader() sets: this->dataType, this->id, this->size

    pos += GSMessage::headerLen;

    if (this->size < pos + 1)
    {
        this->error(GSMessage::ERR_ID - 4); // no data available
        return this;
    }

    int status = data->decode(this->buf + pos, this->msgSize);
    if (status <= 0)
        this->error(status); // data decoding error
    return this;
}