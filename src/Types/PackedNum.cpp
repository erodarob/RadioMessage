#include "PackedNum.h"

PackedNum::PackedNum(PackedSize t)
{
    // set the type
    this->type = t;
    // allocate memory for num and set to 0
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
    // allocate memory for num and set num to n
    this->num = new uint8_t;
    *(uint8_t *)this->num = n;
    this->type = PT_UINT8;
}

PackedNum::PackedNum(uint16_t n)
{
    // allocate memory for num and set num to n
    this->num = new uint16_t;
    *(uint16_t *)this->num = n;
    this->type = PT_UINT16;
}

PackedNum::PackedNum(uint32_t n)
{
    // allocate memory for num and set num to n
    this->num = new uint32_t;
    *(uint32_t *)this->num = n;
    this->type = PT_UINT32;
}

PackedNum::PackedNum(uint64_t n)
{
    // allocate memory for num and set num to n
    this->num = new uint64_t;
    *(uint64_t *)this->num = n;
    this->type = PT_UINT64;
}

PackedNum::PackedNum(const uint8_t *widths, int length)
{
    // get the total number of bits in the encoding
    int total = 0;
    int len = 0;
    for (int i = 0; i < length; i++)
    {
        // if more than the max (64) bits were specified, include only <=64 bits
        if (total + widths[i] > PT_UINT64)
            break;
        total += widths[i];
        len += 1;
    }

    // find the correct type and allocate the correct memory
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

    // keep track of total number of bits
    this->totalBits = total;
    this->totalBytes = (total + (8 - 1)) / 8; // magically round up to nearest int

    // set the internal encoding variable and length
    delete[] this->encoding;
    this->encodingLength = len;
    this->encoding = new uint8_t[this->encodingLength];
    memcpy(this->encoding, widths, this->encodingLength);
}

PackedNum::~PackedNum()
{
    // need to cast correct type to delete void *
    switch (this->type)
    {
    case PT_UINT8:
        delete (uint8_t *)(this->num);
        break;
    case PT_UINT16:
        delete (uint16_t *)(this->num);
        break;
    case PT_UINT32:
        delete (uint32_t *)(this->num);
        break;
    default:
        delete (uint64_t *)(this->num);
        break;
    }
    delete[] this->encoding;
}

bool PackedNum::setEncoding(const uint8_t *widths, int length)
{
    // get the total number of bits
    int total = 0;
    for (int i = 0; i < length; i++)
    {
        total += widths[i];
    }

    // make sure we are not exceeding the max number of bits for the type
    if (total > this->type)
        return false;

    // keep track of total number of bits
    this->totalBits = total;

    // set the internal encoding variable and length
    delete[] this->encoding;
    this->encoding = new uint8_t[length];
    this->encodingLength = length;
    memcpy(this->encoding, widths, length);

    return true;
}

bool PackedNum::pack(uint8_t *vals)
{
    int lengthAdded = 0;
    this->set((uint64_t)0);
    for (int i = 0; i < this->encodingLength; i++)
    {
        // expanded version
        // int pos = this->totalBits - this->encoding[i] - lengthAdded; // the position in the bits
        // int maxVal = (1 << this->encoding[i]) - 1; // the maximum value of this number
        // int val = vals[i] & maxVal; // truncate the given number in case it is larger than the max val
        // this->num += val << pos; // move the number to the correct position to add

        // need to cast num to the correct type
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
        // keep track of where we are in the number
        lengthAdded += this->encoding[i];
    }
    return true;
}

bool PackedNum::unpack(uint8_t *vals)
{
    int lengthRemoved = 0;
    for (int i = 0; i < this->encodingLength; i++)
    {
        // expanded version
        // int pos = (1 << (this->totalBits - lengthRemoved)) - 1; // a binary number containing 0 to the left of the number and 1 to the right and including the number
        // int valPlusTheRight = this->num & pos; // remove everything to the left of the number
        // int shift = this->totalBits - this->encoding[i] - lengthRemoved; // how many bits we need to shift to get just the number
        // vals[i] = valPlusTheRight >> shift; // shift to get just the number

        // need to cast num to the correct type
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
        // keep track of where we are in the number
        lengthRemoved += this->encoding[i];
    }
    return true;
}

void PackedNum::set(uint64_t n)
{
    // cast num to the correct type and set to n
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

bool PackedNum::set(uint8_t *arr)
{
    this->set((uint64_t)0);
    // this is a special case of pack where the encoding is all 8's
    for (int i = 0; i < this->totalBytes; i++)
    {
        // expanded version
        // from unpack, lengthAdded becomes i * 8
        // this->encoding[i] becomes 8
        // int pos = this->totalBits - this->encoding[i] - lengthAdded; // the position in the bits
        // int maxVal = (1 << this->encoding[i]) - 1; // the maximum value of this number
        // int val = vals[i] & maxVal; // truncate the given number in case it is larger than the max val
        // this->num += val << pos; // move the number to the correct position to add

        // need to cast num to the correct type
        switch (this->type)
        {
        case PT_UINT8:
            *(uint8_t *)this->num += (arr[i] & ((1 << 8) - 1)) << (this->totalBits - 8 * (i + 1));
            break;
        case PT_UINT16:
            *(uint16_t *)this->num += (arr[i] & ((1 << 8) - 1)) << (this->totalBits - 8 * (i + 1));
            break;
        case PT_UINT32:
            *(uint32_t *)this->num += (arr[i] & ((1 << 8) - 1)) << (this->totalBits - 8 * (i + 1));
            break;
        case PT_UINT64:
            *(uint64_t *)this->num += (arr[i] & ((1 << 8) - 1)) << (this->totalBits - 8 * (i + 1));
            break;
        default:
            return false;
        }
    }
    return true;
}

uint64_t PackedNum::get()
{
    // cast num to the correct type and truncate
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

bool PackedNum::get(uint8_t *arr)
{
    // this is a special case of unpack where the encoding is all 8's
    for (int i = 0; i < this->totalBytes; i++)
    {
        // expanded version
        // from unpack, lengthRemoved becomes i * 8
        // this->encoding[i] becomes 8
        // int pos = (1 << (this->totalBits - lengthRemoved)) - 1; // a binary number containing 0 to the left of the number and 1 to the right and including the number
        // int valPlusTheRight = this->num & pos; // remove everything to the left of the number
        // int shift = this->totalBits - this->encoding[i] - lengthRemoved; // how many bits we need to shift to get just the number
        // vals[i] = valPlusTheRight >> shift; // shift to get just the number

        // need to cast num to the correct type
        switch (this->type)
        {
        case PT_UINT8:
            arr[i] = (*(uint8_t *)this->num & ((1 << (this->totalBits - i * 8)) - 1)) >> (this->totalBits - 8 * (i + 1));
            break;
        case PT_UINT16:
            arr[i] = (*(uint16_t *)this->num & ((1 << (this->totalBits - i * 8)) - 1)) >> (this->totalBits - 8 * (i + 1));
            break;
        case PT_UINT32:
            arr[i] = (*(uint32_t *)this->num & ((1 << (this->totalBits - i * 8)) - 1)) >> (this->totalBits - 8 * (i + 1));
            break;
        case PT_UINT64:
            arr[i] = (*(uint64_t *)this->num & ((1 << (this->totalBits - i * 8)) - 1)) >> (this->totalBits - 8 * (i + 1));
            break;
        default:
            return false;
        }
    }
    return true;
}