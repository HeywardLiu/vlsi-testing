#include <iostream>
#include <string>
#include "circuit.h"
using namespace std;

// Assignment-3 of VLSI Testing 2022 Fall  

void CIRCUIT::PrintParallelSimStats() 
{ 
	cout << "[-plogicsim] PatternNum = " << PatternNum << endl;  // pack patterns into a word to evaluate value in parallel
    cout << "[-plogicsim] Pattern counts = " << Pattern.GetPatternCount() << endl;  // total numbers of input pattern
    cout << "[-plogicsim] Gate counts (including PI and PO) = " << No_Gate() << endl;
    cout << "[-plogicsim] Total gate evaluation times = " << EvaluateCount << endl;

    double AvgEvaluationTimes = double(EvaluateCount) / Pattern.GetPatternCount();
    cout << "[-plogicsim] Average gate evaluation times (Total gate evaluate times / Pattern counts) = " << AvgEvaluationTimes << endl;
    cout << "[-plogicsim] Average gate evaluation times per gate (%) = " << (AvgEvaluationTimes / No_Gate()) * 100 << endl;
}


// Refer to CIRCUIT::ParallelLogicSimVectors()
// Event-driven Parallel Pattern Logic simulation
void CIRCUIT::ccsParallelLogicSimVectors() 
{
    cout << "Generate a Compiled-Code to Perform Parallel Logic Simulation." << endl;

	genHeader();
	genMainBegin();
	genEvalBegin();
	genPrintIOBegin();
    
    // Same as ParallelLogicSimVectors()
    unsigned pattern_num(0);
    unsigned pattern_idx(0);
	bool flag(true);
    while(!Pattern.eof()){  // Read patterns 
        for(pattern_idx=0; pattern_idx<PatternNum; pattern_idx++){
            if(!Pattern.eof()){ 
                ++pattern_num;
                Pattern.ReadNextPattern(pattern_idx);
            }
            else break;
        }
        Pattern.SetPatternCount(pattern_num);
        genInitPattern();
        
        ofsMain << endl << "evaluate();" << endl;
        ofsMain << "printIO(" << pattern_idx << ");\n\n"; 

        ScheduleAllPIs();
        ccsParallelLogicSim(flag);

        if(flag == true)
            flag = false;
    }
	ccsPrintParallelIOs(pattern_idx);

	genMainEnd();
	genEvalEnd();
	genPrintIOEnd();

	combineFilesToOutput();
}
 

// Refer to CIRCUIT::ParallelLogicSim()
// Simulate PatternNum vectors
void CIRCUIT::ccsParallelLogicSim(bool flag)
{
    GATE* gptr;
    for (unsigned i = 0;i <= MaxLevel;i++) {
        while (!Queue[i].empty()) {
            gptr = Queue[i].front();
            Queue[i].pop_front();
            gptr->ResetFlag(SCHEDULED);
            ccsParallelEvaluate(gptr, flag);
        }
    }
    return;
}


