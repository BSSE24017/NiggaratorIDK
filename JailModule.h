#pragma once
#include "OfficerModule.h"
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
using namespace std;

class DisplayJailInfo {
public:
    virtual void displayJailInfo() const = 0;
    virtual ~DisplayJailInfo() = default;
};

class Prisoner : public DisplayJailInfo {
private:
    int prisonerId;
    string name;
    string crime;
    int sentenceYears;
    int currentCellId;
public:
    Prisoner(int id = 0, const string& n = "", const string& c = "", int years = 0, int cellId = -1)
        : prisonerId(id), name(n), crime(c), sentenceYears(years), currentCellId(cellId) {}
    
    int getId() const { return prisonerId; }
    string getName() const { return name; }
    string getCrime() const { return crime; }
    int getSentenceYears() const { return sentenceYears; }
    int getCurrentCellId() const { return currentCellId; }
    void setCurrentCellId(int cellId) { currentCellId = cellId; }
    
    void displayJailInfo() const override {
        cout << *this << endl;
    }
    
    friend ostream& operator<<(ostream& os, const Prisoner& p) {
        os << "Prisoner ID: " << p.prisonerId << "\n"
           << "Name: " << p.name << "\n"
           << "Crime: " << p.crime << "\n"
           << "Sentence: " << p.sentenceYears << " years\n"
           << "Current Cell ID: " << p.currentCellId;
        return os;
    }
};

class Cell : public DisplayJailInfo {
protected:
    int cellId;
    int capacity;
    vector<Prisoner*> prisoners;
    Officer* assignedOfficer;
public:
    Cell(int id = 0, int cap = 0) : cellId(id), capacity(cap), assignedOfficer(nullptr) {}
    virtual string getType() const = 0;
    int getId() const { return cellId; }
    int getCapacity() const { return capacity; }
    int getCurrentOccupancy() const { return prisoners.size(); }
    bool isFull() const { return prisoners.size() >= capacity; }
    
    bool addPrisoner(Prisoner* prisoner) {
        if (isFull()) return false;
        prisoners.push_back(prisoner);
        prisoner->setCurrentCellId(cellId);
        return true;
    }
    
    bool removePrisoner(int prisonerId) {
        for (auto it = prisoners.begin(); it != prisoners.end(); ++it) {
            if ((*it)->getId() == prisonerId) {
                (*it)->setCurrentCellId(-1);
                prisoners.erase(it);
                return true;
            }
        }
        return false;
    }
    
    vector<Prisoner*> getPrisoners() const { return prisoners; }
    
    void assignOfficer(Officer* officer) { assignedOfficer = officer; }
    Officer* getAssignedOfficer() const { return assignedOfficer; }
    
    void displayJailInfo() const override {
        cout << *this << endl;
    }
    
    virtual ~Cell() {}
    bool operator==(const Cell& other) const { return cellId == other.cellId; }
    friend ostream& operator<<(ostream& os, const Cell& c) {
        os << c.getType() << " (ID: " << c.cellId << ", Cap: " << c.capacity 
           << ", Occupancy: " << c.getCurrentOccupancy() << ")";
        if (c.assignedOfficer) {
            os << "\nAssigned Officer: " << c.assignedOfficer->getName();
        }
        return os;
    }
};

class HighSecurityCell : public Cell {
public:
    HighSecurityCell(int id = 0, int cap = 0) : Cell(id, cap) {}
    string getType() const override { return "High Security"; }
};

class StandardCell : public Cell {
public:
    StandardCell(int id = 0, int cap = 0) : Cell(id, cap) {}
    string getType() const override { return "Standard"; }
};

class Jail {
    static const int NUM_HIGH_SEC_CELLS = 5;
    static const int NUM_STANDARD_CELLS = 10;
    static const int HIGH_SEC_CAPACITY = 2;
    static const int STANDARD_CAPACITY = 4;
    
    vector<HighSecurityCell> highSecCells;
    vector<StandardCell> standardCells;
    map<int, Cell*> cellMap;
    map<int, Prisoner*> prisonerMap;
    
    void initializeCells() {
        // Initialize High Security Cells
        for (int i = 1; i <= NUM_HIGH_SEC_CELLS; i++) {
            HighSecurityCell cell = HighSecurityCell(i, HIGH_SEC_CAPACITY);
            highSecCells.push_back(cell);
            cellMap[i] = &highSecCells[i - 1];
        }
        
        // Initialize Standard Cells
        for (int i = NUM_HIGH_SEC_CELLS + 1; i <= NUM_HIGH_SEC_CELLS + NUM_STANDARD_CELLS; i++) {
            StandardCell cell = StandardCell(i, STANDARD_CAPACITY);
            standardCells.push_back(cell);
            cellMap[i] = &standardCells[i - NUM_HIGH_SEC_CELLS - 1];
        }
    }
    
public:
    Jail();
    ~Jail() {
        // Clean up cells
        for (auto& pair : cellMap) {
            delete pair.second;
        }
        // Clean up prisoners
        for (auto& pair : prisonerMap) {
            delete pair.second;
        }
    }
    
    void listCells();
    void save();
    void load();
    
    // Prisoner management methods
    bool addPrisoner(const Prisoner& prisoner);
    bool assignPrisonerToCell(int prisonerId, int cellId);
    bool changePrisonerCell(int prisonerId, int newCellId);
    bool removePrisoner(int prisonerId);
    void listPrisoners();
    Prisoner* getPrisoner(int prisonerId);
    Cell* getCell(int cellId);
    
    // Officer management methods
    bool assignOfficerToCell(int officerId, int cellId);
    bool removeOfficerFromCell(int cellId);
    void listOfficerAssignments();
    
    // Cell information methods
    int getNumHighSecCells() const { return NUM_HIGH_SEC_CELLS; }
    int getNumStandardCells() const { return NUM_STANDARD_CELLS; }
    int getHighSecCapacity() const { return HIGH_SEC_CAPACITY; }
    int getStandardCapacity() const { return STANDARD_CAPACITY; }
};

class JailRegistry {
    static JailRegistry* instance;
    Jail jail;
    JailRegistry() {}
public:
    static JailRegistry* getInstance();
    Jail& getJail();
};

void jailMenu(); 