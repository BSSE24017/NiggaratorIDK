#pragma once
#include "ListTemplate.h"
#include <string>
#include <iostream>
#include <vector>
#include <map>
#include "nlohmann/json.hpp"
#include <fstream>
#include <sstream>
#include "OfficerModule.h"

using json = nlohmann::json;
using namespace std;

class Vehicle {
protected:
    string type;
    int vehicleId;
    string model;
    int year;
    string color;
    string licensePlate;
    double mileage;
    string fuelType;
    double engineSize;
    string transmissionType;
    int numSeats;
    string vehicleClass;
    string maintenanceHistory;
    bool availabilityStatus;
    double price;

public:
    static int nextId;
    Vehicle();
    Vehicle(string t, int id = 0);
    virtual ~Vehicle() {}

    // Getters
    virtual string getType() const { return type; }
    int getId() const { return vehicleId; }
    string getModel() const { return model; }
    int getYear() const { return year; }
    string getColor() const { return color; }
    string getLicensePlate() const { return licensePlate; }
    double getMileage() const { return mileage; }
    string getFuelType() const { return fuelType; }
    double getEngineSize() const { return engineSize; }
    string getTransmissionType() const { return transmissionType; }
    int getNumSeats() const { return numSeats; }
    string getVehicleClass() const { return vehicleClass; }
    string getMaintenanceHistory() const { return maintenanceHistory; }
    bool getAvailabilityStatus() const { return availabilityStatus; }
    double getPrice() const { return price; }

    // Setters
    void setModel(const string& m) { model = m; }
    void setYear(int y) { year = y; }
    void setColor(const string& c) { color = c; }
    void setLicensePlate(const string& lp) { licensePlate = lp; }
    void setMileage(double m) { mileage = m; }
    void setFuelType(const string& ft) { fuelType = ft; }
    void setEngineSize(double es) { engineSize = es; }
    void setTransmissionType(const string& tt) { transmissionType = tt; }
    void setNumSeats(int ns) { numSeats = ns; }
    void setVehicleClass(const string& vc) { vehicleClass = vc; }
    void setMaintenanceHistory(const string& mh) { maintenanceHistory = mh; }
    void setAvailabilityStatus(bool as) { availabilityStatus = as; }
    void setPrice(double p) { price = p; }

    virtual json toJson() const;
    virtual void fromJson(const json& j);
    friend ostream& operator<<(ostream& os, const Vehicle& v);
};

class Car : public Vehicle {
public:
    Car();
    Car(int id);
    json toJson() const override;
    void fromJson(const json& j) override;
};

class Bike : public Vehicle {
public:
    Bike();
    Bike(int id);
    json toJson() const override;
    void fromJson(const json& j) override;
};

class Patrol {
private:
    string patrolId;
    string area;
    int vehicleId;
    string status;  // "Active", "Completed", "Cancelled"
    string startTime;
    string endTime;
    vector<string> logs;
    vector<Constable> assignedConstables;

public:
    Patrol();
    Patrol(string id, string area, int vehicleId, const vector<Constable>& constables);
    
    // Getters
    string getPatrolId() const { return patrolId; }
    string getArea() const { return area; }
    int getVehicleId() const { return vehicleId; }
    string getStatus() const { return status; }
    const vector<Constable>& getAssignedConstables() const { return assignedConstables; }
    const vector<string>& getLogs() const { return logs; }

    // Setters
    void setArea(const string& newArea) { area = newArea; }
    void setStatus(const string& newStatus);
    void setAssignedConstables(const vector<Constable>& constables) { assignedConstables = constables; }

    // Methods
    void addLog(const string& logEntry);
    void displayInfo() const;
    json toJson() const;
    void fromJson(const json& j);
    friend ostream& operator<<(ostream& os, const Patrol& p);
};

class PatrolFleet {
    ListTemplate<Vehicle> vehicles;
    map<int, Vehicle> vehicleMap;
    map<string, Patrol> patrols;  // patrolId -> Patrol
    static int nextPatrolId;
public:
    PatrolFleet();
    void addVehicle(const Vehicle& v);
    void listVehicles();
    const ListTemplate<Vehicle>& getVehicles() const;
    ListTemplate<Vehicle>& getVehicles();
    bool searchVehicle(int id) const;
    const Vehicle& getVehicle(int id) const;
    bool updateVehicleStatus(int id, const string& status);
    bool updateMaintenanceHistory(int id, const string& history);

    // Patrol management methods
    string createPatrol(int vehicleId, const string& area, const vector<Constable>& constables);
    bool endPatrol(const string& patrolId);
    bool addPatrolLog(const string& patrolId, const string& logEntry);
    void listPatrols() const;
    bool searchPatrol(const string& patrolId) const;
    const Patrol& getPatrol(const string& patrolId) const;
    void saveToFile(const string& filename) const;
    void loadFromFile(const string& filename);
    void saveVehiclesToFile(const string& filename) const;
    void loadVehiclesFromFile(const string& filename);
    void savePatrolsToFile(const string& filename) const;
    void loadPatrolsFromFile(const string& filename);
};

class FleetRegistry {
    static FleetRegistry* instance;
    PatrolFleet fleet;
    FleetRegistry();
public:
    static FleetRegistry* getInstance();
    PatrolFleet& getFleet();
};

void patrolVehiclesMenu(); 