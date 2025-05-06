#pragma once
#include "ListTemplate.h"
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>

class ForensicExpert {
protected:
    std::string name;
    int expertId;
public:
    ForensicExpert(std::string n = "", int id = 0) : name(n), expertId(id) {}
    virtual std::string getSpecialty() const = 0;
    int getId() const { return expertId; }
    std::string getName() const { return name; }
    virtual ~ForensicExpert() {}
    bool operator==(const ForensicExpert& other) const { return expertId == other.expertId; }
    friend std::ostream& operator<<(std::ostream& os, const ForensicExpert& f) {
        os << f.name << " (ID: " << f.expertId << ")";
        return os;
    }
};

class LabTechnician : public ForensicExpert {
public:
    LabTechnician(std::string n = "", int id = 0) : ForensicExpert(n, id) {}
    std::string getSpecialty() const override { return "Lab Technician"; }
};

class FieldAgent : public ForensicExpert {
public:
    FieldAgent(std::string n = "", int id = 0) : ForensicExpert(n, id) {}
    std::string getSpecialty() const override { return "Field Agent"; }
};

class ForensicLab {
    ListTemplate<LabTechnician> labTechs;
    ListTemplate<FieldAgent> fieldAgents;
    std::map<int, ForensicExpert*> expertMap; // For demonstration, not for ownership
public:
    void addLabTech(const LabTechnician& e);
    void addFieldAgent(const FieldAgent& e);
    void listExperts();
    void save();
    void load();
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