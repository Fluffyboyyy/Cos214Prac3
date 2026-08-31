#include <iostream>
#include "CascadingZone.h"
#include "EventComponent.h"

class ObservingStage : public EventComponent, public Observer
{
private:
    std::string stageName;
    bool active{true};

public:
    explicit ObservingStage(std::string name) : stageName(std::move(name)) {}

    void open() override { active = true; }
    void close() override { active = false; std::cout << "  [Stage " << stageName << "] Shutting down amplification systems.\n"; }
    void reportStatus() const override {}
    int getCapacity() const override { return 500; }

    void update(const EventNotice &notice) override
    {
        std::cout << "  [Leaf Observer: Stage " << stageName << "] Received: " << notice.message << "\n";
        if (notice.type == NoticeType::EVACUATE) {
            close();
        }
    }
};

void testTask3() {
    CascadingZone rootVenue("Main Festival Grounds");

    CascadingZone northSector("North Sector");

    CascadingZone mainStageArea("Main Stage Precinct");
    ObservingStage mainStage("Rock Stage");

    rootVenue.attach(&northSector);         
    northSector.attach(&mainStageArea);      
    mainStageArea.attach(&mainStage);       


    rootVenue.issueNotice(NoticeType::EVACUATE, "EMERGENCY: Evacuate North Sector!", 5);

}