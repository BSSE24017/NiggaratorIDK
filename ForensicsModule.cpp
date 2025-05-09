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
    cout << "Lab Tech '" << e.getName() << "' added! Let's hope they don't mix up the test tubes again.\n";
}

void ForensicLab::addFieldAgent(const FieldAgent& e) {
    fieldAgents.add(e);
    cout << "Field Agent '" << e.getName() << "' enlisted. Time to grab those cool shades and chase clues!\n";
}

void ForensicLab::listExperts() {
    cout << "\n--- List of Experts ---\n";
    cout << "Lab Technicians:\n";
    for (const auto& lt : labTechs)
        cout << "- " << lt << " [" << lt.getSpecialty() << "]\n";

    cout << "Field Agents:\n";
    for (const auto& fa : fieldAgents)
        cout << "- " << fa << " [" << fa.getSpecialty() << "]\n";
}

// --------- Evidence Handling ---------
void ForensicLab::assignEvidence(int techId, int caseId, const string& desc) {
    for (size_t i = 0; i < labTechs.size(); ++i) {
        if (labTechs[i].getId() == techId) {
            int evidenceId = evidences.size() + 1;
            evidences.add(Evidence(evidenceId, desc, false, caseId));
            cout << "Evidence assigned to Lab Tech #" << techId << ". Don't spill it!\n";
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
            return;
        }
    }
    cout << "Evidence ID not found!\n";
}

void ForensicLab::listAllEvidence() {
    cout << "\n--- All Evidence ---\n";
    for (const auto& e : evidences)
        cout << e << "\n";
}

// --------- Case Handling ---------
void ForensicLab::linkEvidenceToCase(int caseId) {
    CrimeManager& cm = CrimeRegistry::getInstance()->getManager();
    Case* crimeCase = cm.findCase(caseId);
    if (!crimeCase) {
        cout << "No such case found in main module.\n";
        return;
    }

    cout << "\nLinking Evidence to Case #" << caseId << "...\n";
    for (const auto& ev : evidences) {
        if (ev.getCaseId() == caseId)
            cout << "  > " << ev << "\n";
    }
}

void ForensicLab::linkExpertToCase(int caseId) {
    CrimeManager& cm = CrimeRegistry::getInstance()->getManager();
    Case* crimeCase = cm.findCase(caseId);
    if (!crimeCase) {
        cout << "No such case found in main module.\n";
        return;
    }

    cout << "\nLinking Experts to Case #" << caseId << "...\n";
    for (const auto& agent : fieldAgents) {
        if (agent.getAssignedCase() == caseId)
            cout << "  > " << agent << "\n";
    }
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
