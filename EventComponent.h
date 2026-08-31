#ifndef EVENTCOMPONENT_H
#define EVENTCOMPONENT_H

#include <string>
#include <iostream>
#include <stdexcept>

/**
 * @class EventComponent
 * @brief GoF Component. The common interface shared by every operational
 * unit (Leaf) and every group of units (Composite) in the EventFlow tree.
 *
 * A client (or a CascadingZone acting as one) can call open(), close(),
 * reportStatus() or getCapacity() on any EventComponent without knowing
 * whether it holds a single leaf or an entire nested group. add() and
 * remove() are declared here too, with a default implementation that
 * refuses the operation, so that leaves do not need their own empty
 * overrides: only classes that actually manage children (EventComposite
 * and its subclasses) need to override them.
 */
class EventComponent
{
public:
    /**
     * @brief Virtual destructor. Required because EventFlow deletes
     * components polymorphically through EventComponent* (see
     * EventComposite::~EventComposite()), so every derived destructor
     * must run.
     */
    virtual ~EventComponent() = default;

    /// @brief Brings this component (and, for a Composite, everything inside it) into an active state.
    virtual void open() = 0;

    /// @brief Takes this component (and, for a Composite, everything inside it) out of an active state.
    virtual void close() = 0;

    /// @brief Prints a human-readable status line for this component to standard output.
    virtual void reportStatus() const = 0;

    /**
     * @brief The capacity this component contributes to the festival.
     * @return For a Leaf, its own fixed capacity figure. For a Composite,
     * the sum of its children's capacities.
     */
    virtual int getCapacity() const = 0;

    /**
     * @brief Adds a child component. Only meaningful for a Composite.
     *
     * Ownership: a successful add() (as overridden by EventComposite)
     * transfers ownership of `component` to the composite it was added
     * to. The composite's destructor will delete it.
     *
     * @param component Component to add. Ignored by this default
     * (leaf) implementation, which always throws instead.
     * @throws std::logic_error always, since a Leaf cannot contain children.
     */
    virtual void add(EventComponent *component)
    {
        (void)component;
        throw std::logic_error("Cannot add child to leaf component.");
    }

    /**
     * @brief Removes a child component. Only meaningful for a Composite.
     *
     * Ownership: removing a child (as overridden by EventComposite) does
     * NOT delete it, it only releases ownership. The caller becomes
     * responsible for the pointer, typically by adding it to a different
     * composite immediately afterwards (see CascadingZone::transferChild()).
     *
     * @param component Component to remove. Ignored by this default
     * (leaf) implementation, which always throws instead.
     * @throws std::logic_error always, since a Leaf cannot contain children.
     */
    virtual void remove(EventComponent *component)
    {
        (void)component;
        throw std::logic_error("Cannot remove child from leaf component.");
    }
};
#endif //EVENTCOMPONENT_H
