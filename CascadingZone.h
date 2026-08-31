#ifndef CASCADING_ZONE_H
#define CASCADING_ZONE_H

#include "EventComposite.h"
#include "Subject.h"
#include "Observer.h"
#include <string>

class CascadingZone : public EventComposite, public Subject, public Observer
{
private:
    std::string zoneName;

public:
    explicit CascadingZone(std::string name);
    ~CascadingZone() override = default;

    void update(const EventNotice &notice) override;
    void issueNotice(NoticeType type, const std::string &msg, int severity = 1);

    /**
     * @brief Task 4.2 runtime reorganisation. Moves ownership of `child`
     * from this zone to `newParent` (via the inherited EventComposite
     * add()/remove()), and, if `child` is also registered as an observer
     * of this zone, moves that registration to `newParent` too (via the
     * inherited Subject attach()/detach()). Both the Composite change and
     * the Observer change are handled together so neither is forgotten.
     * @param child Component currently owned by this zone. Must not be nullptr.
     * @param newParent Zone that will take ownership. Must not be nullptr or this.
     */
    void transferChild(EventComponent *child, CascadingZone *newParent);

    std::string getName() const { return zoneName; }
};

#endif // CASCADING_ZONE_H