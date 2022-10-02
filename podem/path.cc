#include <unistd.h>
#include "circuit.h"
#include <iostream>
using namespace std;

/*
    The basic idea is DFS-Traversal of a loop-free combinational circuit.
    We apply Prune-and-Search technique, to prevent from 
    re-visiting a gate which don't contain a subpath to dest gate.
*/
bool CIRCUIT::FindPaths(GATE* StartGate, GATE* EndGate) {  
    PathStack.push_back(StartGate);
    bool find_path = false;

    if(EndGate->GetID() == StartGate->GetID()) {
        find_path = true;
        PathCount++;
        cout << endl;
        for(unsigned i=0; i<PathStack.size(); ++i) {
            cout << PathStack[i]->GetName() << " ";
        }
        cout << endl;
    } else {
        for(unsigned i = 0; i<StartGate->No_Fanout(); ++i) {
            if(StartGate->Fanout(i)->FindDest != false) {    // Not been visited, or Containing a path to dest
                if(FindPaths(StartGate->Fanout(i), EndGate))
                    find_path = true;
            }
        }
    }

    if(!find_path)                      // all subpaths of this gate didn't find dest gate.
        StartGate->FindDest = false;    // to prevent from visiting all subpaths of the gate again, set the gate to false

    PathStack.pop_back();
    return find_path;
}

GATE* CIRCUIT::Get_PIGate(const string& Name) {    // Access by Name
    vector<GATE*>::iterator GateListIt;
    for(GateListIt = PIlist.begin(); GateListIt != PIlist.end(); GateListIt++) {
        if((*GateListIt)->GetName() == Name) 
            return (*GateListIt);
    }
    return nullptr;
}

GATE* CIRCUIT::Get_POGate(const string& Name) {
    vector<GATE*>::iterator GateListIt;
    for(GateListIt = POlist.begin(); GateListIt != POlist.end(); GateListIt++) {
        if((*GateListIt)->GetName() == Name) 
            return (*GateListIt);
    }
    return nullptr;
}

void CIRCUIT::PrintGateInfos(GATE* Gate) {
    cout <<"ID: " << Gate->GetID() << " ,Name: " << Gate->GetName()  
         << ", Function: " << Gate->GetFunction() << endl;
}