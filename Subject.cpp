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
    // Non-obvious design decision: notify() iterates over a snapshot copy
    // of the observers list rather than the member vector directly. An
    // observer's own update() is free to attach() or detach() observers
    // on this same Subject (CascadingZone::update() does exactly this
    // indirectly, by cascading into further notify() calls elsewhere in
    // the tree). If notify() walked the live `observers` vector while a
    // callback mutated it, an iterator could be invalidated mid-loop.
    // Copying first means every observer that was registered at the start
    // of this notify() call is delivered to exactly once, and any
    // attach()/detach() triggered by a callback only takes effect for the
    // *next* notify() call.
    std::vector<Observer *> snapshot = observers;
    for (Observer *obs : snapshot)
    {
        if (obs)
        {
            obs->update(notice);
        }
    }
}
