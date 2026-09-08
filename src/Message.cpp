#include "Message.h"

// constructors

Message::Message()
{
    this->wrs = new Wrapper *[0];
}

Message::Message(uint8_t *data, uint16_t sz)
{
    // make sure we don't copy more than this->maxSize bytes
    if (sz > this->maxSize)
        this->size = this->maxSize;
    else
        this->size = sz;

    memcpy(this->buf, data, this->size);

    this->wrs = new Wrapper *[0];
}

Message::Message(Data *data)
{
    // encode the given data, checking for errors
    int status = data->encode(this->buf, this->maxSize);
    if (status > 0)
    {
        this->size = status;
        this->buf[this->size] = 0;
    }
    else
        this->error(status);

    this->wrs = new Wrapper *[0];
}

Message::~Message()
{
    if (this->wrs != nullptr)
    {
        delete[] this->wrs;
    }
}

Message *Message::encode(Data *data)
{
    // encode the message
    int status = data->encode(this->buf + this->prependLen, this->maxSize - this->prependLen - this->appendLen);
    if (status > 0)
    {
        this->size = status + this->prependLen + this->appendLen;
        this->buf[this->size] = 0;

        // handle wrappers
        this->wrap();
    }
    else
        this->error(status);
    return this;
}

Message *Message::decode(Data *data)
{
    // handle wrappers
    this->unwrap();
    // decode data
    int status = data->decode(this->buf + this->prependLen, this->size - this->prependLen - this->appendLen);
    if (status <= 0)
        this->error(status);
    return this;
}

Message *Message::clear()
{
    // reset the message to all \0 and set the size to 0
    memset(this->buf, 0, this->maxSize);
    this->size = 0;
    return this;
}

Message *Message::wrap()
{
    // we know there is enough space
    uint16_t cumPrependLen = 0;
    uint16_t cumAppendLen = 0;
    for (uint16_t i = 0; i < numWrappers; i++)
    {
        if (this->wrs[i]->enabled)
        {
            // prepend pointer is at the start of the prepended data
            // append pointer is at the end of the appended data
            uint16_t prependLen = this->wrs[i]->prependLen();
            uint16_t appendLen = this->wrs[i]->appendLen();
            int res = this->wrs[i]->wrap(this->buf + this->prependLen - cumPrependLen - prependLen,
                                         this->buf + this->size - (this->appendLen - cumAppendLen - appendLen));
            if (res > 0)
            {
                cumPrependLen += prependLen;
                cumAppendLen += appendLen;
            }
            else
            {
                // error wrapping failure
                // add two errors, one indicating position in code, the next indicating wrapper index in the array
                this->error(Message::ERR_ID - 1);
                this->error(i);
                // don't return since this shouldn't impact later wrappers
            }
        }
    }

    // set size to the correct value, up until now size was the size of the encoded Data
    this->buf[this->size] = 0;

    return this;
}

Message *Message::unwrap()
{
    if (this->size > 0)
    {
        // size includes the encoded data and the wrappers
        uint16_t cumPrependLen = 0;
        uint16_t cumAppendLen = 0;
        // going backwards
        for (int i = numWrappers - 1; i >= 0; i--)
        {
            if (this->wrs[i]->enabled)
            {
                // make sure wrapper was not already unwrapped
                // and there is at least enough space for this wrapper
                if (this->size >= cumPrependLen + this->wrs[i]->prependLen() + cumAppendLen + this->wrs[i]->appendLen())
                {
                    // prepend pointer is at the start of the prepended data
                    // append pointer is at the end of the appended data
                    // check if this wrapper was unwrapped successfully
                    if (!this->wrs[i]->unwrap(this->buf + cumPrependLen, this->buf + this->size - cumAppendLen) > 0)
                    {
                        // error unwrapping failure
                        // add two errors, one indicating position in code, the next indicating wrapper index in the array
                        this->error(Message::ERR_ID - 2);
                        this->error(i);
                        // don't return since this shouldn't impact later wrappers
                    }
                    cumPrependLen += this->wrs[i]->prependLen();
                    cumAppendLen += this->wrs[i]->appendLen();
                }
                else
                {
                    // error not enough data
                    // add two errors, one indicating position in code, the next indicating wrapper index in the array
                    this->error(Message::ERR_ID - 3);
                    this->error(i);
                    return this;
                }
            }
        }
    }
    else
    {
        // error size is zero
        this->error(Message::ERR_ID - 4);
        return this;
    }
    return this;
}

