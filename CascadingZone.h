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

    std::string getName() const { return zoneName; }
};

#endif // CASCADING_ZONE_H