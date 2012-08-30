#ifndef TRACEABILITY_H
#define TRACEABILITY_H

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
        Traceability(string orderName, string manufacturingStep, string workstationName, string inputTime, string startWorkingTime, string endWorkingTime, string outputTime);
        virtual ~Traceability();
        void show(void);
        string getWorkstationName(void);
        double getInputTime(void);
        double getOutputTime(void);
        double getStartWorkingTime(void) ;
        double getEndWorkingTime(void);
        string getOrderName(void);
    private:
        string _orderName;
        string _manufacturingStep;
        string _workstationName;
        double _inputTime;
        double _startWorkingTime;
        double _endWorkingTime;
        double _outputTime;
};

#endif // TRACEABILITY_H
