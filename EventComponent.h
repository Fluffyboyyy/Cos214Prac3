#ifndef EVENTCOMPONENT_H
#define EVENTCOMPONENT_H

#include <string>
#include <iostream>

class EventComponent
{
public:
    virtual ~EventComponent() = default;

    virtual void open() = 0;
    virtual void close() = 0;
    virtual void reportStatus() const = 0;
    virtual int getCapacity() const = 0;

    // Composite management operations
    virtual void add(EventComponent *component)
    {
        throw std::logic_error("Cannot add child to leaf component.");
    }
    virtual void remove(EventComponent *component)
    {
        throw std::logic_error("Cannot remove child from leaf component.");
    }
};
#endif //EVENTCOMPONENT_H