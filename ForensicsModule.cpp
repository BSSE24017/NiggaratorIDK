#include "ForensicsModule.h"

ForensicLabRegistry* ForensicLabRegistry::instance = nullptr;

void ForensicLab::addLabTech(const LabTechnician& e) {
    labTechs.add(e);
    expertMap[e.getId()] = nullptr; // Not used for ownership
}

void ForensicLab::addFieldAgent(const FieldAgent& e) {
    fieldAgents.add(e);
    expertMap[e.getId()] = nullptr;
}
void ForensicLab::listExperts() {
    cout << "Lab Technicians:\n";
    for (auto& e : labTechs) 
        cout << e << " - " << e.getSpecialty() << "\n";
    cout << "Field Agents:\n";
    for (auto& e : fieldAgents) 
        cout << e << " - " << e.getSpecialty() << "\n";
}

void ForensicLab::save() {
    json j;
    j["lab_technicians"] = json::array();
    j["field_agents"] = json::array();

    for (auto& e : labTechs) {
        j["lab_technicians"].push_back({
            {"id", e.getId()},
            {"name", e.getName()}
        });
    }

    for (auto& e : fieldAgents) {
        j["field_agents"].push_back({
            {"id", e.getId()},
            {"name", e.getName()}
        });
    }

    ofstream file("data/forensics.json");
    file << j.dump(4); // pretty print with indent=4
}

void ForensicLab::load() {
    labTechs.clear();
    fieldAgents.clear();
    expertMap.clear();

    ifstream file("data/forensics.json");
    if (!file.is_open()) return;

    json j;
    file >> j;

    for (auto& item : j["lab_technicians"]) {
        int id = item["id"];
        std::string name = item["name"];
        addLabTech(LabTechnician(name, id));
    }

    for (auto& item : j["field_agents"]) {
        int id = item["id"];
        std::string name = item["name"];
        addFieldAgent(FieldAgent(name, id));
    }
}


ForensicLabRegistry* ForensicLabRegistry::getInstance() {
    if (!instance) instance = new ForensicLabRegistry();
    return instance;
}
ForensicLab& ForensicLabRegistry::getLab() { return lab; }

void forensicsMenu() {
    ForensicLab& lab = ForensicLabRegistry::getInstance()->getLab();
    lab.load();
    int choice;
    do {
        cout << "\n--- Forensics Management ---\n";
        cout << "1. Add Lab Technician\n2. Add Field Agent\n3. List Experts\n4. Save\n0. Back\nChoice: ";
        cin >> choice;
        if (choice == 1) {
            string name;
            int id;
            cout << "Enter ID: "; cin >> id;
            cout << "Enter Name: "; cin.ignore(); getline(cin, name);
            lab.addLabTech(LabTechnician(name, id));
        } else if (choice == 2) {
            string name;
            int id;
            cout << "Enter ID: "; cin >> id;
            cout << "Enter Name: "; cin.ignore(); getline(cin, name);
            lab.addFieldAgent(FieldAgent(name, id));
        } else if (choice == 3) {
            lab.listExperts();
        } else if (choice == 4) {
            lab.save();
            cout << "Saved.\n";
        }
        
    } while (choice != 0);
    lab.save();
} 