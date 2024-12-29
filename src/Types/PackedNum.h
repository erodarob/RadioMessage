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

enum PackedType
{
    PT_UINT8 = 8,
    PT_UINT16 = 16,
    PT_UINT32 = 32,
    PT_UINT64 = 64
};

class PackedNum
{
public:
    PackedType type = PT_UINT8;
    void *num;
    uint8_t *encoding = new uint8_t[1];
    int encodingLength = 0;
    int totalBits = 0;
    PackedNum(PackedType t = PT_UINT8);
    PackedNum(uint8_t n);
    PackedNum(uint16_t n);
    PackedNum(uint32_t n);
    PackedNum(uint64_t n);
    PackedNum(const uint8_t *widths, int length);

    ~PackedNum();

    bool setEncoding(const uint8_t *widths, int length);
    bool encode(uint8_t *vals);
    bool decode(uint8_t *vals);

    void set(uint64_t n);
    uint64_t get();
};

#endif