#ifndef FORENSICSMODULE_H
#define FORENSICSMODULE_H

#include "ListTemplate.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include "nlohmann/json.hpp"
using json = nlohmann::json;
using namespace std;

class ForensicExpert {
protected:
    string name;
    int expertId;
public:

//constructor declaration
 
    ForensicExpert(string n="", int id=0 );

   

    //pure virtual for abstract class
    virtual string getSpecialty() const = 0;

    int getId() const;
    string getName() const;

    //virtual destructor
    virtual ~ForensicExpert() {}

    bool operator==(const ForensicExpert& other) const;

    friend ostream& operator<<(ostream& os, const ForensicExpert& f);
};

//child class
class LabTechnician : public ForensicExpert {
public:
    LabTechnician(string n = "", int id = 0);
    string getSpecialty() const override;
};

class FieldAgent : public ForensicExpert {
public:
    FieldAgent(string n = "", int id = 0);
    string getSpecialty() const override;
};

class ForensicLab {
private:
    ListTemplate<LabTechnician> labTechs;
    ListTemplate<FieldAgent> fieldAgents;
    map<int, ForensicExpert*> expertMap;

public:
    void addLabTech(const LabTechnician& e);
    void addFieldAgent(const FieldAgent& e);

    void listExperts();

    void save();
    void load();

    void addLabTech(const string& name, int id);
};

class ForensicLabRegistry {
private:
    static ForensicLabRegistry* instance;
    ForensicLab lab;

    ForensicLabRegistry(); // private constructor

public:
    static ForensicLabRegistry* getInstance();
    ForensicLab& getLab();
};

void forensicsMenu();

#endif // FORENSICSMODULE_H
