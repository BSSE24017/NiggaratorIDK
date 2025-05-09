#include "JailModule.h"
using namespace std;

JailRegistry* JailRegistry::instance = nullptr;

void Jail::listCells() {
    cout << "\nHigh Security Cells (Capacity: " << HIGH_SEC_CAPACITY << " per cell):" << endl;
    for (int i = 1; i <= NUM_HIGH_SEC_CELLS; i++) {
        Cell* cell = cellMap[i];
        cout << *cell << endl;
        cout << "Prisoners in cell:" << endl;
        for (const auto& prisoner : cell->getPrisoners()) {
            cout << *prisoner << endl;
        }
        if (cell->getAssignedOfficer()) {
            cout << "Assigned CO: " << cell->getAssignedOfficer()->getName() 
                 << " (ID: " << cell->getAssignedOfficer()->getId() << ")" << endl;
        }
        cout << "------------------------" << endl;
    }
    
    cout << "\nStandard Cells (Capacity: " << STANDARD_CAPACITY << " per cell):" << endl;
    for (int i = NUM_HIGH_SEC_CELLS + 1; i <= NUM_HIGH_SEC_CELLS + NUM_STANDARD_CELLS; i++) {
        Cell* cell = cellMap[i];
        cout << *cell << endl;
        cout << "Prisoners in cell:" << endl;
        for (const auto& prisoner : cell->getPrisoners()) {
            cout << *prisoner << endl;
        }
        if (cell->getAssignedOfficer()) {
            cout << "Assigned CO: " << cell->getAssignedOfficer()->getName() 
                 << " (ID: " << cell->getAssignedOfficer()->getId() << ")" << endl;
        }
        cout << "------------------------" << endl;
    }
}

void Jail::save() {
    json j;
    json prisonersArray = json::array();
    
    // Save prisoners
    for (const auto& pair : prisonerMap) {
        Prisoner* p = pair.second;
        json prisonerJson = {
            {"id", p->getId()},
            {"name", p->getName()},
            {"crime", p->getCrime()},
            {"sentenceYears", p->getSentenceYears()},
            {"currentCellId", p->getCurrentCellId()}
        };
        prisonersArray.push_back(prisonerJson);
    }
    j["prisoners"] = prisonersArray;
    
    // Save cell assignments
    json cellsArray = json::array();
    for (const auto& pair : cellMap) {
        Cell* cell = pair.second;
        json cellJson = {
            {"id", cell->getId()},
            {"type", cell->getType()},
            {"assignedOfficerId", cell->getAssignedOfficer() ? cell->getAssignedOfficer()->getId() : -1}
        };
        cellsArray.push_back(cellJson);
    }
    j["cells"] = cellsArray;
    
    // Write to file
    ofstream file("jail_data.json");
    if (file.is_open()) {
        file << j.dump(4); // Pretty print with 4 spaces
        file.close();
    } else {
        cout << "Error opening file for saving" << endl;
    }
}

void Jail::load() {
    ifstream file("jail_data.json");
    if (!file.is_open()) {
        cout << "Error opening file for loading" << endl;
        return;
    }
    
    json j;
    file >> j;
    file.close();
    
    // Clear existing data
    prisonerMap.clear();
    
    // Load prisoners
    if (j.contains("prisoners")) {
        for (const auto& prisonerJson : j["prisoners"]) {
            Prisoner* p = new Prisoner(
                prisonerJson["id"],
                prisonerJson["name"],
                prisonerJson["crime"],
                prisonerJson["sentenceYears"],
                prisonerJson["currentCellId"]
            );
            prisonerMap[p->getId()] = p;
            
            // Reassign prisoner to their cell
            int cellId = prisonerJson["currentCellId"];
            if (cellId != -1) {
                auto cellIt = cellMap.find(cellId);
                if (cellIt != cellMap.end()) {
                    cellIt->second->addPrisoner(p);
                }
            }
        }
    }
    
    // Load cell assignments
    if (j.contains("cells")) {
        for (const auto& cellJson : j["cells"]) {
            int cellId = cellJson["id"];
            int officerId = cellJson["assignedOfficerId"];
            
            if (officerId != -1) {
                auto& officerMap = OfficerRegistry::getInstance()->getManager().getOfficerMap();
                auto officerIt = officerMap.find(officerId);
                if (officerIt != officerMap.end()) {
                    cellMap[cellId]->assignOfficer(officerIt->second);
                }
            }
        }
    }
    
    cout << "Loaded " << prisonerMap.size() << " prisoners from file." << endl;
}

JailRegistry* JailRegistry::getInstance() {
    if (instance == nullptr) {
        instance = new JailRegistry();
    }
    return instance;
}

Jail& JailRegistry::getJail() { return jail; }

