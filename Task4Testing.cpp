#include "EventComponent.h"
#include "CascadingZone.h"
#include "EventComponent.cpp"
#include <iostream>

/**
 * @brief Task 4 demonstration: event rules, a conditional decision and
 * runtime reorganisation, built on top of the Task 2 Composite structure
 * and Task 3's CascadingZone/Subject/Observer/EventNotice classes.
 */
void testTask4()
{
    // ---- Build the same small festival tree as Task 2, using CascadingZone
    // for every group so each zone can both own its children (Composite)
    // and relay notices to whichever of them registered as observers. ----
    CascadingZone *rootFestival = new CascadingZone("City Lights Festival");
    CascadingZone *mainZone = new CascadingZone("Main Stage Zone");
    CascadingZone *riverZone = new CascadingZone("River Side Zone");
    CascadingZone *foodCourt = new CascadingZone("Food Court Precinct");

    MainStage *stage = new MainStage("Grand Stage", 15000);
    SecurityGate *gate = new SecurityGate("North Gate", 2000);
    FirstAidTent *medical = new FirstAidTent("North Entrance Medical", 50);
    FoodVendor *taco = new FoodVendor("Taco Stand", 300);
    ShuttleStop *shuttle = new ShuttleStop("West Bus Route", 1200);

    foodCourt->add(taco);
    riverZone->add(foodCourt);
    riverZone->add(shuttle);
    mainZone->add(stage);
    mainZone->add(gate);
    mainZone->add(medical);
    rootFestival->add(mainZone);
    rootFestival->add(riverZone);

    // Observer registration is a deliberately separate step from Composite
    // ownership (Task 1, 1.4(c)/(d)): adding a child above does not
    // automatically register it as an observer.
    mainZone->attach(stage);
    mainZone->attach(gate);
    mainZone->attach(medical);
    riverZone->attach(shuttle);
    foodCourt->attach(taco);
    riverZone->attach(foodCourt); // foodCourt is itself an Observer of riverZone
    rootFestival->attach(mainZone);
    rootFestival->attach(riverZone);

    // ---- 4.1: five concrete leaf types reacting differently to one notice ----
    std::cout << "\n===== 4.1  One WEATHER_ALERT, five different reactions =====\n";
    // Cascades: rootFestival -> mainZone/riverZone -> their own registered
    // observers, including riverZone -> foodCourt -> taco (3 runtime levels).
    rootFestival->issueNotice(NoticeType::WEATHER_ALERT, "Severe storm cell approaching from the west.", 5);
    rootFestival->reportStatus();

    // ---- 4.3: a condition-based decision inside SecurityGate ----
    std::cout << "\n===== 4.3  SecurityGate deciding on a severity threshold =====\n";
    EventNotice mildCapacity(NoticeType::CAPACITY_ALERT, "Gate Monitor", "North Gate queue building up.", 2);
    gate->update(mildCapacity); // below closeThreshold (4): VIP lane only
    EventNotice severeCapacity(NoticeType::CAPACITY_ALERT, "Gate Monitor", "North Gate queue at breaking point.", 4);
    gate->update(severeCapacity); // at/above closeThreshold: closes fully

    // ---- 4.2: runtime reorganisation ----
    std::cout << "\n===== 4.2  Runtime reorganisation: moving the Taco Stand =====\n";
    foodCourt->transferChild(taco, mainZone); // updates ownership AND observer registration
    mainZone->reportStatus();

    // ---- OPEN notice shows the tree recovering, including the transferred unit ----
    std::cout << "\n===== OPEN notice restoring normal operations =====\n";
    rootFestival->issueNotice(NoticeType::OPEN, "Storm has passed. Resuming normal festival operations.", 1);
    rootFestival->reportStatus();

    std::cout << "\nTotal Festival Capacity: " << rootFestival->getCapacity() << " people\n";

    delete rootFestival;
}