// Refer to CIRCUIT::ParallelEvaluate()
// Evaluate parallel value of gptr
void CIRCUIT::ccsParallelEvaluate(GATEPTR gptr, bool flag)
{
    register unsigned i;
    bitset<PatternNum> new_value1(gptr->Fanin(0)->GetValue1());
    bitset<PatternNum> new_value2(gptr->Fanin(0)->GetValue2());

		if(flag == true){
		ofsEval << "G_" << gptr->GetName() << "[0]" << " = " 
					 << "G_" << gptr->Fanin(0)->GetName() << "[0];\n";
		ofsEval << "G_" << gptr->GetName() << "[1]" << " = " 
					 << "G_" << gptr->Fanin(0)->GetName() << "[1];\n";
		}

		EvaluateCount += gptr->No_Fanin();
   		switch(gptr->GetFunction()) {
			case G_AND:
			case G_NAND:
				for (i = 1; i < gptr->No_Fanin(); ++i) {
					new_value1 &= gptr->Fanin(i)->GetValue1();
					new_value2 &= gptr->Fanin(i)->GetValue2();

					if(flag == true){
						ofsEval << "G_" << gptr->GetName() << "[0]" << " &= " 
								<< "G_" << gptr->Fanin(i)->GetName() << "[0];\n";
						ofsEval << "G_" << gptr->GetName() << "[1]" << " &= " 
								<< "G_" << gptr->Fanin(i)->GetName() << "[1];\n";
					}
				}
				break;
			case G_OR:
			case G_NOR:
				for (i = 1; i < gptr->No_Fanin(); ++i) {
					new_value1 |= gptr->Fanin(i)->GetValue1();
					new_value2 |= gptr->Fanin(i)->GetValue2();
					if(flag == true){
						ofsEval << "G_" << gptr->GetName() << "[0]" << " |= " 
									<< "G_" << gptr->Fanin(i)->GetName() << "[0];\n";
						ofsEval << "G_" << gptr->GetName() << "[1]" << " |= " 
									<< "G_" << gptr->Fanin(i)->GetName() << "[1];\n";
					}
				}
				break;
        	default: break;
    } 
    //swap new_value1 and new_value2 to avoid unknown value masked
    if (gptr->Is_Inversion()) {
        new_value1.flip(); new_value2.flip();
        bitset<PatternNum> value(new_value1);
		new_value1 = new_value2; new_value2 = value;

		if(flag == true){
		ofsEval << "temp = G_" << gptr->GetName() << "[0];\n";
		ofsEval << "G_" << gptr->GetName() << "[0]" << " = ~" 
				<< "G_" << gptr->GetName() << "[1];\n";
		ofsEval << "G_" << gptr->GetName() << "[1]" << " = ~temp;\n";
		}
    }
    if (gptr->GetValue1() != new_value1 || gptr->GetValue2() != new_value2) {
        gptr->SetValue1(new_value1);
        gptr->SetValue2(new_value2);
        ScheduleFanout(gptr);
    }
    return;
}

void CIRCUIT::openSimulatorFile(string file_name) {
	system("mkdir ../simulator");
	string str = "../simulator/" + file_name;
	
	ofsOutFile.open(str, ofstream::out | ofstream::trunc);
	ofsHeader.open("../simulator/header", ofstream::out|ofstream::trunc);
	ofsMain.open("../simulator/main", ofstream::out|ofstream::trunc);
	ofsEval.open("../simulator/evaluate", ofstream::out|ofstream::trunc);
	ofsPrintIO.open("../simulator/printIO", ofstream::out|ofstream::trunc);
	if(!ofsOutFile.is_open())
		cout << "Cannot open output file!\n";
	if(!ofsHeader.is_open())
		cout << "Cannot open header!\n";
	if(!ofsMain.is_open())
		cout << "Cannot open main!\n";
	if(!ofsEval.is_open())
		cout << "Cannot open evaluate!\n";
	if(!ofsPrintIO.is_open())
		cout << "Cannot open printIO!\n";
}

void CIRCUIT::genHeader()
{
	ofsHeader << "#include <iostream>" << endl;
	ofsHeader << "#include <ctime>" << endl;
	ofsHeader << "#include <bitset>" << endl;
	ofsHeader << "#include <string>" << endl;
	ofsHeader << "#include <fstream>" << endl;
	ofsHeader << "#include <stdlib.h>" << endl;
	ofsHeader << "using namespace std;" << endl << endl;
	ofsHeader << "const unsigned PatternNum = 16;" << endl << endl;
	ofsHeader << "void evaluate();" << endl;
	ofsHeader << "void printIO(unsigned idx);" << endl << endl;

	ofsHeader << "bitset<PatternNum> temp;\n";
	for(unsigned i=0; i < No_Gate(); i++) {
		ofsHeader << "bitset<PatternNum> G_" << Gate(i)->GetName() << "[2];\n";
	}

    string outputFileName = GetName() + ".out";
	ofsHeader << "ofstream ofs(\"" << outputFileName
			  << "\", ofstream::out | ofstream::trunc);\n";

	ofsHeader << endl;
}

void CIRCUIT::genMainBegin()
{
	ofsMain << "int main()\n{\n";
	ofsMain << "clock_t time_init, time_end;\n";
	ofsMain << "time_init = clock();\n";
}

