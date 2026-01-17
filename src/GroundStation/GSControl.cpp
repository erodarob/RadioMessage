#include "GSControl.h"

GSControl::GSControl(const char *cmd)
{
    this->decode((uint8_t *)cmd, strlen(cmd));
}

GSControl::GSControl(const char *cmd, const char *args)
{
    this->setCmd(cmd, args);
}

GSControl::GSControl(const char *cmd, uint16_t argc, const char **argv)
{
    // copy command
    uint32_t cmdLen = strlen(cmd);

    uint32_t validCmdLen = cmdLen > (sizeof(cmdBuf) - 1) ? (sizeof(cmdBuf) - 1) : cmdLen;

    memcpy(this->cmdBuf, cmd, validCmdLen);
    this->cmdBuf[validCmdLen] = 0; // ensure null termination

    // copy args
    int totalLen = 0;
    bool assembleArgsFail = false;
    for (int i = 0; i < argc; i++)
    {
        // make sure this string fits, subtract 1 for null terminator or space between args
        if (totalLen + strlen(argv[i]) < sizeof(this->argBuf) - 1)
        {
            strcat(this->argBuf, argv[i]);
            // only add space if this isn't the last arg
            if (i != argc - 1)
                strcat(this->argBuf, " ");
            totalLen = strlen(this->argBuf);
        }
        else
        {
            assembleArgsFail = true;
            break;
        }
    }

    this->valid = !(cmdLen > (sizeof(cmdBuf) - 1) || assembleArgsFail);
}

void GSControl::setCmd(const char *cmd, const char *args)
{
    uint32_t cmdLen = strlen(cmd);
    uint32_t argLen = strlen(args);

    uint32_t validCmdLen = cmdLen > (sizeof(cmdBuf) - 1) ? (sizeof(cmdBuf) - 1) : cmdLen;
    uint32_t validArgLen = argLen > (sizeof(argBuf) - 1) ? (sizeof(argBuf) - 1) : argLen;

    memcpy(this->cmdBuf, cmd, validCmdLen);
    memcpy(this->argBuf, args, validArgLen);
    this->cmdBuf[validCmdLen] = 0; // ensure null termination
    this->argBuf[validArgLen] = 0;

    this->valid = !(cmdLen > (sizeof(cmdBuf) - 1) || argLen > (sizeof(argBuf) - 1));
}

bool GSControl::processCmd(GSControl_CB f)
{
    uint16_t argc = 0;
    int maxArgLen = 0;
    int lastArgPos = 0;
    for (int i = 0; i < (int)strlen(this->argBuf); i++)
    {
        if (this->argBuf[i] == ' ')
        {
            argc++;
            if (maxArgLen < i - lastArgPos)
                maxArgLen = i - lastArgPos;
            lastArgPos = i;
        }
    }
    argc++; // add one since last arg doesn't have a space

    char **argv = new char *[argc];
    for (int i = 0; i < argc; i++)
        argv[i] = new char[maxArgLen + 1]; // add 1 for null terminator

    int argIdx = 0;
    lastArgPos = 0;
    int argsLen = strlen(this->argBuf);
    for (int i = 0; i < argsLen; i++)
    {
        if (this->argBuf[i] == ' ' || i == argsLen - 1)
        {
            memcpy(argv[argIdx], this->argBuf + lastArgPos, i - lastArgPos + 1); // include trailing char (' ' or '\0')
            argv[argIdx][i - lastArgPos + 1] = 0;                                // add null terminator
            argIdx++;
            lastArgPos = i + 1; // add 1 to skip space
        }
    }

    bool success = f(this->cmdBuf, argc, argv);

    for (int i = 0; i < argc; i++)
        delete[] argv[i];
    delete[] argv;

    return success;
}

void GSControl::retrieveCmd(char *cmd, uint16_t &argc, char **argv)
{
    argc = 0;
    int maxArgLen = 0;
    int lastArgPos = 0;
    for (int i = 0; i < (int)strlen(this->argBuf); i++)
    {
        if (this->argBuf[i] == ' ')
        {
            argc++;
            if (maxArgLen < i - lastArgPos)
                maxArgLen = i - lastArgPos;
            lastArgPos = i;
        }
    }
    argc++; // add one since last arg doesn't have a space

    argv = new char *[argc];
    for (int i = 0; i < argc; i++)
        argv[i] = new char[maxArgLen + 1]; // add 1 for null terminator

    int argIdx = 0;
    lastArgPos = 0;
    int argsLen = strlen(this->argBuf);
    for (int i = 0; i < argsLen; i++)
    {
        if (this->argBuf[i] == ' ' || i == argsLen - 1)
        {
            memcpy(argv[argIdx], this->argBuf + lastArgPos, i - lastArgPos + 1); // include trailing char (' ' or '\0')
            argv[argIdx][i - lastArgPos + 1] = 0;                                // add null terminator
            argIdx++;
            lastArgPos = i + 1; // add 1 to skip space
        }
    }

    cmd = this->cmdBuf;

    // bool success = f(this->cmdBuf, argc, argv);
}

