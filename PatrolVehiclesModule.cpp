#include "PatrolVehiclesModule.h"

int Vehicle::nextId = 1;
int PatrolFleet::nextPatrolId = 1;

FleetRegistry* FleetRegistry::instance = nullptr;

// Vehicle constructors
Vehicle::Vehicle() : type(""), vehicleId(0), year(0), mileage(0.0), engineSize(0.0), numSeats(0), availabilityStatus(true), price(0.0) {
    vehicleId = nextId++;
}
Vehicle::Vehicle(string t, int id) : type(t) {
    if (id == 0) {
        vehicleId = nextId;
        nextId++;
    } else vehicleId = id;
    availabilityStatus = true;
    mileage = 0.0;
}
Car::Car() : Vehicle("Car") {}
Car::Car(int id) : Vehicle("Car", id) {}
Bike::Bike() : Vehicle("Bike") {}
Bike::Bike(int id) : Vehicle("Bike", id) {}

// Vehicle operator implementation
ostream& operator<<(ostream& os, const Vehicle& v) {
    os << v.type << " (ID: " << v.vehicleId << ")\n"
       << "Model: " << v.model << "\n"
       << "Year: " << v.year << "\n"
       << "Color: " << v.color << "\n"
       << "License Plate: " << v.licensePlate << "\n"
       << "Mileage: " << v.mileage << "\n"
       << "Fuel Type: " << v.fuelType << "\n"
       << "Engine Size: " << v.engineSize << "\n"
       << "Transmission: " << v.transmissionType << "\n"
       << "Seats: " << v.numSeats << "\n"
       << "Class: " << v.vehicleClass << "\n"
       << "Status: " << (v.availabilityStatus ? "Available" : "Unavailable") << "\n"
       << "Maintenance History: " << v.maintenanceHistory;
    return os;
}

json Vehicle::toJson() const {
    try {
        return json{
            {"type", type},
            {"vehicleId", vehicleId},
            {"model", model},
            {"year", year},
            {"color", color},
            {"licensePlate", licensePlate},
            {"mileage", mileage},
            {"fuelType", fuelType},
            {"engineSize", engineSize},
            {"transmissionType", transmissionType},
            {"numSeats", numSeats},
            {"vehicleClass", vehicleClass},
            {"maintenanceHistory", maintenanceHistory},
            {"availabilityStatus", availabilityStatus},
            {"price", price}
        };
    } catch (const exception& e) {
        cout << "Error serializing Vehicle to JSON: " << e.what() << endl;
        return json{};
    }
}

void Vehicle::fromJson(const json& j) {
    try {
        type = j.value("type", "");
        vehicleId = j.value("vehicleId", 0);
        model = j.value("model", "");
        year = j.value("year", 0);
        color = j.value("color", "");
        licensePlate = j.value("licensePlate", "");
        mileage = j.value("mileage", 0.0);
        fuelType = j.value("fuelType", "");
        engineSize = j.value("engineSize", 0.0);
        transmissionType = j.value("transmissionType", "");
        numSeats = j.value("numSeats", 0);
        vehicleClass = j.value("vehicleClass", "");
        maintenanceHistory = j.value("maintenanceHistory", "");
        availabilityStatus = j.value("availabilityStatus", true);
        price = j.value("price", 0.0);
    } catch (const exception& e) {
        cout << "Error deserializing Vehicle from JSON: " << e.what() << endl;
    }
}

json Car::toJson() const {
    try {
        return Vehicle::toJson();
    } catch (const exception& e) {
        cout << "Error serializing Car to JSON: " << e.what() << endl;
        return json{};
    }
}

void Car::fromJson(const json& j) {
    try {
        Vehicle::fromJson(j);
    } catch (const exception& e) {
        cout << "Error deserializing Car from JSON: " << e.what() << endl;
    }
}

json Bike::toJson() const {
    try {
        return Vehicle::toJson();
    } catch (const exception& e) {
        cout << "Error serializing Bike to JSON: " << e.what() << endl;
        return json{};
    }
}

void Bike::fromJson(const json& j) {
    try {
        Vehicle::fromJson(j);
    } catch (const exception& e) {
        cout << "Error deserializing Bike from JSON: " << e.what() << endl;
    }
}

