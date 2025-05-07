#pragma once
#include "ListTemplate.h"
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm> // Added for std::sort

class Case {
protected:
    std::string type;
    int caseId;
    double severity;  // Added severity level
public:
    // Fixed constructor - initialize severity directly
    Case(std::string t = "", int id = 0, double sev = 0.0) : type(t), caseId(id), severity(sev) {}

    virtual std::string getType() const { return type; }
    int getId() const { return caseId; }

    // Polymorphism: virtual function that will be overridden by derived classes
    virtual void displayDetails() const {
        std::cout << "Case Type: " << type << ", ID: " << caseId << ", Severity: " << severity << std::endl;
    }

    // Function to get case priority - will be overridden
    virtual double getPriority() const {
        return severity;
    }

    virtual ~Case() {}

    //operator overloading
    bool operator==(const Case& other) const { return caseId == other.caseId; }

    friend std::ostream& operator<<(std::ostream& os, const Case& c) {
        os << c.type << " (ID: " << c.caseId << ")";
        return os;
    }
};

class Theft : public Case {     //inheritance
private:
    double stolenValue;
public:
    // Function overloading: multiple constructors with different parameters
    Theft(int id = 0) : Case("Theft", id, 2.0), stolenValue(0.0) {}
    Theft(int id, double value) : Case("Theft", id, 2.0), stolenValue(value) {}

    // Polymorphism: override the base class method
    void displayDetails() const override {
        std::cout << "Theft Case - ID: " << caseId << ", Stolen Value: $" << stolenValue << ", Severity: " << severity << std::endl;
    }

    // Override getPriority to calculate based on stolen value
    double getPriority() const override {
        return severity + (stolenValue / 1000.0);  // Higher value = higher priority
    }

    // Getter for stolen value
    double getStolenValue() const { return stolenValue; }
};

class Assault : public Case {    //inheritance
private:
    bool weaponUsed;
public:
    // Function overloading: multiple constructors with different parameters
    Assault(int id = 0) : Case("Assault", id, 5.0), weaponUsed(false) {}
    Assault(int id, bool weapon) : Case("Assault", id, 5.0), weaponUsed(weapon) {}

    // Polymorphism: override the base class method
    void displayDetails() const override {
        std::cout << "Assault Case - ID: " << caseId
            << ", Weapon Used: " << (weaponUsed ? "Yes" : "No")
            << ", Severity: " << severity << std::endl;
    }

    // Override getPriority to account for weapon use
    double getPriority() const override {
        return severity + (weaponUsed ? 3.0 : 0.0);  // Weapon use increases priority
    }

    // Getter for weapon status
    bool wasWeaponUsed() const { return weaponUsed; }
};

class CrimeManager {
    ListTemplate<Case*> cases;      // Changed to store pointers
    std::map<int, Case*> caseMap;   // Changed to store pointers
public:
    // Destructor to clean up memory
    ~CrimeManager() {
        for (auto& pair : caseMap) {
            delete pair.second;
        }
    }

    // Function overloading: multiple addCase functions with different parameters
    void addCase(Case* c);
    void addCase(int id, std::string type);  // Simpler version
    void addCase(int id, std::string type, double extraInfo);  // Version with extra info
    void listCases();
    void listCasesByPriority();  // New function to demonstrate polymorphism
    void save();
    void load();

    // Function to retrieve a case by ID
    Case* findCase(int id);
};

//singleton
class CrimeRegistry {
    static CrimeRegistry* instance;
    CrimeManager manager;         //composition
    CrimeRegistry() {}
public:
    static CrimeRegistry* getInstance();
    CrimeManager& getManager();
};

void crimeMenu();