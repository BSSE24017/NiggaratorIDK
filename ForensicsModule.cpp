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
    std::cout << "Lab Technicians:\n";
    for (auto& e : labTechs) std::cout << e << " - " << e.getSpecialty() << "\n";
    std::cout << "Field Agents:\n";
    for (auto& e : fieldAgents) std::cout << e << " - " << e.getSpecialty() << "\n";
}
void ForensicLab::save() {
    std::ofstream file("data/forensics.txt");
    for (auto& e : labTechs)
        file << e.getId() << "," << e.getName() << ",Lab Technician\n";
    for (auto& e : fieldAgents)
        file << e.getId() << "," << e.getName() << ",Field Agent\n";
    file.close();
}
void ForensicLab::load() {
    labTechs.clear();
    fieldAgents.clear();
    expertMap.clear();
    std::ifstream file("data/forensics.txt");
    std::string line;
    while (getline(file, line)) {
        int id;
        std::string name, type;
        size_t pos1 = line.find(',');
        size_t pos2 = line.rfind(',');
        if (pos1 == std::string::npos || pos2 == std::string::npos) continue;
        id = std::stoi(line.substr(0, pos1));
        name = line.substr(pos1 + 1, pos2 - pos1 - 1);
        type = line.substr(pos2 + 1);
        if (type == "Lab Technician")
            addLabTech(LabTechnician(name, id));
        else if (type == "Field Agent")
            addFieldAgent(FieldAgent(name, id));
    }
    file.close();
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
        std::cout << "\n--- Forensics Management ---\n";
        std::cout << "1. Add Lab Technician\n2. Add Field Agent\n3. List Experts\n4. Save\n0. Back\nChoice: ";
        std::cin >> choice;
        if (choice == 1) {
            std::string name;
            int id;
            std::cout << "Enter ID: "; std::cin >> id;
            std::cout << "Enter Name: "; std::cin.ignore(); getline(std::cin, name);
            lab.addLabTech(LabTechnician(name, id));
        } else if (choice == 2) {
            std::string name;
            int id;
            std::cout << "Enter ID: "; std::cin >> id;
            std::cout << "Enter Name: "; std::cin.ignore(); getline(std::cin, name);
            lab.addFieldAgent(FieldAgent(name, id));
        } else if (choice == 3) {
            lab.listExperts();
        } else if (choice == 4) {
            lab.save();
            std::cout << "Saved.\n";
        }
    } while (choice != 0);
    lab.save();
} 