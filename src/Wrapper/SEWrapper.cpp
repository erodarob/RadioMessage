#include "SEWrapper.h"

int SEWrapper::wrap(uint8_t *prependPos, uint8_t *appendPos)
{
    // set start of message to the start character
    *prependPos = start;
    // set end of message to the end character
    *(appendPos - 1) = end;

    return 1;
}
int SEWrapper::unwrap(uint8_t *prependPos, uint8_t *appendPos)
{
    // check whether the start character and end character are at the start and end of the message
    this->success = *prependPos == this->start && *(appendPos - 1) == this->end;
    if (success)
        return 1;
    return 0;
}

bool SEWrapper::messageComplete(Message *m)
{
    // unwrap the message first
    m->unwrap(this);
    // preserve last result to return, but reset internal variable
    bool result = success;
    success = false;
    return result;
}
