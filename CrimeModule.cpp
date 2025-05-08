#include "CrimeModule.h"
#include <algorithm> // Added for std::sort
#include <fstream>
#include <sstream> // For string stream operations
using json = nlohmann::json;

using namespace std;

CrimeRegistry* CrimeRegistry::instance = nullptr;

void Case::displayDetails() const {
    cout << "Case Type: " << type << ", ID: " << caseId << ", Severity: " << severity;
    if (!date.empty()) {
        cout << ", Date: " << date;
    }
    if (!reportedBy.empty()) {
        cout << ", Reported by: " << reportedBy;
    }
    if (location) {
        cout << ", " << *location;
    }
    cout << endl;
    if (!description.empty()) {
        cout << "  Description: " << description << endl;
    }
}

void Theft::displayDetails() const {
    cout << "Theft Case - ID: " << caseId
        << ", Stolen Value: $" << stolenValue
        << ", Severity: " << severity;

    if (!date.empty()) {
        cout << ", Date: " << date;
    }
    if (!reportedBy.empty()) {
        cout << ", Reported by: " << reportedBy;
    }
    if (!itemStolen.empty()) {
        cout << ", Item: " << itemStolen;
    }
    cout << ", Recovered: " << (recovered ? "Yes" : "No");

    if (location) {
        cout << ", " << *location;
    }
    cout << endl;
    if (!description.empty()) {
        cout << "  Description: " << description << endl;
    }
}

void Assault::displayDetails() const {
    cout << "Assault Case - ID: " << caseId
        << ", Weapon Used: " << (weaponUsed ? "Yes" : "No")
        << ", Severity: " << severity;

    if (!date.empty()) {
        cout << ", Date: " << date;
    }
    if (!reportedBy.empty()) {
        cout << ", Reported by: " << reportedBy;
    }
    if (!victimName.empty()) {
        cout << ", Victim: " << victimName;
    }
    if (injurySeverity > 0) {
        cout << ", Injury Severity: " << injurySeverity;
    }

    if (location) {
        cout << ", " << *location;
    }
    cout << endl;
    if (!description.empty()) {
        cout << "  Description: " << description << endl;
    }
}

// Implementation for Vandalism display
void Vandalism::displayDetails() const {
    cout << "Vandalism Case - ID: " << caseId
        << ", Damage Value: $" << damageValue
        << ", Public Property: " << (publicProperty ? "Yes" : "No")
        << ", Severity: " << severity;

    if (!date.empty()) {
        cout << ", Date: " << date;
    }
    if (!reportedBy.empty()) {
        cout << ", Reported by: " << reportedBy;
    }
    if (!propertyType.empty()) {
        cout << ", Property Type: " << propertyType;
    }

    if (location) {
        cout << ", " << *location;
    }
    cout << endl;
    if (!description.empty()) {
        cout << "  Description: " << description << endl;
    }
}

void CrimeManager::addCase(Case* c) {
    cases.add(c);
    caseMap[c->getId()] = c;

    // Update statistics
    caseTypeCount[c->getType()]++;

    if (c->getType() == "Theft") {
        Theft* theft = dynamic_cast<Theft*>(c);
        if (theft) {
            totalStolenValue += theft->getStolenValue();
        }
    }

    if (c->getPriority() > 7.0) {
        highPriorityCaseCount++;
    }
}

// Function overloading: simpler version
void CrimeManager::addCase(int id, string type) {
    Case* newCase = nullptr;

    if (type == "Theft")
        newCase = new Theft(id);
    else if (type == "Assault")
        newCase = new Assault(id);
    else if (type == "Vandalism") 
        newCase = new Vandalism(id);
    else
        newCase = new Case(type, id);

    addCase(newCase);
}

