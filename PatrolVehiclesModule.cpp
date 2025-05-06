#include "PatrolVehiclesModule.h"

FleetRegistry* FleetRegistry::instance = nullptr;

void PatrolFleet::addVehicle(const Vehicle& v) {
    vehicles.add(v);
    vehicleMap[v.getId()] = v;
}
void PatrolFleet::listVehicles() {
    std::cout << "Vehicles:\n";
    for (auto& v : vehicles) std::cout << v << "\n";
}
void PatrolFleet::save() {
    std::ofstream file("data/vehicles.txt");
    for (auto& v : vehicles) {
        file << v.getId() << "," << v.getType() << "\n";
    }
    file.close();
}
void PatrolFleet::load() {
    vehicles.clear();
    vehicleMap.clear();
    std::ifstream file("data/vehicles.txt");
    std::string line;
    while (getline(file, line)) {
        int id;
        std::string type;
        size_t pos = line.find(',');
        if (pos == std::string::npos) continue;
        id = std::stoi(line.substr(0, pos));
        type = line.substr(pos + 1);
        if (type == "Car")
            addVehicle(Car(id));
        else if (type == "Bike")
            addVehicle(Bike(id));
        else
            addVehicle(Vehicle(type, id));
    }
    file.close();
}

FleetRegistry* FleetRegistry::getInstance() {
    if (!instance) instance = new FleetRegistry();
    return instance;
}
PatrolFleet& FleetRegistry::getFleet() { return fleet; }

void patrolVehiclesMenu() {
    PatrolFleet& fleet = FleetRegistry::getInstance()->getFleet();
    fleet.load();
    int choice;
    do {
        std::cout << "\n--- Patrol Vehicles Management ---\n";
        std::cout << "1. Add Car\n2. Add Bike\n3. List Vehicles\n4. Save\n0. Back\nChoice: ";
        std::cin >> choice;
        if (choice == 1) {
            int id;
            std::cout << "Enter Vehicle ID: "; std::cin >> id;
            fleet.addVehicle(Car(id));
        } else if (choice == 2) {
            int id;
            std::cout << "Enter Vehicle ID: "; std::cin >> id;
            fleet.addVehicle(Bike(id));
        } else if (choice == 3) {
            fleet.listVehicles();
        } else if (choice == 4) {
            fleet.save();
            std::cout << "Saved.\n";
        }
    } while (choice != 0);
    fleet.save();
} 