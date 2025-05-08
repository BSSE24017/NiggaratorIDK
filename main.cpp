#include <iostream>
#include "OfficerModule.h"
#include "CrimeModule.h"
#include "ForensicsModule.h"
#include "PatrolVehiclesModule.h"
#include "JailModule.h"

int main() {
    // Load all data at startup
    OfficerRegistry::getInstance()->getManager().load();
    CrimeRegistry::getInstance()->getManager().load();
    FleetRegistry::getInstance()->getFleet().loadVehiclesFromFile("vehicles.json");
    FleetRegistry::getInstance()->getFleet().loadPatrolsFromFile("patrolling.json");

    int choice;
    do {
        std::cout << "\n=== Law Enforcement Management System ===\n";
        std::cout << "1. Officer Management\n";
        std::cout << "2. Crime Management\n";
        std::cout << "3. Forensics Management\n";
        std::cout << "4. Patrol Vehicles Management\n";
        std::cout << "5. Jail Management\n";
        std::cout << "0. Exit\n";
        std::cout << "Enter choice: ";
        std::cin >> choice;
        switch(choice) {
            case 1: officerMenu(); break;
            case 2: crimeMenu(); break;
            case 3: forensicsMenu(); break;
            case 4: patrolVehiclesMenu(); break;
            case 5: jailMenu(); break;
        }
    } while(choice != 0);
    return 0;
} 
