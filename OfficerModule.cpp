#include "OfficerModule.h"
#include <fstream>
using json = nlohmann::json;
using namespace std;

OfficerRegistry* OfficerRegistry::instance = nullptr;

// Initialize static member
int Officer::nextId = 1;  // Start IDs from 1

// Officer class implementations
Officer* Officer::fromJson(const json& j) {
    string role = j["role"];
    Officer* officer = nullptr;
    
    if (role == "SHO") {
        officer = new SHO(j["name"], j["id"]);
    } else if (role == "CO") {
        officer = new CO(j["name"], j["id"]);
    } else if (role == "ASI") {
        officer = new ASI(j["name"], j["id"]);
    } else if (role == "Head Constable") {
        officer = new HeadConstable(j["name"], j["id"]);
    } else if (role == "Constable") {
        officer = new Constable(j["name"], j["id"]);
    }
    
    if (officer && j.contains("tasks")) {
        for (const auto& task : j["tasks"]) {
            officer->addTask(task);
        }
    }
    return officer;
}

// SHO implementations
void SHO::performDuty() const {
    cout << "Station House Officer " << name << " is managing the police station.\n";
}

vector<string> SHO::getResponsibilities() const {
    return {
        "Manage police station operations",
        "Supervise all officers",
        "Handle major cases",
        "Coordinate with higher authorities"
    };
}

// CO implementations
void CO::performDuty() const {
    cout << "Circle Officer " << name << " is managing the police circle.\n";
}

vector<string> CO::getResponsibilities() const {
    return {
        "Manage police circle operations",
        "Supervise multiple police stations",
        "Handle administrative matters",
        "Coordinate with district authorities"
    };
}

// ASI implementations
void ASI::performDuty() const {
    cout << "Assistant Sub-Inspector " << name << " is handling investigations.\n";
}

vector<string> ASI::getResponsibilities() const {
    return {
        "Conduct investigations",
        "Supervise constables",
        "Handle case documentation",
        "Assist SHO in station management"
    };
}

// HeadConstable implementations
void HeadConstable::performDuty() const {
    cout << "Head Constable " << name << " is supervising constables.\n";
}

vector<string> HeadConstable::getResponsibilities() const {
    return {
        "Supervise constables",
        "Handle routine police work",
        "Maintain station records",
        "Assist in investigations"
    };
}

// Constable implementations
void Constable::performDuty() const {
    cout << "Constable " << name << " is performing routine duties.\n";
}

vector<string> Constable::getResponsibilities() const {
    return {
        "Perform routine police work",
        "Maintain law and order",
        "Assist in investigations",
        "Handle public interactions"
    };
}

// OfficerManager implementations
OfficerManager::~OfficerManager() {
    for (auto* officer : officers) {
        delete officer;
    }
}

void OfficerManager::addOfficer(Officer* o) {
    officers.add(o);
    officerMap[o->getId()] = o;
}

void OfficerManager::listOfficers() {
    for (const auto* officer : officers) {
        cout << *officer << endl;
        cout << "Responsibilities:\n";
        for (const auto& resp : officer->getResponsibilities()) {
            cout << "- " << resp << "\n";
        }
        cout << "\n";
    }
}

json OfficerManager::toJson() const {
    json j;
    json officersArray = json::array();
    
    for (const auto* officer : officers) {
        officersArray.push_back(officer->toJson());
    }
    
    j["officers"] = officersArray;
    return j;
}

void OfficerManager::fromJson(const json& j) {
    // Clear existing officers
    for (auto* officer : officers) {
        delete officer;
    }
    officers.clear();
    officerMap.clear();
    
    // First pass: find the highest ID
    int highestId = 0;
    for (const auto& officerJson : j["officers"]) {
        int id = officerJson["id"];
        if (id > highestId) {
            highestId = id;
        }
    }
    
    // Update the static nextId
    if (highestId >= Officer::nextId) {
        Officer::nextId = highestId + 1;
    }
    
    // Second pass: create officers
    for (const auto& officerJson : j["officers"]) {
        Officer* officer = Officer::fromJson(officerJson);
        if (officer) {
            addOfficer(officer);
        }
    }
}

void OfficerManager::saveToJson(const string& filename) {
    ofstream file(filename);
    if (file.is_open()) {
        json j = toJson();
        file << j.dump(4); // Pretty print with 4 spaces
        file.close();
    }
}

void OfficerManager::loadFromJson(const string& filename) {
    ifstream file(filename);
    if (file.is_open()) {
        json j;
        file >> j;
        fromJson(j);
        file.close();
    }
}

void OfficerManager::save() {
    saveToJson("officers.json");
}

