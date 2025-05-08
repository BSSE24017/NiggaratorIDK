#pragma once
#include "ListTemplate.h"
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include "nlohmann/json.hpp"

using json = nlohmann::json;
using namespace std;

// Forward declarations
class SHO;
class CO;
class Prosecutor;
class HeadConstable;
class Constable;

class Officer {
protected:
    string name;
    int id;
    string rank;
    vector<string> tasks;

public:
    static int nextId;  // Static counter for unique IDs
    Officer(string n = "", int i = -1, string r = "") : name(n), rank(r) {
        id = (i == -1) ? nextId++ : i;
    }
    virtual ~Officer() = default;

    // Basic getters and setters
    int getId() const { return id; }
    string getName() const { return name; }
    string getRank() const { return rank; }
    void setRank(const string& r) { rank = r; }
    
    // Task management
    void addTask(const string& task) { tasks.push_back(task); }
    void removeTask(size_t index) { 
        if (index < tasks.size()) tasks.erase(tasks.begin() + index); 
    }
    const vector<string>& getTasks() const { return tasks; }
    void clearTasks() { tasks.clear(); }
    
    // Virtual functions for rank-specific operations
    virtual string getRole() const = 0;
    virtual void performDuty() const = 0;
    virtual vector<string> getResponsibilities() const = 0;
    
    // JSON serialization
    virtual json toJson() const {
        json j;
        j["name"] = name;
        j["id"] = id;
        j["rank"] = rank;
        j["tasks"] = tasks;
        j["role"] = getRole();
        return j;
    }
    
    // JSON deserialization
    static Officer* fromJson(const json& j);
    
    // Operator Overloading
    bool operator==(const Officer& other) const { return id == other.id; }
    friend ostream& operator<<(ostream& os, const Officer& o) {
        os << o.name << " (" << o.id << ") - " << o.rank;
        return os;
    }
};

class SHO : public Officer {
public:
    SHO(string n = "", int i = -1) : Officer(n, i, "SHO") {}
    
    string getRole() const override { return "SHO"; }
    void performDuty() const override;
    vector<string> getResponsibilities() const override;
};

class CO : public Officer {
public:
    CO(string n = "", int i = -1) : Officer(n, i, "CO") {}
    
    string getRole() const override { return "CO"; }
    void performDuty() const override;
    vector<string> getResponsibilities() const override;
};

class Prosecutor : public Officer {
public:
    Prosecutor(string n = "", int i = -1) : Officer(n, i, "Prosecutor") {}
    
    string getRole() const override { return "Prosecutor"; }
    void performDuty() const override;
    vector<string> getResponsibilities() const override;
};

class HeadConstable : public Officer {
public:
    HeadConstable(string n = "", int i = -1) : Officer(n, i, "Head Constable") {}
    
    string getRole() const override { return "Head Constable"; }
    void performDuty() const override;
    vector<string> getResponsibilities() const override;
};

class Constable : public Officer {
    bool assignedToPatrol = false;
public:
    Constable(string n = "", int i = -1) : Officer(n, i, "Constable") {}
    string getRole() const override { return "Constable"; }
    void performDuty() const override;
    vector<string> getResponsibilities() const override;
    bool isAssigned() const { return assignedToPatrol; }
    void setAssigned(bool val) { assignedToPatrol = val; }
    json toJson() const override {
        json j = Officer::toJson();
        j["assignedToPatrol"] = assignedToPatrol;
        return j;
    }
    void fromJson(const json& j) {
        Officer::fromJson(j);
        assignedToPatrol = j.value("assignedToPatrol", false);
    }
};

// Composition: OfficerManager "has" officers
class OfficerManager {
    ListTemplate<Officer*> officers;  // Changed to store pointers
    map<int, Officer*> officerMap;  // Changed to store pointers
public:
    ~OfficerManager();
    void addOfficer(Officer* o);
    void listOfficers();
    void save();
    void load();
    
    // JSON methods
    json toJson() const;
    void fromJson(const json& j);
    void saveToJson(const string& filename);
    void loadFromJson(const string& filename);
    
    // Getter for officers
    const ListTemplate<Officer*>& getOfficers() const { return officers; }

    // Get all available constables
    vector<Constable> getAvailableConstables() const;

    // Get all prosecutors
    vector<Prosecutor*> getProsecutors() const;

    // Public getter for officerMap
    const map<int, Officer*>& getOfficerMap() const { return officerMap; }
    map<int, Officer*>& getOfficerMap() { return officerMap; }
};

// Singleton
class OfficerRegistry {
    static OfficerRegistry* instance;
    OfficerManager manager;
    OfficerRegistry() {}
public:
    static OfficerRegistry* getInstance();
    OfficerManager& getManager();
};

// Menu
void officerMenu(); 