#ifndef WRAPPER_H
#define WRAPPER_H

#if defined(ARDUINO)
#include <Arduino.h>
#elif defined(_WIN32) || defined(_WIN64) || defined(__unix__) || defined(__APPLE__) // Windows, Linux, or OSX
#include <cstdint>
#include <cstring>
#include <cstdio>
#include <cmath>
using namespace std;
#endif

class Wrapper
{
public:
    bool enabled = false;
    virtual ~Wrapper() {}; // Virtual destructor. Very important

    virtual int wrap(uint8_t *data, uint16_t len) = 0;
    virtual int unwrap(uint8_t *data, uint16_t len, uint16_t &prependLen, uint16_t &appendLen) = 0;

    virtual int prependLen(int mLen = -1) = 0;
    virtual int appendLen(int mLen = -1) = 0;
};

#endif