void OfficerManager::load() {
    loadFromJson("officers.json");
}

OfficerRegistry* OfficerRegistry::getInstance() {
    if (instance == nullptr) {
        instance = new OfficerRegistry();
    }
    return instance;
}

OfficerManager& OfficerRegistry::getManager() {
    return manager;
}

void displayOfficerTasks(const Officer* officer) {
    cout << "\nTasks for " << officer->getName() << " (" << officer->getRank() << "):\n";
    const auto& tasks = officer->getTasks();
    if (tasks.empty()) {
        cout << "No tasks assigned.\n";
        return;
    }
    for (size_t i = 0; i < tasks.size(); ++i) {
        cout << i + 1 << ". " << tasks[i] << "\n";
    }
}

void taskMenu(Officer* officer) {
    while (true) {
        cout << "\nTask Management for " << officer->getName() << "\n";
        cout << "1. View Tasks\n";
        cout << "2. Add Task\n";
        cout << "3. Remove Task\n";
        cout << "4. Clear All Tasks\n";
        cout << "5. View Responsibilities\n";
        cout << "6. Perform Duty\n";
        cout << "7. Back\n";
        cout << "Enter choice: ";
        
        int choice;
        cin >> choice;
        
        switch (choice) {
            case 1:
                displayOfficerTasks(officer);
                break;
            case 2: {
                string task;
                cout << "Enter task description: ";
                cin.ignore();
                getline(cin, task);
                officer->addTask(task);
                cout << "Task added.\n";
                break;
            }
            case 3: {
                displayOfficerTasks(officer);
                if (!officer->getTasks().empty()) {
                    cout << "Enter task number to remove: ";
                    size_t index;
                    cin >> index;
                    if (index > 0 && index <= officer->getTasks().size()) {
                        officer->removeTask(index - 1);
                        cout << "Task removed.\n";
                    } else {
                        cout << "Invalid task number.\n";
                    }
                }
                break;
            }
            case 4:
                officer->clearTasks();
                cout << "All tasks cleared.\n";
                break;
            case 5: {
                cout << "\nResponsibilities:\n";
                for (const auto& resp : officer->getResponsibilities()) {
                    cout << "- " << resp << "\n";
                }
                break;
            }
            case 6:
                officer->performDuty();
                break;
            case 7:
                return;
            default:
                cout << "Invalid choice\n";
        }
    }
}

void officerMenu() {
    OfficerRegistry* registry = OfficerRegistry::getInstance();
    OfficerManager& manager = registry->getManager();
    
    while (true) {
        cout << "\nOfficer Management System\n";
        cout << "1. Add Officer\n";
        cout << "2. List Officers\n";
        cout << "3. Manage Tasks\n";
        cout << "4. Save to JSON\n";
        cout << "5. Load from JSON\n";
        cout << "6. Exit\n";
        cout << "Enter choice: ";
        
        int choice;
        cin >> choice;
        
        switch (choice) {
            case 1: {
                string name;
                cout << "Enter name: ";
                cin >> name;
                
                cout << "Select rank:\n";
                cout << "1. SHO (Station House Officer)\n";
                cout << "2. CO (Circle Officer)\n";
                cout << "3. ASI (Assistant Sub-Inspector)\n";
                cout << "4. Head Constable\n";
                cout << "5. Constable\n";
                cout << "Enter choice (1-5): ";
                
                int rankChoice;
                cin >> rankChoice;
                
                Officer* officer = nullptr;
                switch (rankChoice) {
                    case 1: officer = new SHO(name); break;
                    case 2: officer = new CO(name); break;
                    case 3: officer = new ASI(name); break;
                    case 4: officer = new HeadConstable(name); break;
                    case 5: officer = new Constable(name); break;
                }
                
                if (officer) {
                    manager.addOfficer(officer);
                    cout << "Officer added with ID: " << officer->getId() << endl;
                }
                break;
            }
            case 2:
                manager.listOfficers();
                break;
            case 3: {
                manager.listOfficers();
                cout << "Enter officer ID to manage tasks: ";
                int id;
                cin >> id;
                
                // Find officer by ID
                bool found = false;
                for (const auto* officer : manager.getOfficers()) {
                    if (officer->getId() == id) {
                        taskMenu(const_cast<Officer*>(officer));
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    cout << "Officer not found.\n";
                }
                break;
            }
            case 4:
                manager.saveToJson("officers.json");
                cout << "Saved to officers.json\n";
                break;
            case 5:
                manager.loadFromJson("officers.json");
                cout << "Loaded from officers.json\n";
                break;
            case 6:
                return;
            default:
                cout << "Invalid choice\n";
        }
    }
} 