// Patrol constructors
Patrol::Patrol() : vehicleId(0), status("Inactive"), startTime(""), endTime("") {}
Patrol::Patrol(string id, string area, int vehicleId, const vector<Constable>& constables)
    : patrolId(id), area(area), vehicleId(vehicleId), assignedConstables(constables) {
    status = "Active";
    startTime = "Started";
    endTime = "";
}

json Patrol::toJson() const {
    try {
        json constablesArr = json::array();
        for (const auto& c : assignedConstables)
            constablesArr.push_back(c.toJson());
        return json{
            {"patrolId", patrolId},
            {"area", area},
            {"vehicleId", vehicleId},
            {"status", status},
            {"startTime", startTime},
            {"endTime", endTime},
            {"logs", logs},
            {"assignedConstables", constablesArr}
        };
    } catch (const exception& e) {
        cout << "Error serializing Patrol to JSON: " << e.what() << endl;
        return json{};
    }
}
void Patrol::fromJson(const json& j) {
    try {
        patrolId = j.value("patrolId", "");
        area = j.value("area", "");
        vehicleId = j.value("vehicleId", 0);
        status = j.value("status", "Inactive");
        startTime = j.value("startTime", "");
        endTime = j.value("endTime", "");
        logs = j.value("logs", std::vector<std::string>{});
        assignedConstables.clear();
        if (j.contains("assignedConstables")) {
            for (const auto& jc : j["assignedConstables"]) {
                Constable c;
                c.fromJson(jc);
                assignedConstables.push_back(c);
            }
        }
    } catch (const exception& e) {
        cout << "Error deserializing Patrol from JSON: " << e.what() << endl;
    }
}

void Patrol::setStatus(const string& newStatus) {
    if (newStatus == "Completed" || newStatus == "Cancelled") {
        endTime = "Ended";
    }
    status = newStatus;
}

void Patrol::addLog(const string& logEntry) {
    logs.push_back(logEntry);
}

void Patrol::displayInfo() const {
    cout << "\nPatrol Information:\n";
    cout << "Patrol ID: " << patrolId << "\n";
    cout << "Area: " << area << "\n";
    cout << "Vehicle ID: " << vehicleId << "\n";
    cout << "Status: " << status << "\n";
    cout << "Assigned Constables: ";
    for (const auto& c : assignedConstables)
        cout << c.getName() << " (" << c.getId() << ") ";
    cout << "\nStart Time: " << startTime << "\n";
    if (!endTime.empty()) {
        cout << "End Time: " << endTime << "\n";
    }
    cout << "\nPatrol Logs:\n";
    for (const auto& log : logs) {
        cout << log << "\n";
    }
}

ostream& operator<<(ostream& os, const Patrol& p) {
    os << "Patrol ID: " << p.getPatrolId() << "\n"
       << "Area: " << p.getArea() << "\n"
       << "Vehicle ID: " << p.getVehicleId() << "\n"
       << "Status: " << p.getStatus() << "\n"
       << "Assigned Constables: ";
    for (const auto& c : p.getAssignedConstables())
        os << c.getName() << " (" << c.getId() << ") ";
    os << "\nStart Time: " << p.startTime << "\n";
    if (!p.endTime.empty()) {
        os << "End Time: " << p.endTime << "\n";
    }
    return os;
}

// PatrolFleet constructors
PatrolFleet::PatrolFleet() {}
const vector<Vehicle>& PatrolFleet::getVehicles() const { return vehicles; }
vector<Vehicle>& PatrolFleet::getVehicles() { return vehicles; }

// PatrolFleet implementations
void PatrolFleet::addVehicle(const Vehicle& v) {
    vehicles.push_back(v);
    vehicleMap[v.getId()] = v;
}

void PatrolFleet::listVehicles() {
    cout << "Vehicles:\n";
    for (const auto& pair : vehicleMap) {
        cout << pair.second << "\n";
    }
}

bool PatrolFleet::searchVehicle(int id) const {
    return vehicleMap.find(id) != vehicleMap.end();
}

const Vehicle& PatrolFleet::getVehicle(int id) const {
    return vehicleMap.at(id);
}

bool PatrolFleet::updateVehicleStatus(int id, const string& status) {
    if (vehicleMap.find(id) != vehicleMap.end()) {
        if(status == "Available" || status == "available"){
            vehicleMap[id].setAvailabilityStatus(true);
        }
        else if(status == "Unavailable" || status == "unavailable"){
            vehicleMap[id].setAvailabilityStatus(false);
        }
        else{
            cout << "Invalid status. Please enter 'Available' or 'Unavailable'." << endl;
            return false;
        }
        return true;
    }
    return false;
}

