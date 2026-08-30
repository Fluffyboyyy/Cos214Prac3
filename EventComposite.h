#ifndef EVENTGROUP_H
#define EVENTGROUP_H

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

    void add(EventComponent *component) override;
    void remove(EventComponent *component) override;

    void open() override;
    void close() override;
    void reportStatus() const override;
    int getCapacity() const override;
};

#endif // EVENTGROUP_H