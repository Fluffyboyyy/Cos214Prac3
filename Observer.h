#ifndef OBSERVER_H
#define OBSERVER_H

#include "EventNotice.h"

class Observer
{
public:
    virtual ~Observer() = default;
    virtual void update(const EventNotice &notice) = 0;
};

#endif // OBSERVER_H