#ifndef TRACEABILITYVECTOR_H
#define TRACEABILITYVECTOR_H

#include <algorithm>
#include <limits>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <vector>

#include "traceability.h"

using namespace std;



class TraceabilityVector  : public vector<Traceability> {
public:
    TraceabilityVector();
    void show(void);
    void showWorkstationName(void);
    virtual void push_back ( Traceability traceability ) ;
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
    void image(bool cumulate, string ppmOutputFileName);
private:
    double _minInputTime;
    double _maxInputTime;
    double _minOutputTime;
    double _maxOutputTime;
    double _minWorkingTime;
    double _maxWorkingTime;

    void makeStatistics(Traceability t);
    void setMinInputTime(const double value);
    void setMaxInputTime(const double value);
    void setMinOutputTime(const double value);
    void setMaxOutputTime(const double value);
    void setMinWorkingTime(const double value);
    void setMaxWorkingTime(const double value);
    char ***fData; // used in image
    void allocate_fData(long IHeight, long IWidth);
    void initialize_fData(long IHeight, long IWidth);
    void delete_fData(long IHeight, long IWidth);
};

#endif // TRACEABILITYVECTOR_H
