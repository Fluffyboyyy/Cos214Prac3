#ifndef SUBJECT_H
#define SUBJECT_H

#include <vector>
#include "Observer.h"

/**
 * @class Subject
 * @brief GoF Subject role. Owns the (non-owning, see Observer.h) list of
 * registered Observers and is responsible for notifying them.
 *
 * Registration policy: attaching an already-registered Observer is a
 * harmless no-op (it will not receive update() twice for one notify()
 * call), and detaching an Observer that is not currently registered is
 * safely ignored.
 */
class Subject
{
private:
    std::vector<Observer *> observers; ///< Non-owning pointers, see Observer.h.

public:
    /// @brief Virtual destructor. Does not delete the observers it references, see Observer.h.
    virtual ~Subject() = default;

    /**
     * @brief Registers `observer` to receive future notify() calls.
     * @param observer Observer to register. If nullptr, or if it is
     * already registered, the call is safely ignored.
     */
    virtual void attach(Observer *observer);

    /**
     * @brief Stops `observer` receiving future notify() calls.
     * @param observer Observer to remove. If nullptr, or if it was never
     * registered, the call is safely ignored.
     */
    virtual void detach(Observer *observer);

    /**
     * @brief Calls update(notice) on every currently registered observer.
     * @param notice The notice to deliver.
     */
    virtual void notify(const EventNotice &notice);
};

#endif // SUBJECT_H
