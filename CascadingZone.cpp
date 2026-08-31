#include "CascadingZone.h"
#include <iostream>

CascadingZone::CascadingZone(std::string name)
    : EventComposite(name), zoneName(std::move(name)) {}

void CascadingZone::update(const EventNotice &notice)
{
    std::cout << "[CascadingZone: " << zoneName
              << "] Received notice '" << notice.message
              << "' from " << notice.senderName
              << ". Cascading down to registered child observers\n";

    if (notice.type == NoticeType::CLOSE || notice.type == NoticeType::EVACUATE)
    {
        this->close();
    }

    Subject::notify(notice);
}

void CascadingZone::issueNotice(NoticeType type, const std::string &msg, int severity)
{
    EventNotice notice(type, zoneName, msg, severity);
    std::cout << "\n>>> [ORIGIN: " << zoneName << "] Broadcaster issuing notice: " << msg << " <<<\n";
    Subject::notify(notice);
}

void CascadingZone::transferChild(EventComponent *child, CascadingZone *newParent)
{
    if (!child || !newParent || newParent == this)
    {
        return;
    }

    this->remove(child); // ownership leaves this zone; child is NOT deleted

    Observer *childObserver = dynamic_cast<Observer *>(child);
    if (childObserver)
    {
        this->detach(childObserver); // safe no-op if it was never registered here
    }

    newParent->add(child); // ownership moves to the new zone
    if (childObserver)
    {
        newParent->attach(childObserver); // observer registration moves to the new zone
    }

    std::cout << "[Reorg] Unit transferred from " << zoneName << " to " << newParent->getName() << ".\n";
}