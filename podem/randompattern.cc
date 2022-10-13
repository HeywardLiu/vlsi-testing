#include "circuit.h"
#include <iostream>
#include <ctime>

using namespace std;

/* CIRCUIT */
void CIRCUIT::GenRandomPattern(const string PatternName, const unsigned PatternNum, const bool ContainDontCare) {
    Pattern.SetName(PatternName);
    for(int i = 0; i < No_PI(); i++) {    // Copy All PI-Gates to Pattern 
        Pattern.AppendInList(PIGate(i));
    }
    Pattern.GenRandomPattern(PatternNum, ContainDontCare);
}

/* PATTERN */ 
void PATTERN::OpenOutFileStream() {
    cout << "[OpenOutFileStream] Open File: " << PatternName << endl;

	string str = "mkdir ";  // make dir to save our generated pattern file
	str.append("../input");
	cout << "[PATTERN::OpenOutFileStream] str: " << str << endl;
	system(str.c_str());

	str = "../input";  // set file path
	// str.append(xstr(INDIR));
	str.append("/");
	str.append(PatternName);


	cout << "[PATTERN::OpenOutFileStream] str: " << str << endl;

    if(!ofs.is_open())
        ofs.open(str.c_str(), ofstream::out | ofstream::trunc);

    if(!ofs.is_open())
        cerr << "Can't open pattern file." << endl;
    else
        cout << "Open pattern file successfully.";
}

void PATTERN::GenRandomPattern(const unsigned PatternNum, bool ContainDontCare) {
    OpenOutFileStream();

    no_pi_infile = inlist.size();   // PI to OFS
	for(int i = 0; i < no_pi_infile; i++){
		ofs << "PI " << inlist[i]->GetName();
		if( i < no_pi_infile-1)
			ofs << " ";
	}
	ofs << endl;
    
    srand(time(NULL));    // Random Pattern to OFS
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