// Function overloading: version with extra info
void CrimeManager::addCase(int id, string type, double extraInfo) {
    Case* newCase = nullptr;

    if (type == "Theft")
        newCase = new Theft(id, extraInfo);  // extraInfo is stolenValue
    else if (type == "Assault")
        newCase = new Assault(id, extraInfo > 0);  // extraInfo > 0 means weapon was used
    else if (type == "Vandalism")
        newCase = new Vandalism(id, extraInfo); // extraInfo is damage value
    else
        newCase = new Case(type, id, extraInfo);  // extraInfo is severity

    addCase(newCase);
}

// Location methods (aggregation)
Location* CrimeManager::addLocation(const string& address, const string& city, const string& state, const string& zipCode) {
    Location* loc = new Location(address, city, state, zipCode);
    locations.add(loc);
    return loc;
}

Case* CrimeManager::findCase(int id) {
    auto it = caseMap.find(id);
    if (it != caseMap.end())
        return it->second;
    return nullptr;
}

void CrimeManager::listCases() {
    std::cout << "Cases:\n";
    if (cases.size() == 0) {
        cout << "No cases found.\n";
        return;
    }

    for (auto& c : cases) {
        c->displayDetails();  // Polymorphic call - calls the appropriate version based on object type
    }

    cout << "Total cases: " << cases.size() << endl;
}

// Function demonstrating polymorphism with getPriority()
void CrimeManager::listCasesByPriority() {
    std::cout << "Cases by Priority (Highest First):\n";

    if (cases.size() == 0) {
        cout << "No cases found.\n";
        return;
    }

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
    // Create a JSON array to store cases
    json casesJson = json::array();

    for (auto& c : cases) {
        // Create a JSON object for each case
        json caseJson;
        caseJson["id"] = c->getId();
        caseJson["type"] = c->getType();
        caseJson["severity"] = c->getSeverity();

        // Add common fields
        if (!c->getDescription().empty()) {
            caseJson["description"] = c->getDescription();
        }
        if (!c->getReportedBy().empty()) {
            caseJson["reportedBy"] = c->getReportedBy();
        }
        if (!c->getDate().empty()) {
            caseJson["date"] = c->getDate();
        }

        // Add location information if available
        if (c->getLocation()) {
            caseJson["location"] = {
                {"address", c->getLocation()->getAddress()},
                {"city", c->getLocation()->getCity()},
                {"state", c->getLocation()->getState()},
                {"zipCode", c->getLocation()->getZipCode()}
            };
        }

        // Add type-specific information
        if (c->getType() == "Theft") {
            Theft* theft = dynamic_cast<Theft*>(c);
            if (theft) {
                caseJson["stolenValue"] = theft->getStolenValue();
                caseJson["itemStolen"] = theft->getItemStolen();
                caseJson["recovered"] = theft->isRecovered();
            }
        }
        else if (c->getType() == "Assault") {
            Assault* assault = dynamic_cast<Assault*>(c);
            if (assault) {
                caseJson["weaponUsed"] = assault->wasWeaponUsed();
                caseJson["victimName"] = assault->getVictimName();
                caseJson["injurySeverity"] = assault->getInjurySeverity();
            }
        }
        else if (c->getType() == "Vandalism") {
            Vandalism* vandalism = dynamic_cast<Vandalism*>(c);
            if (vandalism) {
                caseJson["damageValue"] = vandalism->getDamageValue();
                caseJson["propertyType"] = vandalism->getPropertyType();
                caseJson["publicProperty"] = vandalism->isPublicProperty();
            }
        }

        // Add this case to the array
        casesJson.push_back(caseJson);
    }

    // Write JSON to file
    ofstream file("crimes.json");
    file << casesJson.dump(4); // Indent with 4 spaces for better readability
    file.close();

    cout << "Saved " << cases.size() << " cases to file." << endl;
}

