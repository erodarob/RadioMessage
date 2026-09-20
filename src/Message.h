#ifndef MESSAGE_H
#define MESSAGE_H

#if defined(ARDUINO)
#include <Arduino.h>
#elif defined(_WIN32) || defined(_WIN64) || defined(__unix__) || defined(__APPLE__) // Windows, Linux, or OSX
#include <cstdint>
#include <cstring>
#include <cstdio>
#include <cmath>
using namespace std;
#endif

#include "Data.h"
#include "Wrapper.h"

class Message
{
public:
#ifndef MSG_SIZE_OVRD
    /** maximum message size */
    static const uint16_t maxSize = 10e3;
#else
    /** maximum message size */
    static const uint16_t maxSize = MSG_SIZE_OVRD;
#endif
    /** Message error ID */
    static const int ERR_ID = -0;
    /** the buffer that stores the Message data, actual size is ```Message::maxSize+1```,
     * but the last byte should always be 0 to prevent issues with C string functions */
    uint8_t buf[Message::maxSize + 1] = {0};
    /** size of the buffer, max is ```Message::maxSize``` */
    uint16_t size = 0;

    /**
     * Message default constructor
     */
    Message();

    /**
     * Message constructor
     * @param rawData a pointer to an array of encoded data to place into the Message buffer
     * @param sz the size of the array
     */
    Message(uint8_t *rawData, uint16_t sz);

    /**
     * Message constructor
     * @param data a pointer to a Data object that will be encoded into the Message buffer
     */
    Message(Data *data);

    /**
     * Message destructor
     */
    ~Message();

    // use return type Message* so we can stack operators e.g., ```Message()->encode()->print()```

    /**
     * Encodes data from a Data object into the Message buffer
     * @param data a pointer to the Data object to encode data from
     * @return a pointer to this Message
     */
    virtual Message *encode(Data *data);
    /**
     * Decodes data from the Message buffer into a Data object
     * @param data a pointer to the Data object to decode data into
     * @return a pointer to this Message
     */
    virtual Message *decode(Data *data);
    /**
     * Clears all stored data by setting buffer memory to 0, and buffer size to 0
     * @return a pointer to this Message
     */
    Message *clear();

    /**
     * Wrap the current buffer, assumes it is padded with enough space, using all registered Wrappers.
     * @return a pointer to this Message
     */
    Message *wrap();
    /**
     * Unwrap the current buffer, assuming it is complete, using all registered Wrappers.
     * @return a pointer to this Message
     */
    Message *unwrap();
    /**
     * Unwrap the current buffer using the specifed Wrapper, taking into account the proper ordering of registered Wrappers.
     * Can be used to unwrap wrappers before the message is completed (e.g. to unwrap a length header), but the Wrapper must have a prepended section.
     * @param wr the Wrapper to unwrap, must be registered with the Message
     * @return a pointer to this Message
     */
    Message *unwrap(Wrapper *wr);

    /**
     * Append an array of data to the Message buffer, fails if final message size will be too large
     * @param data a pointer to the array to copy data from
     * @param sz the number of bytes to copy
     * @return a pointer to this Message
     */
    Message *append(uint8_t *data, uint16_t sz);
    /**
     * Append a single byte to the Message buffer, fails if final message size will be too large
     * @param data the byte to copy into the buffer
     * @return a pointer to this Message
     */
    Message *append(uint8_t data);
    /**
     * Get a sequence of bytes from the end of the Message buffer, removing them from the buffer
     * @param data a pointer to the array to copy data into
     * @param sz a reference to a variable containing the number of bytes to be copied, will be set to the actual number of bytes copied
     * @return a pointer to this Message
     */
    Message *pop(uint8_t *data, uint16_t &sz);
    /**
     * Get a sequence of bytes from the front of the Message buffer, removing them from the buffer
     * @param data a pointer to the array to copy data into
     * @param sz a reference to a variable containing the number of bytes to be copied, will be set to the actual number of bytes copied
     * @return a pointer to this Message
     */
    Message *shift(uint8_t *data, uint16_t &sz);

    // utility methods

