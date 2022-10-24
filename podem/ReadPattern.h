#ifndef READPATTERN_H
#define READPATTERN_H

#include <fstream>
#include "gate.h"
using namespace std;

class PATTERN
{
    private:
        ifstream patterninput;
        vector<GATE*> inlist;
        int no_pi_infile;
        
        /**** Assignment 2 ****/
        string PatternName;
        ofstream ofs;

        /**** Assignment 3 ****/
        unsigned long long PatternCount;
    public:
        PATTERN(): no_pi_infile(0), PatternCount(0){}
        void Initialize(char* InFileName, int no_pi, string TAG);
        //Assign next input pattern to PI
        void ReadNextPattern();
        void ReadNextPattern_t();
	    void ReadNextPattern(unsigned idx);
        bool eof()
        {
            return (patterninput.eof());
        }
        
        /**** Assignment 2 ****/
        void SetName(string Name) {PatternName = Name;}   // part-a
        void OpenOutFileStream();
        void AppendPIGate(GATE* Gate) {inlist.push_back(Gate);}
        void GenRandomPattern(const unsigned PatternNum, const bool ContainDontCare);
        void Mod_ReadNextPattern();  // part-b

        /**** Assignment 3 ****/
        unsigned long long GetPatternCount() { return PatternCount; }
        void SetPatternCount(unsigned long long num) { PatternCount = num; }
};
#endif
