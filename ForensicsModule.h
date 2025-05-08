#ifndef FORENSICSMODULE_H
#define FORENSICSMODULE_H

#include "ListTemplate.h"
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include "nlohmann/json.hpp"

using namespace std;
using json = nlohmann::json;

class ForensicExpert {
protected:
    string name;
    int expertId;
public:
    ForensicExpert(string n = "", int id = 0);
    virtual string getSpecialty() const = 0;
    int getId() const;
    string getName() const;
    virtual ~ForensicExpert();
    bool operator==(const ForensicExpert& other) const;
    friend ostream& operator<<(ostream& os, const ForensicExpert& f);
};

class LabTechnician : public ForensicExpert {
public:
    LabTechnician(string n = "", int id = 0);
    string getSpecialty() const override;
};

class FieldAgent : public ForensicExpert {
public:
    FieldAgent(string n = "", int id = 0);
    string getSpecialty() const override;
};

class Evidence {
    int evidenceId;
    string description;
    string status;
public:
    Evidence(int id = 0, string desc = "", string stat = "Unprocessed");
    int getId() const;
    string getDescription() const;
    string getStatus() const;
    void setStatus(const string& newStatus);
    friend ostream& operator<<(ostream& os, const Evidence& e);
};

class ForensicLab {
    ListTemplate<LabTechnician> labTechs;
    ListTemplate<FieldAgent> fieldAgents;
    map<int, ForensicExpert*> expertMap;
    map<int, vector<Evidence>> assignedEvidence;
public:
    void addLabTech(const LabTechnician& e);
    void addFieldAgent(const FieldAgent& e);
    void addLabTech(const string& name, int id);
    void listExperts();
    void assignEvidence(int techId, const Evidence& ev);
    void viewEvidence(int techId);
    void removeExpert(int id);
    void save();
    void load();
};

class ForensicLabRegistry {
    static ForensicLabRegistry* instance;
    ForensicLab lab;
    ForensicLabRegistry();
public:
    static ForensicLabRegistry* getInstance();
    ForensicLab& getLab();
};

void forensicsMenu();

#endif // FORENSICSMODULE_H
