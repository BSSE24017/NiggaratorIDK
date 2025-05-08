#include "ForensicsModule.h"

ForensicLabRegistry* ForensicLabRegistry::instance = nullptr;

ForensicExpert::ForensicExpert(string n, int id) : name(n), expertId(id) {}
ForensicExpert::~ForensicExpert() {}
int ForensicExpert::getId() const { return expertId; }
string ForensicExpert::getName() const { return name; }
bool ForensicExpert::operator==(const ForensicExpert& other) const { return expertId == other.expertId; }

ostream& operator<<(ostream& os, const ForensicExpert& f) {
    os << f.name << " (ID: " << f.expertId << ")";
    return os;
}

LabTechnician::LabTechnician(string n, int id) : ForensicExpert(n, id) {}
string LabTechnician::getSpecialty() const { return "Lab Technician"; }

FieldAgent::FieldAgent(string n, int id) : ForensicExpert(n, id) {}
string FieldAgent::getSpecialty() const { return "Field Agent"; }

Evidence::Evidence(int id, string desc, string stat)
    : evidenceId(id), description(desc), status(stat) {}

int Evidence::getId() const { return evidenceId; }
string Evidence::getDescription() const { return description; }
string Evidence::getStatus() const { return status; }
void Evidence::setStatus(const string& newStatus) { status = newStatus; }

ostream& operator<<(ostream& os, const Evidence& e) {
    os << "Evidence ID: " << e.evidenceId << ", Description: " << e.description << ", Status: " << e.status;
    return os;
}

void ForensicLab::addLabTech(const LabTechnician& e) {
    labTechs.add(e);
    expertMap[e.getId()] = nullptr;
}

void ForensicLab::addFieldAgent(const FieldAgent& e) {
    fieldAgents.add(e);
    expertMap[e.getId()] = nullptr;
}

void ForensicLab::addLabTech(const string& name, int id) {
    addLabTech(LabTechnician(name, id));
}

void ForensicLab::listExperts() {
    cout << "Lab Technicians:\n";
    for (int i = 0; i < labTechs.size(); ++i)
        cout << labTechs[i] << " - " << labTechs[i].getSpecialty() << "\n";

    cout << "Field Agents:\n";
    for (int i = 0; i < fieldAgents.size(); ++i)
        cout << fieldAgents[i] << " - " << fieldAgents[i].getSpecialty() << "\n";
}

void ForensicLab::assignEvidence(int techId, const Evidence& ev) {
    assignedEvidence[techId].push_back(ev);
    cout << "Evidence assigned to Technician #" << techId << ". Hope they know what they’re doing.\n";
}

void ForensicLab::viewEvidence(int techId) {
    if (assignedEvidence.count(techId) == 0 || assignedEvidence[techId].empty()) {
        cout << "No evidence assigned to Technician #" << techId << ". They might be napping.\n";
        return;
    }

    cout << "Evidence assigned to Technician #" << techId << ":\n";
    for (const auto& e : assignedEvidence[techId]) {
        cout << e << "\n";
    }
}

void ForensicLab::removeExpert(int id) {
    bool removed = false;
    for (int i = 0; i < labTechs.size(); ++i) {
        if (labTechs[i].getId() == id) {
            labTechs.removeAt(i);
            removed = true;
            break;
        }
    }

    for (int i = 0; i < fieldAgents.size(); ++i) {
        if (fieldAgents[i].getId() == id) {
            fieldAgents.removeAt(i);
            removed = true;
            break;
        }
    }

    expertMap.erase(id);
    assignedEvidence.erase(id);

    if (removed)
        cout << "Expert removed. Let’s hope they weren’t on a breakthrough.\n";
    else
        cout << "No expert found with that ID.\n";
}

