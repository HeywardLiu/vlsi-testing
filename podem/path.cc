#include "circuit.h"
#include <iostream>
using namespace std;

void CIRCUIT::FindAllPaths(GATE* StartGate, GATE* EndGate) {
    vector<GATE*>::iterator GateListIt;
    
    
}

GATE* CIRCUIT::PIGate(const string& Name) {    // Access by Name
    vector<GATE*>::iterator GateListIt;
    for(GateListIt = PIlist.begin(); GateListIt != PIlist.end(); GateListIt++) {
        if((*GateListIt)->GetName() == Name) {
            cout << "Name: " << (*GateListIt)->GetName() << ", ID: " << (*GateListIt)->GetID() 
                 << ", Function: " << (*GateListIt)->GetFunction() << endl;
            return (*GateListIt);
        }
    }
    return NULL;
}

GATE* CIRCUIT::POGate(const string& Name) {
    vector<GATE*>::iterator GateListIt;
    for(GateListIt = POlist.begin(); GateListIt != POlist.end(); GateListIt++) {
        if((*GateListIt)->GetName() == Name) {
            cout << "Name: " << (*GateListIt)->GetName() << ", ID: " << (*GateListIt)->GetID() 
                 <<", Function: " << (*GateListIt)->GetFunction() << endl;
            return (*GateListIt);
        }
    }
    return NULL;
}