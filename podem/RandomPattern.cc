#include "circuit.h"
#include <iostream>
#include <ctime>

using namespace std;

/* CIRCUIT */
void CIRCUIT::GenRandomPattern(const string PatternName, const unsigned PatternNum, const bool ContainDontCare) {
    Pattern.SetName(PatternName);
    for(int i = 0; i < No_PI(); i++) {    // Copy All PI-Gates to Pattern 
        Pattern.AppendPIGate(PIGate(i));
    }
    Pattern.GenRandomPattern(PatternNum, ContainDontCare);
}

/* PATTERN */ 
void PATTERN::OpenOutFileStream() {
    system("mkdir -p ../random_pattern");  // make directory to save pattern file only if it doesn't exsit

    string PatternFilePath = "../random_pattern/";
	PatternFilePath.append(PatternName);

    if(!ofs.is_open())
        ofs.open(PatternFilePath.c_str(), ofstream::out | ofstream::trunc);  // overwrite if pattern exsit

    if(!ofs.is_open())
        cerr << "**[-pattern] Fail to open pattern file: " << PatternFilePath << endl;
    else
        cout << "**[-pattern] Successfully open pattern file: " << PatternFilePath << endl;
}

void PATTERN::GenRandomPattern(const unsigned PatternNum, bool ContainDontCare) {
    OpenOutFileStream();  // open pattern file

    no_pi_infile = inlist.size();   // write all PI to OFS (pattern file)
	for(int i = 0; i < no_pi_infile; i++){
		ofs << "PI " << inlist[i]->GetName();
		if( i < no_pi_infile-1)
			ofs << " ";
	}
	ofs << endl;
    
    srand(time(NULL));    // generate a random pattern to OFS (pattern file)
	for(int i = 0; i < PatternNum; i++){
		for(int j = 0; j < no_pi_infile; j++){
			int random;
            if(ContainDontCare)
                random = rand()%3;
            else
                random = rand()%2;

            if(random == 2)
                ofs << 'X';
            else
    			ofs << random;	
		}	
		ofs << endl;
	}
}
