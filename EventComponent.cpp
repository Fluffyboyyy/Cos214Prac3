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

class FoodVendor : public EventComponent
{
private:
    std::string vendorName;
    bool serving;
    int queueCap;

public:
    FoodVendor(std::string n, int cap) : vendorName(n), queueCap(cap), serving(false) {}

    void open() override
    {
        serving = true;
        std::cout << "[Vendor] " << vendorName << " is open for food orders.\n";
    }
    void close() override
    {
        serving = false;
        std::cout << "[Vendor] " << vendorName << " halted kitchen service.\n";
    }
    void reportStatus() const override
    {
        std::cout << "  - Vendor: " << vendorName << " | Kitchen: " << (serving ? "ACTIVE" : "PAUSED") << " | Line Cap: " << queueCap << "\n";
    }
    int getCapacity() const override { return queueCap; }
};

class FirstAidTent : public EventComponent
{
private:
    std::string location;
    bool active;
    int bedCapacity;

public:
    FirstAidTent(std::string loc, int beds) : location(loc), bedCapacity(beds), active(true) {}

    void open() override
    {
        active = true;
        std::cout << "[Medical] First Aid at " << location << " is operational.\n";
    }
    void close() override
    {
        active = false;
        std::cout << "[Medical] First Aid at " << location << " suspended.\n";
    }
    void reportStatus() const override
    {
        std::cout << "  - Medical: " << location << " | Status: " << (active ? "ACTIVE" : "STANDBY") << " | Beds: " << bedCapacity << "\n";
    }
    int getCapacity() const override { return bedCapacity; }
};

class ShuttleStop : public EventComponent
{
private:
    std::string routeName;
    bool running;
    int platformCap;

public:
    ShuttleStop(std::string r, int cap) : routeName(r), platformCap(cap), running(false) {}

    void open() override
    {
        running = true;
        std::cout << "[Transport] Shuttle " << routeName << " is boarding.\n";
    }
    void close() override
    {
        running = false;
        std::cout << "[Transport] Shuttle " << routeName << " suspended.\n";
    }
    void reportStatus() const override
    {
        std::cout << "  - Shuttle: " << routeName << " | Transport: " << (running ? "RUNNING" : "STOPPED") << " | Platform Cap: " << platformCap << "\n";
    }
    int getCapacity() const override { return platformCap; }
};