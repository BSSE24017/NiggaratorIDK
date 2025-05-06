#include "JailModule.h"

JailRegistry* JailRegistry::instance = nullptr;

void Jail::addHighSecCell(const HighSecurityCell& c) {
    highSecCells.add(c);
    cellMap[c.getId()] = nullptr;
}
void Jail::addStandardCell(const StandardCell& c) {
    standardCells.add(c);
    cellMap[c.getId()] = nullptr;
}
void Jail::listCells() {
    std::cout << "High Security Cells:\n";
    for (auto& c : highSecCells) std::cout << c << "\n";
    std::cout << "Standard Cells:\n";
    for (auto& c : standardCells) std::cout << c << "\n";
}
void Jail::save() {
    std::ofstream file("data/jail.txt");
    for (auto& c : highSecCells)
        file << c.getId() << "," << c.getCapacity() << ",High Security\n";
    for (auto& c : standardCells)
        file << c.getId() << "," << c.getCapacity() << ",Standard\n";
    file.close();
}
void Jail::load() {
    highSecCells.clear();
    standardCells.clear();
    cellMap.clear();
    std::ifstream file("data/jail.txt");
    std::string line;
    while (getline(file, line)) {
        int id, cap;
        std::string type;
        size_t pos1 = line.find(',');
        size_t pos2 = line.rfind(',');
        if (pos1 == std::string::npos || pos2 == std::string::npos) continue;
        id = std::stoi(line.substr(0, pos1));
        cap = std::stoi(line.substr(pos1 + 1, pos2 - pos1 - 1));
        type = line.substr(pos2 + 1);
        if (type == "High Security")
            addHighSecCell(HighSecurityCell(id, cap));
        else if (type == "Standard")
            addStandardCell(StandardCell(id, cap));
    }
    file.close();
}

JailRegistry* JailRegistry::getInstance() {
    if (!instance) instance = new JailRegistry();
    return instance;
}
Jail& JailRegistry::getJail() { return jail; }

void jailMenu() {
    Jail& jail = JailRegistry::getInstance()->getJail();
    jail.load();
    int choice;
    do {
        std::cout << "\n--- Jail Management ---\n";
        std::cout << "1. Add High Security Cell\n2. Add Standard Cell\n3. List Cells\n4. Save\n0. Back\nChoice: ";
        std::cin >> choice;
        if (choice == 1) {
            int id, cap;
            std::cout << "Enter Cell ID: "; std::cin >> id;
            std::cout << "Enter Capacity: "; std::cin >> cap;
            jail.addHighSecCell(HighSecurityCell(id, cap));
        } else if (choice == 2) {
            int id, cap;
            std::cout << "Enter Cell ID: "; std::cin >> id;
            std::cout << "Enter Capacity: "; std::cin >> cap;
            jail.addStandardCell(StandardCell(id, cap));
        } else if (choice == 3) {
            jail.listCells();
        } else if (choice == 4) {
            jail.save();
            std::cout << "Saved.\n";
        }
    } while (choice != 0);
    jail.save();
} 