#include <iostream>
#include <ctime>
#include "circuit.h"
#include "GetLongOpt.h"
#include "ReadPattern.h"
#include <string>
#include <unistd.h>
#include <sys/types.h>

using namespace std;


// All defined in readcircuit.l
extern char* yytext;
extern FILE *yyin;
extern CIRCUIT Circuit;
extern int yyparse (void);
extern bool ParseError;

extern void Interactive();

GetLongOpt option;



int SetupOption(int argc, char ** argv)
{
    option.usage("[options] input_circuit_file");
    /* Assignment-0 */
    option.enroll("ass0", GetLongOpt::NoValue,    
            "This is assignment-0 of 2022 VLSI-Testing course.", 0);
    /* Assignment-1 */
    option.enroll("path", GetLongOpt::NoValue,    
            "list and count all possible paths connecting the given PI and PO.", 0);
    /* Assignment-2 */
    option.enroll("pattern", GetLongOpt::NoValue,
            "generate a random pattern.", 0);
    option.enroll("unknown", GetLongOpt::NoValue,
            "enable don't-care (X) in our generated pattern.", 0);
    option.enroll("num",GetLongOpt::MandatoryValue,
            "specify the number of the generated pattern", 0);
    option.enroll("mod_logicsim", GetLongOpt::NoValue,
            "Simulate a circuit through CPU instructions.", 0);
    /* Assignment-3 */
    option.enroll("simulator", GetLongOpt::MandatoryValue,
            "Generate a cpp code for logic simulating of given circuit.", 0);
    /* */
    option.enroll("help", GetLongOpt::NoValue,
            "print this help summary", 0);
    option.enroll("logicsim", GetLongOpt::NoValue,
            "run logic simulation", 0);
    option.enroll("plogicsim", GetLongOpt::NoValue,
            "run parallel logic simulation", 0);
    option.enroll("fsim", GetLongOpt::NoValue,
            "run stuck-at fault simulation", 0);
    option.enroll("stfsim", GetLongOpt::NoValue,
            "run single pattern single transition-fault simulation", 0);
    option.enroll("transition", GetLongOpt::NoValue,
            "run transition-fault ATPG", 0);
    option.enroll("input", GetLongOpt::MandatoryValue,
            "set the input pattern file", 0);
    option.enroll("output", GetLongOpt::MandatoryValue,
            "set the output pattern file", 0);
    option.enroll("bt", GetLongOpt::OptionalValue,
            "set the backtrack limit", 0);
    option.enroll("start", GetLongOpt::MandatoryValue,
            "set the start gate of of path", 0);
    option.enroll("end", GetLongOpt::MandatoryValue,
            "set the end gate of of path", 0);
    int optind = option.parse(argc, argv);
    if ( optind < 1 ) { exit(0); }
    if ( option.retrieve("help") ) {
        option.usage();
        exit(0);
    }
    return optind;
}

int main(int argc, char ** argv)
{
    /* Monitor Memory usage */
    int pid=(int) getpid();
    char buf[1024];
    sprintf(buf, "cat /proc/%d/statm",pid);
    system(buf);
    /***********************/

    int optind = SetupOption(argc, argv);
    clock_t time_init, time_end;
    time_init = clock();
    //Setup File
    if (optind < argc) {
        if ((yyin = fopen(argv[optind], "r")) == NULL) {
            cout << "Can't open circuit file: " << argv[optind] << endl;
            exit( -1);
        }
        else {
            string circuit_name = argv[optind];
            string::size_type idx = circuit_name.rfind('/');
            if (idx != string::npos) { circuit_name = circuit_name.substr(idx+1); }
            idx = circuit_name.find(".bench");
            if (idx != string::npos) { circuit_name = circuit_name.substr(0,idx); }
            Circuit.SetName(circuit_name);
        }
    }
    else {
        cout << "Input circuit file missing" << endl;
        option.usage();
        return -1;
    }
    cout << "Start parsing input file\n";
    yyparse();
    if (ParseError) {
        cerr << "Please correct error and try Again.\n";
        return -1;
    }
    fclose(yyin);
    Circuit.FanoutList();
    Circuit.SetupIO_ID();
    Circuit.Levelize();
    Circuit.Check_Levelization();
    Circuit.InitializeQueue();

    if(option.retrieve("ass0")) {               // Assignment-0 
        cout << "This is assignment 0 of 2022 VLSI Testing course." << endl;
        // Circuit.PrintNetlist();
        Circuit.ShowStatistics();
    } 
    else if (option.retrieve("path")) {         // Assignment-1
        Circuit.Path(option.retrieve("start"), option.retrieve("end"));    
    }
    else if (option.retrieve("pattern")) {      // Assignment-2, part-a
        int PatternNum = stoi(option.retrieve("num"));
        string PatternName = option.retrieve("output");;
        
        cout << "**[-pattern] Generate " << PatternNum << " patterns for " 
             << Circuit.GetName() << ".bench" << endl;

        if(option.retrieve("unknown"))
            Circuit.GenRandomPattern(PatternName, PatternNum, true);
        else
            Circuit.GenRandomPattern(PatternName, PatternNum, false);
    }
    else if(option.retrieve("mod_logicsim")) {  // Assignment-2, part-b
        Circuit.InitPattern(option.retrieve("input"));
        Circuit.Mod_LogicSimVectors();
    }
    else if(option.retrieve("simulator")) {     // Assignment-3, part-c
        Circuit.InitPattern(option.retrieve("input"));
        string ccs_file_name = option.retrieve("simulator");  // name of compiled-code-simulation file
        Circuit.openSimulatorFile(ccs_file_name);
        Circuit.ccsParallelLogicSimVectors();
    }
    else if (option.retrieve("logicsim")) {
        //logic simulator
        Circuit.InitPattern(option.retrieve("input"));
        Circuit.LogicSimVectors();
    }
    else if (option.retrieve("plogicsim")) {    // Assignment-3, part-a
        //parallel logic simulator
        Circuit.InitPattern(option.retrieve("input"));
        Circuit.ParallelLogicSimVectors();
    }
    else if (option.retrieve("stfsim")) {
        //single pattern single transition-fault simulation
        Circuit.MarkOutputGate();
        Circuit.GenerateAllTFaultList();
        Circuit.InitPattern(option.retrieve("input"));
        Circuit.TFaultSimVectors();
    }
    else if (option.retrieve("transition")) {
        Circuit.MarkOutputGate();
        Circuit.GenerateAllTFaultList();
        Circuit.SortFaninByLevel();
        if (option.retrieve("bt")) {
            Circuit.SetBackTrackLimit(atoi(option.retrieve("bt")));
        }
        Circuit.TFAtpg();
    }
    else {
        Circuit.GenerateAllFaultList();
        Circuit.SortFaninByLevel();
        Circuit.MarkOutputGate();
        if (option.retrieve("fsim")) {
            //stuck-at fault simulator
            Circuit.InitPattern(option.retrieve("input"));
            Circuit.FaultSimVectors();
        }

        else {
            if (option.retrieve("bt")) {
                Circuit.SetBackTrackLimit(atoi(option.retrieve("bt")));
            }
            //stuck-at fualt ATPG
            Circuit.Atpg();
        }
    }
    time_end = clock();
    cout << "total CPU time = " << double(time_end - time_init)/CLOCKS_PER_SEC << endl;
    cout << endl;
    
    /* Monitor Memory Usage*/
    // system("ps aux | grep atpg");
    /*********************/
    return 0;
}
