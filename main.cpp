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
#include "traceabilitytoolbox.h"
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
