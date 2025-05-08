#pragma once
#include "ListTemplate.h"
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm> 
#include <sstream> // For string stream operations
#include "nlohmann/json.hpp"

using json = nlohmann::json;
using namespace std;

// Forward declaration for Location class
class Location;

class Case {
protected:
    string type;
    int caseId;
    double severity;
    Location* location; // Association(specifically aggregation)
    string description; 
    string reportedBy; 
    string date; 

public:
    Case(string t = "", int id = 0, double sev = 0.0)
        : type(t), caseId(id), severity(sev), location(nullptr),
        description(""), reportedBy(""), date("") {}

    // Setters
    void setType(const string& t) {
        type = t;
    }
    void setId(int id) {
        caseId = id;
    }
    void setSeverity(double sev) {
        severity = sev;
    }
    void setLocation(Location* loc) {
        location = loc;
    }
    void setDescription(const string& desc) {
        description = desc;
    }
    void setReportedBy(const string& reporter) {
        reportedBy = reporter;
    }
    void setDate(const string& d) {
        date = d;
    }

    // Getters
    virtual string getType() const {
        return type;
    }
    int getId() const {
        return caseId;
    }
    double getSeverity() const { return severity; }
    Location* getLocation() const { return location; }
    string getDescription() const { return description; }
    string getReportedBy() const { return reportedBy; }
    string getDate() const { return date; }

    // Polymorphism
    virtual void displayDetails() const;

    // Function to get case priority - will be overridden
    virtual double getPriority() const {
        return severity;
    }

    //Method to get full case information as a string
    virtual string getFullInfo() const {
        stringstream ss;
        ss << "Case Type: " << type << "\n"
            << "ID: " << caseId << "\n"
            << "Severity: " << severity << "\n";

        if (!description.empty())
            ss << "Description: " << description << "\n";

        if (!reportedBy.empty())
            ss << "Reported By: " << reportedBy << "\n";

        if (!date.empty())
            ss << "Date: " << date << "\n";

        return ss.str();
    }

    virtual ~Case() {}

    // Operator overloading
    bool operator==(const Case& other) const {
        return caseId == other.caseId;
    }

    friend ostream& operator<<(ostream& os, const Case& c) {
        os << c.type << " (ID: " << c.caseId << ")";
        return os;
    }
};

class Location {
private:
    string address;
    string city;
    string state;
    string zipCode;

public:
    Location(const string& addr = "", const string& c = "", const string& s = "", const string& zip = "")
        : address(addr), city(c), state(s), zipCode(zip) {}

    // Setters
    void setAddress(const string& addr) { address = addr; }
    void setCity(const string& c) { city = c; }
    void setState(const string& s) { state = s; }
    void setZipCode(const string& zip) { zipCode = zip; } 

    // Getters
    string getAddress() const { return address; }
    string getCity() const { return city; }
    string getState() const { return state; }
    string getZipCode() const { return zipCode; } 

    friend ostream& operator<<(ostream& os, const Location& loc) {
        os << "Location: " << loc.address << ", " << loc.city << ", " << loc.state;
        if (!loc.zipCode.empty())
            os << " " << loc.zipCode;
        return os;
    }
};

class Theft : public Case {
private:
    double stolenValue;
    string itemStolen;
    bool recovered;

public:
    // Function overloading: multiple constructors with different parameters
    Theft(int id = 0) : Case("Theft", id, 2.0), stolenValue(0.0), itemStolen(""), recovered(false) {}
    Theft(int id, double value, const string& item = "", bool isRecovered = false)
        : Case("Theft", id, 2.0), stolenValue(value), itemStolen(item), recovered(isRecovered) {}
    // Setters
    void setStolenValue(double value) { stolenValue = value; }
    void setItemStolen(const string& item) { itemStolen = item; } 
    void setRecovered(bool isRecovered) { recovered = isRecovered; } 

    // Getters
    double getStolenValue() const { return stolenValue; }
    string getItemStolen() const { return itemStolen; } 
    bool isRecovered() const { return recovered; } 

    // Polymorphism: override the base class method
    void displayDetails() const override;

    // Override getPriority to calculate based on stolen value
    double getPriority() const override {
        double priority = severity + (stolenValue / 1000.0);
        if (recovered) priority -= 1.0; // Lower priority if items are recovered
        return priority;
    }

    // Override getFullInfo to include theft-specific details
    string getFullInfo() const override {
        stringstream ss;
        ss << Case::getFullInfo();
        ss << "Stolen Value: $" << stolenValue << "\n";

        if (!itemStolen.empty())
            ss << "Item Stolen: " << itemStolen << "\n";

        ss << "Recovered: " << (recovered ? "Yes" : "No") << "\n";
        return ss.str();
    }
};

