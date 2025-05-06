#pragma once
#include "ListTemplate.h"
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>

class Vehicle {
protected:
    std::string type;
    int vehicleId;
public:
    Vehicle(std::string t = "", int id = 0) : type(t), vehicleId(id) {}
    virtual std::string getType() const { return type; }
    int getId() const { return vehicleId; }
    virtual ~Vehicle() {}
    bool operator==(const Vehicle& other) const { return vehicleId == other.vehicleId; }
    friend std::ostream& operator<<(std::ostream& os, const Vehicle& v) {
        os << v.type << " (ID: " << v.vehicleId << ")";
        return os;
    }
};

class Car : public Vehicle {
public:
    Car(int id = 0) : Vehicle("Car", id) {}
};

class Bike : public Vehicle {
public:
    Bike(int id = 0) : Vehicle("Bike", id) {}
};

class PatrolFleet {
    ListTemplate<Vehicle> vehicles;
    std::map<int, Vehicle> vehicleMap;
public:
    void addVehicle(const Vehicle& v);
    void listVehicles();
    void save();
    void load();
};

class FleetRegistry {
    static FleetRegistry* instance;
    PatrolFleet fleet;
    FleetRegistry() {}
public:
    static FleetRegistry* getInstance();
    PatrolFleet& getFleet();
};

void patrolVehiclesMenu(); 