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

// the different possible sizes of the number
enum PackedType
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
    // the size of the number
    PackedType type = PT_UINT8;
    // the number
    void *num;
    // the encoding for the number
    uint8_t *encoding = new uint8_t[1];
    // the length of the encoding
    int encodingLength = 0;
    // the total number of bits in the encoding
    int totalBits = 0;
    // PackedNum constructor
    // - t: the size of the number
    PackedNum(PackedType t = PT_UINT8);
    // PackedNum constructor
    // - n: the number, sets the type to PT_UINT8
    PackedNum(uint8_t n);
    // PackedNum constructor
    // - n: the number, sets the type to PT_UINT16
    PackedNum(uint16_t n);
    // PackedNum constructor
    // - n: the number, sets the type to PT_UINT32
    PackedNum(uint32_t n);
    // PackedNum constructor
    // - n: the number, sets the type to PT_UINT64
    PackedNum(uint64_t n);
    // PackedNum constructor
    // - widths: the width of each number in the encoding
    // - length: the number of entries in widths
    PackedNum(const uint8_t *widths, int length);

    // destructor
    ~PackedNum();

    // set encoding after constructor
    // - widths: the width of each number in the encoding
    // - length: the number of entries in widths
    bool setEncoding(const uint8_t *widths, int length);
    // encode values into the number based on encoding
    // - vals: the values to encode (same length as encoding)
    bool encode(uint8_t *vals);
    // decode into values from number based on encoding
    // - vals: an array to decode values into (same length as encoding)
    bool decode(uint8_t *vals);

    // set the value of the number, automatically truncated and converted to correct type
    // - n: the new number
    void set(uint64_t n);
    // get the value of the number, automatically truncated, cast to correct type if necessary
    uint64_t get();
};

#endif