#include <iostream>
#include "CascadingZone.h"
#include "EventComponent.h"

/**
 * @class ObservingStage
 * @brief A minimal standalone Leaf/Observer used only by testTask3() to
 * demonstrate that any EventComponent implementing Observer can register
 * with a CascadingZone, independently of the Composite ownership tree.
 * It is deliberately not added to any composite's children in this demo,
 * only attach()ed, so it is stack-allocated rather than heap-allocated:
 * nothing will ever try to delete it through an EventComponent*.
 */
class ObservingStage : public EventComponent, public Observer
{
private:
    std::string stageName;
    bool active{true};

public:
    /// @brief Constructs an observing stage.
    /// @param name Display name.
    explicit ObservingStage(std::string name) : stageName(std::move(name)) {}

    /// @brief Marks the stage active.
    void open() override { active = true; }

    /// @brief Marks the stage inactive and shuts down amplification.
    void close() override { active = false; std::cout << "  [Stage " << stageName << "] Shutting down amplification systems.\n"; }

    /// @brief No-op: this demo class does not print its own status.
    void reportStatus() const override {}

    /// @brief Fixed demonstration capacity.
    /// @return Always 500.
    int getCapacity() const override { return 500; }

    /**
     * @brief Reacts to a notice by logging it, and closes on EVACUATE.
     * @param notice The notice delivered by whichever zone this stage is attached to.
     */
    void update(const EventNotice &notice) override
    {
        std::cout << "  [Leaf Observer: Stage " << stageName << "] Received: " << notice.message << "\n";
        if (notice.type == NoticeType::EVACUATE) {
            close();
        }
    }
};

/**
 * @brief Task 3 demonstration: attaches a three-level chain of
 * CascadingZone/Observer objects (rootVenue -> northSector ->
 * mainStageArea -> mainStage) purely through attach(), independently of
 * any Composite ownership, then issues a single EVACUATE notice from the
 * root and shows it cascading down all three levels to the leaf.
 */
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