void CrimeManager::load() {
    // Clean up previous data
    for (auto& pair : caseMap) {
        delete pair.second;
    }
    cases.clear();
    caseMap.clear();

    // Clean up locations
    for (auto& loc : locations) {
        delete loc;
    }
    locations.clear();

    // Reset statistics
    caseTypeCount.clear();
    totalStolenValue = 0.0;
    highPriorityCaseCount = 0;

    try {
        // Open and read the JSON file
        ifstream file("crimes.json");
        if (!file.is_open()) {
            cout << "No saved data found or could not open file." << endl;
            return;
        }

        json casesJson;
        file >> casesJson;
        file.close();

        // Process each case in the JSON array
        for (const auto& caseJson : casesJson) {
            int id = caseJson["id"];
            string type = caseJson["type"];

            Case* newCase = nullptr;

            if (type == "Theft") {
                double stolenValue = caseJson.contains("stolenValue") ? caseJson["stolenValue"].get<double>() : 0.0;
                string itemStolen = caseJson.contains("itemStolen") ? caseJson["itemStolen"].get<string>() : "";
                bool recovered = caseJson.contains("recovered") ? caseJson["recovered"].get<bool>() : false;

                Theft* theft = new Theft(id, stolenValue, itemStolen, recovered);
                theft->setSeverity(caseJson["severity"]);

                if (caseJson.contains("itemStolen")) {
                    theft->setItemStolen(caseJson["itemStolen"]);
                }
                if (caseJson.contains("recovered")) {
                    theft->setRecovered(caseJson["recovered"]);
                }

                newCase = theft;
                totalStolenValue += stolenValue;
            }
            else if (type == "Assault") {
                bool weaponUsed = caseJson.contains("weaponUsed") ? caseJson["weaponUsed"].get<bool>() : false;
                string victimName = caseJson.contains("victimName") ? caseJson["victimName"].get<string>() : "";
                int injurySeverity = caseJson.contains("injurySeverity") ? caseJson["injurySeverity"].get<int>() : 0;

                Assault* assault = new Assault(id, weaponUsed, victimName, injurySeverity);
                assault->setSeverity(caseJson["severity"]);

                if (caseJson.contains("victimName")) {
                    assault->setVictimName(caseJson["victimName"]);
                }
                if (caseJson.contains("injurySeverity")) {
                    assault->setInjurySeverity(caseJson["injurySeverity"]);
                }

                newCase = assault;
            }
            else if (type == "Vandalism") {
                double damageValue = caseJson.contains("damageValue") ? caseJson["damageValue"].get<double>() : 0.0;
                string propertyType = caseJson.contains("propertyType") ? caseJson["propertyType"].get<string>() : "";
                bool publicProperty = caseJson.contains("publicProperty") ? caseJson["publicProperty"].get<bool>() : false;

                Vandalism* vandalism = new Vandalism(id, damageValue, propertyType, publicProperty);
                vandalism->setSeverity(caseJson["severity"]);

                if (caseJson.contains("propertyType")) {
                    vandalism->setPropertyType(caseJson["propertyType"]);
                }
                if (caseJson.contains("publicProperty")) {
                    vandalism->setPublicProperty(caseJson["publicProperty"]);
                }

                newCase = vandalism;
            }
            else {
                newCase = new Case(type, id, caseJson["severity"]);
            }

            // Set common fields if they exist
            if (caseJson.contains("description")) {
                newCase->setDescription(caseJson["description"]);
            }
            if (caseJson.contains("reportedBy")) {
                newCase->setReportedBy(caseJson["reportedBy"]);
            }
            if (caseJson.contains("date")) {
                newCase->setDate(caseJson["date"]);
            }

            // Check if location data exists
            if (caseJson.contains("location")) {
                string address = caseJson["location"]["address"];
                string city = caseJson["location"]["city"];
                string state = caseJson["location"]["state"];
                string zipCode = caseJson["location"].contains("zipCode") ?
                    caseJson["location"]["zipCode"].get<string>() : "";

                Location* loc = addLocation(address, city, state, zipCode);
                newCase->setLocation(loc);
            }

            // Update statistics
            caseTypeCount[type]++;
            if (newCase->getPriority() > 7.0) {
                highPriorityCaseCount++;
            }

            // Add the case
            addCase(newCase);
        }

        cout << "Loaded " << cases.size() << " cases from file." << endl;
    }
    catch (const exception& e) {
        cout << "Error loading data: " << e.what() << endl;
    }
}
void CrimeManager::filterCasesByType(const string& type) {
    cout << "\nCases of type '" << type << "':\n";
    bool found = false;

    for (auto& c : cases) {
        if (c->getType() == type) {
            c->displayDetails();
            found = true;
        }
    }

    if (!found) {
        cout << "No cases found of type '" << type << "'.\n";
    }
}

