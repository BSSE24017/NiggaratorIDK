#pragma once
#include "ListTemplate.h"
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm> // Added for std::sort
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
	Location* location; // Aggregation: Case has-a Location

public:
	Case(string t = "", int id = 0, double sev = 0.0) : type(t), caseId(id), severity(sev), location(nullptr) {}

	// Setters
	void setType(const string& t) { type = t; }
	void setId(int id) { caseId = id; }
	void setSeverity(double sev) { severity = sev; }
	void setLocation(Location* loc) { location = loc; }

	//getters
	virtual string getType() const {
		return type;
	}
	int getId() const {
		return caseId;
	}
	double getSeverity() const { return severity; }
	Location* getLocation() const { return location; }

	// Polymorphism
	virtual void displayDetails() const;  

	// Function to get case priority - will be overridden
	virtual double getPriority() const {
		return severity;
	}

	virtual ~Case() {}

	//operator overloading
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

public:
	Location(const string& addr = "", const string& c = "", const string& s = "")
		: address(addr), city(c), state(s) {}

	// Setters
	void setAddress(const string& addr) { address = addr; }
	void setCity(const string& c) { city = c; }
	void setState(const string& s) { state = s; }

	// Getters
	string getAddress() const { return address; }
	string getCity() const { return city; }
	string getState() const { return state; }

	friend ostream& operator<<(ostream& os, const Location& loc) {
		os << "Location: " << loc.address << ", " << loc.city << ", " << loc.state;
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

	// Setter
	void setStolenValue(double value) { stolenValue = value; }

	// Getter
	double getStolenValue() const { return stolenValue; }

	// Polymorphism: override the base class method
	void displayDetails() const override;

	// Override getPriority to calculate based on stolen value
	double getPriority() const override {
		return severity + (stolenValue / 1000.0);  // Higher value = higher priority
	}
};

class Assault : public Case {    //inheritance
private:
	bool weaponUsed;
public:
	// Function overloading: multiple constructors with different parameters
	Assault(int id = 0) : Case("Assault", id, 5.0), weaponUsed(false) {}
	Assault(int id, bool weapon) : Case("Assault", id, 5.0), weaponUsed(weapon) {}

	// Setter
	void setWeaponUsed(bool weapon) { weaponUsed = weapon; }
	// Getter 
	bool wasWeaponUsed() const {
		return weaponUsed;
	}
	// Polymorphism: override the base class method
	void displayDetails() const override;

	// Override getPriority to account for weapon use
	double getPriority() const override {
		return severity + (weaponUsed ? 3.0 : 0.0);  // Weapon use increases priority
	}
};

class CrimeManager {
	ListTemplate<Case*> cases;
	map<int, Case*> caseMap;
	ListTemplate<Location*> locations; // Added locations list - aggregation

public:
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
	Location* addLocation(const string& address, const string& city, const string& state);
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