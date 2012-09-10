//#define NDEBUG // enable this to disable assert() macro
#include <assert.h>
#include <algorithm>
#include <limits>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <stdexcept>
#include <utility>
#include <vector>
#include "traceability.h"
#include "traceabilitytoolbox.h"
#include "traceabilityvector.h"

using namespace std;

void testFSM(void);
void test_multimap(void);
void test_multimap_traceability(TraceabilityVector &traceabilities);

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

void test_multimap(void) {

    // Compare (<) function not required since it is built into string class.
    // else declaration would comparison function in multimap definition.
    // i.e. multimap<string, int, compare> m;

    multimap<string, int> m;

    m.insert(pair<string, int>("a", 1));
    m.insert(pair<string, int>("c", 2));
    m.insert(pair<string, int>("b", 3));
    m.insert(pair<string, int>("b", 4));
    m.insert(pair<string, int>("a", 5));
    m.insert(pair<string, int>("b", 6));

    cout << "Number of elements with key a: " << m.count("a") << endl;
    cout << "Number of elements with key b: " << m.count("b") << endl;
    cout << "Number of elements with key c: " << m.count("c") << endl;

    cout << "Elements in m: " << endl;
    for (multimap<string, int>::iterator it = m.begin();
            it != m.end();
            ++it) {
        cout << "  [" << (*it).first << ", " << (*it).second << "]" << endl;
    }

    pair<multimap<string, int>::iterator, multimap<string, int>::iterator> ppp;

    // equal_range(b) returns pair<iterator,iterator> representing the range
    // of element with key b
    ppp = m.equal_range("b");

    // Loop through range of maps of key "b"
    cout << endl << "Range of \"b\" elements:" << endl;
    for (multimap<string, int>::iterator it2 = ppp.first;
            it2 != ppp.second;
            ++it2) {
        cout << "  [" << (*it2).first << ", " << (*it2).second << "]" << endl;
    }

// Can't do this (??)
//   cout << ppp.first << endl;
//   cout << ppp.second << endl;

    m.clear();

}

void test_multimap_traceability(TraceabilityVector &traceabilities) {
    multimap<string, Traceability*>::iterator it;
    //multimap<string, Traceability*> m = traceabilities.getTaceabilities_by_orderName("order6-0");
    traceabilities.show_orderNames();
    traceabilities.show_workstationNames();
    multimap<string, Traceability*> m = traceabilities.getTaceabilities_by_workstationName("M3");

    cout << "Elements in map: " << "(" << m.size() << ")"<< endl;
    for (it = m.begin(); it != m.end(); ++it) {
        cout << " \t[" << it->first << ", " << ((Traceability *)(*it).second)->toString() << "]" << endl;
    }
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
    make_image(traceabilities, true);
    //show_image();
    //testFSM();

    //test_multimap();
    test_multimap_traceability(traceabilities);

    return 0;
}
