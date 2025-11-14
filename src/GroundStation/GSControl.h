#ifndef GSCONTROL_H
#define GSCONTROL_H

// TODO deconflict type numbers

#include "../Data.h"
#include "../Types/PackedNum.h"

class GSControl : public Data
{
public:
    // maximum total command size
    static const uint16_t maxCmdSize = 20;
    // maximum total arguments size
    static const uint16_t maxArgSize = 200;
    // GSControl type
    static const uint8_t type = 0x01;

    // buffer to store message command
    char cmdBuf[maxCmdSize] = {0};
    // buffer to store message args
    char argBuf[maxArgSize] = {0};

    // whether the command was successfully fit into the internal variables
    bool valid = false;

    // GSControl default constructor
    GSControl() {};

    // GSControl constructor
    // - cmd : the command and arguments for the message, must be null terminated
    GSControl(const char *cmd);

    // GSControl constructor
    // - cmd : the command for the message, must be null terminated
    // - args : the arguments for the message, must be null terminated
    GSControl(const char *cmd, const char *args);

    // GSControl constructor
    // - cmd : the command for the message, must be null terminated
    // - argc : the number of arguments for the message
    // - argv : the values of the arguments for the message, must be null terminated
    GSControl(const char *cmd, uint16_t argc, const char **argv);

    // process the command contained within the GSControl object using the callback function f
    bool processCmd(bool (*f)(char *, uint16_t, char **));

    // encode the data stored in the ```Data``` object and place the result in ```data```
    uint16_t encode(uint8_t *data, uint16_t sz) override;
    // decode the data stored in ```data``` and place it in the ```Data``` object
    uint16_t decode(uint8_t *data, uint16_t sz) override;

    // place the data in the ```Data``` object in the ```json``` string, ```sz``` is the max size of the string, ```deviceId``` can be set based on hardware
    uint16_t toJSON(char *json, uint16_t sz, int deviceId) override;
    // place the data in the ```json``` string in the ```Data``` object, ```sz``` is the max size of the string, ```deviceId``` can be set based on hardware
    uint16_t fromJSON(char *json, uint16_t sz, int &deviceId) override;
};

#endif