void CrimeManager::showDetailedCaseReport(int id) {
    Case* c = findCase(id);
    if (!c) {
        cout << "Case not found with ID: " << id << endl;
        return;
    }

    cout << "\n===== DETAILED CASE REPORT =====\n";
    cout << c->getFullInfo();

    if (c->getLocation()) {
        cout << "Location:\n"
            << "  Address: " << c->getLocation()->getAddress() << "\n"
            << "  City: " << c->getLocation()->getCity() << "\n"
            << "  State: " << c->getLocation()->getState() << "\n";

        if (!c->getLocation()->getZipCode().empty()) {
            cout << "  ZIP/Postal Code: " << c->getLocation()->getZipCode() << "\n";
        }
    }

    cout << "Case Priority: " << c->getPriority() << "\n";
    cout << "===================================\n";
}

void CrimeManager::updateCaseDetails(int id) {
    Case* c = findCase(id);
    if (!c) {
        cout << "Case not found with ID: " << id << endl;
        return;
    }

    int choice;
    cout << "\n--- Update Case Details ---\n";
    cout << "1. Update Description\n";
    cout << "2. Update Severity\n";
    cout << "3. Update Reported By\n";
    cout << "4. Update Date\n";

    // Type-specific update options
    if (c->getType() == "Theft") {
        cout << "5. Update Stolen Value\n";
        cout << "6. Update Item Stolen\n";
        cout << "7. Update Recovery Status\n";
    }
    else if (c->getType() == "Assault") {
        cout << "5. Update Weapon Used Status\n";
        cout << "6. Update Victim Name\n";
        cout << "7. Update Injury Severity\n";
    }
    else if (c->getType() == "Vandalism") {
        cout << "5. Update Damage Value\n";
        cout << "6. Update Property Type\n";
        cout << "7. Update Public Property Status\n";
    }

    cout << "0. Cancel\n";
    cout << "Choice: ";
    cin >> choice;
    cin.ignore(); // Clear newline

    string strInput;
    double numInput;
    char charInput;

    switch (choice) {
    case 1: // Description
        cout << "Enter new description: ";
        getline(cin, strInput);
        c->setDescription(strInput);
        break;
    case 2: // Severity
        cout << "Enter new severity (1-10): ";
        cin >> numInput;
        c->setSeverity(numInput);
        break;
    case 3: // Reported By
        cout << "Enter reporter name: ";
        getline(cin, strInput);
        c->setReportedBy(strInput);
        break;
    case 4: // Date
        cout << "Enter date (format YYYY-MM-DD): ";
        getline(cin, strInput);
        c->setDate(strInput);
        break;
    case 5: // Type-specific fields
        if (c->getType() == "Theft") {
            cout << "Enter new stolen value: $";
            cin >> numInput;
            dynamic_cast<Theft*>(c)->setStolenValue(numInput);
        }
        else if (c->getType() == "Assault") {
            cout << "Was weapon used? (y/n): ";
            cin >> charInput;
            dynamic_cast<Assault*>(c)->setWeaponUsed(charInput == 'y' || charInput == 'Y');
        }
        else if (c->getType() == "Vandalism") {
            cout << "Enter new damage value: $";
            cin >> numInput;
            dynamic_cast<Vandalism*>(c)->setDamageValue(numInput);
        }
        break;
    case 6:
        if (c->getType() == "Theft") {
            cout << "Enter item stolen: ";
            cin.ignore();
            getline(cin, strInput);
            dynamic_cast<Theft*>(c)->setItemStolen(strInput);
        }
        else if (c->getType() == "Assault") {
            cout << "Enter victim name: ";
            getline(cin, strInput);
            dynamic_cast<Assault*>(c)->setVictimName(strInput);
        }
        else if (c->getType() == "Vandalism") {
            cout << "Enter property type: ";
            getline(cin, strInput);
            dynamic_cast<Vandalism*>(c)->setPropertyType(strInput);
        }
        break;
    case 7:
        if (c->getType() == "Theft") {
            cout << "Has item been recovered? (y/n): ";
            cin >> charInput;
            dynamic_cast<Theft*>(c)->setRecovered(charInput == 'y' || charInput == 'Y');
        }
        else if (c->getType() == "Assault") {
            cout << "Enter injury severity (1-10): ";
            cin >> numInput;
            dynamic_cast<Assault*>(c)->setInjurySeverity(numInput);
        }
        else if (c->getType() == "Vandalism") {
            cout << "Is it public property? (y/n): ";
            cin >> charInput;
            dynamic_cast<Vandalism*>(c)->setPublicProperty(charInput == 'y' || charInput == 'Y');
        }
        break;
    case 0:
        return;
    default:
        cout << "Invalid choice.\n";
        return;
    }

    cout << "Case updated successfully.\n";
}

