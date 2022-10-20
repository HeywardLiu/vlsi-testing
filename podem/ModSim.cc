#include "circuit.h"
#include "ReadPattern.h"
#include <iostream>

using namespace std;

/*  
    Implement Assignment-2 of part-b
    Mimic the implementation of original logic simulation
    Compute results by bitset instead of costum datatype.
*/

void CIRCUIT::Mod_LogicSimVectors()
{
    cout << "**[-mod_logicsim] Run modified version of logic simulation." << endl;
    //read test patterns
    while (!Pattern.eof()) {
        Pattern.Mod_ReadNextPattern();
        SchedulePI();    // borrow original implemented function
        Mod_LogicSim();
        Mod_PrintIO();
    }
    return;
}

void PATTERN::Mod_ReadNextPattern()
{
    char V;
    for (int i = 0;i < no_pi_infile;i++) {
        patterninput >> V;   // read circuit input of pattern file
        if (V == '0') {      // logical 0 
            if (inlist[i]->Mod_GetValue().to_string() != "00") { 
                inlist[i]->SetFlag(SCHEDULED);
                bitset<2> bits(0x0);
                inlist[i]->Mod_SetValue(bits);
            }
        }
        else if (V == '1') {  // logical 1
            if (inlist[i]->Mod_GetValue().to_string() != "11") { 
                inlist[i]->SetFlag(SCHEDULED);
                bitset<2> bits(0x3);
                inlist[i]->Mod_SetValue(bits);
            }
        }
        else if (V == 'X') {  // Don't-care
            if (inlist[i]->Mod_GetValue().to_string() != "01") { 
                inlist[i]->SetFlag(SCHEDULED);
                bitset<2> bits(0x1);
                inlist[i]->Mod_SetValue(bits);
            }
        }
    }
    //Take care of newline to force eof() function correctly
    patterninput >> V;
    if (!patterninput.eof()) patterninput.unget();

    return;
}


void CIRCUIT::Mod_LogicSim() {
    GATE* gptr;
    bitset<2> new_value;
    for (unsigned i = 0;i <= MaxLevel;i++) {
        while (!Queue[i].empty()) {
            gptr = Queue[i].front();
            Queue[i].pop_front();
            gptr->ResetFlag(SCHEDULED);
            new_value = Mod_Evaluate(gptr);
            if (new_value != gptr->GetValue()) {
                gptr->Mod_SetValue(new_value);
                ScheduleFanout(gptr);
            }
        }
    }
    return;
}


bitset<2> CIRCUIT::Mod_Evaluate(GATEPTR gptr)
{
    GATEFUNC fun(gptr->GetFunction());
    VALUE cv(CV[fun]); //controling value
    bitset<2> value(gptr->Fanin(0)->Mod_GetValue());
    switch (fun) {
        case G_AND:
        case G_NAND:
            for (unsigned i = 1;i<gptr->No_Fanin() && (VALUE) value.to_ulong() != cv;++i) {
                value = value & gptr->Fanin(i)->Mod_GetValue();  // perform logical AND
            }
            break;
        case G_OR:
        case G_NOR:
            for (unsigned i = 1;i<gptr->No_Fanin() && (VALUE) value.to_ulong() != cv;++i) {
                value = value | gptr->Fanin(i)->Mod_GetValue();  // perfor logical OR
            }
            break;
        default: break;
    }
    //NAND, NOR and NOT
    if (gptr->Is_Inversion()){ 
        if(value.to_string() != "01")
            value = ~value;
    }
		
    return value;
}


void CIRCUIT::Mod_PrintIO() 
{
    int i;
    bitset<2> temp;
    cout << "PI: ";
    for (i = 0;i<No_PI();++i){
        temp = PIGate(i)->Mod_GetValue();
        if(temp.to_string() == "00")
            cout << '0';
        else if(temp.to_string() == "11")
            cout << '1';
        else if(temp.to_string() == "01")
            cout << 'X'; 
    }

    cout << " PO: ";
    for (i = 0;i<No_PO();++i){
        temp = POGate(i)->Mod_GetValue();
        if(temp.to_string() == "00")
            cout << '0';
        else if(temp.to_string() == "11")
            cout << '1';
        else if(temp.to_string() == "01")
            cout << 'X'; 
    }
    cout << endl;
    return;
}