bool Jail::assignOfficerToCell(int officerId, int cellId) {
    auto cellIt = cellMap.find(cellId);
    if (cellIt == cellMap.end()) {
        return false;
    }
    
    auto& officerMap = OfficerRegistry::getInstance()->getManager().getOfficerMap();
    auto officerIt = officerMap.find(officerId);
    if (officerIt == officerMap.end()) {
        return false;
    }
    
    // Check if the officer is a CO
    if (officerIt->second->getRole() != "CO") {
        cout << "Error: Only Correctional Officers (CO) can be assigned to cells." << endl;
        return false;
    }
    
    cellIt->second->assignOfficer(officerIt->second);
    save(); // Auto-save after assigning officer
    return true;
}

bool Jail::removeOfficerFromCell(int cellId) {
    auto cellIt = cellMap.find(cellId);
    if (cellIt == cellMap.end()) {
        return false;
    }
    
    cellIt->second->assignOfficer(nullptr);
    save(); // Auto-save after removing officer
    return true;
}

void Jail::listOfficerAssignments() {
    cout << "\nOfficer Assignments:" << endl;
    for (const auto& pair : cellMap) {
        Cell* cell = pair.second;
        cout << "Cell " << cell->getId() << " (" << cell->getType() << "): ";
        if (cell->getAssignedOfficer()) {
            cout << cell->getAssignedOfficer()->getName() 
                 << " (ID: " << cell->getAssignedOfficer()->getId() << ")";
        } else {
            cout << "No officer assigned";
        }
        cout << endl;
    }
}

void jailMenu() {
    JailRegistry* registry = JailRegistry::getInstance();
    Jail& jail = registry->getJail();
    
    while (true) {
        cout << "\nJail Management System" << endl;
        cout << "1. View Cell Information" << endl;
        cout << "2. Add Prisoner" << endl;
        cout << "3. Assign Prisoner to Cell" << endl;
        cout << "4. Change Prisoner's Cell" << endl;
        cout << "5. Remove Prisoner" << endl;
        cout << "6. List All Prisoners" << endl;
        cout << "7. Assign Officer to Cell" << endl;
        cout << "8. Remove Officer from Cell" << endl;
        cout << "9. List Officer Assignments" << endl;
        cout << "0. Exit" << endl;
        cout << "Enter your choice: ";
        
        int choice;
        cin >> choice;
        
        switch (choice) {
            case 1: {
                cout << "\nJail Configuration:" << endl;
                cout << "High Security Cells: " << jail.getNumHighSecCells() 
                     << " (Capacity: " << jail.getHighSecCapacity() << " per cell)" << endl;
                cout << "Standard Cells: " << jail.getNumStandardCells() 
                     << " (Capacity: " << jail.getStandardCapacity() << " per cell)" << endl;
                jail.listCells();
                break;
            }
            case 2: {
                int id, years;
                string name, crime;
                cout << "Enter prisoner ID: ";
                cin >> id;
                cout << "Enter prisoner name: ";
                cin.ignore();
                getline(cin, name);
                
                // Validate crime input
                bool validCrime = false;
                do {
                    cout << "Enter crime (theft/vandalism/assault/general): ";
                    getline(cin, crime);
                    // Convert to lowercase for comparison
                    string crimeLower = crime;
                    transform(crimeLower.begin(), crimeLower.end(), crimeLower.begin(), ::tolower);
                    
                    if (crimeLower == "theft" || crimeLower == "vandalism" || 
                        crimeLower == "assault" || crimeLower == "general") {
                        validCrime = true;
                        // Store the original case
                        crime = crimeLower;
                    } else {
                        cout << "Invalid crime type. Please enter one of: theft, vandalism, assault, or general" << endl;
                    }
                } while (!validCrime);
                
                cout << "Enter sentence years: ";
                cin >> years;
                if (jail.addPrisoner(Prisoner(id, name, crime, years))) {
                    cout << "Prisoner added successfully" << endl;
                } else {
                    cout << "Failed to add prisoner" << endl;
                }
                break;
            }
            case 3: {
                int prisonerId, cellId;
                cout << "Enter prisoner ID: ";
                cin >> prisonerId;
                cout << "Enter cell ID (1-" << jail.getNumHighSecCells() + jail.getNumStandardCells() << "): ";
                cin >> cellId;
                if (jail.assignPrisonerToCell(prisonerId, cellId)) {
                    cout << "Prisoner assigned successfully" << endl;
                } else {
                    cout << "Failed to assign prisoner" << endl;
                }
                break;
            }
            case 4: {
                int prisonerId, newCellId;
                cout << "Enter prisoner ID: ";
                cin >> prisonerId;
                cout << "Enter new cell ID (1-" << jail.getNumHighSecCells() + jail.getNumStandardCells() << "): ";
                cin >> newCellId;
                if (jail.changePrisonerCell(prisonerId, newCellId)) {
                    cout << "Cell changed successfully" << endl;
                } else {
                    cout << "Failed to change cell" << endl;
                }
                break;
            }
            case 5: {
                int prisonerId;
                cout << "Enter prisoner ID: ";
                cin >> prisonerId;
                if (jail.removePrisoner(prisonerId)) {
                    cout << "Prisoner removed successfully" << endl;
                } else {
                    cout << "Failed to remove prisoner" << endl;
                }
                break;
            }
            case 6:
                jail.listPrisoners();
                break;
            case 7: {
                int cellId;
                cout << "Enter cell ID (1-" << jail.getNumHighSecCells() + jail.getNumStandardCells() << "): ";
                cin >> cellId;
                
                // Show available COs
                cout << "\nAvailable Correctional Officers:" << endl;
                auto& officerMap = OfficerRegistry::getInstance()->getManager().getOfficerMap();
                bool hasAvailableCOs = false;
                
                for (const auto& pair : officerMap) {
                    if (pair.second->getRole() == "CO") {
                        cout << "ID: " << pair.second->getId() 
                             << ", Name: " << pair.second->getName() << endl;
                        hasAvailableCOs = true;
                    }
                }
                
                if (!hasAvailableCOs) {
                    cout << "No Correctional Officers available." << endl;
                    break;
                }
                
                int officerId;
                cout << "Enter CO ID to assign: ";
                cin >> officerId;
                
                if (jail.assignOfficerToCell(officerId, cellId)) {
                    cout << "Officer assigned successfully" << endl;
                } else {
                    cout << "Failed to assign officer" << endl;
                }
                break;
            }
            case 8: {
                int cellId;
                cout << "Enter cell ID (1-" << jail.getNumHighSecCells() + jail.getNumStandardCells() << "): ";
                cin >> cellId;
                if (jail.removeOfficerFromCell(cellId)) {
                    cout << "Officer removed successfully" << endl;
                } else {
                    cout << "Failed to remove officer" << endl;
                }
                break;
            }
            case 9:
                jail.listOfficerAssignments();
                break;
            case 0:
                jail.save(); // Final save before exit
                return;
            default:
                cout << "Invalid choice" << endl;
        }
    }
}

