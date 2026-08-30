#include "EventComponent.h"

class MainStage : public EventComponent
{
private:
    std::string name;
    bool isOpen;
    int capacity;

public:
    MainStage(std::string n, int cap) : name(n), capacity(cap), isOpen(false) {}

    void open() override
    {
        isOpen = true;
        std::cout << "[Stage] " << name << " is NOW PLAYING!\n";
    }
    void close() override
    {
        isOpen = false;
        std::cout << "[Stage] " << name << " has STOPPED.\n";
    }
    void reportStatus() const override
    {
        std::cout << "  - Stage: " << name << " | Status: " << (isOpen ? "OPEN" : "CLOSED") << " | Cap: " << capacity << "\n";
    }
    int getCapacity() const override
    {
        return capacity;
    }
};

class SecurityGate : public EventComponent
{
private:
    std::string gateName;
    bool processing;
    int maxFlow;

public:
    SecurityGate(std::string n, int flow) : gateName(n), maxFlow(flow), processing(false) {}

    void open() override
    {
        processing = true;
        std::cout << "[Gate] " << gateName << " is NOW OPEN!\n";
    }
    void close() override
    {
        processing = false;
        std::cout << "[Gate] " << gateName << " is CLOSED.\n";
    }
    void reportStatus() const override
    {
        std::cout << "  - Gate: " << gateName << " | Processing: " << (processing ? "YES" : "NO") << " | Max Flow: " << maxFlow << "\n";
    }
    int getCapacity() const override 
    { 
        return maxFlow; 
    }
};
