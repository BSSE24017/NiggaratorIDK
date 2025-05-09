#ifndef FORENSICSMODULE_H
#define FORENSICSMODULE_H

#include "ListTemplate.h"
#include "crimeModule.h" // Needed to access CrimeManager
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include "nlohmann/json.hpp"

using json = nlohmann::json;
using namespace std;

// ------------------------ EVIDENCE CLASS ------------------------
class Evidence {
    int evidenceId;
    string description;
    bool isProcessed;
    int caseId;

public:
    Evidence(int id = 0, string desc = "", bool processed = false, int cid = 0)
        : evidenceId(id), description(desc), isProcessed(processed), caseId(cid) {}

    int getId() const { return evidenceId; }
    string getDescription() const { return description; }
    bool getStatus() const { return isProcessed; }
    int getCaseId() const { return caseId; }

    void markProcessed() { isProcessed = true; }

    friend ostream& operator<<(ostream& os, const Evidence& ev) {
        os << "Evidence #" << ev.evidenceId << " | Case: " << ev.caseId 
           << " | Desc: " << ev.description << " | Status: "
           << (ev.isProcessed ? "Processed" : "Unprocessed");
        return os;
    }
};

// ------------------------ EXPERTS ------------------------
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

    bool operator==(const ForensicExpert& other) const {
        return expertId == other.expertId;
    }

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
    int assignedCaseId;

public:
    FieldAgent(string n = "", int id = 0, int cid = -1) : ForensicExpert(n, id), assignedCaseId(cid) {}

    string getSpecialty() const override { return "Field Agent"; }

    int getAssignedCase() const { return assignedCaseId; }
    void setCaseId(int cid) { assignedCaseId = cid; }

    friend ostream& operator<<(ostream& os, const FieldAgent& f) {
        os << f.name << " (ID: " << f.expertId << ") | Assigned Case: " 
           << (f.assignedCaseId >= 0 ? to_string(f.assignedCaseId) : "None");
        return os;
    }
};

// ------------------------ FORENSIC LAB ------------------------
class ForensicLab {
    ListTemplate<LabTechnician> labTechs;
    ListTemplate<FieldAgent> fieldAgents;
    ListTemplate<Evidence> evidences;

public:
    void addLabTech(const LabTechnician& e);
    void addFieldAgent(const FieldAgent& e);
    void listExperts();
    void save();
    void load();

    void assignEvidence(int techId, int caseId, const string& desc);
    void markEvidenceProcessed(int evId);
    void listAllEvidence();

    void linkEvidenceToCase(int caseId);
    void linkExpertToCase(int caseId);
    void displayForensicsForCase(int caseId);
    void checkCaseProgress(int caseId);
    void markCaseResolved(int caseId);
};

// ------------------------ SINGLETON ------------------------
class ForensicLabRegistry {
    static ForensicLabRegistry* instance;
    ForensicLab lab;
    ForensicLabRegistry() {}

public:
    static ForensicLabRegistry* getInstance();
    ForensicLab& getLab();
};

// ------------------------ MENU ------------------------
void forensicsMenu();

#endif // FORENSICSMODULE_H