void GSControl::cleanup(uint16_t argc, char **argv)
{
    if (argv != nullptr)
    {
        for (int i = 0; i < argc; i++)
            if (argv[i] != nullptr)
                delete[] argv[i];
        delete[] argv;
    }
}

int GSControl::encode(uint8_t *data, uint16_t sz)
{
    int index = 0;
    int cmdLen = strlen(this->cmdBuf);
    int argLen = strlen(this->argBuf);

    // check that the message can fit the cmd
    if (sz < cmdLen)
        return GSControl::ERR_ID - 1;

    // copy cmd
    memcpy(data + index, this->cmdBuf, cmdLen);
    index += cmdLen;
    data[index++] = ' '; // add space between cmd and args
    data[index] = 0;     // null terminate

    // check that the message can fit the args
    if (sz < index + argLen)
        return GSControl::ERR_ID - 2;

    // copy args
    memcpy(data + index, this->argBuf, argLen);
    index += argLen;
    data[index] = 0; // null terminate

    return index;
}

int GSControl::decode(uint8_t *data, uint16_t sz)
{
    // assume typical command structure with spaces separating args
    for (uint32_t i = 0; i < sz; i++)
    {
        if (data[i] == ' ')
        {
            // copy command
            // cut this portion of the string at the max size of cmdBuf if it's too long
            uint32_t cmdOffset = i > (sizeof(cmdBuf) - 1) ? (sizeof(cmdBuf) - 1) : i;
            memcpy(this->cmdBuf, data, cmdOffset);
            this->cmdBuf[cmdOffset] = 0; // ensure null terminated
            cmdOffset++;                 // skip space character

            // copy args
            // cut this portion of the string at the max size of argBuf if it's too long
            int argLen = (sz - cmdOffset) > (sizeof(argBuf) - 1) ? (sizeof(argBuf) - 1) : (sz - cmdOffset);
            memcpy(this->argBuf, data + cmdOffset, argLen);
            this->argBuf[argLen] = 0; // ensure null terminated

            this->valid = !((sz - cmdOffset) > (sizeof(argBuf) - 1) || i > (sizeof(cmdBuf) - 1));
            break;
        }
    }

    if (this->valid)
        // return total length added to buffers +1 for space in between
        return strlen(this->cmdBuf) + 1 + strlen(this->argBuf);
    return GSControl::ERR_ID - 3;
}

int GSControl::toJSON(char *json, uint16_t sz, int deviceId)
{
    uint16_t result = (uint16_t)snprintf(json, sz, "{\"type\":\"GSControl\",\"deviceId\":%d,\"data\":{\"valid\":%d,\"cmd\":\"%s\",\"args\":\"%s\"}}",
                                         deviceId, this->valid, this->cmdBuf, this->argBuf);
    if (result >= sz)
    {
        // output too large
        return GSControl::ERR_ID - 4;
    }

    // result should be the index of the \0
    return result;
}

int GSControl::fromJSON(char *json, uint16_t sz, int &deviceId)
{
    // strings to store data in
    char deviceIdStr[5] = {0};
    char validStr[5] = {0};

    // extract each string
    if (!extractStr(json, sz, "\"deviceId\":", ',', deviceIdStr, sizeof(deviceIdStr)))
        return GSControl::ERR_ID - 5;
    if (!extractStr(json, sz, "\"valid\":", ',', validStr, sizeof(validStr)))
        return GSControl::ERR_ID - 6;
    if (!extractStr(json, sz, "\"cmd\":\"", '\"', this->cmdBuf, sizeof(this->cmdBuf)))
        return GSControl::ERR_ID - 7;
    if (!extractStr(json, sz, "\"args\":\"", '\"', this->argBuf, sizeof(this->argBuf)))
        return GSControl::ERR_ID - 8;

    // convert to correct data type
    deviceId = atoi(deviceIdStr);
    this->valid = (bool)atoi(validStr);

    return sz;
}