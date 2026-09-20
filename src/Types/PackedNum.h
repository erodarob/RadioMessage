#ifndef PACKED_NUM_H
#define PACKED_NUM_H

#if defined(ARDUINO)
#include <Arduino.h>
#elif defined(_WIN32) || defined(_WIN64) || defined(__unix__) || defined(__APPLE__) // Windows, Linux, or OSX
#include <cstdint>
#include <cstring>
#include <cstdio>
#include <cmath>
using namespace std;
#endif

/**
 * The different possible sizes of the number
 */
enum PackedSize
{
    PT_UINT8 = 8,
    PT_UINT16 = 16,
    PT_UINT32 = 32,
    PT_UINT64 = 64
};

// a number packed with other numbers
class PackedNum
{
public:
    /** the size of the number */
    PackedSize type = PT_UINT8;
    /** the number */
    void *num;
    /** the encoding for the number */
    uint8_t *encoding = new uint8_t[1];
    /** the length of the encoding */
    int encodingLength = 0;
    /** the total number of bits in the encoding */
    int totalBits = 0;
    /** the total number of bytes in the encoding, rounded up */
    int totalBytes = 0;

    /**
     * PackedNum constructor
     * @param t the size of the number
     */
    PackedNum(PackedSize t = PT_UINT8);
    /**
     * PackedNum constructor
     * @param n the number, sets the type to PT_UINT8
     */
    PackedNum(uint8_t n);
    /**
     * PackedNum constructor
     * @param n the number, sets the type to PT_UINT16
     */
    PackedNum(uint16_t n);
    /**
     * PackedNum constructor
     * @param n the number, sets the type to PT_UINT32
     */
    PackedNum(uint32_t n);
    /**
     * PackedNum constructor
     * @param n the number, sets the type to PT_UINT64
     */
    PackedNum(uint64_t n);
    /**
     * PackedNum constructor
     * @param widths the width of each number in the encoding
     * @param length the number of entries in widths
     */
    PackedNum(const uint8_t *widths, int length);

    /**
     * PackedNum destructor
     */
    ~PackedNum();

    /**
     * Set encoding after constructor
     * @param widths the width of each number in the encoding
     * @param length the number of entries in widths
     * @return whether the encoding is valid for this type
     */
    bool setEncoding(const uint8_t *widths, int length);
    /**
     * Encode values into the number based on encoding
     * @param vals the values to encode (same length as encoding)
     * @return whether the packing was successful
     */
    bool pack(uint64_t *vals);
    /**
     * Decode into values from number based on encoding
     * @param vals an array to decode values into (same length as encoding)
     * @return whether the unpacking was successful
     */
    bool unpack(uint64_t *vals);

    /**
     * set the value of the number, automatically truncated and converted to correct type
     * @param n the new number
     */
    void set(uint64_t n);
    /**
     * Set the value of the number, read from an uint8_t array
     * @param arr the array to read from, must be as long as the number of bytes in this number (check totalBytes)
     * @return whether the number was set correctly
     */
    bool set(uint8_t *arr);
    /**
     * Get the value of the number, automatically truncated
     * @return the value of the number, cast to correct type if necessary
     */
    uint64_t get();
    /**
     * Get the value of the number, placed into a uint8_t array
     * @param arr the array to place the number into, must be as long as the number of bytes in this number (check totalBytes)
     * @return whether the number was retrieved correctly
     */
    bool get(uint8_t *arr);
};

#endif