#include <iostream>
#include "circuit.h"

using namespace std;

/* 
Print the folloing terms:
[v]    # inputs
[v]    # outputs
[v]    # total gates of (NOT + OR + NOR + AND + NAND)
[v]    # gates for each type
[v]    # flip-flops
[v]    # total signal nets
[v]    # branch nets
[v]    # stem nets
[v]    avg(fanout/gate) 
*/

void CIRCUIT::ShowStatistics() {
    cout << endl;
    cout << "Showing statistics of circuit: " << GetName() << ".bench" << endl;
    cout << "Number of inputs: " << No_PI() << endl;
    cout << "Number of outputs: " << No_PO() << endl;

    CountGATEFUNC();
    cout << "Number of gates of (NOT, OR, NOR, AND, NAND): " << No_Total_Gate() << endl;
    cout << "Number of NOT gates: " << No_NOT_Gate() << endl;
    cout << "Number of OR gates: " << No_OR_Gate() << endl;
    cout << "Number of NOR gates: " << No_NOR_Gate() << endl;
    cout << "Number of AND gates: " << No_AND_Gate() << endl;
    cout << "Number of NAND gates: " << No_NAND_Gate() << endl;
    cout << "Number of Flip-Flops: " << No_DFF() << endl;

    CountNet();
    cout << "Number of signal nets: " << No_Total_Signal_Net() << endl;
    cout << "Number of branch nets: " << No_Branch_Net() << endl;
    cout << "Number of stem nets: " << No_Stem_Net() << endl;
    cout << "Avg Fanout (fanouts/gate) of the given circuit: " << No_Avg_Fanout() << endl;
}

void CIRCUIT::CountGATEFUNC() {
    vector<GATE*>::iterator It;
    for(It = Netlist.begin(); It != Netlist.end(); It++) {
        GateCounts[(*It)->GetFunction()] += 1;
    }
}

void CIRCUIT::CountNet() {
    vector<GATE*>::iterator It;
    for(It = Netlist.begin(); It != Netlist.end(); It++) {
        if((*It)->No_Fanout() > 1) {    // The gate has branch net
            StemNetCount += 1;
            BranchNetCount += (*It)->No_Fanout();
            ToTalNetCount = ToTalNetCount + (*It)->No_Fanout() + 1;
        } else {
            ToTalNetCount += (*It)->No_Fanout();
        }
    }
}

float CIRCUIT::No_Avg_Fanout() {
    vector<GATE*>::iterator It;
    unsigned TotalFanout = 0;
    for(It = Netlist.begin(); It != Netlist.end(); It++) {
        TotalFanout += (*It)->No_Fanout();
    }
    return float(TotalFanout)/float(Netlist.size());
}

unsigned CIRCUIT::No_Total_Gate() {
    return No_NOT_Gate() + No_OR_Gate() + No_NOR_Gate() + No_AND_Gate() + No_NAND_Gate();
}

void CIRCUIT::PrintNetlist() {    
    // Traverse the Netlist and trace attributes of GATE in the Netlist
    vector<GATE*>::iterator It;
    for(It = Netlist.begin(); It != Netlist.end(); It++) {
        cout << "Name: " << (*It)->GetName() << ", ";
        cout << "ID: "<< (*It)->GetID() << ", ";
        cout << "Function: " << (*It)->GetFunction() << endl;
        cout << "Fan-out: " << (*It)->No_Fanout() << endl;
    }
    cout << endl;
}