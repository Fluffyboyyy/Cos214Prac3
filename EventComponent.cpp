#include "EventComponent.h"
#include "Observer.h"

/**
 * @class MainStage
 * @brief Concrete Leaf. Also a ConcreteObserver so it can register with
 * whichever CascadingZone contains it and react to a notice.
 *
 * Event rule (Task 4.1): a WEATHER_ALERT or EVACUATE pauses the show
 * (outdoor performances cannot safely continue), but a CAPACITY_ALERT is
 * only logged, since the stage itself has no queue to manage.
 */
class MainStage : public EventComponent, public Observer
{
private:
    std::string name;
    bool isOpen;
    int capacity;

public:
    /**
     * @brief Constructs a stage.
     * @param n Display name.
     * @param cap Audience capacity, returned by getCapacity().
     */
    MainStage(std::string n, int cap) : name(n), capacity(cap), isOpen(false) {}

    /// @brief Starts the performance.
    void open() override
    {
        isOpen = true;
        std::cout << "[Stage] " << name << " is NOW PLAYING!\n";
    }

    /// @brief Stops the performance.
    void close() override
    {
        isOpen = false;
        std::cout << "[Stage] " << name << " has STOPPED.\n";
    }

    /// @brief Prints this stage's name, open/closed status and capacity.
    void reportStatus() const override
    {
        std::cout << "  - Stage: " << name << " | Status: " << (isOpen ? "OPEN" : "CLOSED") << " | Cap: " << capacity << "\n";
    }

    /**
     * @brief This stage's audience capacity.
     * @return The capacity passed to the constructor.
     */
    int getCapacity() const override
    {
        return capacity;
    }

    /**
     * @brief Reacts to a notice. See the class-level event rule above.
     * @param notice The notice being delivered by whichever CascadingZone this stage is attached to.
     */
    void update(const EventNotice &notice) override
    {
        switch (notice.type)
        {
        case NoticeType::WEATHER_ALERT:
        case NoticeType::EVACUATE:
            std::cout << "[Stage] " << name << " pausing the performance: " << notice.message << "\n";
            close();
            break;
        case NoticeType::OPEN:
            std::cout << "[Stage] " << name << " resuming the performance.\n";
            open();
            break;
        case NoticeType::CAPACITY_ALERT:
            std::cout << "[Stage] " << name << " noted a capacity alert but keeps playing.\n";
            break;
        default:
            std::cout << "[Stage] " << name << " has no reaction to this notice.\n";
            break;
        }
    }
};

/**
 * @class SecurityGate
 * @brief Concrete Leaf and ConcreteObserver. Controls attendee admission.
 *
 * Event rule (Task 4.1) and conditional decision (Task 4.3): on a
 * CAPACITY_ALERT the gate checks the notice's severityLevel against its
 * own closeThreshold. At or above the threshold it closes completely;
 * below it, it only restricts to a single VIP fast-track lane (original
 * feature, see activateVipLane()). This is the exact "if severity >=
 * threshold / else" condition Task 5's SD3 alt fragment models.
 */
class SecurityGate : public EventComponent, public Observer
{
private:
    std::string gateName;
    bool processing;
    int maxFlow;
    bool vipLaneOnly = false;                 ///< Original feature: fast-track lane during a moderate alert.
    static const int closeThreshold = 4;      ///< severityLevel at/above which the gate closes fully.

public:
    /**
     * @brief Constructs a gate.
     * @param n Display name.
     * @param flow Maximum attendee flow, returned by getCapacity().
     */
    SecurityGate(std::string n, int flow) : gateName(n), maxFlow(flow), processing(false) {}

    /// @brief Opens the gate to normal admission and clears any VIP-lane restriction.
    void open() override
    {
        processing = true;
        vipLaneOnly = false;
        std::cout << "[Gate] " << gateName << " is NOW OPEN!\n";
    }

    /// @brief Closes the gate entirely and clears any VIP-lane restriction.
    void close() override
    {
        processing = false;
        vipLaneOnly = false;
        std::cout << "[Gate] " << gateName << " is CLOSED.\n";
    }

    /// @brief Prints this gate's name, admission state (open, closed, or VIP-lane-only) and max flow.
    void reportStatus() const override
    {
        std::string state = !processing ? "NO" : (vipLaneOnly ? "VIP LANE ONLY" : "YES");
        std::cout << "  - Gate: " << gateName << " | Processing: " << state << " | Max Flow: " << maxFlow << "\n";
    }

    /**
     * @brief This gate's maximum attendee flow.
     * @return The flow figure passed to the constructor.
     */
    int getCapacity() const override
    {
        return maxFlow;
    }

