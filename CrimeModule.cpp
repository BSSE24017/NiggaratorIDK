#include "CrimeModule.h"

CrimeRegistry* CrimeRegistry::instance = nullptr;

void CrimeManager::addCase(const Case& c) {
    cases.add(c);
    caseMap[c.getId()] = c;
}

void CrimeManager::listCases() {
    std::cout << "Cases:\n";
    for (auto& c : cases) {
        std::cout << c << "\n";
    }
}

void CrimeManager::save() {
    std::ofstream file("data/crimes.txt");
    for (auto& c : cases) {
        file << c.getId() << "," << c.getType() << "\n";
    }
    file.close();
}

void CrimeManager::load() {
    cases.clear();
    caseMap.clear();
    std::ifstream file("data/crimes.txt");
    std::string line;
    while (getline(file, line)) {
        int id;
        std::string type;
        size_t pos = line.find(',');
        if (pos == std::string::npos) continue;
        id = std::stoi(line.substr(0, pos));
        type = line.substr(pos + 1);
        if (type == "Theft")
            addCase(Theft(id));
        else if (type == "Assault")
            addCase(Assault(id));
        else
            addCase(Case(type, id));
    }
    file.close();
}

CrimeRegistry* CrimeRegistry::getInstance() {
    if (!instance) instance = new CrimeRegistry();
    return instance;
}
CrimeManager& CrimeRegistry::getManager() { return manager; }

void crimeMenu() {
    CrimeManager& mgr = CrimeRegistry::getInstance()->getManager();
    mgr.load();
    int choice;
    do {
        std::cout << "\n--- Crime Management ---\n";
        std::cout << "1. Add Case\n2. List Cases\n3. Save\n0. Back\nChoice: ";
        std::cin >> choice;
        if (choice == 1) {
            int id;
            std::string type;
            std::cout << "Enter Case ID: "; std::cin >> id;
            std::cout << "Type (Theft/Assault/Other): "; std::cin.ignore(); getline(std::cin, type);
            if (type == "Theft")
                mgr.addCase(Theft(id));
            else if (type == "Assault")
                mgr.addCase(Assault(id));
            else
                mgr.addCase(Case(type, id));
        } else if (choice == 2) {
            mgr.listCases();
        } else if (choice == 3) {
            mgr.save();
            std::cout << "Saved.\n";
        }
    } while (choice != 0);
    mgr.save();
} 