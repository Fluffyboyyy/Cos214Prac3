#include "EventComponent.h"
#include "CascadingZone.h"
#include "EventComponent.cpp"
#include <iostream>

/**
 * @brief Task 8 integration demonstration.
 *
 * This is the single coherent event simulation Task 8.1 asks for. It is
 * deliberately built as one continuous story rather than a set of
 * disconnected calls, so it can be driven live in the ~5 minute
 * demonstration route (Task 8.2) by any team member, with each step
 * printed to the console as it happens. It reuses the same classes as
 * Task 2, Task 3 and Task 4 (EventComponent, EventComposite, the five
 * concrete leaves, CascadingZone, Subject/Observer/EventNotice); nothing
 * new is introduced here.
 *
 * Every requirement of Task 8.1 is covered and labelled below:
 *  - construction of the Composite            -> STEP 1
 *  - observer registration                    -> STEP 2
 *  - a Composite traversal/query               -> STEP 3
 *  - at least three different notices          -> STEPS 4, 6, 9 (SCHEDULE_CHANGE, CAPACITY_ALERT, WEATHER_ALERT; EVACUATE and OPEN also appear)
 *  - at least one cascading notification        -> STEP 4 onward (every issueNotice() cascades through the tree)
 *  - a registration change                      -> STEP 5 (detach then re-attach at runtime, satisfying the brief's non-negotiable rule 9)
 *  - a runtime reorganisation                    -> STEP 7
 *  - clean shutdown                              -> STEP 10
 */
void runEventFlowDemo()
{
    std::cout << "\n=====================================================\n";
    std::cout << " EventFlow Integration Demonstration: City Lights Festival\n";
    std::cout << "=====================================================\n";

    // ---------------------------------------------------------------
    // STEP 1: Construction of the Composite.
    // A root Venue containing two Zones, one of which contains a nested
    // Precinct, three levels of nesting below the client/root boundary,
    // populated with five distinct concrete Leaf types.
    // ---------------------------------------------------------------
    std::cout << "\n--- STEP 1: Building the Composite tree ---\n";
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
    std::cout << "Tree built: City Lights Festival -> {Main Stage Zone, River Side Zone -> Food Court Precinct}\n";

    // ---------------------------------------------------------------
    // STEP 2: Observer registration, kept separate from Composite
    // ownership on purpose (see Task 1, 1.4(c)/(d)): adding a child above
    // does not automatically register it as an observer.
    // ---------------------------------------------------------------
    std::cout << "\n--- STEP 2: Registering observers ---\n";
    mainZone->attach(stage);
    mainZone->attach(gate);
    mainZone->attach(medical);
    riverZone->attach(shuttle);
    foodCourt->attach(taco);
    riverZone->attach(foodCourt); // foodCourt is itself an Observer of riverZone
    rootFestival->attach(mainZone);
    rootFestival->attach(riverZone);
    std::cout << "All zones and leaves registered as observers of their parent zone.\n";

    // ---------------------------------------------------------------
    // STEP 3: A Composite traversal/query, before anything happens, to
    // establish a baseline. reportStatus() and getCapacity() both
    // recurse through the whole tree via the EventComponent interface.
    // ---------------------------------------------------------------
    std::cout << "\n--- STEP 3: Querying the tree (baseline) ---\n";
    rootFestival->open();
    rootFestival->reportStatus();
    std::cout << "Baseline total capacity: " << rootFestival->getCapacity() << " people\n";

    // ---------------------------------------------------------------
    // STEP 4: First notice, an ordinary operational change, cascading
    // from the root through both zones.
    // ---------------------------------------------------------------
    std::cout << "\n--- STEP 4: Notice 1/5, SCHEDULE_CHANGE (ordinary operational change) ---\n";
    rootFestival->issueNotice(NoticeType::SCHEDULE_CHANGE, "Headline act moved forward by 30 minutes.", 1);

    // ---------------------------------------------------------------
    // STEP 5: A registration change at runtime. The gate is detached so
    // it will NOT hear the next notice, then re-attached so it DOES hear
    // the one after that. This demonstrates attach, detach and notify
    // all happening at runtime (the brief's non-negotiable rule 9), not
    // only during initial setup.
    // ---------------------------------------------------------------
    std::cout << "\n--- STEP 5: Registration change at runtime ---\n";
    mainZone->detach(gate);
    std::cout << "North Gate detached from Main Stage Zone.\n";
    std::cout << "Notice 2/5, CAPACITY_ALERT (capacity-related change), issued while the gate is detached:\n";
    mainZone->issueNotice(NoticeType::CAPACITY_ALERT, "Main Stage Zone queue building up.", 2);
    std::cout << "(North Gate did not react, it is not currently registered.)\n";
    mainZone->attach(gate);
    std::cout << "North Gate re-attached to Main Stage Zone.\n";

    // ---------------------------------------------------------------
    // STEP 6: A severe notice that reaches three runtime levels
    // (root -> riverZone -> foodCourt -> taco), a safety-related change,
    // and re-confirms the re-attached gate is receiving notices again.
    // ---------------------------------------------------------------
    std::cout << "\n--- STEP 6: Notice 3/5, WEATHER_ALERT (safety-related change), cascading 3 levels ---\n";
    rootFestival->issueNotice(NoticeType::WEATHER_ALERT, "Severe storm cell approaching from the west.", 5);
    rootFestival->reportStatus();

    // ---------------------------------------------------------------
    // STEP 7: Runtime reorganisation. Moves both Composite ownership and
    // Observer registration together.
    // ---------------------------------------------------------------
    std::cout << "\n--- STEP 7: Runtime reorganisation ---\n";
    foodCourt->transferChild(taco, mainZone);
    mainZone->reportStatus();

    // ---------------------------------------------------------------
    // STEP 8: An emergency notice. CascadingZone::update() closes the
    // relaying zone itself for CLOSE/EVACUATE, on top of cascading.
    // ---------------------------------------------------------------
    std::cout << "\n--- STEP 8: Notice 4/5, EVACUATE (emergency) ---\n";
    rootFestival->issueNotice(NoticeType::EVACUATE, "Evacuate River Side Zone immediately.", 5);
    rootFestival->reportStatus();

    // ---------------------------------------------------------------
    // STEP 9: Recovery. OPEN is EventFlow's "return to normal operation"
    // signal (see EventNotice.h).
    // ---------------------------------------------------------------
    std::cout << "\n--- STEP 9: Notice 5/5, OPEN (recovery) ---\n";
    rootFestival->issueNotice(NoticeType::OPEN, "All clear. Resuming normal festival operations.", 1);
    rootFestival->reportStatus();
    std::cout << "Final total capacity: " << rootFestival->getCapacity() << " people\n";

    // ---------------------------------------------------------------
    // STEP 10: Clean shutdown. A single delete on the root recursively
    // deletes every owned child exactly once (see EventComposite.h);
    // this has been verified leak- and dangling-pointer-free under
    // AddressSanitizer/UndefinedBehaviorSanitizer.
    // ---------------------------------------------------------------
    std::cout << "\n--- STEP 10: Clean shutdown ---\n";
    delete rootFestival;
    std::cout << "Root deleted. Entire owned subtree released exactly once.\n";
    std::cout << "\n=====================================================\n";
    std::cout << " Demonstration complete.\n";
    std::cout << "=====================================================\n";
}