    /**
     * @brief Original feature (Task 4.4): restricts the gate to a single
     * fast-track lane instead of closing it entirely, for attendees who
     * need to reach a zone urgently (e.g. medical escorts) even while the
     * gate is otherwise limiting admission. Kept private to SecurityGate:
     * no other class needs to know this state exists.
     */
    void activateVipLane()
    {
        processing = true;
        vipLaneOnly = true;
        std::cout << "[Gate] " << gateName << " restricted to VIP fast-track lane only.\n";
    }

    /**
     * @brief Reacts to a notice. See the class-level event rule and
     * conditional decision above.
     * @param notice The notice being delivered by whichever CascadingZone this gate is attached to.
     */
    void update(const EventNotice &notice) override
    {
        switch (notice.type)
        {
        case NoticeType::CAPACITY_ALERT:
            if (notice.severityLevel >= closeThreshold)
            {
                std::cout << "[Gate] " << gateName << " severity at or above threshold, closing fully.\n";
                close();
            }
            else
            {
                activateVipLane();
            }
            break;
        case NoticeType::EVACUATE:
            std::cout << "[Gate] " << gateName << " stopping admission for evacuation: " << notice.message << "\n";
            close();
            break;
        case NoticeType::OPEN:
            std::cout << "[Gate] " << gateName << " resuming normal admission.\n";
            open();
            break;
        default:
            std::cout << "[Gate] " << gateName << " has no reaction to this notice.\n";
            break;
        }
    }
};

/**
 * @class FoodVendor
 * @brief Concrete Leaf and ConcreteObserver. Serves food from a stall.
 *
 * Event rule (Task 4.1): a WEATHER_ALERT or EVACUATE suspends kitchen
 * service, but a CAPACITY_ALERT is ignored, since a vendor's own queue is
 * managed separately from festival-wide crowd figures.
 */
class FoodVendor : public EventComponent, public Observer
{
private:
    std::string vendorName;
    bool serving;
    int queueCap;

public:
    /**
     * @brief Constructs a vendor stall.
     * @param n Display name.
     * @param cap Queue line capacity, returned by getCapacity().
     */
    FoodVendor(std::string n, int cap) : vendorName(n), queueCap(cap), serving(false) {}

    /// @brief Opens the stall for orders.
    void open() override
    {
        serving = true;
        std::cout << "[Vendor] " << vendorName << " is open for food orders.\n";
    }

    /// @brief Halts kitchen service.
    void close() override
    {
        serving = false;
        std::cout << "[Vendor] " << vendorName << " halted kitchen service.\n";
    }

    /// @brief Prints this vendor's name, kitchen state and queue capacity.
    void reportStatus() const override
    {
        std::cout << "  - Vendor: " << vendorName << " | Kitchen: " << (serving ? "ACTIVE" : "PAUSED") << " | Line Cap: " << queueCap << "\n";
    }

    /**
     * @brief This vendor's queue line capacity.
     * @return The capacity passed to the constructor.
     */
    int getCapacity() const override { return queueCap; }

    /**
     * @brief Reacts to a notice. See the class-level event rule above.
     * @param notice The notice being delivered by whichever CascadingZone this vendor is attached to.
     */
    void update(const EventNotice &notice) override
    {
        switch (notice.type)
        {
        case NoticeType::WEATHER_ALERT:
        case NoticeType::EVACUATE:
            std::cout << "[Vendor] " << vendorName << " suspending service: " << notice.message << "\n";
            close();
            break;
        case NoticeType::OPEN:
            std::cout << "[Vendor] " << vendorName << " reopening for orders.\n";
            open();
            break;
        default:
            std::cout << "[Vendor] " << vendorName << " continues trading through this notice.\n";
            break;
        }
    }
};

/**
 * @class FirstAidTent
 * @brief Concrete Leaf and ConcreteObserver. Provides first aid (the
 * MedicalTeam unit).
 *
 * Event rule (Task 4.1): unlike every other leaf, FirstAidTent stays
 * active through a WEATHER_ALERT or EVACUATE, since attendees are more
 * likely to need medical help during an incident, not less.
 */
class FirstAidTent : public EventComponent, public Observer
{
private:
    std::string location;
    bool active;
    int bedCapacity;
    bool backupPowerActive = false;       ///< Original feature: emergency power for a safety-critical unit.
    static const int backupThreshold = 4; ///< severityLevel at/above which backup power kicks in.

public:
    /**
     * @brief Constructs a first aid tent, active by default.
     * @param loc Display name / location.
     * @param beds Bed capacity, returned by getCapacity().
     */
    FirstAidTent(std::string loc, int beds) : location(loc), bedCapacity(beds), active(true) {}

    /// @brief Marks this tent as operational.
    void open() override
    {
        active = true;
        std::cout << "[Medical] First Aid at " << location << " is operational.\n";
    }

