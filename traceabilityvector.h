#ifndef TRACEABILITYVECTOR_H
#define TRACEABILITYVECTOR_H

//#define NDEBUG
#include <assert.h>
#include <algorithm>
#include <limits>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <stdexcept>
#include <vector>

#include "traceability.h"

using namespace std;

class TraceabilityVector  : public vector<Traceability> {
public:
    TraceabilityVector();
    virtual ~TraceabilityVector();
    void show(void);
    void showWorkstationName(void);
    virtual void push_back (Traceability & traceability);
    double minInputTime();
    double maxInputTime() ;
    double minOutputTime();
    double maxOutputTime();
    double minWorkingTime();
    double maxWorkingTime();
    void sortByOrderName();
    void sortByWorkstationName();
    void sortByWorkstationName_AND_OrderName() ;
    string graphvizWorkstationOriented(void);
    multimap<string, Traceability*> getTaceabilities_by_orderName(void);
    multimap<string, Traceability*> getTaceabilities_by_orderName(string orderName);
    multimap<string, Traceability*> getTaceabilities_by_workstationName(void);
    multimap<string, Traceability*> getTaceabilities_by_workstationName(string workstationName);

    set<string> getOrderNames(void);
    set<string> getWorkstationNames(void);
    void show_orderNames(void);
    void show_workstationNames(void);

private:
    double _minInputTime;
    double _maxInputTime;
    double _minOutputTime;
    double _maxOutputTime;
    double _minWorkingTime;
    double _maxWorkingTime;
    multimap<string, Traceability*> _traceabilities_by_orderName;
    multimap<string, Traceability*> _traceabilities_by_workstationName;

    void makeStatistics(Traceability t);
    void setMinInputTime(const double value);
    void setMaxInputTime(const double value);
    void setMinOutputTime(const double value);
    void setMaxOutputTime(const double value);
    void setMinWorkingTime(const double value);
    void setMaxWorkingTime(const double value);
};

#endif // TRACEABILITYVECTOR_H
