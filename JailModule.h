#pragma once
#include "ListTemplate.h"
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>

class Cell {
protected:
    int cellId;
    int capacity;
public:
    Cell(int id = 0, int cap = 0) : cellId(id), capacity(cap) {}
    virtual std::string getType() const = 0;
    int getId() const { return cellId; }
    int getCapacity() const { return capacity; }
    virtual ~Cell() {}
    bool operator==(const Cell& other) const { return cellId == other.cellId; }
    friend std::ostream& operator<<(std::ostream& os, const Cell& c) {
        os << c.getType() << " (ID: " << c.cellId << ", Cap: " << c.capacity << ")";
        return os;
    }
};

class HighSecurityCell : public Cell {
public:
    HighSecurityCell(int id = 0, int cap = 0) : Cell(id, cap) {}
    std::string getType() const override { return "High Security"; }
};

class StandardCell : public Cell {
public:
    StandardCell(int id = 0, int cap = 0) : Cell(id, cap) {}
    std::string getType() const override { return "Standard"; }
};

class Jail {
    ListTemplate<HighSecurityCell> highSecCells;
    ListTemplate<StandardCell> standardCells;
    std::map<int, Cell*> cellMap; // For demonstration, not for ownership
public:
    void addHighSecCell(const HighSecurityCell& c);
    void addStandardCell(const StandardCell& c);
    void listCells();
    void save();
    void load();
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