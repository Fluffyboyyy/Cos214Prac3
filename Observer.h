#ifndef OBSERVER_H
#define OBSERVER_H

#include "EventNotice.h"

/**
 * @class Observer
 * @brief GoF Observer role. Anything that needs to react to an
 * EventNotice implements update().
 *
 * @note Non-obvious design decision: ownership. A Subject stores
 * Observer* in a plain std::vector<Observer*> (see Subject.h) and never
 * deletes them. Registering as an observer is only ever a statement of
 * interest, never a transfer of ownership: an Observer's real lifetime is
 * decided by whoever structurally owns it in the Composite tree (its
 * parent EventComposite, or, for a CascadingZone, whichever composite
 * owns it as a component). Keeping Subject's pointers non-owning avoids a
 * conflict between two different classes both believing they are
 * responsible for deleting the same object, which is exactly the kind of
 * ambiguity that leads to a double free or a leak.
 */
class Observer
{
public:
    /// @brief Virtual destructor, required for safe polymorphic deletion of any Observer.
    virtual ~Observer() = default;

    /**
     * @brief Called by a Subject when something this observer registered
     * interest in has happened.
     * @param notice The notice describing what happened. Passed by const
     * reference since Observer does not own or outlive the notice.
     */
    virtual void update(const EventNotice &notice) = 0;
};

#endif // OBSERVER_H
