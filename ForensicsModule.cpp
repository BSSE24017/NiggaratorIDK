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

// --------- Lab Functions ---------
void ForensicLab::addLabTech(const LabTechnician& e) {
    labTechs.add(e);
    cout << "Lab Tech '" << e.getName() << "' added! \n";
    cout << "(Forensics kit? Check. Notepad? Check. Instant noodles? Also check.)" << endl;
}

void ForensicLab::addFieldAgent(const FieldAgent& e) {
    fieldAgents.add(e);
    cout << "Field Agent '" << e.getName() << "' enlisted. \n";
}

void ForensicLab::listExperts() {
    cout << "\n--- List of Experts ---\n";
    cout << "Lab Technicians:\n";
    for (const auto& lt : labTechs)
        cout << "- " << lt << " [" << lt.getSpecialty() << "]\n";

    cout << "Field Agents:\n";
    for (const auto& fa : fieldAgents)
        cout << "- " << fa << " [" << fa.getSpecialty() << "]\n";
    cout << "I should really write a book after this one: 'Detective Diaries and Midnight Snacks.'" << endl;
}

// --------- Evidence Handling ---------
void ForensicLab::assignEvidence(int techId, int caseId, const string& desc) {
    for (size_t i = 0; i < labTechs.size(); ++i) {
        if (labTechs[i].getId() == techId) {
            int evidenceId = evidences.size() + 1;
            evidences.add(Evidence(evidenceId, desc, false, caseId));
            cout << "Evidence assigned to Lab Tech #" << techId << ". Don't spill it!\n";
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
    for (const auto& e : evidences)
        cout << e << "\n";
    cout << "The only thing messier than the crime scene was my desktop during finals." << endl;
}

// --------- Case Handling ---------
void ForensicLab::linkEvidenceToCase(int caseId) {
    CrimeManager& cm = CrimeRegistry::getInstance()->getManager();
    Case* crimeCase = cm.findCase(caseId);
    if (!crimeCase) {
        cout << "No such case found in main module.\n";
        return;
    }
    cout << "\n--- Evidence List ---\n";
    bool found = false;
    for (const auto& ev : evidences) {
        cout << "  > ID: " << ev.getId() << " | Desc: " << ev.getDescription() << " | Linked to Case: " << (ev.getCaseId() > 0 ? to_string(ev.getCaseId()) : "None") << "\n";
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
            evidences[i] = Evidence(evidences[i].getId(), evidences[i].getDescription(), evidences[i].getStatus(), caseId);
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
        cout << "No such case found in main module.\n";
        return;
    }
    cout << "\n--- Field Agent List ---\n";
    bool found = false;
    for (const auto& agent : fieldAgents) {
        cout << "  > ID: " << agent.getId() << " | Name: " << agent.getName() << " | Assigned Case: " << (agent.getAssignedCase() >= 0 ? to_string(agent.getAssignedCase()) : "None") << "\n";
        found = true;
    }
    if (!found) {
        cout << "No field agents available to link.\n";
        return;
    }
    int agentId;
    cout << "Enter Field Agent ID to assign to Case #" << caseId << ": ";
    cin >> agentId;
    for (size_t i = 0; i < fieldAgents.size(); ++i) {
        if (fieldAgents[i].getId() == agentId) {
            if (fieldAgents[i].getAssignedCase() == caseId) {
                cout << "Field Agent already assigned to this case.\n";
                return;
            }
            fieldAgents[i].setCaseId(caseId);
            cout << "Field Agent ID " << agentId << " assigned to Case #" << caseId << ".\n";
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
    json data;
    for (const auto& ev : evidences) {
        data["evidences"].push_back({
            {"id", ev.getId()},
            {"desc", ev.getDescription()},
            {"status", ev.getStatus()},
            {"case", ev.getCaseId()}
        });
    }

    ofstream file("forensics_data.json");
    file << setw(4) << data;
    file.close();
    cout << "Data saved. Hope no one hacks us now!\n";
    cout << "Broke the case with nothing but intuition and a slightly overconfident attitude." << endl;
}

void ForensicLab::load() {
    ifstream file("forensics_data.json");
    if (!file.is_open()) {
        cout << "No saved file found. Starting fresh like a new crime scene.\n";
        return;
    }

    json data;
    file >> data;

    evidences.clear();
    for (const auto& e : data["evidences"])
        evidences.add(Evidence(e["id"], e["desc"], e["status"], e["case"]));

    file.close();
    cout << "Data loaded. Welcome back, detective!\n";
    cout << "Tried to be cool and say 'Elementary', but I tripped on the evidence bag." << endl;
}

// --------- Menu ---------
void forensicsMenu() {
    ForensicLab& lab = ForensicLabRegistry::getInstance()->getLab();
    lab.load();

    int choice;
    do {
        cout << "\n======= FORENSICS MODULE =======\n";
        cout << "1. Add Lab Technician\n";
        cout << "2. Add Field Agent\n";
        cout << "3. Assign Evidence to Lab Tech\n";
        cout << "4. Mark Evidence as Processed\n";
        cout << "5. List All Evidence\n";
        cout << "6. List All Experts\n";
        cout << "7. Link Evidence to Case\n";
        cout << "8. Link Experts to Case\n";
        cout << "9. Display Forensics for Case\n";
        cout << "10. Check Case Progress (from Main Module)\n";
        cout << "11. Mark Case as Resolved (from Main Module)\n";
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
                string name;
                int id, caseId;
                cout << "Enter Field Agent name: ";
                cin.ignore();
                getline(cin, name);
                cout << "Enter ID: ";
                cin >> id;
                cout << "Assign to Case ID: ";
                cin >> caseId;
                lab.addFieldAgent(FieldAgent(name, id, caseId));
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
            case 0:
                lab.save();
                cout << "Exiting Forensics Module... Magnifying glass safely holstered.\n";
                break;
            default:
                cout << "Invalid choice. Are you solving crimes or creating them?\n";
        }
    } while (choice != 0);
}
