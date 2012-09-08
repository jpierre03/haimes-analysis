//#define NDEBUG // enable this to disable assert() macro
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

/*****************************************/
/*** Test FSM */
#include <iostream>
#include <map>
#include <string>

using namespace std;

class State {
public:
    const string Action( int i ) {
        return DoSomething(i);
    }
private:
    virtual const string DoSomething( int i ) = 0;
};

class StateA: public State {
private:
    const string DoSomething( int i ) {
        cout << "A -> ";
        return (i%2==0) ? "B" : "C";
    }
};

class StateB: public State {
private:
    const string DoSomething( int i ) {
        cout << "B -> ";
        return (i%3==0) ? "C" : "A";
    }
};

class StateC: public State {
private:
    const string DoSomething( int i ) {
        cout << "C -> ";
        return (i%4==0) ? "B" : "D";
    }
};
class StateD: public State {
private:
    const string DoSomething( int i ) {
        cout << "D -> ";
        return (i%4==0) ? "B" : "A";
    }
};

class Graph {
public:
    Graph() {
        states_["A"] = new StateA;
        states_["B"] = new StateB;
        states_["C"] = new StateC;
        states_["D"] = new StateD;
        currentState_ = states_["A"];
    }

    void DoSomething( int i ) {
        string nextState = currentState_->Action(i);
        currentState_ = states_[nextState];
    }

private:
    map<string, State*> states_;
    State * currentState_;
};

void testFSM(void) {
    Graph graph;
    for (int i=0; i<20*1000 ; i++)
        graph.DoSomething(i);

    cout << "end" << endl;
}
/*** END Test FSM */
/*****************************************/

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

void showStatistics(TraceabilityVector & traceabilities) {
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
}

void showGraphviz(TraceabilityVector & traceabilities) {
    cout << "-----------------------------" << endl;
    cout << traceabilities.graphvizWorkstationOriented();
    cout << "-----------------------------" << endl;
}

void make_graphvizWorkstationOriented(TraceabilityVector & traceabilities) {
    ofstream myfile;
    myfile.open ("test.gv");
    myfile << traceabilities.graphvizWorkstationOriented();
    myfile << endl;
    myfile.close();
}

void make_image(TraceabilityVector & traceabilities, bool isCumulate=false,const string output_ppm_file="traceabilities.ppm") {
    //traceabilities.sortByOrderName();
    //traceabilities.sortByWorkstationName();
    traceabilities.sortByWorkstationName_AND_OrderName();
    //traceabilities.image();
    traceabilities.image(isCumulate, output_ppm_file);

    system("eog traceabilities.ppm");
}

int main (int argc, char *argv[]) {
    //const string INPUT_CSV_FILE="resultsSimulationTraceability2.csv";
    const string INPUT_CSV_FILE="resultsSimulationTraceability.csv";
    //const string INPUT_CSV_FILE="/home/jpierre03/GIT-depot/dev-haimes/resultsSimulationTraceability.csv";
    TraceabilityVector traceabilities=import_traceabilities_from_CSV_file(INPUT_CSV_FILE);

    assert(traceabilities.size()>0);

    //traceabilities.show();
    //traceabilities.showWorkstationName();
    showStatistics(traceabilities);
    //showGraphviz(traceabilities);
    //make_graphvizWorkstationOriented(traceabilities);
    make_image(traceabilities);
    //testFSM();

    return 0;
}