void ForensicLab::save() {
    json j;
    j["lab_technicians"] = json::array();
    j["field_agents"] = json::array();
    j["assigned_evidence"] = json::object();

    for (int i = 0; i < labTechs.size(); ++i) {
        auto& e = labTechs[i];
        j["lab_technicians"].push_back({ {"id", e.getId()}, {"name", e.getName()} });
    }

    for (int i = 0; i < fieldAgents.size(); ++i) {
        auto& e = fieldAgents[i];
        j["field_agents"].push_back({ {"id", e.getId()}, {"name", e.getName()} });
    }

    for (auto& pair : assignedEvidence) {
        vector<json> evidenceList;
        for (const auto& ev : pair.second) {
            evidenceList.push_back({ {"id", ev.getId()}, {"desc", ev.getDescription()}, {"status", ev.getStatus()} });
        }
        j["assigned_evidence"][to_string(pair.first)] = evidenceList;
    }

    ofstream file("data/forensics.json");
    file << j.dump(4);
}

void ForensicLab::load() {
    labTechs.clear();
    fieldAgents.clear();
    expertMap.clear();
    assignedEvidence.clear();

    ifstream file("data/forensics.json");
    if (!file.is_open()) return;

    json j;
    file >> j;

    for (auto& item : j["lab_technicians"]) {
        int id = item["id"];
        string name = item["name"];
        addLabTech(LabTechnician(name, id));
    }

    for (auto& item : j["field_agents"]) {
        int id = item["id"];
        string name = item["name"];
        addFieldAgent(FieldAgent(name, id));
    }

    for (auto& pair : j["assigned_evidence"].items()) {
        int techId = stoi(pair.key());
        for (auto& evJson : pair.value()) {
            int id = evJson["id"];
            string desc = evJson["desc"];
            string stat = evJson["status"];
            assignEvidence(techId, Evidence(id, desc, stat));
        }
    }
}

ForensicLabRegistry::ForensicLabRegistry() {}

ForensicLabRegistry* ForensicLabRegistry::getInstance() {
    if (!instance)
        instance = new ForensicLabRegistry();
    return instance;
}

ForensicLab& ForensicLabRegistry::getLab() {
    return lab;
}

void forensicsMenu() {
    ForensicLab& lab = ForensicLabRegistry::getInstance()->getLab();
    lab.load();
    int choice;

    do {
        cout << "\n--- Forensics Management Menu ---\n";
        cout << "1. Add Lab Technician\n";
        cout << "2. Add Field Agent\n";
        cout << "3. List All Experts\n";
        cout << "4. Assign Evidence to Technician\n";
        cout << "5. View Evidence Assigned to Technician\n";
        cout << "6. Remove an Expert\n";
        cout << "7. Save\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1 || choice == 2) {
            string name;
            int id;
            cout << "Enter ID: "; cin >> id;
            cout << "Enter Name: "; cin.ignore(); getline(cin, name);
            if (choice == 1)
                lab.addLabTech(name, id);
            else
                lab.addFieldAgent(FieldAgent(name, id));
        }
        else if (choice == 3) {
            lab.listExperts();
        }
        else if (choice == 4) {
            int techId, evidenceId;
            string desc;
            cout << "Enter Technician ID: "; cin >> techId;
            cout << "Enter Evidence ID: "; cin >> evidenceId;
            cout << "Enter Description: "; cin.ignore(); getline(cin, desc);
            lab.assignEvidence(techId, Evidence(evidenceId, desc));
        }
        else if (choice == 5) {
            int techId;
            cout << "Enter Technician ID: ";
            cin >> techId;
            lab.viewEvidence(techId);
        }
        else if (choice == 6) {
            int id;
            cout << "Enter Expert ID to remove: ";
            cin >> id;
            lab.removeExpert(id);
        }
        else if (choice == 7) {
            lab.save();
            cout << "Data saved successfully.\n";
        }

    } while (choice != 0);

    lab.save();
    cout << "Exiting Forensics Module. Don't forget to clean your lab station.\n";
}
