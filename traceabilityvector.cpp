#include "traceabilityvector.h"

using namespace std;

void replaceAll(std::string& str, const std::string& from, const std::string& to) {
    if(from.empty())
        return;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}

//public:
TraceabilityVector::TraceabilityVector() {
    // input
    _minInputTime=numeric_limits<double>::max( );
    _maxInputTime=numeric_limits<double>::min( );

    // output
    _minOutputTime=numeric_limits<double>::max( );
    _maxOutputTime=numeric_limits<double>::min( );

    // working
    _minWorkingTime=numeric_limits<double>::max( );
    _maxWorkingTime=numeric_limits<double>::min( );
}

TraceabilityVector::~TraceabilityVector() {
    cout << "TraceabilityVector::~TraceabilityVector destructor" << endl;
}

void TraceabilityVector::show(void) {
    for (TraceabilityVector::iterator it = begin(); it!=end(); ++it) {
        (*it).show();
    }
}

void TraceabilityVector::showWorkstationName(void) {
    for (TraceabilityVector::iterator it = begin(); it!=end(); ++it) {
        cout << (*it).getWorkstationName() << endl;
    }
}

void TraceabilityVector::push_back (Traceability &traceability ) {
    assert(traceability.getWorkstationName().size()>0);

    vector<Traceability>::push_back(traceability);
    _traceabilities_by_orderName.insert(
        make_pair(traceability.getOrderName(),&traceability));

    _traceabilities_by_workstationName.insert(
        make_pair(traceability.getWorkstationName(),&traceability));

    makeStatistics(traceability);
}

double TraceabilityVector::minInputTime() {
    return _minInputTime;
}

double TraceabilityVector::maxInputTime() {
    return _maxInputTime;
}

double TraceabilityVector::minOutputTime() {
    return _minOutputTime;
}

double TraceabilityVector::maxOutputTime() {
    return _maxOutputTime;
}

double TraceabilityVector::minWorkingTime() {
    return _minWorkingTime;
}

double TraceabilityVector::maxWorkingTime() {
    return _maxWorkingTime;
}

void TraceabilityVector::sortByOrderName() {
    stable_sort(begin(), end(),[](Traceability a, Traceability b) {
        return a.getOrderName() > b.getOrderName();
    });
}

void TraceabilityVector::sortByWorkstationName() {
    stable_sort(begin(), end(), [](Traceability a, Traceability b) {
        return a.getWorkstationName() > b.getWorkstationName();
    });
}

void TraceabilityVector::sortByWorkstationName_AND_OrderName() {
    stable_sort(begin(), end(), [](Traceability a, Traceability b) {
        return !(a.getOrderName() <= b.getOrderName()) && (a.getStartWorkingTime() >= b.getEndWorkingTime());
    });
}

string TraceabilityVector::graphvizWorkstationOriented(void) {
    string s;
    s+="digraph G {\n";
    s+="	subgraph cluster_0 {\n";
    s+="		style=filled;\n";
    s+="		color=lightgrey;\n";
    s+="		node [style=filled,color=white];\n";
    // customize Time
    //s+="		t0 -> t1 -> t2 -> t3 -> t4 -> t5 -> t6 -> t7 -> t8 -> t9 -> t10;";
    for (long i=(long)minInputTime(); i<(long)maxOutputTime(); i++) {
        s+="		";
        s+="t";
        s+= to_string(i);
        s+=" -> ";
        s+="t";
        s+= to_string(i+1);
        s+=";\n";

        //cout << (*it).getWorkstationName() << endl;
    }
    s+="		label = \"Timeline\";\n";
    s+="	}\n";
    // end of cluster_o

    // for all traceabilities
    for (TraceabilityVector::iterator it = begin(); it!=end(); ++it) {
        string ws=(*it).getWorkstationName();
        string order=(*it).getOrderName();
        replaceAll(order, "-", "_");
        string id=ws+"_"+order;

        //cout <<  ws << endl;

        // traceability (*it)
        s+="	subgraph cluster_"+id+" {\n";
        s+="		style=filled;\n";
        s+="		color=lightgrey;\n";
        s+="		node [style=filled,color=white];\n";
        s+="		label = \""+(*it).getOrderName()+"\";\n";
        s+="		"+id+"_Input ->t"+to_string((int)(*it).getInputTime())+"\n";
        s+="		"+id+"_Start ->t"+to_string((int)(*it).getStartWorkingTime())+"\n";
        s+="		"+id+"_Stop ->t"+to_string((int)(*it).getEndWorkingTime())+"\n";
        s+="		"+id+"_Output ->t"+to_string((int)(*it).getOutputTime())+"\n";

        //s+="		"+id+"_Input ->t0\n";

        //M2_input -> M2_StartWorking -> M2_EndWorking -> M2_output;
        s+="	}\n";
    }
    // end of digraph
    s+="\n";
    s+="}";
    s+="\n";
    return s;
}

