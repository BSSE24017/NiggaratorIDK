#include "CrimeModule.h"
#include <algorithm> // Added for std::sort
using namespace std;

CrimeRegistry* CrimeRegistry::instance = nullptr;

void Case::displayDetails() const {
	cout << "Case Type: " << type << ", ID: " << caseId << ", Severity: " << severity;
	if (location) {
		cout << ", " << *location;
	}
	cout << endl;
}

void Theft::displayDetails() const {
	cout << "Theft Case - ID: " << caseId << ", Stolen Value: $" << stolenValue << ", Severity: " << severity;
	if (location) {
		cout << ", " << *location;
	}
	cout << endl;
}

void Assault::displayDetails() const {
	cout << "Assault Case - ID: " << caseId
		<< ", Weapon Used: " << (weaponUsed ? "Yes" : "No")
		<< ", Severity: " << severity;
	if (location) {
		cout << ", " << *location;
	}
	cout << endl;
}

void CrimeManager::addCase(Case* c) {
	cases.add(c);
	caseMap[c->getId()] = c;
}

// Function overloading: simpler version
void CrimeManager::addCase(int id, string type) {
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
void CrimeManager::addCase(int id, string type, double extraInfo) {
	Case* newCase = nullptr;

	if (type == "Theft")
		newCase = new Theft(id, extraInfo);  // extraInfo is stolenValue
	else if (type == "Assault")
		newCase = new Assault(id, extraInfo > 0);  // extraInfo > 0 means weapon was used
	else
		newCase = new Case(type, id, extraInfo);  // extraInfo is severity

	addCase(newCase);
}

// Location methods (aggregation)
Location* CrimeManager::addLocation(const string& address, const string& city, const string& state) {
	Location* loc = new Location(address, city, state);
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

	ifstream file("data/crimes.txt");
	string line;
	while (getline(file, line)) {
		int id;
		string type;
		size_t pos = line.find(',');
		if (pos == string::npos) continue;

		id = stoi(line.substr(0, pos));
		string remainder = line.substr(pos + 1);

		pos = remainder.find(',');
		if (pos == string::npos) {
			// Simple case with no extra info
			type = remainder;
			addCase(id, type);
		}
		else {
			// Case with extra info
			type = remainder.substr(0, pos);
			double extraInfo = stod(remainder.substr(pos + 1));

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
		cout << "\n--- Crime Management ---\n";
		cout << "1. Add Case\n";
		cout << "2. List Cases\n";
		cout << "3. List Cases By Priority\n";
		cout << "4. Save\n";
		cout << "5. Find Case\n";
		cout << "6. Add Location to Case\n";
		cout << "0. Back\n";
		cout << "Choice: ";
		cin >> choice;

		if (choice == 1) {
			int id;
			string type;
			cout << "Enter Case ID: ";
			cin >> id;
			cout << "Type (Theft/Assault/Other): ";
			cin.ignore();
			getline(cin, type);

			if (type == "Theft") {
				double value;
				cout << "Enter stolen value: $";
				cin >> value;
				mgr.addCase(id, "Theft", value);
			}
			else if (type == "Assault") {
				char weapon;
				cout << "Was weapon used? (y/n): ";
				cin >> weapon;
				mgr.addCase(id, "Assault", (weapon == 'y' || weapon == 'Y') ? 1.0 : 0.0);
			}
			else {
				double severity;
				cout << "Enter severity (1-10): ";
				cin >> severity;
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
			cout << "Saved.\n";
		}
		else if (choice == 5) {
			int id;
			cout << "Enter Case ID to find: ";
			cin >> id;

			Case* found = mgr.findCase(id);
			if (found) {
				cout << "Found case: ";
				found->displayDetails();  // Polymorphic call
			}
			else {
				cout << "Case not found.\n";
			}
		}
		else if (choice == 6) {
			int id;
			string address, city, state;
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

				Location* loc = mgr.addLocation(address, city, state);
				found->setLocation(loc);
				cout << "Location added to case.\n";
			}
			else {
				cout << "Case not found.\n";
			}
		}
	} while (choice != 0);
	mgr.save();
}