#ifndef FORENSICSMODULE_H
#define FORENSICSMODULE_H

#include "crimeModule.h" // Needed to access CrimeManager
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include "nlohmann/json.hpp"
#include "OfficerModule.h"

using json = nlohmann::json;
using namespace std;

class DisplayForensicsInfo {
public:
    virtual void displayForensicsInfo() const = 0;
    virtual ~DisplayForensicsInfo() = default;
};

// ------------------------ EVIDENCE CLASS ------------------------
class Evidence : public DisplayForensicsInfo {
private:
    int evidenceId;
    string description;
    bool isProcessed;
    int caseId;  //association with case

public:
    Evidence(int i = 0, const string& desc = "", bool processed = false, int cid = 0)
        : evidenceId(i), description(desc), isProcessed(processed), caseId(cid) {}

    // Getters
    int getId() const { return evidenceId; }
    string getDescription() const { return description; }
    bool getStatus() const { return isProcessed; }
    int getCaseId() const { return caseId; }

    // Setters
    void setDescription(const string& desc) { description = desc; }
    void setStatus(bool processed) { isProcessed = processed; }
    void setCaseId(int cid) { caseId = cid; }
    void markProcessed() { isProcessed = true; }

    void displayForensicsInfo() const override;

    friend ostream& operator<<(ostream& os, const Evidence& ev) {
        os << "Evidence #" << ev.evidenceId << " | Case: " << ev.caseId 
           << " | Desc: " << ev.description << " | Status: "
           << (ev.isProcessed ? "Processed" : "Unprocessed");
        return os;
    }
};

// ------------------------ EXPERTS ------------------------
class ForensicExpert : public DisplayForensicsInfo {
protected:
    string name;
    int expertId;
    string specialization;
    vector<string> certifications;
    int assignedCaseId;  // Association with a case

public:
    ForensicExpert(const string& n = "", int i = 0, const string& spec = "")
        : name(n), expertId(i), specialization(spec), assignedCaseId(0) {}

    // Getters
    string getName() const { return name; }
    int getId() const { return expertId; }
    string getSpecialization() const { return specialization; }
    const vector<string>& getCertifications() const { return certifications; }
    int getAssignedCaseId() const { return assignedCaseId; }
    virtual string getSpecialty() const = 0;  // Pure virtual function

    // Setters
    void setName(const string& n) { name = n; }
    void setSpecialization(const string& spec) { specialization = spec; }
    void addCertification(const string& cert) { certifications.push_back(cert); }
    void setAssignedCaseId(int caseId) { assignedCaseId = caseId; }

    virtual void displayForensicsInfo() const override;
    friend ostream& operator<<(ostream& os, const ForensicExpert& fe) {
        os << fe.name << " (ID: " << fe.expertId << ")";
        return os;
    }
};

class LabTechnician : public ForensicExpert {
public:
    LabTechnician(const string& n = "", int i = 0, const string& spec = "")
        : ForensicExpert(n, i, spec) {}

    string getSpecialty() const override { return "Lab Technician"; }
    void displayForensicsInfo() const override;
};

class FieldAgent : public ForensicExpert {
private:
    HeadConstable* assignedOfficer;  // Association with HeadConstable

public:
    FieldAgent(HeadConstable* officer, const string& n = "", int i = 0, int cid = -1)
        : ForensicExpert(n, i), assignedOfficer(officer) {
        setAssignedCaseId(cid);
    }

    string getSpecialty() const override { return "Field Agent"; }
    HeadConstable* getAssignedOfficer() const { return assignedOfficer; }
    void setAssignedOfficer(HeadConstable* officer) { assignedOfficer = officer; }

    int getAssignedCase() const { return assignedCaseId; }
    void setCaseId(int cid) { assignedCaseId = cid; }

    void displayForensicsInfo() const override;

    friend ostream& operator<<(ostream& os, const FieldAgent& f) {
        os << f.name << " (ID: " << f.expertId << ") | Assigned Case: " 
           << (f.assignedCaseId >= 0 ? to_string(f.assignedCaseId) : "None")
           << " | Head Constable: " << (f.assignedOfficer ? f.assignedOfficer->getName() : "None");
        return os;
    }
};

// ------------------------ FORENSIC LAB ------------------------
class ForensicLab {
    vector<LabTechnician> labTechs;  //composition
    vector<FieldAgent> fieldAgents;
    vector<Evidence> evidences;

public:
    void addLabTech(const LabTechnician& e);  //aggregation
    void addFieldAgent(HeadConstable* officer);  // Changed to take HeadConstable
    void listExperts();
    void save();
    void load();

    void assignEvidence(int techId, int caseId, const string& desc);
    void markEvidenceProcessed(int evId);
    void listAllEvidence();

    // Interactively link (assign) evidence to a case
    void linkEvidenceToCase(int caseId);
    // Interactively link (assign) a field agent to a case
    void linkExpertToCase(int caseId);
    void displayForensicsForCase(int caseId);
    void checkCaseProgress(int caseId);
    void markCaseResolved(int caseId);

    // Get available Head Constables
    vector<HeadConstable*> getHeadConstables() const {
        vector<HeadConstable*> headConstables;
        OfficerManager& om = OfficerRegistry::getInstance()->getManager();
        const vector<Officer*>& officers = om.getOfficers();
        
        for (const auto& officer : officers) {
            if (HeadConstable* hc = dynamic_cast<HeadConstable*>(officer)) {
                headConstables.push_back(hc);
            }
        }
        return headConstables;
    }
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
void searchEvidenceByCaseID();
#endif // FORENSICSMODULE_H

