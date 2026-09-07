#include "SEWrapper.h"

int SEWrapper::wrap(uint8_t *prependPos, uint8_t *appendPos)
{
    *prependPos = start;
    *(appendPos - 1) = end;

    return 1;
}
int SEWrapper::unwrap(uint8_t *prependPos, uint8_t *appendPos)
{
    this->success = *prependPos == this->start && *(appendPos - 1) == this->end;
    if (success)
        return 1;
    return 0;
}

bool SEWrapper::messageComplete(Message *m)
{
    m->unwrap(this);
    // preserve last result to return, but reset internal variable
    bool result = success;
    success = false;
    return result;
}
