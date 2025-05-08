#include "ForensicsModule.h"
#include <iomanip>

// --------- Lab Functions ---------
void ForensicLab::addLabTech(const LabTechnician& e) {
    labTechs.add(e);
    cout << "Lab Tech '" << e.getName() << "' added! Let's hope they don’t mix up the test tubes again.\n";
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
void ForensicLab::addCase(int caseId, const string& title) {
    caseFiles.add(CaseFile(caseId, title));
    cout << "Case #" << caseId << " added. Time to dig into the mystery!\n";
}

void ForensicLab::editCase(int caseId, const string& newTitle) {
    for (size_t i = 0; i < caseFiles.size(); ++i) {
        if (caseFiles[i].getId() == caseId) {
            caseFiles[i].setTitle(newTitle);
            cout << "Case #" << caseId << " title updated to '" << newTitle << "'. Sherlock would approve!\n";
            return;
        }
    }
    cout << "Case not found!\n";
}

void ForensicLab::checkCaseProgress(int caseId) {
    for (const auto& c : caseFiles) {
        if (c.getId() == caseId) {
            cout << "Case #" << caseId << " is currently [" << c.getStatus() << "]\n";
            return;
        }
    }
    cout << "No such case found.\n";
}

void ForensicLab::markCaseResolved(int caseId) {
    for (size_t i = 0; i < caseFiles.size(); ++i) {
        if (caseFiles[i].getId() == caseId) {
            caseFiles[i].setStatus("Resolved");
            cout << "Case #" << caseId << " has been resolved. Confetti incoming!\n";
            return;
        }
    }
    cout << "No case found.\n";
}

void ForensicLab::listAllCases() {
    cout << "\n--- All Cases ---\n";
    for (const auto& c : caseFiles)
        cout << c << "\n";
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

    for (const auto& c : caseFiles) {
        data["cases"].push_back({
            {"id", c.getId()},
            {"title", c.getTitle()},
            {"status", c.getStatus()}
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

    caseFiles.clear();
    for (const auto& c : data["cases"])
        caseFiles.add(CaseFile(c["id"], c["title"], c["status"]));

    file.close();
    cout << "Data loaded. Welcome back, detective!\n";
}

// --------- Singleton Access ---------
ForensicLabRegistry* ForensicLabRegistry::instance = nullptr;

ForensicLabRegistry* ForensicLabRegistry::getInstance() {
    if (!instance)
        instance = new ForensicLabRegistry();
    return instance;
}

ForensicLab& ForensicLabRegistry::getLab() {
    return lab;
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
        cout << "3. List Experts\n";
        cout << "4. Add Case File\n";
        cout << "5. Edit Case Title\n";
        cout << "6. Check Case Progress\n";
        cout << "7. Mark Case as Resolved\n";
        cout << "8. List All Cases\n";
        cout << "9. Assign Evidence to Lab Tech\n";
        cout << "10. Mark Evidence Processed\n";
        cout << "11. List All Evidence\n";
        cout << "0. Save & Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                string name;
                int id;
                cout << "Enter name: ";
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
                cout << "Enter name: ";
                cin.ignore();
                getline(cin, name);
                cout << "Enter ID: ";
                cin >> id;
                cout << "Assign to Case ID: ";
                cin >> caseId;
                lab.addFieldAgent(FieldAgent(name, id, caseId));
                break;
            }
            case 3:
                lab.listExperts();
                break;
            case 4: {
                int id;
                string title;
                cout << "Enter Case ID: ";
                cin >> id;
                cin.ignore();
                cout << "Enter Title: ";
                getline(cin, title);
                lab.addCase(id, title);
                break;
            }
            case 5: {
                int id;
                string title;
                cout << "Enter Case ID: ";
                cin >> id;
                cin.ignore();
                cout << "Enter new title: ";
                getline(cin, title);
                lab.editCase(id, title);
                break;
            }
            case 6: {
                int id;
                cout << "Enter Case ID: ";
                cin >> id;
                lab.checkCaseProgress(id);
                break;
            }
            case 7: {
                int id;
                cout << "Enter Case ID to mark resolved: ";
                cin >> id;
                lab.markCaseResolved(id);
                break;
            }
            case 8:
                lab.listAllCases();
                break;
            case 9: {
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
            case 10: {
                int evId;
                cout << "Enter Evidence ID to mark processed: ";
                cin >> evId;
                lab.markEvidenceProcessed(evId);
                break;
            }
            case 11:
                lab.listAllEvidence();
                break;
            case 0:
                lab.save();
                cout << "Exiting Forensics Module... Don’t forget your magnifying glass!\n";
                break;
            default:
                cout << "Invalid choice. Try again with less caffeine.\n";
        }
    } while (choice != 0);
}
