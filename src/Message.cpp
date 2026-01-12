#include "Message.h"

// constructors

Message::Message(uint8_t *data, uint16_t sz)
{
    // make sure we don't copy more than this->maxSize bytes
    if (sz > this->maxSize)
        this->size = this->maxSize;
    else
        this->size = sz;

    memcpy(this->buf, data, this->size);
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
}

Message *Message::encode(Data *data)
{
    // encode the message
    int status = data->encode(this->buf, this->maxSize);
    if (status > 0)
    {
        this->size = status;
        this->buf[this->size] = 0;
    }
    else
        this->error(status);
    return this;
}

Message *Message::decode(Data *data)
{
    int status = data->decode(this->buf, this->size);
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
        this->error(Message::ERR_ID - 1);
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
        this->error(Message::ERR_ID - 2);
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
        this->error(Message::ERR_ID - 3);
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
        this->error(Message::ERR_ID - 4);
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
        this->size += sz;
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
        this->error(Message::ERR_ID - 5);
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

#ifdef ARDUINO

Message *Message::print(Stream &Serial)
{
    Serial.println((char *)this->buf);
    return this;
}

Message *Message::write(Stream &Serial)
{
    Serial.write(this->buf, this->size);
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