#include <iostream>
#include "circuit.h"

using namespace std;

/* 
Print the folloing terms:
v    # inputs
v    # outputs
v    # total gates of (NOT + OR + NOR + AND + NAND)
v    # gates for each type
v    # flip-flops
    # total signal nets
    # branch nets
    # stem nets
    avg(fanout/gate) 
*/

void CIRCUIT::ShowStatistics() {
    cout << "Number of inputs: " << No_PI() << endl;
    cout << "Number of outputs: " << No_PO() << endl;

    CountTotalGates();
    cout << "Number of gates of (NOT, OR, NOR, AND, NAND): " << TotalGateCounts << endl;
    cout << "Number of NOT gates: " << No_NOR_Gate() << endl;
    cout << "Number of OR gates: " << No_OR_Gate() << endl;
    cout << "Number of NOR gates: " << No_NOR_Gate() << endl;
    cout << "Number of AND gates: " << No_AND_Gate() << endl;
    cout << "Number of NAND gates: " << No_NAND_Gate() << endl;
    cout << "Number of Dtype-Flip-Flops: " << No_DFF() << endl;


    // cout <<
}

/* 
enum GATEFUNC {
    G_PI, G_PO, G_PPI, G_PPO,
    G_NOT, G_AND, G_NAND, G_OR, G_NOR, 
    G_DFF, G_BUF, G_BAD 
};
*/
void CIRCUIT::CountGATEFUNC() {
    vector<GATE*>::iterator It;
    for(It = Netlist.begin(); It != Netlist.end(); It++) {
        GateCounts[(*It)->GetFunction()] += 1;
    }
}

void CIRCUIT::CountTotalGates() {
    GateCounts.clear();
    CountGATEFUNC();
    TotalGateCounts = 0;
    for(int i=4; i<=8; ++i) {
        TotalGateCounts += GateCounts[i];
    }
}




void CIRCUIT::PrintNetlist() {    // Print Netlist to trace structure of GATE
    vector<GATE*>::iterator It;
    for(It = Netlist.begin(); It != Netlist.end(); It++) {
        cout << "Name: " << (*It)->GetName() << ", ";
        cout << "ID: "<< (*It)->GetID() << ", ";
        cout << "Function: " << (*It)->GetFunction() << endl;
    }
}