#ifndef GSCONTROL_H
#define GSCONTROL_H

#include "../Data.h"
#include "../Types/PackedNum.h"

typedef bool (*GSControl_CB)(char *, uint16_t, char **);

class GSControl : public Data
{
public:
    /** GSControl type */
    static const uint8_t type = 0x01;
    /** GSControl error ID */
    static const int ERR_ID = -type * 100;
    /** maximum total command size */
    static const uint16_t maxCmdSize = 20;
    /** maximum total arguments size */
    static const uint16_t maxArgSize = 200;

    /** buffer to store message command */
    char cmdBuf[maxCmdSize + 1] = {0};
    /** buffer to store message args */
    char argBuf[maxArgSize + 1] = {0};

    /** whether the command was successfully fit into the internal variables */
    bool valid = false;

    /**
     * GSControl default constructor
     */
    GSControl() {};

    /**
     * GSControl constructor
     * @param data the command and arguments for the message, must be null terminated
     */
    GSControl(const char *data);
    /**
     * GSControl constructor
     * @param cmd the command for the message, must be null terminated
     * @param args the arguments for the message, must be null terminated
     */
    GSControl(const char *cmd, const char *args);
    /**
     * GSControl constructor
     * @param cmd the command for the message, must be null terminated
     * @param argc the number of arguments for the message
     * @param argv the values of the arguments for the message, must be null terminated
     */
    GSControl(const char *cmd, uint16_t argc, const char **argv);

    /**
     * Set the command and arguments
     * @param cmd the command for the message, must be null terminated
     * @param args the arguments for the message, must be null terminated
     */
    void setCmd(const char *cmd, const char *args);

    /**
     * Process the command contained within the GSControl object using the provided callback function
     * @param f the callback function to use
     * @return whether the callback processed the command successfully
     */
    bool processCmd(GSControl_CB f);
    /**
     * Place the contents of the command in the supplied variables
     * @param cmd a double pointer which will be set to a pointer to the command character array
     * @param argc a pointer to a number place the number of arguments in
     * @param argv a triple pointer which will be point to a dynamically allocated array of c strings containing the arguments, must be cleaned up with cleanup()
     */
    void retrieveCmd(char **cmd, uint16_t *argc, char ***argv);
    /**
     * Clean up memory used to previously retrieve the arguments
     * @param argc the number of arguments
     * @param argv the triple pointer previously passed to retrieveCmd()
     */
    void cleanup(uint16_t argc, char ***argv);

    /**
     * Encode the data stored in the Data object and place it in the provided buffer
     * @param data a pointer to the array to place the data in
     * @param sz the maximum length of the data array
     * @return the result of the encoding, the length of bytes added if successful, otherwise an error code less than 0
     */
    int encode(uint8_t *data, uint16_t sz) override;
    /**
     * Decode the data stored in the the provided buffer and place it in the Data object
     * @param data a pointer to the array to extract data from
     * @param sz the length of the data array
     * @return the result of the decoding, the length of bytes decoded if successful, otherwise an error code less than 0
     */
    int decode(uint8_t *data, uint16_t sz) override;

    /**
     * Encode the data in the Data object into a JSON format
     * @param json the string to place the JSON into
     * @param sz the maximum size of the json string
     * @param deviceId a number indicating the hardware the data came from
     * @return the result of the encoding, the length of bytes added if successful, otherwise an error code less than 0
     */
    int toJSON(char *json, uint16_t sz, int deviceId) override;
    /**
     * Decode the data from a JSON format into the Data object
     * @param json the JSON string to extract data from
     * @param sz the size of the json string
     * @param deviceId a number indicating the hardware the data came from, decoded separately from the JSON
     * @return the result of the decoding, greater than 0 if successful, otherwise an error code less than 0
     */
    int fromJSON(char *json, uint16_t sz, int &deviceId) override;
};

#endif