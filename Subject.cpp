#include "Subject.h"
#include <algorithm>

void Subject::attach(Observer *observer)
{
    if (!observer)
        return;

    auto it = std::find(observers.begin(), observers.end(), observer);
    if (it == observers.end())
    {
        observers.push_back(observer);
    }
}

void Subject::detach(Observer *observer)
{
    if (!observer)
        return;

    auto it = std::find(observers.begin(), observers.end(), observer);
    if (it != observers.end())
    {
        observers.erase(it);
    }
}

void Subject::notify(const EventNotice &notice)
{
    std::vector<Observer *> snapshot = observers;
    for (Observer *obs : snapshot)
    {
        if (obs)
        {
            obs->update(notice);
        }
    }
}