#include "ForensicsModule.h"
#include <iomanip>

ForensicLabRegistry* ForensicLabRegistry::instance = nullptr;

ForensicLabRegistry* ForensicLabRegistry::getInstance() {
    if (!instance)
        instance = new ForensicLabRegistry();
    return instance;
}

ForensicLab& ForensicLabRegistry::getLab() {
    return lab;
}

//map usaageee
void searchEvidenceByCaseID() {
    ifstream file("forensics_data.json");
    if (!file.is_open()) {
        cout << "Unable to open forensics_data.json" << endl;
        return;
    }

    json data;
    file >> data;

    map<int, vector<json>> caseMap;

    // Fill the map using for loop with indexing
    for (int i = 0; i < data["evidences"].size(); i++) {
        int caseID = data["evidences"][i]["case"];
        caseMap[caseID].push_back(data["evidences"][i]);
    }

    int searchID;
    cout << "Enter Case ID to search evidence for: ";
    cin >> searchID;

    if (caseMap.find(searchID) != caseMap.end()) {
        cout << "\nEvidences for Case ID " << searchID << ":" << endl;
        vector<json> evidences = caseMap[searchID];

        for (int i = 0; i < evidences.size(); i++) {
            cout << "---------------------------" << endl;
            cout << "ID: " << evidences[i]["id"] << endl;
            cout << "Description: " << evidences[i]["desc"] << endl;
            cout << "Status: " << (evidences[i]["status"] ? "Processed " : "Pending ") << endl;
        }
        cout << "---------------------------" << endl;
    } else {
        cout << "No evidence found for Case ID " << searchID << "." << endl;
    }
}

// --------- Lab Functions ---------
void ForensicLab::addLabTech(const LabTechnician& e) {
    labTechs.push_back(e);
    cout << "Lab Tech '" << e.getName() << "' added! \n";
    cout << "(Forensics kit? Check. Notepad? Check. Instant noodles? Also check.)" << endl;
}

void ForensicLab::addFieldAgent(HeadConstable* officer) {
    if (!officer) {
        cout << "Invalid Head Constable provided.\n";
        return;
    }
    fieldAgents.push_back(FieldAgent(officer, officer->getName(), officer->getId()));
    cout << "Field Agent created from Head Constable '" << officer->getName() << "' enlisted.\n";
}

void ForensicLab::listExperts() {
    cout << "\n--- List of Experts ---\n";
    cout << "Lab Technicians:\n";
    for (const auto& lt : labTechs) {
        lt.displayForensicsInfo();
        cout << "---------------------------" << endl;
    }

    cout << "\nField Agents:\n";
    for (const auto& fa : fieldAgents) {
        fa.displayForensicsInfo();
        cout << "---------------------------" << endl;
    }
    cout << "I should really write a book after this one: 'Detective Diaries and Midnight Snacks.'" << endl;
}

// --------- Evidence Handling ---------
void ForensicLab::assignEvidence(int techId, int caseId, const string& desc) {
    // First verify if the case exists in crime module
    CrimeManager& cm = CrimeRegistry::getInstance()->getManager();
    Case* crimeCase = cm.findCase(caseId);
    if (!crimeCase) {
        cout << "Error: Case ID " << caseId << " does not exist in the crime module.\n";
        return;
    }

    for (size_t i = 0; i < labTechs.size(); ++i) {
        if (labTechs[i].getId() == techId) {
            int evidenceId = evidences.size() + 1;
            evidences.push_back(Evidence(evidenceId, desc, false, caseId));
            cout << "Evidence assigned to Lab Tech #" << techId << " for Case #" << caseId << ". Don't spill it!\n";
            cout << "(Had to bribe myself with snacks to get through this mess. Worth it.)" << endl;
            return;
        }
    }
    cout << "Oops! Lab Technician with ID #" << techId << " not found.\n";
}

void ForensicLab::markEvidenceProcessed(int evId) {
    for (size_t i = 0; i < evidences.size(); ++i) {
        if (evidences[i].getId() == evId) {
            evidences[i].markProcessed();
            cout << "Evidence #" << evId << " marked as processed. Lab smells a bit funky now.\n";
            cout << "Evidence screamed the answer louder than a metal concert." << endl;
            return;
        }
    }
    cout << "Evidence ID not found!\n";
}

void ForensicLab::listAllEvidence() {
    cout << "\n--- All Evidence ---\n";
    for (const auto& e : evidences) {
        e.displayForensicsInfo();
        cout << "---------------------------" << endl;
    }
    cout << "The only thing messier than the crime scene was my desktop during finals." << endl;
}

