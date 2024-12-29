#include "PackedNum.h"

PackedNum::PackedNum(PackedType t)
{
    this->type = t;
    switch (this->type)
    {
    case PT_UINT16:
        this->num = new uint16_t;
        *(uint16_t *)this->num = 0;
        break;
    case PT_UINT32:
        this->num = new uint32_t;
        *(uint32_t *)this->num = 0;
        break;
    case PT_UINT64:
        this->num = new uint64_t;
        *(uint64_t *)this->num = 0;
        break;
    default:
        this->num = new uint8_t;
        *(uint8_t *)this->num = 0;
        break;
    }
}

PackedNum::PackedNum(uint8_t n)
{
    this->num = new uint8_t;
    *(uint8_t *)this->num = n;
    this->type = PT_UINT8;
}

PackedNum::PackedNum(uint16_t n)
{
    this->num = new uint16_t;
    *(uint16_t *)this->num = n;
    this->type = PT_UINT16;
}

PackedNum::PackedNum(uint32_t n)
{
    this->num = new uint32_t;
    *(uint32_t *)this->num = n;
    this->type = PT_UINT32;
}

PackedNum::PackedNum(uint64_t n)
{
    this->num = new uint64_t;
    *(uint64_t *)this->num = n;
    this->type = PT_UINT64;
}

PackedNum::PackedNum(const uint8_t *widths, int length)
{
    int total = 0;
    int len = 0;
    for (int i = 0; i < length; i++)
    {
        if (total + widths[i] > PT_UINT64)
            break;
        total += widths[i];
        len += 1;
    }

    if (total > PT_UINT32)
    {
        this->num = new uint64_t;
        *(uint64_t *)this->num = 0;
        this->type = PT_UINT64;
    }
    else if (total > PT_UINT16)
    {
        this->num = new uint32_t;
        *(uint32_t *)this->num = 0;
        this->type = PT_UINT32;
    }
    else if (total > PT_UINT8)
    {
        this->num = new uint16_t;
        *(uint16_t *)this->num = 0;
        this->type = PT_UINT16;
    }
    else
    {
        this->num = new uint8_t;
        *(uint8_t *)this->num = 0;
        this->type = PT_UINT8;
    }

    this->totalBits = total;

    delete[] this->encoding;
    this->encodingLength = len;
    this->encoding = new uint8_t[this->encodingLength];
    memcpy(this->encoding, widths, this->encodingLength);
}

PackedNum::~PackedNum()
{
    switch (this->type)
    {
    case PT_UINT8:
        delete ((uint8_t *)(this->num));
        break;
    case PT_UINT16:
        delete (uint16_t *)this->num;
        break;
    case PT_UINT32:
        delete (uint32_t *)this->num;
        break;
    default:
        delete (uint64_t *)this->num;
        break;
    }
    delete[] this->encoding;
}

bool PackedNum::setEncoding(const uint8_t *widths, int length)
{
    int total = 0;
    for (int i = 0; i < length; i++)
    {
        total += widths[i];
    }

    if (total > this->type)
        return false;

    this->totalBits = total;

    delete[] this->encoding;
    this->encoding = new uint8_t[length];
    this->encodingLength = length;
    memcpy(this->encoding, widths, sizeof(this->encoding));

    return true;
}

bool PackedNum::encode(uint8_t *vals)
{
    int lengthAdded = 0;
    uint8_t t = 0;
    for (int i = 0; i < this->encodingLength; i++)
    {
        // expanded version
        // int pos = this->totalBits - this->encoding[i] - lengthAdded; // the position in the bits
        // int maxVal = (1 << this->encoding[i]) - 1; // the maximum value of this number
        // int val = vals[i] & maxVal; // truncate the given number in case it is larger than the max val
        // this->num += val << pos; // move the number to the correct position to add
        switch (this->type)
        {
        case PT_UINT8:
            *(uint8_t *)this->num += (vals[i] & ((1 << this->encoding[i]) - 1)) << (this->totalBits - this->encoding[i] - lengthAdded);
            break;
        case PT_UINT16:
            *(uint16_t *)this->num += (vals[i] & ((1 << this->encoding[i]) - 1)) << (this->totalBits - this->encoding[i] - lengthAdded);
            break;
        case PT_UINT32:
            *(uint32_t *)this->num += (vals[i] & ((1 << this->encoding[i]) - 1)) << (this->totalBits - this->encoding[i] - lengthAdded);
            break;
        case PT_UINT64:
            *(uint64_t *)this->num += (vals[i] & ((1 << this->encoding[i]) - 1)) << (this->totalBits - this->encoding[i] - lengthAdded);
            break;
        default:
            return false;
        }
        lengthAdded += this->encoding[i];
    }
    return true;
}

bool PackedNum::decode(uint8_t *vals)
{
    int lengthRemoved = 0;
    for (int i = 0; i < this->encodingLength; i++)
    {
        // expanded version
        // int pos = (1 << (this->totalBits - lengthRemoved)) - 1; // a binary number containing 0 to the left of the number and 1 to the right and including the number
        // int valPlusTheRight = this->num & pos; // remove everything to the left of the number
        // int shift = this->totalBits - this->encoding[i] - lengthRemoved; // how many bits we need to shift to get just the number
        // vals[i] = valPlusTheRight >> shift; // shift to get just the number
        switch (this->type)
        {
        case PT_UINT8:
            vals[i] = (*(uint8_t *)this->num & ((1 << (this->totalBits - lengthRemoved)) - 1)) >> (this->totalBits - this->encoding[i] - lengthRemoved);
            break;
        case PT_UINT16:
            vals[i] = (*(uint16_t *)this->num & ((1 << (this->totalBits - lengthRemoved)) - 1)) >> (this->totalBits - this->encoding[i] - lengthRemoved);
            break;
        case PT_UINT32:
            vals[i] = (*(uint32_t *)this->num & ((1 << (this->totalBits - lengthRemoved)) - 1)) >> (this->totalBits - this->encoding[i] - lengthRemoved);
            break;
        case PT_UINT64:
            vals[i] = (*(uint64_t *)this->num & ((1 << (this->totalBits - lengthRemoved)) - 1)) >> (this->totalBits - this->encoding[i] - lengthRemoved);
            break;
        default:
            return false;
        }
        lengthRemoved += this->encoding[i];
    }
    return true;
}

void PackedNum::set(uint64_t n)
{
    switch (this->type)
    {
    case PT_UINT8:
        *(uint8_t *)this->num = n & 0xFF;
        break;
    case PT_UINT16:
        *(uint16_t *)this->num = n & 0xFFFF;
        break;
    case PT_UINT32:
        *(uint32_t *)this->num = n & 0xFFFFFFFF;
        break;
    default:
        *(uint64_t *)this->num = n;
        break;
    }
}

uint64_t PackedNum::get()
{
    switch (this->type)
    {
    case PT_UINT8:
        return *(uint64_t *)this->num & 0xFF;
    case PT_UINT16:
        return *(uint64_t *)this->num & 0xFFFF;
    case PT_UINT32:
        return *(uint64_t *)this->num & 0xFFFFFFFF;
    default:
        return *(uint64_t *)this->num;
    }
}