multimap<string, Traceability*> TraceabilityVector::getTaceabilities_by_orderName(void) {
    return _traceabilities_by_orderName;
}
multimap<string, Traceability*> TraceabilityVector::getTaceabilities_by_workstationName(void) {
    return _traceabilities_by_workstationName;
}

multimap<string, Traceability*> TraceabilityVector::getTaceabilities_by_orderName(string orderName) {
    multimap<string, Traceability*> traceabilities;
    pair<multimap<string, Traceability*>::iterator, multimap<string, Traceability*>::iterator> ppp;
    multimap<string, Traceability*>::iterator it;

    ppp = getTaceabilities_by_orderName().equal_range(orderName);

    // Loop through range of maps of key ""
    for (it = ppp.first; it != ppp.second; ++it) {
        traceabilities.insert(make_pair(
                                  (*it).second->getOrderName(),
                                  (*it).second)
                             );
    }
    return traceabilities;
}

multimap<string, Traceability*> TraceabilityVector::getTaceabilities_by_workstationName(string workstationName) {
    multimap<string, Traceability*> traceabilities;
    pair<multimap<string, Traceability*>::iterator, multimap<string, Traceability*>::iterator> ppp;
    multimap<string, Traceability*>::iterator it;

    ppp = getTaceabilities_by_workstationName().equal_range(workstationName);

    // Loop through range of maps of key ""
    for (it = ppp.first; it != ppp.second; ++it) {
        traceabilities.insert(make_pair(
                                  (*it).second->getWorkstationName(),
                                  (*it).second)
                             );
    }
    return traceabilities;
}

set<string> TraceabilityVector::getOrderNames(void) {
    vector<Traceability> m= *this;
    vector<Traceability>::iterator it;

    set<string> result;
    for( it = m.begin(); it != m.end(); ++it) {
        result.insert((*it).getOrderName());
    }
    return result;
}
set<string> TraceabilityVector::getWorkstationNames(void) {
    vector<Traceability> m= *this;
    vector<Traceability>::iterator it;

    set<string> result;
    for( it = m.begin(); it != m.end(); ++it) {
        result.insert((*it).getWorkstationName());
    }
    return result;
}

void TraceabilityVector::show_orderNames(void) {
    set<string>::iterator it;
    set<string> names=getOrderNames();

    cout << "Order Names" << endl;
    cout << "\t";
    for( it = names.begin(); it != names.end(); ++it) {
        cout << (*it) << "  ";
    }
    cout << endl;
}

void TraceabilityVector::show_workstationNames(void) {
    set<string>::iterator it;
    set<string> names=getWorkstationNames();

    cout << "Workstation Names" << endl;
    cout << "\t";
    for( it = names.begin(); it != names.end(); ++it) {
        cout << (*it) << "  ";
    }
    cout << endl;
}

//private:
void TraceabilityVector::makeStatistics(Traceability t) {
    // input
    setMinInputTime(t.getInputTime());
    setMaxInputTime(t.getInputTime());
    // output
    setMinOutputTime(t.getOutputTime());
    setMaxOutputTime(t.getOutputTime());
    // working
    setMinWorkingTime(t.getEndWorkingTime()-t.getStartWorkingTime());
    setMaxWorkingTime(t.getEndWorkingTime()-t.getStartWorkingTime());
}
void TraceabilityVector::setMinInputTime(const double value) {
    if(_minInputTime > value) {
        _minInputTime=value;
    }
}
void TraceabilityVector::setMaxInputTime(const double value) {
    if(_maxInputTime < value) {
        _maxInputTime=value;
    }
}
void TraceabilityVector::setMinOutputTime(const double value) {
    if(_minOutputTime > value) {
        _minOutputTime=value;
    }
}
void TraceabilityVector::setMaxOutputTime(const double value) {
    if(_maxOutputTime < value) {
        _maxOutputTime=value;
    }
}
void TraceabilityVector::setMinWorkingTime(const double value) {
    if(_minWorkingTime > value) {
        _minWorkingTime=value;
    }
}
void TraceabilityVector::setMaxWorkingTime(const double value) {
    if(_maxWorkingTime < value) {
        _maxWorkingTime=value;
    }
}
