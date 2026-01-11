#include "APRSText.h"

APRSText::APRSText(APRSConfig config) : APRSData(config) {}

APRSText::APRSText(APRSConfig config, char msg[67], char addressee[9]) : APRSData(config)
{
    this->msgLen = strlen(msg);
    this->addrLen = strlen(addressee);

    if (this->msgLen > this->maxMsgLen)
        this->msgLen = this->maxMsgLen;
    if (this->msgLen == this->maxMsgLen)
        this->msg[this->maxMsgLen] = 0;
    if (this->addrLen > this->maxAddrLen)
        this->addrLen = this->maxAddrLen;
    if (this->addrLen == this->maxAddrLen)
        this->addressee[this->maxAddrLen] = 0;

    memcpy(this->msg, msg, this->msgLen);
    memcpy(this->addressee, addressee, this->addrLen);
}

int APRSText::encode(uint8_t *data, uint16_t sz)
{
    uint16_t pos = 0;

    // APRS header
    int err = this->encodeHeader(data, sz, pos);
    // check for errors
    if (err < 0)
        return APRSText::ERR_ID + err;

    // text message addressee:message

    if (sz < pos + this->addrLen + this->msgLen)
        return APRSText::ERR_ID - 10; // error not enough space for text message
    if (this->config.type != TextMessage)
        return APRSText::ERR_ID - 11; // error wrong APRS message type

    data[pos++] = this->config.type;

    // add the addressee, with space padding if the addressee is not 9 chars
    for (int i = 0; i < 9; i++)
    {
        if (i < this->addrLen)
            data[pos++] = (uint8_t)this->addressee[i];
        else
            data[pos++] = ' ';
    }
    data[pos++] = ':';

    // add the message
    for (int i = 0; i < this->msgLen; i++)
    {
        data[pos++] = (uint8_t)this->msg[i];
    }

    return pos;
}

int APRSText::decode(uint8_t *data, uint16_t sz)
{
    uint16_t pos = 0;

    int err = this->decodeHeader(data, sz, pos);
    if (err < 0)
        return APRSText::ERR_ID + err;

    this->config.type = data[pos++];
    if (this->config.type != TextMessage)
        return APRSText::ERR_ID - 12; // error wrong APRS message type

    // get addressee
    this->addrLen = 0;
    while (pos < sz && data[pos] != ':' && this->addrLen < this->maxAddrLen)
    {
        char c = data[pos++];
        if (c != ' ') // don't include padding spaces
            this->addressee[this->addrLen++] = c;
    }
    this->addressee[this->addrLen] = 0; // make addressee a valid c string

    pos++; // skip :

    // get message text
    this->msgLen = 0;
    while (pos < sz && this->msgLen < this->maxMsgLen)
    {
        this->msg[this->msgLen++] = data[pos++];
    }
    this->msg[this->msgLen] = 0; // make msg a valid c string

    return pos;
}

int APRSText::toJSON(char *json, uint16_t sz, int deviceId)
{
    uint16_t result = (uint16_t)snprintf(json, sz, "{\"type\":\"APRSText\",\"deviceId\":%d,\"data\":{\"message\":\"%s\",\"addressee\":\"%s\"}}", deviceId, this->msg, this->addressee);

    if (result < sz)
    {
        // ran properly
        return result;
    }
    // output too large
    return APRSText::ERR_ID - 13;
}

int APRSText::fromJSON(char *json, uint16_t sz, int &deviceId)
{
    // strings to store data in
    char deviceIdStr[5] = {0};

    // extract each string
    if (!extractStr(json, sz, "\"deviceId\":", ',', deviceIdStr))
        return APRSText::ERR_ID - 14;
    if (!extractStr(json, sz, "\"message\":\"", '"', this->msg))
        return APRSText::ERR_ID - 15;
    if (!extractStr(json, sz, "\"addressee\":\"", '"', this->addressee))
        return APRSText::ERR_ID - 16;

    // convert to correct data type
    deviceId = atoi(deviceIdStr);
    this->msgLen = strlen(this->msg);
    this->addrLen = strlen(this->addressee);

    return sz;
}