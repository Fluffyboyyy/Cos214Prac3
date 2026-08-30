#ifndef EVENTCOMPOSITE_H
#define EVENTCOMPOSITE_H

#include "EventComponent.h"
#include <string>
#include <vector>

class EventComposite : public EventComponent
{
private:
    std::string groupName;
    std::vector<EventComponent *> children;

public:
    explicit EventComposite(std::string name);
    ~EventComposite() override;

    //Basic vector operation
    void add(EventComponent *component) override;
    void remove(EventComponent *component) override;

    //Basic Info functions
    void open() override;
    void close() override;
    void reportStatus() const override;
    int getCapacity() const override;
};

#endif //EVENTCOMPOSITE_H