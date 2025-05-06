#pragma once
#include "ListTemplate.h"
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>

// Inheritance
class Person {
public:
    virtual std::string getRole() const = 0;
    virtual ~Person() {}
};

class Officer : public Person {
protected:
    std::string name;
    int id;
public:
    Officer(std::string n = "", int i = 0) : name(n), id(i) {}
    virtual std::string getRole() const override { return "Officer"; }
    int getId() const { return id; }
    std::string getName() const { return name; }
    // Operator Overloading
    bool operator==(const Officer& other) const { return id == other.id; }
    friend std::ostream& operator<<(std::ostream& os, const Officer& o) {
        os << o.name << " (" << o.id << ")";
        return os;
    }
};

class SHO : public Officer {
public:
    SHO(std::string n = "", int i = 0) : Officer(n, i) {}
    std::string getRole() const override { return "SHO"; }
};

class Constable : public Officer {
public:
    Constable(std::string n = "", int i = 0) : Officer(n, i) {}
    std::string getRole() const override { return "Constable"; }
};

// Composition: OfficerManager "has" officers
class OfficerManager {
    ListTemplate<Officer> officers;
    std::map<int, Officer> officerMap;
public:
    void addOfficer(const Officer& o);
    void listOfficers();
    void save();
    void load();
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