#pragma once
#include "ListTemplate.h"
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>

class Case {
protected:
    std::string type;
    int caseId;
public:
    Case(std::string t = "", int id = 0) : type(t), caseId(id) {}
    virtual std::string getType() const { return type; }
    int getId() const { return caseId; }
    virtual ~Case() {}
    bool operator==(const Case& other) const { return caseId == other.caseId; }
    friend std::ostream& operator<<(std::ostream& os, const Case& c) {
        os << c.type << " (ID: " << c.caseId << ")";
        return os;
    }
};

class Theft : public Case {
public:
    Theft(int id = 0) : Case("Theft", id) {}
};

class Assault : public Case {
public:
    Assault(int id = 0) : Case("Assault", id) {}
};

class CrimeManager {
    ListTemplate<Case> cases;
    std::map<int, Case> caseMap;
public:
    void addCase(const Case& c);
    void listCases();
    void save();
    void load();
};

class CrimeRegistry {
    static CrimeRegistry* instance;
    CrimeManager manager;
    CrimeRegistry() {}
public:
    static CrimeRegistry* getInstance();
    CrimeManager& getManager();
};

void crimeMenu(); 