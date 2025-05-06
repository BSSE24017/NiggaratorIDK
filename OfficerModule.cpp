#include "OfficerModule.h"

OfficerRegistry* OfficerRegistry::instance = nullptr;

void OfficerManager::addOfficer(const Officer& o) {
    officers.add(o);
    officerMap[o.getId()] = o;
}

void OfficerManager::listOfficers() {
    std::cout << "Officers:\n";
    for (auto& o : officers) {
        std::cout << o << " - " << o.getRole() << "\n";
    }
}

void OfficerManager::save() {
    std::ofstream file("data/officers.txt");
    for (auto& o : officers) {
        file << o.getId() << "," << o.getName() << "," << o.getRole() << "\n";
    }
    file.close();
}

void OfficerManager::load() {
    officers.clear();
    officerMap.clear();
    std::ifstream file("data/officers.txt");
    std::string line;
    while (getline(file, line)) {
        int id;
        std::string name, role;
        size_t pos1 = line.find(',');
        size_t pos2 = line.rfind(',');
        if (pos1 == std::string::npos || pos2 == std::string::npos) continue;
        id = std::stoi(line.substr(0, pos1));
        name = line.substr(pos1 + 1, pos2 - pos1 - 1);
        role = line.substr(pos2 + 1);
        if (role == "SHO")
            addOfficer(SHO(name, id));
        else if (role == "Constable")
            addOfficer(Constable(name, id));
        else
            addOfficer(Officer(name, id));
    }
    file.close();
}

OfficerRegistry* OfficerRegistry::getInstance() {
    if (!instance) instance = new OfficerRegistry();
    return instance;
}
OfficerManager& OfficerRegistry::getManager() { return manager; }

void officerMenu() {
    OfficerManager& mgr = OfficerRegistry::getInstance()->getManager();
    mgr.load();
    int choice;
    do {
        std::cout << "\n--- Officer Management ---\n";
        std::cout << "1. Add Officer\n2. List Officers\n3. Save\n0. Back\nChoice: ";
        std::cin >> choice;
        if (choice == 1) {
            std::string name, role;
            int id;
            std::cout << "Enter ID: "; std::cin >> id;
            std::cout << "Enter Name: "; std::cin.ignore(); getline(std::cin, name);
            std::cout << "Role (SHO/Constable/Other): "; getline(std::cin, role);
            if (role == "SHO")
                mgr.addOfficer(SHO(name, id));
            else if (role == "Constable")
                mgr.addOfficer(Constable(name, id));
            else
                mgr.addOfficer(Officer(name, id));
        } else if (choice == 2) {
            mgr.listOfficers();
        } else if (choice == 3) {
            mgr.save();
            std::cout << "Saved.\n";
        }
    } while (choice != 0);
    mgr.save();
} 