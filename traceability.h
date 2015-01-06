#ifndef TRACEABILITY_H
#define TRACEABILITY_H

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

using namespace std;

class Traceability {
public:
	Traceability(string orderName="DefaultOrderName",
                           string manufacturingStep="-1",
                           string workstationName="DefaultWorkstationName",
                           string inputTime="-1",
                           string startWorkingTime="-1",
                           string endWorkingTime="-1",
                           string outputTime="-1");
    virtual ~Traceability();
    void show(void);
    string getWorkstationName(void);
    double getInputTime(void);
    double getOutputTime(void);
    double getStartWorkingTime(void) ;
    double getEndWorkingTime(void);
    string getOrderName(void);
    // Streams
    string toString(void);
    void toString(ostream &out);
    void toString(string &str);

private:
    string _orderName;
    string _manufacturingStep;
    string _workstationName;
    double _inputTime;
    double _startWorkingTime;
    double _endWorkingTime;
    double _outputTime;
};

ostream &operator<<(ostream &out, Traceability &p);
string &operator+(string str, Traceability &p);

#endif // TRACEABILITY_H
