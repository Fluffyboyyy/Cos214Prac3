#ifndef CASCADING_ZONE_H
#define CASCADING_ZONE_H

#include "EventComposite.h"
#include "Subject.h"
#include "Observer.h"
#include <string>

/**
 * @class CascadingZone
 * @brief A Composite group (EventGroup) that also plays both the Subject
 * and Observer roles, so it can be told about a notice by whatever is
 * above it and relay that same notice to whichever of its own registered
 * observers sit below it.
 *
 * These are two separate collaborations, not one pattern doing double
 * duty: the Observer role concerns this zone's relationship with
 * whatever sits above it in the notification chain, and the Subject role
 * concerns its relationship with whatever sits below it. Composite
 * ownership (inherited from EventComposite, via add()/remove()) and
 * Observer registration (inherited from Subject, via attach()/detach())
 * are also kept deliberately separate: adding a component to this zone's
 * children does not automatically register it as an observer, and vice
 * versa. Not every EventComposite needs to cascade notices, only the
 * ones built as a CascadingZone.
 */
class CascadingZone : public EventComposite, public Subject, public Observer
{
private:
    std::string zoneName;

public:
    /**
     * @brief Constructs an empty zone with no children and no registered observers.
     * @param name Display name, used both as the EventComposite group name and as this zone's senderName when it issues a notice.
     */
    explicit CascadingZone(std::string name);

    /// @brief Defaulted virtual destructor. Component ownership and destruction is handled by the inherited EventComposite::~EventComposite().
    ~CascadingZone() override = default;

    /**
     * @brief ConcreteObserver behaviour: react to a notice relayed from
     * whatever this zone is attached to, then cascade it further down by
     * notifying this zone's own registered observers.
     *
     * A CLOSE or EVACUATE notice also closes this zone itself (via the
     * inherited EventComposite::close(), which recursively closes every
     * child), on top of being relayed onward. Other notice types are only
     * relayed, since not every notice implies the relaying zone itself
     * should change state, the leaves further down decide that
     * individually in their own update() overrides.
     *
     * @param notice The notice being cascaded through the tree.
     */
    void update(const EventNotice &notice) override;

    /**
     * @brief ConcreteSubject behaviour: originates a brand new notice from
     * this zone (rather than relaying one received from above) and
     * notifies this zone's registered observers directly.
     * @param type Kind of notice to issue.
     * @param msg Human-readable description.
     * @param severity Severity/urgency figure, defaults to 1 (low).
     */
    void issueNotice(NoticeType type, const std::string &msg, int severity = 1);

    /**
     * @brief Task 4.2 runtime reorganisation. Moves ownership of `child`
     * from this zone to `newParent` (via the inherited EventComposite
     * add()/remove()), and, if `child` is also registered as an observer
     * of this zone, moves that registration to `newParent` too (via the
     * inherited Subject attach()/detach()). Both the Composite change and
     * the Observer change are handled together so neither is forgotten.
     *
     * @note Non-obvious design decision: `child` is tested with
     * dynamic_cast<Observer*> rather than assumed to be an Observer.
     * EventComponent does not require every component to implement
     * Observer, only concrete leaves and CascadingZone happen to, so a
     * safe runtime check is needed before the observer-registration half
     * of the transfer can run. A child that is not an Observer (there are
     * none in the current design, but the interface allows for one) still
     * has its ownership transferred correctly; only the attach()/detach()
     * step is skipped for it.
     *
     * @param child Component currently owned by this zone. Must not be nullptr.
     * @param newParent Zone that will take ownership. Must not be nullptr or this.
     */
    void transferChild(EventComponent *child, CascadingZone *newParent);

    /**
     * @brief This zone's display name.
     * @return The name passed to the constructor.
     */
    std::string getName() const { return zoneName; }
};

#endif // CASCADING_ZONE_H
