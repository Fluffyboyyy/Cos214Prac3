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