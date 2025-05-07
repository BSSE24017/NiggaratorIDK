#include "CrimeModule.h"
#include <algorithm> // Added for std::sort

CrimeRegistry* CrimeRegistry::instance = nullptr;

void CrimeManager::addCase(Case* c) {
    cases.add(c);
    caseMap[c->getId()] = c;
}

// Function overloading: simpler version
void CrimeManager::addCase(int id, std::string type) {
    Case* newCase = nullptr;

    if (type == "Theft")
        newCase = new Theft(id);
    else if (type == "Assault")
        newCase = new Assault(id);
    else
        newCase = new Case(type, id);

    addCase(newCase);
}

// Function overloading: version with extra info
void CrimeManager::addCase(int id, std::string type, double extraInfo) {
    Case* newCase = nullptr;

    if (type == "Theft")
        newCase = new Theft(id, extraInfo);  // extraInfo is stolenValue
    else if (type == "Assault")
        newCase = new Assault(id, extraInfo > 0);  // extraInfo > 0 means weapon was used
    else
        newCase = new Case(type, id, extraInfo);  // extraInfo is severity

    addCase(newCase);
}

Case* CrimeManager::findCase(int id) {
    auto it = caseMap.find(id);
    if (it != caseMap.end())
        return it->second;
    return nullptr;
}

void CrimeManager::listCases() {
    std::cout << "Cases:\n";
    for (auto& c : cases) {
        c->displayDetails();  // Polymorphic call - calls the appropriate version based on object type
    }
}

// New function demonstrating polymorphism with getPriority()
void CrimeManager::listCasesByPriority() {
    std::cout << "Cases by Priority (Highest First):\n";

    // Create a temporary vector to sort
    std::vector<Case*> sortedCases;
    for (auto& c : cases) {
        sortedCases.push_back(c);
    }

    // Sort by priority (higher priority first)
    std::sort(sortedCases.begin(), sortedCases.end(),
        [](Case* a, Case* b) { return a->getPriority() > b->getPriority(); });

    // Display sorted cases
    for (auto& c : sortedCases) {
        std::cout << "Priority: " << c->getPriority() << " - ";
        c->displayDetails();
    }
}

void CrimeManager::save() {
    std::ofstream file("data/crimes.txt");
    for (auto& c : cases) {
        file << c->getId() << "," << c->getType();

        // Add extra information based on case type
        if (c->getType() == "Theft") {
            Theft* theft = dynamic_cast<Theft*>(c);
            if (theft) {
                file << "," << theft->getStolenValue();
            }
        }
        else if (c->getType() == "Assault") {
            Assault* assault = dynamic_cast<Assault*>(c);
            if (assault) {
                file << "," << (assault->wasWeaponUsed() ? "1" : "0");
            }
        }
        file << "\n";
    }
    file.close();
}

void CrimeManager::load() {
    // Clean up previous data
    for (auto& pair : caseMap) {
        delete pair.second;
    }
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
        std::string remainder = line.substr(pos + 1);

        pos = remainder.find(',');
        if (pos == std::string::npos) {
            // Simple case with no extra info
            type = remainder;
            addCase(id, type);
        }
        else {
            // Case with extra info
            type = remainder.substr(0, pos);
            double extraInfo = std::stod(remainder.substr(pos + 1));

            addCase(id, type, extraInfo);
        }
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
        std::cout << "1. Add Case\n";
        std::cout << "2. List Cases\n";
        std::cout << "3. List Cases By Priority\n";  // New option
        std::cout << "4. Save\n";
        std::cout << "5. Find Case\n";  // New option
        std::cout << "0. Back\n";
        std::cout << "Choice: ";
        std::cin >> choice;

        if (choice == 1) {
            int id;
            std::string type;
            std::cout << "Enter Case ID: ";
            std::cin >> id;
            std::cout << "Type (Theft/Assault/Other): ";
            std::cin.ignore();
            getline(std::cin, type);

            if (type == "Theft") {
                double value;
                std::cout << "Enter stolen value: $";
                std::cin >> value;
                mgr.addCase(id, "Theft", value);
            }
            else if (type == "Assault") {
                char weapon;
                std::cout << "Was weapon used? (y/n): ";
                std::cin >> weapon;
                mgr.addCase(id, "Assault", (weapon == 'y' || weapon == 'Y') ? 1.0 : 0.0);
            }
            else {
                double severity;
                std::cout << "Enter severity (1-10): ";
                std::cin >> severity;
                mgr.addCase(id, type, severity);
            }

        }
        else if (choice == 2) {
            mgr.listCases();
        }
        else if (choice == 3) {
            mgr.listCasesByPriority();  // New function showing polymorphism
        }
        else if (choice == 4) {
            mgr.save();
            std::cout << "Saved.\n";
        }
        else if (choice == 5) {
            int id;
            std::cout << "Enter Case ID to find: ";
            std::cin >> id;

            Case* found = mgr.findCase(id);
            if (found) {
                std::cout << "Found case: ";
                found->displayDetails();  // Polymorphic call
            }
            else {
                std::cout << "Case not found.\n";
            }
        }
    } while (choice != 0);
    mgr.save();
}