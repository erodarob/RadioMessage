#include "APRSCmd.h"

APRSCmd::APRSCmd(APRSConfig config) : APRSData(config)
{
}
APRSCmd::APRSCmd(APRSConfig config, uint8_t cmd, uint16_t args) : APRSData(config), cmd(cmd), args(args)
{
}

APRSCmd::APRSCmd(APRSConfig config, uint8_t cmd, uint8_t *args, uint8_t *encoding, uint8_t length) : APRSData(config), cmd(cmd), args(encoding, length)
{
    this->args.pack(args);
}

bool APRSCmd::getArgs(uint8_t *args)
{
    return this->args.unpack(args);
}

uint16_t APRSCmd::encode(uint8_t *data, uint16_t sz)
{
    uint16_t pos = 0;

    // APRS header
    this->encodeHeader(data, sz, pos);

    // command ("ccaaa")
    // cc = command
    // aaa = arguments

    if (sz < pos + 5)
        return 0; // error too small for command

    numtoBase91(data, pos, this->cmd, 2);        // this is quite inefficient (2^8 = 256, but we are using 91^2 = 8281 to represent it)
    numtoBase91(data, pos, this->args.get(), 3); // also quite inefficient (2^16 = 65536, but we are using 91^3 = 753571 to represent it)

    return pos;
}

uint16_t APRSCmd::decode(uint8_t *data, uint16_t sz)
{
    uint16_t pos = 0;
    uint32_t decodedNum = 0;

    this->decodeHeader(data, sz, pos);

    // command ("ccaaa")
    // cc = command
    // aaa = arguments

    if (sz < pos + 5)
        return 0; // error too small for command

    // decode command data
    base91toNum(data, pos, decodedNum, 2);
    this->cmd = decodedNum & 0x00FF;
    base91toNum(data, pos, decodedNum, 3);
    this->args.set(decodedNum);

    return pos;
}

uint16_t APRSCmd::toJSON(char *json, uint16_t sz, int deviceId)
{
    uint16_t result = (uint16_t)snprintf(json, sz, "{\"type\":\"APRSCmd\",\"deviceId\":%d,\"data\":{\"cmd\":\"%#x\",\"args\":\"%#x\"}}", deviceId, this->cmd, (uint16_t)this->args.get());

    if (result < sz)
    {
        // ran properly
        return result;
    }
    // output too large
    return 0;
}

uint16_t APRSCmd::fromJSON(char *json, uint16_t sz, int &deviceId)
{
    // strings to store data in
    char deviceIdStr[5] = {0};
    char cmdStr[5] = {0};
    char argsStr[7] = {0};
    // extract each string
    if (!extractStr(json, sz, "\"deviceId\":", ',', deviceIdStr, sizeof(deviceIdStr)))
        return 0;
    if (!extractStr(json, sz, "\"cmd\":\"", '"', cmdStr, sizeof(cmdStr)))
        return 0;
    if (!extractStr(json, sz, "\"args\":\"", '"', argsStr, sizeof(argsStr)))
        return 0;

    // convert to correct data type
    deviceId = atoi(deviceIdStr);
    this->cmd = strtol(cmdStr, NULL, 16);
    this->args.set(strtol(argsStr, NULL, 16));

    return 1;
}