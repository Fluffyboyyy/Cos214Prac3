#ifndef EVENT_NOTICE_H
#define EVENT_NOTICE_H

#include <string>

enum class NoticeType
{
    OPEN,
    CLOSE,
    SCHEDULE_CHANGE,
    CAPACITY_ALERT,
    WEATHER_ALERT,
    EVACUATE
};

struct EventNotice
{
    NoticeType type;
    std::string senderName;
    std::string message;
    int severityLevel;

    EventNotice(NoticeType t, std::string sender, std::string msg, int severity = 1)
        : type(t), senderName(std::move(sender)), message(std::move(msg)), severityLevel(severity) {}
};

#endif // EVENT_NOTICE_H