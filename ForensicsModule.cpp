#include "ForensicsModule.h"

// --- ForensicExpert methods ---

ForensicExpert::ForensicExpert(string n, int id) {
    name = n;
    expertId = id;
}

int ForensicExpert::getId() const {
    return expertId;
}

string ForensicExpert::getName() const {
    return name;
}

bool ForensicExpert::operator==(const ForensicExpert& other) const {
    return expertId == other.expertId;
}

ostream& operator<<(ostream& os, const ForensicExpert& f) {
    os << f.name << " (ID: " << f.expertId << ")";
    return os;
}

// --- LabTechnician methods ---

LabTechnician::LabTechnician(string n, int id) : ForensicExpert(n, id) {}

string LabTechnician::getSpecialty() const {
    return "Lab Technician";
}

// --- FieldAgent methods ---

FieldAgent::FieldAgent(string n, int id) : ForensicExpert(n, id) {}

string FieldAgent::getSpecialty() const {
    return "Field Agent";
}

// --- ForensicLab methods ---

void ForensicLab::addLabTech(const LabTechnician& e) {
    labTechs.add(e);
    expertMap[e.getId()] = nullptr;
}

void ForensicLab::addFieldAgent(const FieldAgent& e) {
    fieldAgents.add(e);
    expertMap[e.getId()] = nullptr;
}

void ForensicLab::listExperts() {
    cout << "\n--- Lab Technicians ---\n";
    for (int i = 0; i < labTechs.size(); i++) {
        LabTechnician e = labTechs[i];
        cout << e << " - " << e.getSpecialty() << endl;
    }

    cout << "\n--- Field Agents ---\n";
    for (int i = 0; i < fieldAgents.size(); i++) {
        FieldAgent e = fieldAgents[i];
        cout << e << " - " << e.getSpecialty() << endl;
    }
}

void ForensicLab::save() {
    json j;
    j["lab_technicians"] = json::array();
    j["field_agents"] = json::array();

    for (int i = 0; i < labTechs.size(); i++) {
        LabTechnician e = labTechs[i];
        j["lab_technicians"].push_back({
            {"id", e.getId()},
            {"name", e.getName()}
        });
    }

    for (int i = 0; i < fieldAgents.size(); i++) {
        FieldAgent e = fieldAgents[i];
        j["field_agents"].push_back({
            {"id", e.getId()},
            {"name", e.getName()}
        });
    }

    ofstream file("data/forensics.json");
    file << j.dump(4); // pretty print
}

void ForensicLab::load() {
    labTechs.clear();
    fieldAgents.clear();
    expertMap.clear();

    

    ifstream file("data/forensics.json");
  

    if (!file.is_open()) {
        cout << "Warning: Could not open forensics.json.\n";
        return;
    }
    
    json j;
    file >> j;

    for (auto& item : j["lab_technicians"]) {
        int id = item["id"];
        string name = item["name"];
        addLabTech(name, id);
    }

    for (auto& item : j["field_agents"]) {
        int id = item["id"];
        string name = item["name"];
        addFieldAgent(FieldAgent(name, id));
    }
}

void ForensicLab::addLabTech(const string& name, int id) {
    LabTechnician tech(name, id);
    addLabTech(tech);
}

// --- Singleton Registry ---

ForensicLabRegistry* ForensicLabRegistry::instance = nullptr;

ForensicLabRegistry::ForensicLabRegistry() {}

ForensicLabRegistry* ForensicLabRegistry::getInstance() {
    if (instance == nullptr) {
        instance = new ForensicLabRegistry();
    }
    return instance;
}

ForensicLab& ForensicLabRegistry::getLab() {
    return lab;
}

// --- Menu ---

void forensicsMenu() {
    ForensicLab& lab = ForensicLabRegistry::getInstance()->getLab();
    lab.load();

    int choice;

    do {
        cout << "\n--- Forensics Management Menu ---\n";
        cout << "1. Add Lab Technician\n";
        cout << "2. Add Field Agent\n";
        cout << "3. List All Experts\n";
        cout << "4. Save Data\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            string name;
            int id;
            cout << "Enter Technician ID: ";
            cin >> id;
            cin.ignore();
            cout << "Enter Name: ";
            getline(cin, name);
            lab.addLabTech(name, id);
        }

        else if (choice == 2) {
            string name;
            int id;
            cout << "Enter Field Agent ID: ";
            cin >> id;
            cin.ignore();
            cout << "Enter Name: ";
            getline(cin, name);
            lab.addFieldAgent(FieldAgent(name, id));
        }

        else if (choice == 3) {
            lab.listExperts();
        }

        else if (choice == 4) {
            lab.save();
            cout << "Data saved successfully.\n";
        }

    } while (choice != 0);

    lab.save();
}
