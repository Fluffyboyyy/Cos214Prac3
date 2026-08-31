#include "EventComposite.h"
#include <iostream>
#include <algorithm>

EventComposite::EventComposite(std::string name) : groupName(name) {}

EventComposite::~EventComposite()
{
    for (auto child : children)
    {
        delete child;
    }
    children.clear();
}

void EventComposite::add(EventComponent *component)
{
    if (component)
    {
        children.push_back(component);
    }
}

void EventComposite::remove(EventComponent *component)
{
    auto it = std::find(children.begin(), children.end(), component);
    if (it != children.end())
    {
        children.erase(it);
    }
}

void EventComposite::open()
{
    std::cout << "\n--- Opening Zone: " << groupName << " ---\n";
    for (auto child : children)
    {
        child->open();
    }
}

void EventComposite::close()
{
    std::cout << "\n--- Closing Zone: " << groupName << " ---\n";
    for (auto child : children)
    {
        child->close();
    }
}

void EventComposite::reportStatus() const
{
    std::cout << "\n[Group Zone] " << groupName << " status report:\n";
    for (auto child : children)
    {
        child->reportStatus();
    }
}

int EventComposite::getCapacity() const
{
    int totalCapacity = 0;
    for (auto child : children)
    {
        totalCapacity += child->getCapacity();
    }
    return totalCapacity;
}