class Assault : public Case {
private:
    bool weaponUsed;
    string victimName;
    int injurySeverity; // Scale of 1-10

public:
    // Function overloading: multiple constructors with different parameters
    Assault(int id = 0) : Case("Assault", id, 5.0), weaponUsed(false), victimName(""), injurySeverity(0) {}
    Assault(int id, bool weapon, const string& name = "", int severity = 0)
        : Case("Assault", id, 5.0), weaponUsed(weapon), victimName(name), injurySeverity(severity) {}
    // Setters
    void setWeaponUsed(bool weapon) { weaponUsed = weapon; }
    void setVictimName(const string& name) { victimName = name; } 
    void setInjurySeverity(int severity) { injurySeverity = severity; }

    // Getters 
    bool wasWeaponUsed() const { return weaponUsed; }
    string getVictimName() const { return victimName; } 
    int getInjurySeverity() const { return injurySeverity; } 

    // Polymorphism: override the base class method
    void displayDetails() const override;

    // Override getPriority to account for weapon use and injury severity
    double getPriority() const override {
        return severity + (weaponUsed ? 3.0 : 0.0) + (injurySeverity * 0.5);
    }

    // Override getFullInfo to include assault-specific details
    string getFullInfo() const override {
        stringstream ss;
        ss << Case::getFullInfo();
        ss << "Weapon Used: " << (weaponUsed ? "Yes" : "No") << "\n";

        if (!victimName.empty())
            ss << "Victim: " << victimName << "\n";

        if (injurySeverity > 0)
            ss << "Injury Severity: " << injurySeverity << " (scale 1-10)\n";

        return ss.str();
    }
};
class Vandalism : public Case {
private:
    double damageValue;
    string propertyType;
    bool publicProperty;

public:
    Vandalism(int id = 0)
        : Case("Vandalism", id, 2.0), damageValue(0.0), propertyType(""), publicProperty(false) {}

    Vandalism(int id, double damage, const string& ptype = "", bool isPublic = false)
        : Case("Vandalism", id, 2.0), damageValue(damage), propertyType(ptype), publicProperty(isPublic) {}
    // Setters
    void setDamageValue(double value) { damageValue = value; }
    void setPropertyType(const string& Ptype) { propertyType = Ptype; }
    void setPublicProperty(bool isPublic) { publicProperty = isPublic; }

    // Getters
    double getDamageValue() const { return damageValue; }
    string getPropertyType() const { return propertyType; }
    bool isPublicProperty() const { return publicProperty; }

    // Override display method
    void displayDetails() const override;

    // Override priority calculation
    double getPriority() const override {
        double priority = severity + (damageValue / 2000.0);
        if (publicProperty) priority += 1.0; // Higher priority for public property
        return priority;
    }

    // Override getFullInfo
    string getFullInfo() const override {
        stringstream ss;
        ss << Case::getFullInfo();
        ss << "Damage Value: $" << damageValue << "\n";

        if (!propertyType.empty())
            ss << "Property Type: " << propertyType << "\n";

        ss << "Public Property: " << (publicProperty ? "Yes" : "No") << "\n";
        return ss.str();
    }
};

class CrimeManager {
    // Template class usage
    ListTemplate<Case*> cases; // Aggregation
    map<int, Case*> caseMap;
    ListTemplate<Location*> locations; //aggregation

    //Statistics tracking
    map<string, int> caseTypeCount;
    double totalStolenValue;
    int highPriorityCaseCount; // Cases with priority > 7.0

public:
    CrimeManager() : totalStolenValue(0.0), highPriorityCaseCount(0) {}

    // Destructor to clean up memory
    ~CrimeManager() {
        for (auto& pair : caseMap) {
            delete pair.second;
        }
        for (auto& loc : locations) {
            delete loc;
        }
    }

    // Function overloading: multiple addCase functions with different parameters
    void addCase(Case* c);
    void addCase(int id, string type);  // Simpler version
    void addCase(int id, string type, double extraInfo);  // Version with extra info

    Location* addLocation(const string& address, const string& city, const string& state, const string& zipCode = "");
    void listCases();
    void listCasesByPriority();  // Function to demonstrate polymorphism
    void save();
    void load();
    Case* findCase(int id);      //
    void filterCasesByType(const string& type);
    void showDetailedCaseReport(int id);
    void updateCaseDetails(int id);
    void generateStatistics();
    void deleteCase(int id);
    void filterCasesByLocation(const string& city);
};

// Singleton
class CrimeRegistry {
    static CrimeRegistry* instance;
    CrimeManager manager;  // Composition
    CrimeRegistry() {}
public:
    static CrimeRegistry* getInstance();
    CrimeManager& getManager();
};

void crimeMenu();