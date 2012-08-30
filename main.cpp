//#define NDEBUG
#include <assert.h>
#include <algorithm>
#include <limits>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <vector>
#include "traceability.h"
#include "traceabilityvector.h"

using namespace std;

template <class T>
inline std::string to_string (const T& t) {
    std::stringstream ss;
    ss << t;
    return ss.str();
}

TraceabilityVector import_traceabilities_from_CSV_file(const string fileName) {
    const int MAX_ITEMS=7+1;

    ifstream inFile (fileName);
    string line;
    int linenum = 0;
    TraceabilityVector traceabilities;

    while (getline (inFile, line)) {
        linenum++;
        istringstream linestream(line);
        string item;
        int itemnum = 0;
        string items[MAX_ITEMS];
        //cout << "\nLine #" << linenum << ":" << endl;
        while (getline (linestream, item, ',')) {
            itemnum++;
            //cout << "Item #" << itemnum << ": " << item << endl;
            if(itemnum<MAX_ITEMS) { // pour éviter les segfault
                items[itemnum]=item;
            } else {
                cerr<< "il y a trops d'éléments sur la ligne" << endl;
            }
        }
        if(itemnum==MAX_ITEMS-1) {
            Traceability t(
                items[1],
                items[2],
                items[3],
                items[4],
                items[5],
                items[6],
                items[7]);
            traceabilities.push_back(t);
        }
    }
    return traceabilities;
}

int main () {
    const string INPUT_CSV_FILE="resultsSimulationTraceability.csv";
    //const string INPUT_CSV_FILE="/home/jpierre03/GIT-depot/dev-haimes/resultsSimulationTraceability.csv";
    const string OUTPUT_PPM_FILE="traceabilities.ppm";

    TraceabilityVector traceabilities=import_traceabilities_from_CSV_file(INPUT_CSV_FILE);

    assert(traceabilities.size()>0);

    //traceabilities.show();
    //traceabilities.showWorkstationName();
    cout << "minInputTime: " <<traceabilities.minInputTime() << endl;
    cout << "maxInputTime: " <<traceabilities.maxInputTime() << endl;
    cout << endl;
    cout << "minWorkingTime: " <<traceabilities.minWorkingTime() << endl;
    cout << "maxWorkingTime: " <<traceabilities.maxWorkingTime() << endl;
    cout << endl;
    cout << "minOutputTime: " <<traceabilities.minOutputTime() << endl;
    cout << "maxOutputTime: " <<traceabilities.maxOutputTime() << endl;
    cout << endl;

    assert(traceabilities.minInputTime()>=0);
    assert(traceabilities.maxInputTime()>0);
    assert(traceabilities.minWorkingTime()>=0);
    assert(traceabilities.maxWorkingTime()>0);
    assert(traceabilities.minOutputTime()>=0);
    assert(traceabilities.maxOutputTime()>0);

    /*
    cout << "-----------------------------" << endl;
    cout << traceabilities.graphvizWorkstationOriented();
    cout << "-----------------------------" << endl;

    ofstream myfile;
    myfile.open ("test.gv");
    myfile << traceabilities.graphvizWorkstationOriented();
    myfile << endl;
    myfile.close();
    */

    //traceabilities.sortByOrderName();
    //traceabilities.sortByWorkstationName();
    traceabilities.sortByWorkstationName_AND_OrderName();
    //traceabilities.image();
    traceabilities.image(true, OUTPUT_PPM_FILE);

    system("eog traceabilities.ppm");

    return 0;
}