Message *Message::unwrap(Wrapper *wr)
{
    // check if wrapper is enabled
    if (!wr->enabled)
        return this;

    uint16_t cumPrependLen = 0;
    uint16_t cumAppendLen = 0;
    for (uint16_t i = 0; i < numWrappers; i++)
    {
        // check if this pointer is in the list
        if (wr == this->wrs[i])
        {
            // can only unwrap wrappers with a prepended component out of order
            if (this->wrs[i]->prependLen() > 0)
            {
                // dumb size check
                // ignores the size of the message itself, but prevents reading a prepended section as part of an appended section
                // if wrapper has no append section
                // message must be longer than all earlier prepended wrappers, including this wrapper
                // if the wrapper has an append section
                // message must be longer than the entire prepend section and any preceeding append sections
                if (this->size >= (this->prependLen - cumPrependLen) &&
                    (this->wrs[i]->appendLen() == 0 || this->size > this->prependLen + cumAppendLen + this->wrs[i]->appendLen()))
                {
                    if (!this->wrs[i]->unwrap(this->buf + (this->prependLen - cumPrependLen - this->wrs[i]->prependLen()),
                                              this->buf + this->size - (this->appendLen - cumAppendLen - this->wrs[i]->appendLen())))
                    {
                        // error unwrapping failure
                        // the wrapper where this error occurs is given, so no need to indicate index here
                        this->error(Message::ERR_ID - 5);
                    }
                    // done since there is only one wrapper to unwrap
                    return this;
                }
                else
                {
                    // error not enough data
                    this->error(Message::ERR_ID - 6);
                    return this;
                }
            }
            else
            {
                // error wrapper is an appended wrapper
                this->error(Message::ERR_ID - 7);
                return this;
            }
        }

        // always keep track of cumulative length so that we know where to look for a wrapper in the middle of the list
        cumPrependLen += this->wrs[i]->prependLen();
        cumAppendLen += this->wrs[i]->appendLen();
    }

    // error wrapper not found
    this->error(Message::ERR_ID - 8);
    return this;
}

Message *Message::append(uint8_t *data, uint16_t sz)
{
    // check if there's enough space to add the data
    if (this->size + sz <= this->maxSize)
    {
        // add the data, starting at this->size and copying sz bytes
        this->fill(data, this->size, sz);
        this->buf[this->size] = 0;
    }
    else
    {
        this->error(Message::ERR_ID - 9);
    }
    return this;
}

Message *Message::append(uint8_t data)
{
    // check if there's enough space to add the data
    if (this->size + 1 <= this->maxSize)
    {
        // add the data to the end of the buffer
        this->buf[this->size++] = data;

        this->buf[this->size] = 0;
    }
    else
    {
        this->error(Message::ERR_ID - 10);
    }
    return this;
}

Message *Message::pop(uint8_t *data, uint16_t &sz)
{
    // check if the message is longer than sz
    if (this->size - sz > 0)
    {
        // put sz bytes in data, starting at this->size - sz
        if (data != nullptr)
        {
            this->get(data, sz, this->size - sz);
            data[sz] = 0;
        }

        // "remove" copied bytes
        this->size -= sz;
        this->buf[this->size] = 0;
    }
    // if the message is shorter than sz
    else if (this->size > 0)
    {
        // put this->size bytes in data
        sz = this->size;
        if (data != nullptr)
        {
            this->get(data, sz);
            data[sz] = 0;
        }

        // "remove" copied bytes
        this->size -= sz;
        this->buf[this->size] = 0;
    }
    else
    {
        sz = 0;
        this->error(Message::ERR_ID - 11);
    }
    return this;
}

Message *Message::shift(uint8_t *data, uint16_t &sz)
{
    // check if the message is longer than sz
    if (this->size - sz > 0)
    {
        // put sz bytes in data, starting at 0
        if (data != nullptr)
        {
            this->get(data, sz);
            data[sz] = 0;
        }

        // "remove" copied bytes
        // shift the array
        this->size -= sz;
        memcpy(this->buf, this->buf + sz, this->size);
        this->buf[this->size] = 0;
    }
    // if the message is shorter than sz
    else if (this->size > 0)
    {
        // put this->size bytes in data, starting at 0
        sz = this->size;
        if (data != nullptr)
        {
            this->get(data, sz);
            data[sz] = 0;
        }

        // "remove" copied bytes
        this->size -= sz;
        // just in case this->size bytes were not copied, otherwise technically this should do nothing
        memcpy(this->buf, this->buf + sz, this->size);
        this->buf[this->size] = 0;
    }
    else
    {
        sz = 0;
        this->error(Message::ERR_ID - 12);
    }
    return this;
}

// utility methods

Message *Message::fill(uint8_t *data, uint16_t sz)
{
    this->fill(data, 0, sz);
    return this;
}