// --------- Case Handling ---------
void ForensicLab::linkEvidenceToCase(int caseId) {
    CrimeManager& cm = CrimeRegistry::getInstance()->getManager();
    Case* crimeCase = cm.findCase(caseId);
    if (!crimeCase) {
        cout << "Error: Case ID " << caseId << " does not exist in the crime module.\n";
        return;
    }

    cout << "\n--- Evidence List ---\n";
    bool found = false;
    for (const auto& ev : evidences) {
        cout << "  > ID: " << ev.getId() << " | Desc: " << ev.getDescription() 
             << " | Linked to Case: " << (ev.getCaseId() > 0 ? to_string(ev.getCaseId()) : "None") << "\n";
        found = true;
    }
    if (!found) {
        cout << "No evidence available to link.\n";
        return;
    }

    int evidenceId;
    cout << "Enter Evidence ID to link to Case #" << caseId << ": ";
    cin >> evidenceId;

    for (size_t i = 0; i < evidences.size(); ++i) {
        if (evidences[i].getId() == evidenceId) {
            if (evidences[i].getCaseId() == caseId) {
                cout << "Evidence already linked to this case.\n";
                return;
            }
            evidences[i] = Evidence(evidences[i].getId(), evidences[i].getDescription(), 
                                  evidences[i].getStatus(), caseId);
            cout << "Evidence ID " << evidenceId << " linked to Case #" << caseId << ".\n";
            cout << "Caught red-handed. Literally. The suspect had tomato sauce all over." << endl;
            return;
        }
    }
    cout << "Evidence ID not found.\n";
}

void ForensicLab::linkExpertToCase(int caseId) {
    CrimeManager& cm = CrimeRegistry::getInstance()->getManager();
    Case* crimeCase = cm.findCase(caseId);
    if (!crimeCase) {
        cout << "Error: Case ID " << caseId << " does not exist in the crime module.\n";
        return;
    }

    // Get available Head Constables
    vector<HeadConstable*> headConstables = getHeadConstables();
    if (headConstables.empty()) {
        cout << "No Head Constables available for assignment.\n";
        return;
    }

    cout << "\n--- Available Head Constables ---\n";
    for (size_t i = 0; i < headConstables.size(); i++) {
        cout << i + 1 << ". " << headConstables[i]->getName() 
             << " (ID: " << headConstables[i]->getId() << ")\n";
    }

    int choice;
    cout << "Select a Head Constable (1-" << headConstables.size() << "): ";
    cin >> choice;

    if (choice < 1 || choice > headConstables.size()) {
        cout << "Invalid selection.\n";
        return;
    }

    HeadConstable* selectedHC = headConstables[choice - 1];
    
    cout << "\n--- Field Agent List ---\n";
    bool found = false;
    for (const auto& agent : fieldAgents) {
        cout << "  > ID: " << agent.getId() << " | Name: " << agent.getName() 
             << " | Assigned Case: " << (agent.getAssignedCase() >= 0 ? to_string(agent.getAssignedCase()) : "None") << "\n";
        found = true;
    }
    if (!found) {
        cout << "No field agents available to link.\n";
        return;
    }

    int agentId;
    cout << "Enter Field Agent ID to assign to Case #" << caseId << ": ";
    cin >> agentId;

    for (auto& agent : fieldAgents) {
        if (agent.getId() == agentId) {
            if (agent.getAssignedCase() == caseId) {
                cout << "Field Agent already assigned to this case.\n";
                return;
            }
            agent.setCaseId(caseId);
            agent.setAssignedOfficer(selectedHC);
            cout << "Field Agent ID " << agentId << " assigned to Case #" << caseId 
                 << " with Head Constable " << selectedHC->getName() << ".\n";
            cout << "Almost arrested myself by mistake. Note to self: don't skip breakfast." << endl;
            return;
        }
    }
    cout << "Field Agent ID not found.\n";
}

void ForensicLab::displayForensicsForCase(int caseId) {
    cout << "\n=== Forensics Report for Case #" << caseId << " ===\n";
    linkEvidenceToCase(caseId);
    linkExpertToCase(caseId);
}

void ForensicLab::checkCaseProgress(int caseId) {
    CrimeManager& cm = CrimeRegistry::getInstance()->getManager();
    Case* crimeCase = cm.findCase(caseId);
    if (!crimeCase) {
        cout << "No such case found in main module.\n";
        return;
    }
    cout << "\n--- Case Progress for Case #" << caseId << " ---\n";
    crimeCase->displayDetails();
    linkEvidenceToCase(caseId);
    linkExpertToCase(caseId);
}

