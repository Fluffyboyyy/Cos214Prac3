#ifndef EVENT_NOTICE_H
#define EVENT_NOTICE_H

#include <string>

/**
 * @brief The kinds of notice a CascadingZone can issue or relay.
 *
 * Six values are provided, meeting the practical's minimum of six, and
 * covering one ordinary operational change (OPEN/CLOSE/SCHEDULE_CHANGE),
 * one capacity-related change (CAPACITY_ALERT) and one safety-related
 * change (WEATHER_ALERT/EVACUATE). There is no separate PAUSE/RESUME
 * pair: OPEN is used consistently across EventFlow as the "return to
 * normal operation" signal, so every leaf only needs to react to one
 * "things are fine again" case rather than two overlapping ones.
 */
enum class NoticeType
{
    OPEN,            ///< Resume/open: return to normal operation.
    CLOSE,           ///< Close: stop operating until further notice.
    SCHEDULE_CHANGE, ///< An ordinary change to the running order.
    CAPACITY_ALERT,  ///< Crowd or queue figures require a response.
    WEATHER_ALERT,   ///< A weather condition requires a safety response.
    EVACUATE         ///< An emergency evacuation is underway.
};

/**
 * @brief A single piece of event-wide news, passed from a Subject to its
 * Observers.
 *
 * EventFlow uses a push model: whoever issues the notice (see
 * CascadingZone::issueNotice()) packages everything an Observer could
 * need, including a rough severity figure, into this struct and hands it
 * to update(), rather than making the Observer call back into the
 * Subject to ask what happened. EventNotice is a plain struct rather than
 * a class with private fields, since it is an immutable value passed by
 * const reference and has no behaviour of its own to protect.
 */
struct EventNotice
{
    NoticeType type;        ///< What kind of notice this is.
    std::string senderName; ///< Name of the zone or control point that issued it.
    std::string message;    ///< Human-readable description, shown in status output.
    int severityLevel;      ///< Generic severity/urgency figure used by conditional reactions (e.g. SecurityGate's threshold check).

    /**
     * @brief Constructs a notice ready to be passed to Subject::notify().
     * @param t Kind of notice.
     * @param sender Name of the zone or control point issuing it.
     * @param msg Human-readable description.
     * @param severity Severity/urgency figure, defaults to 1 (low).
     */
    EventNotice(NoticeType t, std::string sender, std::string msg, int severity = 1)
        : type(t), senderName(std::move(sender)), message(std::move(msg)), severityLevel(severity) {}
};

#endif // EVENT_NOTICE_H