void CrimeManager::generateStatistics() {
    cout << "\n===== CRIME STATISTICS =====\n";

    // Total case count
    cout << "Total Cases: " << cases.size() << endl;

    // Cases by type
    cout << "\nCases by Type:\n";
    for (const auto& pair : caseTypeCount) {
        cout << pair.first << ": " << pair.second << " cases";

        // Calculate percentage
        if (cases.size() > 0) {
            double percentage = (static_cast<double>(pair.second) / cases.size()) * 100.0;
            cout << " (" << percentage << "%)";
        }
        cout << endl;
    }

    // High priority cases
    cout << "\nHigh Priority Cases (Priority > 7.0): " << highPriorityCaseCount;
    if (cases.size() > 0) {
        double percentage = (static_cast<double>(highPriorityCaseCount) / cases.size()) * 100.0;
        cout << " (" << percentage << "%)";
    }
    cout << endl;

    // Total stolen value (from theft cases)
    cout << "Total Value of Stolen Items: $" << totalStolenValue << endl;

    // Location statistics
    map<string, int> casesByCity;
    for (auto& c : cases) {
        if (c->getLocation()) {
            casesByCity[c->getLocation()->getCity()]++;
        }
    }

    cout << "\nCases by City:\n";
    for (const auto& pair : casesByCity) {
        cout << pair.first << ": " << pair.second << " cases\n";
    }

    cout << "===========================\n";
}

void CrimeManager::deleteCase(int id) {
    auto it = caseMap.find(id);
    if (it == caseMap.end()) {
        cout << "Case not found with ID: " << id << endl;
        return;
    }

    // Find the index in the list to remove
    Case* caseToDelete = it->second;
    size_t index = 0;
    bool found = false;

    for (size_t i = 0; i < cases.size(); i++) {
        if (cases[i] == caseToDelete) {
            index = i;
            found = true;
            break;
        }
    }

    if (found) {
        // Update statistics before deleting
        caseTypeCount[caseToDelete->getType()]--;

        if (caseToDelete->getType() == "Theft") {
            Theft* theft = dynamic_cast<Theft*>(caseToDelete);
            if (theft) {
                totalStolenValue -= theft->getStolenValue();
            }
        }

        if (caseToDelete->getPriority() > 7.0) {
            highPriorityCaseCount--;
        }

        // Free memory and remove from containers
        delete caseToDelete;
        cases.removeAt(index);
        caseMap.erase(it);

        cout << "Case " << id << " deleted successfully.\n";
    }
    else {
        cout << "Error finding case in list.\n";
    }
}