bool PatrolFleet::updateMaintenanceHistory(int id, const string& history) {
    if (vehicleMap.find(id) != vehicleMap.end()) {
        vehicleMap[id].setMaintenanceHistory(history);
        return true;
    }
    return false;
}

string PatrolFleet::createPatrol(int vehicleId, const string& area, const vector<Constable>& constables) {
    if (!searchVehicle(vehicleId)) {
        return "";
    }
    if (!vehicleMap[vehicleId].getAvailabilityStatus()) {
        return "";
    }
    string patrolId = "P" + to_string(nextPatrolId++);
    patrols[patrolId] = Patrol(patrolId, area, vehicleId, constables);
    vehicleMap[vehicleId].setAvailabilityStatus(false);
    return patrolId;
}

bool PatrolFleet::endPatrol(const string& patrolId) {
    if (patrols.find(patrolId) == patrols.end()) {
        return false;
    }
    Patrol& patrol = patrols[patrolId];
    patrol.setStatus("Completed");
    vehicleMap[patrol.getVehicleId()].setAvailabilityStatus(true);
    return true;
}

bool PatrolFleet::addPatrolLog(const string& patrolId, const string& logEntry) {
    if (patrols.find(patrolId) == patrols.end()) {
        return false;
    }
    patrols[patrolId].addLog(logEntry);
    return true;
}

void PatrolFleet::listPatrols() const {
    cout << "\nActive Patrols:\n";
    for (const auto& pair : patrols) {
        if (pair.second.getStatus() == "Active") {
            cout << pair.second << "\n";
        }
    }
}

bool PatrolFleet::searchPatrol(const string& patrolId) const {
    return patrols.find(patrolId) != patrols.end();
}

const Patrol& PatrolFleet::getPatrol(const string& patrolId) const {
    return patrols.at(patrolId);
}

// PatrolFleet JSON persistence
void PatrolFleet::saveVehiclesToFile(const string& filename) const {
    try {
        json jArr = json::array();
        for (const auto& pair : vehicleMap) {
            jArr.push_back(pair.second.toJson());
        }
        std::ofstream file(filename);
        if (file) file << jArr.dump(4);
    } catch (const exception& e) {
        cout << "Error saving vehicles to file: " << e.what() << endl;
    }
}

void PatrolFleet::loadVehiclesFromFile(const string& filename) {
    try {
        ifstream file(filename);
        if (!file) return;
        json jArr;
        file >> jArr;
        vehicles.clear();
        vehicleMap.clear();
        for (const auto& jv : jArr) {
            string type = jv.value("type", "");
            Vehicle* v = nullptr;
            if (type == "Car") v = new Car();
            else if (type == "Bike") v = new Bike();
            else v = new Vehicle();
            v->fromJson(jv);
            vehicles.push_back(*v);
            vehicleMap[v->getId()] = *v;
            delete v;
        }
    } catch (const exception& e) {
        cout << "Error loading vehicles from file: " << e.what() << endl;
    }
}

void PatrolFleet::savePatrolsToFile(const string& filename) const {
    try {
        json jArr = json::array();
        for (const auto& pair : patrols) {
            jArr.push_back(pair.second.toJson());
        }
        std::ofstream file(filename);
        if (file) file << jArr.dump(4);
    } catch (const exception& e) {
        cout << "Error saving patrols to file: " << e.what() << endl;
    }
}

void PatrolFleet::loadPatrolsFromFile(const string& filename) {
    try {
        std::ifstream file(filename);
        if (!file) return;
        json jArr;
        file >> jArr;
        patrols.clear();
        for (const auto& jp : jArr) {
            Patrol p;
            p.fromJson(jp);
            patrols[p.getPatrolId()] = p;
        }
    } catch (const exception& e) {
        cout << "Error loading patrols from file: " << e.what() << endl;
    }
}

// FleetRegistry constructors
FleetRegistry::FleetRegistry() {}

// FleetRegistry implementations
FleetRegistry* FleetRegistry::getInstance() {
    if (!instance) instance = new FleetRegistry();
    return instance;
}

PatrolFleet& FleetRegistry::getFleet() { return fleet; }