Message *Message::fill(uint8_t *data, uint16_t start, uint16_t sz)
{
    // check if start + sz is larger than this->maxSize
    if (start + sz > this->maxSize)
    {
        // copy only this->maxSize bytes
        memcpy(this->buf + start, data, this->maxSize - start);
        this->size = this->maxSize;
    }
    // if start + sz is smaller than this->maxSize
    else
    {
        // copy sz bytes
        memcpy(this->buf + start, data, sz);
        this->size = start + sz;
    }
    return this;
}

Message *Message::get(uint8_t *data)
{
    // simply copy the whole message into data
    memcpy(data, this->buf, this->size);
    return this;
}

Message *Message::get(uint8_t *data, uint16_t &sz)
{
    // put sz bytes in data, starting at 0 and ending at sz
    this->get(data, sz, 0, sz);
    return this;
}

Message *Message::get(uint8_t *data, uint16_t &sz, uint16_t start)
{
    // put sz bytes in data, starting at start and ending at start + sz
    this->get(data, sz, start, start + sz);
    return this;
}

Message *Message::get(uint8_t *data, uint16_t &sz, uint16_t start, uint16_t end)
{
    // make sure the start index is less than the end index and that in total they are less than this->size
    if (start < end && end - start > this->size)
    {
        // copy the until the end of the message, setting sz to the number of bytes copied
        sz = this->size - start;
        memcpy(data, this->buf + start, sz);
    }
    // if the total is less than size and start is less than end
    else if (start < end)
    {
        // copy end - start bytes into data
        sz = end - start;
        memcpy(data, this->buf + start, sz);
    }
    else
    {
        sz = 0;
        this->error(Message::ERR_ID - 13);
    }
    return this;
}

bool Message::hasError()
{
    return this->err;
}

char *Message::errors()
{
    this->err = false;
    return this->errStr;
}

void Message::error(int errVal)
{
    if (!this->err)
    {
        this->err = true;
        snprintf(errStr, sizeof(errStr), "!! Error:%d\n", errVal);
    }
    else
    {
        int len = strlen(errStr);
        snprintf(errStr + len - 1, sizeof(errStr) - len, ",%d\n", errVal);
    }
}

Message *Message::reg(Wrapper *wr)
{
    // allocate memory in the array for the new wrapper
    Wrapper **newWrs = new Wrapper *[numWrappers + 1];

    if (this->wrs != nullptr)
    {
        for (uint16_t i = 0; i < numWrappers; i++)
        {
            newWrs[i] = this->wrs[i];
        }

        delete[] this->wrs;
    }
    newWrs[numWrappers] = wr;
    numWrappers++;
    this->wrs = newWrs;

    this->prependLen += wr->prependLen();
    this->appendLen += wr->appendLen();

    return this;
}

Message *Message::unreg(Wrapper *wr)
{
    // decrease array size for removed wrapper
    Wrapper **newWrs = new Wrapper *[numWrappers - 1];

    // wrap everything in a nullptr check
    // cause if the wrapper array doesn't exist, there's nothing to do anyway
    if (this->wrs != nullptr)
    {
        // find the index of the wrapper to be removed
        int wrFound = -1;
        for (uint16_t i = 0; i < numWrappers; i++)
        {
            if (this->wrs[i] == wr)
            {
                wrFound = i;
                break;
            }
        }

        if (wrFound >= 0)
        {
            // assemble new array
            for (uint16_t i = 0; i < numWrappers; i++)
            {
                // if before wrapper to be removed, indices are equal
                if (i < wrFound)
                {
                    newWrs[i] = this->wrs[i];
                }
                // if after newWrs is one less than wrs
                if (i > wrFound)
                {
                    newWrs[i - 1] = this->wrs[i];
                }
            }
            delete[] this->wrs;
        }
        else
        {
            // warning, wrapper not previously registered
            // don't need to actually create an error here
            return this;
        }

        numWrappers--;
        this->wrs = newWrs;

        this->prependLen -= wr->prependLen();
        this->appendLen -= wr->appendLen();
    }

    return this;
}

Message *Message::enable(Wrapper *wr)
{
    wr->enabled = true;
}

Message *Message::disable(Wrapper *wr)
{
    wr->enabled = false;
}

#ifdef ARDUINO

Message *Message::print(Stream &s)
{
    s.println((char *)this->buf);
    return this;
}

Message *Message::write(Stream &s)
{
    s.write(this->buf, this->size);
    return this;
}

#elif defined(_WIN32) || defined(_WIN64) || defined(__unix__) || defined(__APPLE__)

Message *Message::print()
{
    printf("%s\n", (char *)this->buf);
    return this;
}

Message *Message::write()
{
    for (int i = 0; i < this->size; i++)
        printf("%c", this->buf[i]);
    return this;
}

#endif