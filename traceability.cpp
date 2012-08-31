#include "traceability.h"

#include <algorithm>
#include <limits>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <vector>

using namespace std;

class BadConversion : public std::runtime_error {
public:
    BadConversion(std::string const& s)
        : std::runtime_error(s)
    { }
};

inline double convertToDouble(std::string const& s) {
    std::istringstream i(s);
    double x;
    if (!(i >> x))
        throw BadConversion("convertToDouble(\"" + s + "\")");
    return x;
}



Traceability::Traceability(string orderName="DefaultOrderName",
               string manufacturingStep="-1",
               string workstationName="DefaultWorkstationName",
               string inputTime="-1",
               string startWorkingTime="-1",
               string endWorkingTime="-1",
               string outputTime="-1") {

    _orderName=orderName;
    _manufacturingStep= manufacturingStep;
    _workstationName=   workstationName;
    _inputTime  =       convertToDouble(inputTime);
    _startWorkingTime=  convertToDouble(startWorkingTime);
    _endWorkingTime=    convertToDouble(endWorkingTime);
    _outputTime=        convertToDouble(outputTime);
}
Traceability::~Traceability() {}
void Traceability::show(void) {
    cout << "Traceability: (" << this << ")"<< endl;
    cout << "\torderName:\t\t" <<  _orderName<< endl;
    cout << "\tmanufacturingStep:\t" <<  _manufacturingStep<< endl;
    cout << "\tworkstationName:\t" <<  _workstationName<< endl;
    cout << "\tinputTime:\t\t" <<  _inputTime<< endl;
    cout << "\tstartWorkingTime:\t" <<  _startWorkingTime<< endl;
    cout << "\tendWorkingTime:\t\t" <<  _endWorkingTime<< endl;
    cout << "\toutputTime:\t\t" <<  _outputTime<< endl;
}

string Traceability::getWorkstationName(void) {
    return _workstationName;
}

double Traceability::getInputTime(void) {
    return _inputTime;
}

double Traceability::getOutputTime(void) {
    return _outputTime;
}

double Traceability::getStartWorkingTime(void) {
    return _startWorkingTime;
}

double Traceability::getEndWorkingTime(void) {
    return _endWorkingTime;
}

string Traceability::getOrderName(void) {
    return _orderName;
}