    /// @brief Suspends this tent.
    void close() override
    {
        active = false;
        std::cout << "[Medical] First Aid at " << location << " suspended.\n";
    }

    /// @brief Prints this tent's location, operational state (including backup power) and bed capacity.
    void reportStatus() const override
    {
        std::string state = active ? (backupPowerActive ? "ACTIVE (BACKUP POWER)" : "ACTIVE") : "STANDBY";
        std::cout << "  - Medical: " << location << " | Status: " << state << " | Beds: " << bedCapacity << "\n";
    }

    /**
     * @brief This tent's bed capacity.
     * @return The capacity passed to the constructor.
     */
    int getCapacity() const override { return bedCapacity; }

    /**
     * @brief Original feature (Task 4.4): switches this tent to backup
     * power instead of closing, so it can keep treating attendees even if
     * the festival's main power is cut during a severe weather alert.
     * Kept private to FirstAidTent: no other class needs to know backup
     * power exists.
     */
    void activateBackupPower()
    {
        backupPowerActive = true;
        std::cout << "[Medical] " << location << " switching to backup power, remaining operational.\n";
    }

    /**
     * @brief Reacts to a notice. See the class-level event rule above.
     * @param notice The notice being delivered by whichever CascadingZone this tent is attached to.
     */
    void update(const EventNotice &notice) override
    {
        switch (notice.type)
        {
        case NoticeType::WEATHER_ALERT:
            if (notice.severityLevel >= backupThreshold)
            {
                activateBackupPower();
            }
            else
            {
                std::cout << "[Medical] " << location << " remains operational through the weather alert.\n";
            }
            break;
        case NoticeType::EVACUATE:
            std::cout << "[Medical] " << location << " remains operational to assist evacuation.\n";
            break;
        case NoticeType::OPEN:
            backupPowerActive = false;
            std::cout << "[Medical] " << location << " back on main power.\n";
            break;
        default:
            std::cout << "[Medical] " << location << " continues normal operation.\n";
            break;
        }
    }
};

/**
 * @class ShuttleStop
 * @brief Concrete Leaf and ConcreteObserver. Runs a transport route.
 *
 * Event rule (Task 4.1): a ShuttleStop never simply closes on
 * WEATHER_ALERT or EVACUATE the way a stage or vendor does. Instead it
 * reroutes, since transport is exactly what attendees need most during an
 * incident.
 */
class ShuttleStop : public EventComponent, public Observer
{
private:
    std::string routeName;
    bool running;
    int platformCap;
    bool onEmergencyRoute = false; ///< Original feature: dynamic rerouting.

public:
    /**
     * @brief Constructs a shuttle stop.
     * @param r Route name.
     * @param cap Platform capacity, returned by getCapacity().
     */
    ShuttleStop(std::string r, int cap) : routeName(r), platformCap(cap), running(false) {}

    /// @brief Starts boarding on the current route.
    void open() override
    {
        running = true;
        std::cout << "[Transport] Shuttle " << routeName << " is boarding.\n";
    }

    /// @brief Suspends the shuttle.
    void close() override
    {
        running = false;
        std::cout << "[Transport] Shuttle " << routeName << " suspended.\n";
    }

    /// @brief Prints this shuttle's route name, running state (including emergency routing) and platform capacity.
    void reportStatus() const override
    {
        std::string state = !running ? "STOPPED" : (onEmergencyRoute ? "RUNNING (EMERGENCY ROUTE)" : "RUNNING");
        std::cout << "  - Shuttle: " << routeName << " | Transport: " << state << " | Platform Cap: " << platformCap << "\n";
    }

    /**
     * @brief This shuttle stop's platform capacity.
     * @return The capacity passed to the constructor.
     */
    int getCapacity() const override { return platformCap; }

    /**
     * @brief Original feature (Task 4.4): switches to a dedicated
     * emergency route away from the affected zone and keeps the shuttle
     * running, instead of stopping it like every other leaf does.
     */
    void rerouteToEmergencyPath()
    {
        onEmergencyRoute = true;
        running = true;
        std::cout << "[Transport] Shuttle " << routeName << " rerouted to the emergency evacuation path.\n";
    }

    /**
     * @brief Reacts to a notice. See the class-level event rule above.
     * @param notice The notice being delivered by whichever CascadingZone this shuttle stop is attached to.
     */
    void update(const EventNotice &notice) override
    {
        switch (notice.type)
        {
        case NoticeType::EVACUATE:
        case NoticeType::WEATHER_ALERT:
            rerouteToEmergencyPath();
            break;
        case NoticeType::OPEN:
            onEmergencyRoute = false;
            std::cout << "[Transport] Shuttle " << routeName << " back on its normal route.\n";
            open();
            break;
        default:
            std::cout << "[Transport] Shuttle " << routeName << " continues its normal route.\n";
            break;
        }
    }
};
