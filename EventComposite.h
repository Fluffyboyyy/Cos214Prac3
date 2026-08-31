#ifndef EVENTCOMPOSITE_H
#define EVENTCOMPOSITE_H

#include "EventComponent.h"
#include <string>
#include <vector>

/**
 * @class EventComposite
 * @brief GoF Composite (EventGroup). Owns a collection of EventComponent
 * children, which may themselves be Leaves or further EventComposite
 * groups, and recursively forwards the four common operations to them.
 *
 * Ownership: EventComposite is the sole owner of every pointer in its
 * children vector. Only the composite currently holding a child in that
 * vector may delete it. When a child is moved between composites (see
 * CascadingZone::transferChild()), remove() releases ownership without
 * deleting, so the new parent's add() can take it over cleanly, and the
 * object is never owned by two composites at once.
 */
class EventComposite : public EventComponent
{
private:
    std::string groupName;
    std::vector<EventComponent *> children; ///< Owning pointers: deleted in ~EventComposite().

public:
    /**
     * @brief Constructs an empty group with no children.
     * @param name Display name for this group, used in open()/close()/reportStatus() output.
     */
    explicit EventComposite(std::string name);

    /**
     * @brief Deletes every child currently owned by this composite exactly
     * once, then destroys the (now empty) children vector. Because
     * children are stored as EventComponent*, this delete runs through
     * the correct derived destructor for each child, including further
     * nested EventComposite objects, so an entire subtree is released
     * with a single delete on the root.
     */
    ~EventComposite() override;

    /**
     * @brief Adds `component` as a child of this group and takes
     * ownership of it.
     * @param component Component to add. If nullptr, the call is
     * silently ignored. Ownership passes to this composite.
     */
    void add(EventComponent *component) override;

    /**
     * @brief Removes `component` from this group's children, if present.
     * The removed component is NOT deleted, ownership is simply released;
     * the caller is responsible for the pointer afterwards.
     * @param component Component to remove. If it is not currently a
     * child of this group, the call is safely ignored.
     */
    void remove(EventComponent *component) override;

    /// @brief Opens this group by opening every child, recursively.
    void open() override;

    /// @brief Closes this group by closing every child, recursively.
    void close() override;

    /// @brief Prints this group's name, then each child's status, recursively.
    void reportStatus() const override;

    /**
     * @brief Recursively sums the capacity of every child.
     * @return The total capacity of this group and everything nested inside it.
     */
    int getCapacity() const override;
};

#endif //EVENTCOMPOSITE_H
