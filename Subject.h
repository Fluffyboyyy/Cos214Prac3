#ifndef SUBJECT_H
#define SUBJECT_H

#include <vector>
#include "Observer.h"

class Subject
{
private:
    std::vector<Observer *> observers;

public:
    virtual ~Subject() = default;

    virtual void attach(Observer *observer);
    virtual void detach(Observer *observer);

    virtual void notify(const EventNotice &notice);
};

#endif // SUBJECT_H