// Menu implementation
void vehicleMenu(PatrolFleet& fleet) {
    int choice;
    do {
        cout << "\n--- Vehicle Management ---\n";
        cout << "1. Add Car\n"
             << "2. Add Bike\n"
             << "3. List Vehicles\n"
             << "4. Search Vehicle\n"
             << "5. Update Availability\n"
             << "6. Update Maintenance History\n"
             << "7. Save Vehicles\n"
             << "8. Load Vehicles\n"
             << "0. Back\n"
             << "Choice: ";
        cin >> choice;
        
        if (choice == 1 || choice == 2) {
            Vehicle* v = nullptr;
            if (choice == 1) {
                v = new Car();
            }
            else {
                v = new Bike();
            }
            
            string input;
            int numInput;
            double doubleInput;
            
            // Required fields
            cout << "Enter model (e.g., Toyota Camry): ";
            cin.ignore();
            getline(cin, input);
            v->setModel(input);
            
            cout << "Enter license plate (e.g., ABC123): ";
            getline(cin, input);
            v->setLicensePlate(input);
            
            // Optional fields
            cout << "Enter year (press Enter to skip): ";
            getline(cin, input);
            if (!input.empty()) {
                v->setYear(stoi(input));
            }
            
            cout << "Enter color (press Enter to skip): ";
            getline(cin, input);
            if (!input.empty()) {
                v->setColor(input);
            }
            
            cout << "Enter mileage in km (press Enter to skip): ";
            getline(cin, input);
            if (!input.empty()) {
                v->setMileage(stod(input));
            }
            
            cout << "Enter maintenance history (press Enter to skip): ";
            getline(cin, input);
            if (!input.empty()) {
                v->setMaintenanceHistory(input);
            }
            
            fleet.addVehicle(*v);
            cout << "Vehicle added with ID: " << v->getId() << endl;
            delete v;
        }
        else if (choice == 3) {
            fleet.listVehicles();
        }
        else if (choice == 4) {
            int searchId;
            cout << "Enter vehicle ID to search (e.g., 1, 2, 3): ";
            cin >> searchId;
            if (fleet.searchVehicle(searchId)) {
                cout << fleet.getVehicle(searchId) << endl;
            } else {
                cout << "Vehicle not found.\n";
            }
        }
        else if (choice == 5) {
            int updateId;
            cout << "Enter vehicle ID to update availability (e.g., 1, 2, 3): ";
            cin >> updateId;
            string newStatus;
            cout << "Enter new availability (type 'Available' or 'Unavailable'): ";
            cin.ignore();
            getline(cin, newStatus);
            if (fleet.updateVehicleStatus(updateId, newStatus)) {
                cout << "Availability updated.\n";
            } else cout << "Vehicle not found.\n";
        }
        else if (choice == 6) {
            int updateId;
            cout << "Enter vehicle ID to update maintenance history (e.g., 1, 2, 3): ";
            cin >> updateId;
            string newHistory;
            cout << "Enter new maintenance history (e.g., 'Oil change at 10000km'): ";
            cin.ignore();
            getline(cin, newHistory);
            if (fleet.updateMaintenanceHistory(updateId, newHistory)) {
                cout << "Maintenance history updated.\n";
            } else cout << "Vehicle not found.\n";
        }
        else if (choice == 7) {
            fleet.saveVehiclesToFile("vehicles.json");
            cout << "Vehicles saved to vehicles.json\n";
        }
        else if (choice == 8) {
            fleet.loadVehiclesFromFile("vehicles.json");
            cout << "Vehicles loaded from vehicles.json\n";
        }
    } while (choice != 0);
}

