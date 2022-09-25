#include <unistd.h>
#include "circuit.h"
#include <iostream>
using namespace std;

void CIRCUIT::InitVisit() {
    vector<GATE*>::iterator GateListIt;
    for(GateListIt = Netlist.begin(); GateListIt != Netlist.end(); GateListIt++) {
        (*GateListIt)->IsVisit = false;
    }
}

void CIRCUIT::FindPaths(GATE* StartGate, GATE* EndGate) {  // DFS Traversal for loop-free digraph 
    PathStack.push_back(StartGate);
    StartGate->IsVisit = true;

    if(EndGate->GetID() == StartGate->GetID()) {    // Match
        PathCount++;
        cout << endl;
        for(unsigned i=0; i<PathStack.size(); ++i) {
            cout << PathStack[i]->GetName() << " ";
        }
        cout << endl;
    } else {
        for(unsigned i = 0; i<StartGate->No_Fanout(); ++i) {            
            if(StartGate->Fanout(i)->IsVisit == false) {
                FindPaths(StartGate->Fanout(i), EndGate);
            }
        }
    }

    StartGate->IsVisit=false;
    PathStack.pop_back();
}

GATE* CIRCUIT::PIGate(const string& Name) {    // Access by Name
    vector<GATE*>::iterator GateListIt;
    for(GateListIt = PIlist.begin(); GateListIt != PIlist.end(); GateListIt++) {
        if((*GateListIt)->GetName() == Name) 
            return (*GateListIt);
    }
    return nullptr;
}

GATE* CIRCUIT::POGate(const string& Name) {
    vector<GATE*>::iterator GateListIt;
    for(GateListIt = POlist.begin(); GateListIt != POlist.end(); GateListIt++) {
        if((*GateListIt)->GetName() == Name) 
            return (*GateListIt);
    }
    return nullptr;
}

void CIRCUIT::PrintGateInfos(GATE* Gate) {
    cout << Gate->GetName() << " ";
    // cout <<"ID: " << Gate->GetID() << " ,Name: " << Gate->GetName()  
    //      << ", Function: " << Gate->GetFunction() << endl;
}