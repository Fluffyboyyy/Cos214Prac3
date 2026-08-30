#include "EventComponent.h"
#include "EventComposite.h"
#include "EventComponent.cpp"
#include <iostream>

void testTask2()
{
    EventComposite *rootFestival = new EventComposite("City Lights Festival");

    EventComposite *mainZone = new EventComposite("Main Stage Zone");

    MainStage *stage = new MainStage("Grand Stage", 15000);
    SecurityGate *gate = new SecurityGate("North Gate", 2000);

    mainZone->add(stage);
    mainZone->add(gate);

    rootFestival->add(mainZone);

    rootFestival->open();
    rootFestival->reportStatus();
    std::cout << "\nTotal Festival Capacity: " << rootFestival->getCapacity() << " people\n";

    delete rootFestival;
}