void CrimeManager::filterCasesByLocation(const string& city) {
    cout << "\nCases in " << city << ":\n";
    bool found = false;

    for (auto& c : cases) {
        if (c->getLocation() && c->getLocation()->getCity() == city) {
            c->displayDetails();
            found = true;
        }
    }

    if (!found) {
        cout << "No cases found in " << city << ".\n";
    }
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
        cout << "\n=== Crime Management System ===\n";
        cout << "1. Add Case\n";
        cout << "2. List Cases\n";
        cout << "3. List Cases By Priority\n";
        cout << "4. Find Case\n";
        cout << "5. Add Location to Case\n";
        cout << "6. Filter Cases by Type\n";
        cout << "7. Filter Cases by Location\n";
        cout << "8. Detailed Case Report\n";
        cout << "9. Update Case Details\n";
        cout << "10. Generate Statistics\n";
        cout << "11. Delete Case\n";
        cout << "12. Save\n";
        cout << "0. Exit\n";
        cout << "Choice: ";
        cin >> choice;

        switch (choice) {
        case 1: { // Add Case
            int id;
            string type;
            cout << "Enter Case ID: ";
            cin >> id;

            // Check if case ID already exists
            if (mgr.findCase(id)) {
                cout << "Error: Case ID " << id << " already exists.\n";
                break;
            }

            cout << "Type (Theft/Assault/Vandalism/Other): ";
            cin.ignore();
            getline(cin, type);

            if (type == "Theft") {
                double value;
                string itemStolen;
                char recovered;

                cout << "Enter stolen value: $";
                cin >> value;

                Theft* theft = new Theft(id, value, itemStolen, recovered);
                cout << "Enter item stolen: ";
                cin.ignore();
                getline(cin, itemStolen);
                theft->setItemStolen(itemStolen);

                cout << "Has the item been recovered? (y/n): ";
                cin >> recovered;
                theft->setRecovered(recovered == 'y' || recovered == 'Y');

                // Common fields
                string description, reportedBy, date;
                cout << "Enter description: ";
                cin.ignore();
                getline(cin, description);
                theft->setDescription(description);

                cout << "Enter reported by: ";
                getline(cin, reportedBy);
                theft->setReportedBy(reportedBy);

                cout << "Enter date (YYYY-MM-DD): ";
                getline(cin, date);
                theft->setDate(date);

                mgr.addCase(theft);
            }
            else if (type == "Assault") {
                char weapon;
                string victimName;
                int injurySeverity;

                cout << "Was weapon used? (y/n): ";
                cin >> weapon;
                bool weaponUsed = (weapon == 'y' || weapon == 'Y');

                Assault* assault = new Assault(id, weaponUsed, victimName, injurySeverity);
                cout << "Enter victim name: ";
                cin.ignore();
                getline(cin, victimName);
                assault->setVictimName(victimName);

                cout << "Enter injury severity (1-10): ";
                cin >> injurySeverity;
                assault->setInjurySeverity(injurySeverity);

                // Common fields
                string description, reportedBy, date;
                cout << "Enter description: ";
                cin.ignore();
                getline(cin, description);
                assault->setDescription(description);

                cout << "Enter reported by: ";
                getline(cin, reportedBy);
                assault->setReportedBy(reportedBy);

                cout << "Enter date (YYYY-MM-DD): ";
                getline(cin, date);
                assault->setDate(date);

                mgr.addCase(assault);
            }
            else if (type == "Vandalism") {
                double damageValue;
                string propertyType;
                char publicProp;

                cout << "Enter damage value: $";
                cin >> damageValue;

                Vandalism* vandalism = new Vandalism( id, damageValue, propertyType, publicProp);

                cout << "Enter property type: ";
                cin.ignore();
                getline(cin, propertyType);
                vandalism->setPropertyType(propertyType);

                cout << "Is it public property? (y/n): ";
                cin >> publicProp;
                vandalism->setPublicProperty(publicProp == 'y' || publicProp == 'Y');

                // Common fields
                string description, reportedBy, date;
                cout << "Enter description: ";
                cin.ignore();
                getline(cin, description);
                vandalism->setDescription(description);

                cout << "Enter reported by: ";
                getline(cin, reportedBy);
                vandalism->setReportedBy(reportedBy);

                cout << "Enter date (YYYY-MM-DD): ";
                getline(cin, date);
                vandalism->setDate(date);

                mgr.addCase(vandalism);
            }
            else {
                double severity;
                cout << "Enter severity (1-10): ";
                cin >> severity;

                Case* generalCase = new Case(type, id, severity);

                // Common fields
                string description, reportedBy, date;
                cout << "Enter description: ";
                cin.ignore();
                getline(cin, description);
                generalCase->setDescription(description);

                cout << "Enter reported by: ";
                getline(cin, reportedBy);
                generalCase->setReportedBy(reportedBy);

                cout << "Enter date (YYYY-MM-DD): ";
                getline(cin, date);
                generalCase->setDate(date);

                mgr.addCase(generalCase);
            }
            cout << "Case added successfully.\n";
            break;
        }
        case 2: // List Cases
            mgr.listCases();
            break;
        case 3: // List Cases By Priority
            mgr.listCasesByPriority();
            break;
        case 4: { // Find Case
            int id;
            cout << "Enter Case ID to find: ";
            cin >> id;

            Case* found = mgr.findCase(id);
            if (found) {
                cout << "Found case: ";
                found->displayDetails();
            }
            else {
                cout << "Case not found.\n";
            }
            break;
        }
        case 5: { // Add Location to Case
            int id;
            string address, city, state, zipCode;
            cout << "Enter Case ID to add location: ";
            cin >> id;

            Case* found = mgr.findCase(id);
            if (found) {
                cin.ignore();
                cout << "Enter address: ";
                getline(cin, address);
                cout << "Enter city: ";
                getline(cin, city);
                cout << "Enter state: ";
                getline(cin, state);
                cout << "Enter ZIP/postal code: ";
                getline(cin, zipCode);

                Location* loc = mgr.addLocation(address, city, state, zipCode);
                found->setLocation(loc);
                cout << "Location added to case.\n";
            }
            else {
                cout << "Case not found.\n";
            }
            break;
        }
        case 6: { // Filter Cases by Type
            string type;
            cout << "Enter case type (Theft/Assault/Vandalism/etc): ";
            cin.ignore();
            getline(cin, type);
            mgr.filterCasesByType(type);
            break;
        }
        case 7: { // Filter Cases by Location
            string city;
            cout << "Enter city name: ";
            cin.ignore();
            getline(cin, city);
            mgr.filterCasesByLocation(city);
            break;
        }
        case 8: { // Detailed Case Report
            int id;
            cout << "Enter Case ID for detailed report: ";
            cin >> id;
            mgr.showDetailedCaseReport(id);
            break;
        }
        case 9: { // Update Case Details
            int id;
            cout << "Enter Case ID to update: ";
            cin >> id;
            mgr.updateCaseDetails(id);
            break;
        }
        case 10: // Generate Statistics
            mgr.generateStatistics();
            break;
        case 11: { // Delete Case
            int id;
            cout << "Enter Case ID to delete: ";
            cin >> id;

            // Confirm deletion
            char confirm;
            cout << "Are you sure you want to delete case " << id << "? (y/n): ";
            cin >> confirm;

            if (confirm == 'y' || confirm == 'Y') {
                mgr.deleteCase(id);
            }
            else {
                cout << "Deletion cancelled.\n";
            }
            break;
        }
        case 12: // Save
            mgr.save();
            cout << "All data saved.\n";
            break;
        case 0: // Exit
            cout << "Saving before exit...\n";
            mgr.save();
            cout << "Exiting program. Goodbye!\n";
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 0);
}