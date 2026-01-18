#ifndef GSCONTROL_H
#define GSCONTROL_H

#include "../Data.h"
#include "../Types/PackedNum.h"

typedef bool (*GSControl_CB)(char *, uint16_t, char **);

class GSControl : public Data
{
public:
    // maximum total command size
    static const uint16_t maxCmdSize = 20;
    // maximum total arguments size
    static const uint16_t maxArgSize = 200;
    // GSControl type
    static const uint8_t type = 0x01;
    // type error ID
    static const int ERR_ID = -type * 100;

    // buffer to store message command
    char cmdBuf[maxCmdSize + 1] = {0};
    // buffer to store message args
    char argBuf[maxArgSize + 1] = {0};

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

    void setCmd(const char *cmd, const char *args);

    // process the command contained within the GSControl object using the callback function f
    bool processCmd(GSControl_CB f);

    void retrieveCmd(char **cmd, uint16_t *argc, char ***argv);

    void cleanup(uint16_t argc, char **argv);

    // encode the data stored in the ```Data``` object and place the result in ```data```
    int encode(uint8_t *data, uint16_t sz) override;
    // decode the data stored in ```data``` and place it in the ```Data``` object
    int decode(uint8_t *data, uint16_t sz) override;

    // place the data in the ```Data``` object in the ```json``` string, ```sz``` is the max size of the string, ```deviceId``` can be set based on hardware
    int toJSON(char *json, uint16_t sz, int deviceId) override;
    // place the data in the ```json``` string in the ```Data``` object, ```sz``` is the max size of the string, ```deviceId``` can be set based on hardware
    int fromJSON(char *json, uint16_t sz, int &deviceId) override;
};

#endif