void CIRCUIT::genMainEnd()
{
	ofsMain << "time_end = clock();\n";
	ofsMain << "cout << \"Total CPU Time = \" << double(time_end - \
							time_init)/CLOCKS_PER_SEC << endl;\n";
	ofsMain << "return 0;\n}\n";
}

void CIRCUIT::genEvalBegin()
{
	ofsEval << "void evaluate()\n{\n";
}

void CIRCUIT::genEvalEnd()
{
	ofsEval << "}\n";
}

void CIRCUIT::genInitPattern()
{
	vector<GATE*> PatternInlist = Pattern.getInlist();

	for(unsigned int i = 0; i<PatternInlist.size(); i++) {
		ofsMain << "G_" << PatternInlist[i]->GetName() << "[0] = " 
                        << PatternInlist[i]->getWireValue()[0].to_ulong()
						<< ";" << endl;
		ofsMain << "G_" << PatternInlist[i]->GetName() << "[1] = " 
                        << PatternInlist[i]->getWireValue()[1].to_ulong()
						<< ";" << endl;
	}
}

void CIRCUIT::combineFilesToOutput()
{
	ofsHeader.close(); ofsMain.close(); ofsEval.close(); ofsPrintIO.close();
	ifstream ifs;

	ifs.open("../simulator/header");
	if(ifs.is_open()){
		ofsOutFile << ifs.rdbuf();
		ifs.close();
	}
	ifs.open("../simulator/main");
	if(ifs.is_open()){
		ofsOutFile << ifs.rdbuf();
		ifs.close();
	}
	ifs.open("../simulator/evaluate");
	if(ifs.is_open()){
		ofsOutFile << ifs.rdbuf();
		ifs.close();
	}
	ifs.open("../simulator/printIO");
	if(ifs.is_open()){
		ofsOutFile << ifs.rdbuf();
		ifs.close();
	}

	system("rm -f ../simulator/header");
	system("rm -f ../simulator/main");
	system("rm -f ../simulator/evaluate");
	system("rm -f ../simulator/printIO");
}

void CIRCUIT::genPrintIOBegin()
{
	ofsPrintIO << "void printIO(unsigned idx)\n{\n";
}

void CIRCUIT::genPrintIOEnd()
{
	ofsPrintIO << "}\n";
}

void CIRCUIT::ccsPrintParallelIOs(unsigned idx)
{
	ofsPrintIO << "for(unsigned i=0; i < idx; i++){\n";
	for(unsigned i=0; i < No_PI(); i++){
		ofsPrintIO << "\tif(G_" << PIGate(i)->GetName() << "[0][i] == 0){\n";

		ofsPrintIO << "\tif(G_" << PIGate(i)->GetName() << "[1][i] == 1)\n";
		ofsPrintIO << "\tofs << \'F\';\n";
		ofsPrintIO << "\telse\n\tofs << \'0\';\n";

		ofsPrintIO << "\t}\n\telse{\n";

		ofsPrintIO << "\tif(G_" << PIGate(i)->GetName() << "[1][i] == 1)\n";
		ofsPrintIO << "\tofs << \'1\';\n";
		ofsPrintIO << "\telse\n\tofs << \'2\';\n";

		ofsPrintIO << "\t}\n";
	}
	
	ofsPrintIO << "\tofs << \" \";\n";

	for(unsigned i=0; i < No_PO(); i++){
		ofsPrintIO << "\tif(G_" << POGate(i)->GetName() << "[0][i] == 0){\n";

		ofsPrintIO << "\tif(G_" << POGate(i)->GetName() << "[1][i] == 1)\n";
        ofsPrintIO << "\tofs << \'F\';\n";
        ofsPrintIO << "\telse\n\tofs << \'0\';\n";

		ofsPrintIO << "\t}\n\telse{\n";

		ofsPrintIO << "\tif(G_" << POGate(i)->GetName() << "[1][i] == 1)\n";
		ofsPrintIO << "\tofs << \'1\';\n";
		ofsPrintIO << "\telse\n\tofs << \'2\';\n";

		ofsPrintIO << "\t}\n";
	}
	ofsPrintIO << "\tofs << endl;\n";

	ofsPrintIO << "}\n";
}
