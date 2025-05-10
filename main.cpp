#include <iostream>
#include "OfficerModule.h"
#include "CrimeModule.h"
#include "ForensicsModule.h"
#include "PatrolVehiclesModule.h"
#include "JailModule.h"
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <iomanip>
#include "nlohmann/json.hpp"

using json = nlohmann::json;
using namespace std;

// bool fileExists(const string& filename) {
//     ifstream file(filename);
//     return file.good();
// }

// void loadAllData() {
//     cout << "\nLoading system data...\n";
    
//     try {
//         cout << "Loading officers... ";
//         // Load officers
//         if (fileExists("officers.json")) {
//             auto& officerManager = OfficerRegistry::getInstance()->getManager();
//             officerManager.load();
//             cout << "Done\n";
//         } else {
//             cout << "No officers file found\n";
//         }
        
//         cout << "Loading vehicles... ";
//         // Load vehicles and patrols
//         auto& fleet = FleetRegistry::getInstance()->getFleet();
//         if (fileExists("vehicles.json")) {
//             fleet.loadVehiclesFromFile("vehicles.json");
//             cout << "Done\n";
//         } else {
//             cout << "No vehicles file found\n";
//         }
        
//         cout << "Loading patrols... ";
//         if (fileExists("patrolling.json")) {
//             fleet.loadPatrolsFromFile("patrolling.json");
//             cout << "Done\n";
//         } else {
//             cout << "No patrols file found\n";
//         }
        
//         cout << "Loading crime data... ";
//         // Load crime data
//         if (fileExists("crimes.json")) {
//             auto& crimeManager = CrimeRegistry::getInstance()->getManager();
//             crimeManager.load();
//             cout << "Done\n";
//         } else {
//             cout << "No crime data file found\n";
//         }
        
//         cout << "Loading forensics data... ";
//         // Load forensics data
//         if (fileExists("forensics_data.json")) {
//             auto& forensicsLab = ForensicLabRegistry::getInstance()->getLab();
//             forensicsLab.load();
//             cout << "Done\n";
//         } else {
//             cout << "No forensics data file found\n";
//         }
        
//         cout << "Loading jail data... ";
//         // Load jail data
//         if (fileExists("jail_data.json")) {
//             auto& jail = JailRegistry::getInstance()->getJail();
//             jail.load();
//             cout << "Done\n";
//         } else {
//             cout << "No jail data file found\n";
//         }
        
//         cout << "All data loaded successfully.\n";
//     } catch (const exception& e) {
//         cout << "\nError loading data: " << e.what() << endl;
//         cout << "Error occurred during load operation. Some data may not have been loaded.\n";
//     } catch (...) {
//         cout << "\nUnknown error occurred during load operation.\n";
//         cout << "Some data may not have been loaded.\n";
//     }
// }

// void saveAllData() {
//     cout << "\nSaving system data...\n";
    
//     try {
//         cout << "Saving freeloaders... ";
//         // Save officers
//         auto& officerManager = OfficerRegistry::getInstance()->getManager();
//         officerManager.save();
//         cout << "Done\n";
        
//         cout << "Saving vehicles and patrols... ";
//         // Save vehicles and patrols
//         auto& fleet = FleetRegistry::getInstance()->getFleet();
//         fleet.saveVehiclesToFile("vehicles.json");
//         fleet.savePatrolsToFile("patrolling.json");
//         cout << "Done\n";
        
//         cout << "Saving crime reports to make new spider homes... ";
//         // Save crime data
//         auto& crimeManager = CrimeRegistry::getInstance()->getManager();
//         crimeManager.save();
//         cout << "Done\n";
        
//         cout << "Saving forensics data, cz why not take peoples privacy in name of law and order... ";
//         // Save forensics data
//         auto& forensicsLab = ForensicLabRegistry::getInstance()->getLab();
//         forensicsLab.save();
//         cout << "Done\n";
        
//         cout << "Throw- i mean, saving real indeed criminals in jail... ";
//         // Save jail data
//         auto& jail = JailRegistry::getInstance()->getJail();
//         jail.save();
//         cout << "Done\n";
        
//         cout << "All data saved successfully.\n";
//     } catch (const exception& e) {
//         cout << "\nError saving data: " << e.what() << endl;
//         cout << "Error occurred during save operation. Some data may not have been saved.\n";
//     } catch (...) {
//         cout << "\nUnknown error occurred during save operation.\n";
//         cout << "Some data may not have been saved.\n";
//     }
// }

void displayMainMenu() {
    cout << "\n"
         << "╔══════════════════════════════════════════════════════════╗\n"
         << "║              LAW ENFORCEMENT MANAGEMENT SYSTEM           ║\n"
         << "╠══════════════════════════════════════════════════════════╣\n"
         << "║ 1. Officer Management                                    ║\n"
         << "║ 2. Crime Management                                      ║\n"
         << "║ 3. Forensics Management                                  ║\n"
         << "║ 4. Patrol Vehicles Management                            ║\n"
         << "║ 5. Jail Management                                       ║\n"
         << "║ 0. Exit                                                  ║\n"
         << "╠══════════════════════════════════════════════════════════╣\n"
         << "║ Choice: ";
}

int main() {
    try {
        // Load all data on startup
        //loadAllData();
        
        int choice;
        do {
            displayMainMenu();
            cin >> choice;
            cout << "╚══════════════════════════════════════════════════════════╝\n";
            
            switch(choice) {
                case 1: officerMenu(); break;
                case 2: crimeMenu(); break;
                case 3: forensicsMenu(); break;
                case 4: patrolVehiclesMenu(); break;
                case 5: jailMenu(); break;
            }
        } while(choice != 0);
        
        // Save all data before exiting
        //saveAllData();
        
    } catch (const exception& e) {
        cout << "Error: " << e.what() << endl;
        return 1;
    } catch (...) {
        cout << "Unknown error occurred.\n";
        return 1;
    }
    
    return 0;
} 
