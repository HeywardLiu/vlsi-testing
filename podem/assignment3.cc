#include "circuit.h"
#include <iostream>
using namespace std;

void CIRCUIT::PrintPSimStats() { 
    double AvgGateEvalTimes = double(EvaluateCount) / Pattern.GetPatternCount();
    cout << "Pattern counts = " << Pattern.GetPatternCount() << endl;
    cout << "Gate counts (including PI and PO) = " << No_Gate() << endl;
    cout << "Total gate evaluation times = " << EvaluateCount << endl;
    cout << "Average gate evaluation times (evaluate times / pattern counts) = " << AvgGateEvalTimes << endl;
    cout << "Average gate evaluation times per gate (%) = " << (AvgGateEvalTimes / No_Gate()) * 100 << endl;
}