void patrolMenu(PatrolFleet& fleet) {
    int choice;
    do {
        cout << "\n--- Patrol Management ---\n";
        cout << "1. Create New Patrol\n"
             << "2. End Patrol\n"
             << "3. Add Patrol Log\n"
             << "4. List Active Patrols\n"
             << "5. Search Patrol\n"
             << "6. Save Patrols\n"
             << "7. Load Patrols\n"
             << "0. Back\n"
             << "Choice: ";
        cin >> choice;
        
        if (choice == 1) {
            int vehicleId;
            string area;
            cout << "Enter vehicle ID for patrol: ";
            cin >> vehicleId;
            cin.ignore();
            cout << "Enter patrol area: ";
            getline(cin, area);

             // Fetch available constables
            auto& officerManager = OfficerRegistry::getInstance()->getManager();
            std::vector<Constable> available = officerManager.getAvailableConstables();
            if (available.empty()) {
                cout << "No available constables for patrol assignment.\n";
                continue;
            }
            cout << "Available Constables:\n";
            for (size_t i = 0; i < available.size(); ++i) {
                cout << i+1 << ". " << available[i].getName() << " (ID: " << available[i].getId() << ")\n";
            }
            cout << "Enter the numbers of constables to assign (comma separated, e.g., 1,3): ";
            string input;
            getline(cin, input);

            vector<Constable> selected;
            stringstream ss(input);
            string token;
            while (getline(ss, token, ',')) {
                int idx = stoi(token) - 1;
                if (idx >= 0 && idx < (int)available.size()) {
                    selected.push_back(available[idx]);
                }
            }
            if (selected.empty()) {
                cout << "No constables selected. Patrol not created.\n";
                continue;
            }

            // Mark selected constables as assigned
            for (auto& c : selected) {
                Officer* ptr = officerManager.getOfficerMap()[c.getId()];
                if (ptr && ptr->getRole() == "Constable")
                    static_cast<Constable*>(ptr)->setAssigned(true);
            }

            string patrolId = fleet.createPatrol(vehicleId, area, selected);
            if (!patrolId.empty()) {
                cout << "Patrol created with ID: " << patrolId << endl;
            } else {
                cout << "Failed to create patrol. Vehicle might not be available or not found.\n";
                // Unassign if patrol creation failed
                for (auto& c : selected) {
                    Officer* ptr = officerManager.getOfficerMap()[c.getId()];
                    if (ptr && ptr->getRole() == "Constable")
                        static_cast<Constable*>(ptr)->setAssigned(false);
                }
            }
        }
        else if (choice == 2) {
        string patrolId;
        cout << "Enter patrol ID to end (e.g., P1, P2): ";
        cin >> patrolId;
        if (fleet.searchPatrol(patrolId)) {
                // Unassign all constables in this patrol
            const Patrol& patrol = fleet.getPatrol(patrolId);
            auto& officerManager = OfficerRegistry::getInstance()->getManager();
            for (const auto& c : patrol.getAssignedConstables()) {
                Officer* ptr = officerManager.getOfficerMap()[c.getId()];
                if (ptr && ptr->getRole() == "Constable")
                    static_cast<Constable*>(ptr)->setAssigned(false);
                }
            }
            if (fleet.endPatrol(patrolId)) {
                cout << "Patrol ended successfully.\n";
            } else {
                cout << "Patrol not found.\n";
            }
        }
        else if (choice == 3) {
            string patrolId, logEntry;
            cout << "Enter patrol ID: ";
            cin >> patrolId;
            cin.ignore();
            cout << "Enter log entry: ";
            getline(cin, logEntry);
            if (fleet.addPatrolLog(patrolId, logEntry)) {
                cout << "Log entry added.\n";
            } else {
                cout << "Patrol not found.\n";
            }
        }
        else if (choice == 4) {
            fleet.listPatrols();
        }
        else if (choice == 5) {
            string patrolId;
            cout << "Enter patrol ID to search (e.g., P1, P2): ";
            cin >> patrolId;
            if (fleet.searchPatrol(patrolId)) {
                fleet.getPatrol(patrolId).displayInfo();
            } else {
                cout << "Patrol not found.\n";
            }
        }
        else if (choice == 6) {
            fleet.savePatrolsToFile("patrolling.json");
            cout << "Patrols saved to patrolling.json\n";
        }
        else if (choice == 7) {
            fleet.loadPatrolsFromFile("patrolling.json");
            cout << "Patrols loaded from patrolling.json\n";
        }
    } while (choice != 0);
}

void patrolVehiclesMenu() {
    PatrolFleet& fleet = FleetRegistry::getInstance()->getFleet();
    
    int choice;
    do {
        cout << "\n--- Patrol Vehicles System ---\n";
        cout << "1. Patrols\n"
             << "2. Vehicles\n"
             << "0. Exit\n"
             << "Choice: ";
        cin >> choice;
        
        if (choice == 1) {
            patrolMenu(fleet);
        }
        else if (choice == 2) {
            vehicleMenu(fleet);
        }
    } while (choice != 0);
}

void Vehicle::displayInfo() const {
    cout << "Vehicle Info:" << endl;
    cout << *this << endl;
}

void Car::displayInfo() const {
    cout << "Car Info:" << endl;
    cout << *this << endl;
}

void Bike::displayInfo() const {
    cout << "Bike Info:" << endl;
    cout << *this << endl;
} 