Jail::Jail() { 
    initializeCells();
    load(); // Auto-load data when jail is created
}

bool Jail::addPrisoner(const Prisoner& prisoner) {
    if (prisonerMap.find(prisoner.getId()) != prisonerMap.end()) {
        return false; // Prisoner already exists
    }
    prisonerMap[prisoner.getId()] = new Prisoner(prisoner);
    save(); // Auto-save after adding prisoner
    return true;
}

bool Jail::assignPrisonerToCell(int prisonerId, int cellId) {
    auto prisonerIt = prisonerMap.find(prisonerId);
    auto cellIt = cellMap.find(cellId);
    
    if (prisonerIt == prisonerMap.end() || cellIt == cellMap.end()) {
        return false; // Prisoner or cell not found
    }
    
    Prisoner* prisoner = prisonerIt->second;
    Cell* cell = cellIt->second;
    
    // Remove from current cell if assigned
    if (prisoner->getCurrentCellId() != -1) {
        Cell* currentCell = cellMap[prisoner->getCurrentCellId()];
        currentCell->removePrisoner(prisonerId);
    }
    
    bool result = cell->addPrisoner(prisoner);
    if (result) {
        save(); // Auto-save after successful assignment
    }
    return result;
}

bool Jail::changePrisonerCell(int prisonerId, int newCellId) {
    bool result = assignPrisonerToCell(prisonerId, newCellId);
    if (result) {
        save(); // Auto-save after successful cell change
    }
    return result;
}

bool Jail::removePrisoner(int prisonerId) {
    auto prisonerIt = prisonerMap.find(prisonerId);
    if (prisonerIt == prisonerMap.end()) {
        return false;
    }
    
    Prisoner* prisoner = prisonerIt->second;
    if (prisoner->getCurrentCellId() != -1) {
        Cell* currentCell = cellMap[prisoner->getCurrentCellId()];
        currentCell->removePrisoner(prisonerId);
    }
    
    delete prisoner;
    prisonerMap.erase(prisonerIt);
    save(); // Auto-save after removing prisoner
    return true;
}

void Jail::listPrisoners() {
    cout << "\nAll Prisoners:" << endl;
    for (const auto& pair : prisonerMap) {
        cout << *pair.second << endl;
        cout << "------------------------" << endl;
    }
}

Prisoner* Jail::getPrisoner(int prisonerId) {
    auto it = prisonerMap.find(prisonerId);
    return it != prisonerMap.end() ? it->second : nullptr;
}

Cell* Jail::getCell(int cellId) {
    auto it = cellMap.find(cellId);
    return it != cellMap.end() ? it->second : nullptr;
} 