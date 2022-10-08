#include <unistd.h>
#include "circuit.h"
#include <iostream>
#include <string>
using namespace std;

void CIRCUIT::Path(const string& SrcGateName, const string& DestGateName) {
    cout << "This is path option." << endl;
    GATE* SrcGate = GetPIGate(SrcGateName);
    GATE* DestGate = GetPOGate(DestGateName);

    if(SrcGate && DestGate) {
        FindPaths(SrcGate, DestGate);
        cout << endl <<  "The number of paths from " 
             << SrcGateName << " to " << DestGateName
             << ": " << No_PathCount() << endl;
    } else if(!SrcGate && DestGate) {
        cout << SrcGateName + " isn't a input of the circuit." <<endl;
    } else if(SrcGate && !DestGate) {
        cout << DestGateName + " isn't a output of the circuit" <<endl;
    } else {
        cout << SrcGateName + " isn't a input of the circuit." <<endl;
        cout << DestGateName + " isn't a output of the circuit" <<endl;
    }
}

/*
    The basic idea is DFS-Traversal of a loop-free combinational circuit.
    We apply Prune-and-Search technique, to prevent from 
    re-visiting a gate whose fanouts don't contain a subpath to dest gate.
*/
bool CIRCUIT::FindPaths(GATE* SrcGate, GATE* DestGate) {  
    PathStack.push_back(SrcGate);
    bool find_path = false;

    if(DestGate->GetID() == SrcGate->GetID()) {
        find_path = true;
        PathCount++;
        PrintPathStack();
    } else {
        for(unsigned i = 0; i<SrcGate->No_Fanout(); ++i) {
            if(SrcGate->Fanout(i)->FindDest != false) {
                // not been visited, or containing a path to dest 
                if(FindPaths(SrcGate->Fanout(i), DestGate))
                    find_path = true;
            }
        }
    }
    
    if(!find_path)
        // the gate whose fanouts don't contain a subpath to dest gate.
        SrcGate->FindDest = false;    

    PathStack.pop_back();
    return find_path;
}

GATE* CIRCUIT::GetPIGate(const string& PIGateName) {
    vector<GATE*>::iterator GateListIt;
    for(GateListIt = PIlist.begin(); GateListIt != PIlist.end(); GateListIt++) {
        if((*GateListIt)->GetName() == PIGateName)
            return (*GateListIt);
    }
    return nullptr;
}

GATE* CIRCUIT::GetPOGate(const string& POGateName) {
    vector<GATE*>::iterator GateListIt;
    for(GateListIt = POlist.begin(); GateListIt != POlist.end(); GateListIt++) {
        if((*GateListIt)->GetName() == POGateName)
            return (*GateListIt);
    }
    return nullptr;
}

void CIRCUIT::PrintPathStack() {
    cout << endl;
    for(unsigned i=0; i<PathStack.size(); ++i) {
        cout << PathStack[i]->GetName() << " ";
    }
    cout << endl;
}

void CIRCUIT::PrintGateInfos(GATE* Gate) {
    cout <<"ID: " << Gate->GetID() << " ,Name: " << Gate->GetName()
         << ", Function: " << Gate->GetFunction() << endl;
}