void ForensicLab::markCaseResolved(int caseId) {
    CrimeManager& cm = CrimeRegistry::getInstance()->getManager();
    Case* crimeCase = cm.findCase(caseId);
    if (!crimeCase) {
        cout << "No such case found in main module.\n";
        return;
    }
    cout << "\nMarking Case #" << caseId << " as resolved...\n";
    cout << "Case marked as resolved (for demonstration, no status field in Case).\n";
    cout << "Put two and two together, and surprisingly it wasn't five this time." << endl;
}

// --------- Save & Load ---------
void ForensicLab::save() {
    try {
        json data;
        
        // Save evidences
        for (const auto& ev : evidences) {
            data["evidences"].push_back({
                {"id", ev.getId()},
                {"desc", ev.getDescription()},
                {"status", ev.getStatus()},
                {"case", ev.getCaseId()}
            });
        }

        // Save field agents with their head constables
        for (const auto& agent : fieldAgents) {
            json agentData = {
                {"name", agent.getName()},
                {"id", agent.getId()},
                {"assignedCase", agent.getAssignedCase()}
            };
            
            if (agent.getAssignedOfficer()) {
                agentData["headConstable"] = {
                    {"id", agent.getAssignedOfficer()->getId()},
                    {"name", agent.getAssignedOfficer()->getName()}
                };
            }
            
            data["fieldAgents"].push_back(agentData);
        }

        // Save lab technicians
        for (const auto& tech : labTechs) {
            data["labTechs"].push_back({
                {"name", tech.getName()},
                {"id", tech.getId()},
                {"specialization", tech.getSpecialization()},
                {"assignedCase", tech.getAssignedCaseId()}
            });
        }

        ofstream file("forensics_data.json");
        file << setw(4) << data;
        file.close();
        cout << "Data saved. Hope no one hacks us now!\n";
        cout << "Broke the case with nothing but intuition and a slightly overconfident attitude." << endl;
    } catch (const exception& e) {
        cout << "Error saving forensics data: " << e.what() << endl;
    }
}

void ForensicLab::load() {
    try {
        ifstream file("forensics_data.json");
        if (!file.is_open()) {
            cout << "No saved file found. Starting fresh like a new crime scene.\n";
            return;
        }

        json data;
        file >> data;

        evidences.clear();
        fieldAgents.clear();
        labTechs.clear();

        // Load evidences
        if (data.contains("evidences")) {
            for (const auto& e : data["evidences"]) {
                evidences.push_back(Evidence(e["id"], e["desc"], e["status"], e["case"]));
            }
        }

        // Load field agents with their head constables
        if (data.contains("fieldAgents")) {
            OfficerManager& om = OfficerRegistry::getInstance()->getManager();
            for (const auto& a : data["fieldAgents"]) {
                HeadConstable* hc = nullptr;
                if (a.contains("headConstable")) {
                    // Find the head constable in the officer manager
                    const vector<Officer*>& officers = om.getOfficers();
                    for (const auto& officer : officers) {
                        if (HeadConstable* potentialHC = dynamic_cast<HeadConstable*>(officer)) {
                            if (potentialHC->getId() == a["headConstable"]["id"]) {
                                hc = potentialHC;
                                break;
                            }
                        }
                    }
                }
                fieldAgents.push_back(FieldAgent(hc, a["name"], a["id"], a["assignedCase"]));
            }
        }

        // Load lab technicians
        if (data.contains("labTechs")) {
            for (const auto& t : data["labTechs"]) {
                LabTechnician tech(t["name"], t["id"], t["specialization"]);
                tech.setAssignedCaseId(t["assignedCase"]);
                labTechs.push_back(tech);
            }
        }

        file.close();
        cout << "Data loaded. Welcome back, detective!\n";
        cout << "Tried to be cool and say 'Elementary', but I tripped on the evidence bag." << endl;
    } catch (const exception& e) {
        cout << "Error loading forensics data: " << e.what() << endl;
    }
}

