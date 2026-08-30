#include "EventComponent.h"
#include "EventComposite.h"
#include "EventComponent.cpp"
#include <iostream>


void testTask2() {
    //root
    EventComposite* rootFestival = new EventComposite("City Lights Festival");

    //Zones
    EventComposite* mainZone = new EventComposite("Main Stage Zone");
    EventComposite* riverZone = new EventComposite("River Side Zone");

    //Sub-Zones
    EventComposite* foodCourt = new EventComposite("Food Court Precinct");

    //Leaf nodes
    MainStage* stage = new MainStage("Grand Stage", 15000);
    SecurityGate* gate = new SecurityGate("North Gate", 2000);
    FirstAidTent* medical = new FirstAidTent("North Entrance Medical", 50);
    FoodVendor* taco = new FoodVendor("Taco Stand", 300);
    ShuttleStop* shuttle = new ShuttleStop("West Bus Route", 1200);

    foodCourt->add(taco);
    
    riverZone->add(foodCourt);
    riverZone->add(shuttle);

    mainZone->add(stage);
    mainZone->add(gate);
    mainZone->add(medical);

    rootFestival->add(mainZone);
    rootFestival->add(riverZone);

    rootFestival->open();
    rootFestival->reportStatus();
    std::cout << "\nTotal Festival Capacity: " << rootFestival->getCapacity() << " people\n";

    delete rootFestival; 
}