    /**
     * Copies from the given array into the Message buffer
     * @param data a pointer to the array to copy data from
     * @param sz the number of bytes to be copied, must be less than ```Message::maxSize```
     * @return a pointer to this Message
     */
    Message *fill(uint8_t *data, uint16_t sz);
    /**
     * Copies from the given array into the Message buffer
     * @param data a pointer to the array to copy data from
     * @param start the index to start copying to in the Message
     * @param sz the number of bytes to be copied, must be less than ```Message::maxSize - start```
     * @return a pointer to this Message
     */
    Message *fill(uint8_t *data, uint16_t start, uint16_t sz);

    /**
     * Copies the entire Message buffer into the given array
     * @param data a pointer to the array to copy data into
     * @return a pointer to this Message
     */
    Message *get(uint8_t *data);
    /**
     * Copies from the Message buffer into the given array
     * @param data a pointer to the array to copy data into
     * @param sz a reference to a variable containing the number of bytes to be copied, will be set to the actual number of bytes copied
     * @return a pointer to this Message
     */
    Message *get(uint8_t *data, uint16_t &sz);
    /**
     * Copies from the Message buffer into the given array
     * @param data a pointer to the array to copy data into
     * @param sz a reference to a variable containing the number of bytes to be copied, will be set to the actual number of bytes copied
     * @param start the index to start copying from in the Message
     * @return a pointer to this Message
     */
    Message *get(uint8_t *data, uint16_t &sz, uint16_t start);
    /**
     * Copies from the Message buffer into the given array
     * @param data a pointer to the array to copy data into
     * @param sz a reference to a variable that will be set to the number of bytes copied
     * @param start the index to start copying from in the Message
     * @param end the index to stop copying from in the Message
     * @return a pointer to this Message
     */
    Message *get(uint8_t *data, uint16_t &sz, uint16_t start, uint16_t end);

    // error handling

    /**
     * Check if there are pending errors
     * @return whether there are pending errors
     */
    bool hasError();
    /**
     * Get the pending errors
     * @return pointer to a null terminated character array containing the error codes
     */
    char *errors();

    // wrappers

    /**
     * Register a Wrapper with this Message to be used in wrapping and unwrapping data
     * @param wr a pointer to the wrapper to register
     * @return a pointer to this Message
     */
    Message *reg(Wrapper *wr);
    /**
     * Unregsiter a previously registered Wrapper
     * @param wr a pointer to the wrapper to unregister
     * @return a pointer to this Message
     */
    Message *unreg(Wrapper *wr);

    /**
     * Enable a previously disabled Wrapper
     * @param wr a pointer to the Wrapper to enable
     * @return a pointer to this Message
     */
    Message *enable(Wrapper *wr);
    /**
     * Disable a previously registered Wrapper without needing to unregister it.
     * The Wrapper will not be added to the encoded Message.
     * @param wr a pointer to the Wrapper to disable
     * @return a pointer to this Message
     */
    Message *disable(Wrapper *wr);

#ifdef ARDUINO
    /**
     * Prints the contents of this Message over the given Stream, \n terminated.
     * Note: the contents of the Message must be a valid C string
     * @param s reference to the Stream to use (e.g. Serial)
     * @return a pointer to this Message
     */
    Message *print(Stream &s);

    /**
     * Writes the contents of the message over the given stream
     * Note: the contents of the Message are not required to be a valid C string
     * @param s reference to the Stream to use (e.g. Serial)
     * @return a pointer to this Message
     */
    Message *write(Stream &s);
#elif defined(_WIN32) || defined(_WIN64) || defined(__unix__) || defined(__APPLE__)
    /**
     * Prints the contents of this Message over stdout, \n terminated.
     * Note: the contents of the Message must be a valid C string
     * @return a pointer to this Message
     */
    Message *print();

    /**
     * Writes the contents of the message over stdout
     * Note: the contents of the Message are not required to be a valid C string
     * @return a pointer to this Message
     */
    Message *write();
#endif

protected:
    /** whether there are pending errors */
    bool err = false;
    /** a string containing the pending errors */
    char errStr[32] = {0};

    /**
     * Add a new error to the error string
     * @param err the error number
     */
    void error(int err);

private:
    /** an array of all registered Wrappers */
    Wrapper **wrs = nullptr;
    /** the number of registered Wrappers */
    uint16_t numWrappers = 0;

    /** the total length of all prepended Wrapper sections */
    uint16_t prependLen = 0;
    /** the total length of all appeneded Wrapper sections */
    uint16_t appendLen = 0;
};

#endif