// --------- Menu ---------
void forensicsMenu() {
    ForensicLab& lab = ForensicLabRegistry::getInstance()->getLab();
    lab.load();

    int choice;
    do {
        cout << "\n======= FORENSICS MODULE =======\n";
        cout << "1. Add Lab Technician\n";
        cout << "2. Add Field Agent from Head Constable\n";
        cout << "3. Assign Evidence to Lab Tech\n";
        cout << "4. Mark Evidence as Processed\n";
        cout << "5. List All Evidence\n";
        cout << "6. List All Experts\n";
        cout << "7. Link Evidence to Case\n";
        cout << "8. Link Experts to Case\n";
        cout << "9. Display Forensics for Case\n";
        cout << "10. Check Case Progress (from Main Module)\n";
        cout << "11. Mark Case as Resolved (from Main Module)\n";
        cout << "12. Search Evidence by case ID\n";
        cout << "0. Save & Exit\n";
        cout << "Enter your choice, detective: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                string name;
                int id;
                cout << "Enter Lab Tech name: ";
                cin.ignore();
                getline(cin, name);
                cout << "Enter ID: ";
                cin >> id;
                lab.addLabTech(LabTechnician(name, id));
                break;
            }
            case 2: {
                vector<HeadConstable*> headConstables = lab.getHeadConstables();
                if (headConstables.empty()) {
                    cout << "No Head Constables available.\n";
                    break;
                }

                cout << "\n--- Available Head Constables ---\n";
                for (size_t i = 0; i < headConstables.size(); i++) {
                    cout << i + 1 << ". " << headConstables[i]->getName() 
                         << " (ID: " << headConstables[i]->getId() << ")\n";
                }

                int choice;
                cout << "Select a Head Constable to create Field Agent (1-" << headConstables.size() << "): ";
                cin >> choice;

                if (choice < 1 || choice > headConstables.size()) {
                    cout << "Invalid selection.\n";
                    break;
                }

                lab.addFieldAgent(headConstables[choice - 1]);
                break;
            }
            case 3: {
                int techId, caseId;
                string desc;
                cout << "Enter Lab Tech ID: ";
                cin >> techId;
                cout << "Enter Case ID: ";
                cin >> caseId;
                cin.ignore();
                cout << "Enter Evidence Description: ";
                getline(cin, desc);
                lab.assignEvidence(techId, caseId, desc);
                break;
            }
            case 4: {
                int evId;
                cout << "Enter Evidence ID to mark processed: ";
                cin >> evId;
                lab.markEvidenceProcessed(evId);
                break;
            }
            case 5:
                lab.listAllEvidence();
                break;
            case 6:
                lab.listExperts();
                break;
            case 7: {
                int caseId;
                cout << "Enter Case ID to link evidence: ";
                cin >> caseId;
                lab.linkEvidenceToCase(caseId);
                break;
            }
            case 8: {
                int caseId;
                cout << "Enter Case ID to link experts: ";
                cin >> caseId;
                lab.linkExpertToCase(caseId);
                break;
            }
            case 9: {
                int caseId;
                cout << "Enter Case ID to display forensics: ";
                cin >> caseId;
                lab.displayForensicsForCase(caseId);
                break;
            }
            case 10: {
                int id;
                cout << "Enter Case ID to check progress: ";
                cin >> id;
                lab.checkCaseProgress(id);
                break;
            }
            case 11: {
                int id;
                cout << "Enter Case ID to mark resolved: ";
                cin >> id;
                lab.markCaseResolved(id);
                break;
            }
            case 12: {
                searchEvidenceByCaseID();
                break;
            }
            case 0:
                lab.save();
                cout << "Exiting Forensics Module... Magnifying glass safely holstered.\n";
                break;
            default:
                cout << "Invalid choice. Are you solving crimes or creating them?\n";
        }
    } while (choice != 0);
}

void Evidence::displayForensicsInfo() const {
    cout << "\n=== Evidence Details ===" << endl;
    cout << "ID: " << evidenceId << endl;
    cout << "Description: " << description << endl;
    cout << "Status: " << (isProcessed ? "Processed" : "Pending") << endl;
    if (caseId > 0) {
        cout << "Linked to Case: " << caseId << endl;
    }
}

void ForensicExpert::displayForensicsInfo() const {
    cout << "Expert: " << name << " (ID: " << expertId << ")" << endl;
    cout << "Specialty: " << getSpecialty() << endl;
    if (!specialization.empty()) {
        cout << "Specialization: " << specialization << endl;
    }
    if (!certifications.empty()) {
        cout << "Certifications: ";
        for (const auto& cert : certifications) {
            cout << cert << ", ";
        }
        cout << endl;
    }
    if (assignedCaseId > 0) {
        cout << "Assigned to Case: " << assignedCaseId << endl;
    }
}

void LabTechnician::displayForensicsInfo() const {
    cout << "\n=== Lab Technician Details ===" << endl;
    ForensicExpert::displayForensicsInfo();
    cout << "Role: Laboratory Analysis" << endl;
}

void FieldAgent::displayForensicsInfo() const {
    cout << "\n=== Field Agent Details ===" << endl;
    ForensicExpert::displayForensicsInfo();
    cout << "Role: Field Investigation" << endl;
    if (assignedOfficer) {
        cout << "Assigned Head Constable: " << assignedOfficer->getName() 
             << " (ID: " << assignedOfficer->getId() << ")" << endl;
    } else {
        cout << "No Head Constable assigned" << endl;
    }
}
