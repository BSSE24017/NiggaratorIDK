#ifndef FORENSICSMODULE_H
#define FORENSICSMODULE_H


#include "ListTemplate.h"
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include "nlohmann/json.hpp"
using json = nlohmann::json;
using namespace std;


class ForensicExpert {
protected:
    string name;
    int expertId;
public:
    ForensicExpert(string n = "", int id = 0) : name(n), expertId(id) {}
    virtual string getSpecialty() const = 0;
    int getId() const { return expertId; }
    string getName() const { return name; }
    virtual ~ForensicExpert() {}
    bool operator==(const ForensicExpert& other) const { return expertId == other.expertId; }
    friend ostream& operator<<(ostream& os, const ForensicExpert& f) {
        os << f.name << " (ID: " << f.expertId << ")";
        return os;
    }
};

class LabTechnician : public ForensicExpert {
public:
    LabTechnician(string n = "", int id = 0) : ForensicExpert(n, id) {}
    string getSpecialty() const override { return "Lab Technician"; }
};

class FieldAgent : public ForensicExpert {
public:
    FieldAgent(string n = "", int id = 0) : ForensicExpert(n, id) {}
    string getSpecialty() const override { return "Field Agent"; }
};

class ForensicLab {
    ListTemplate<LabTechnician> labTechs;
    ListTemplate<FieldAgent> fieldAgents;
    map<int, ForensicExpert*> expertMap; // For demonstration, not for ownership
public:
    void addLabTech(const LabTechnician& e);
    void addFieldAgent(const FieldAgent& e);
    void listExperts();
    void save();
    void load();
    void addLabTech(const string& name, int id) {
        addLabTech(LabTechnician(name, id));
    }
    
};

class ForensicLabRegistry {
    static ForensicLabRegistry* instance;
    ForensicLab lab;
    ForensicLabRegistry() {}
public:
    static ForensicLabRegistry* getInstance();
    ForensicLab& getLab();
};

void forensicsMenu(); 


#endif // FORENSICSMODULE_H