#ifndef TRACEABILITYIMAGE_H
#define TRACEABILITYIMAGE_H

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
#include "traceabilityvector.h"

using namespace std;

class TraceabilityImage {
public:
    TraceabilityImage(TraceabilityVector traceabilities);
    virtual ~TraceabilityImage();
    void image(const bool cumulate, const string ppmOutputFileName);
    char ***getData(void){return fData;};
    unsigned long getHeight(void){return _IHeight;};
    unsigned long getWidth(void){return _IWidth;};

protected:
private:
    TraceabilityVector _traceabilities;
    char ***fData;
    unsigned long _IHeight;
    unsigned long _IWidth;

    void allocate_fData(const unsigned long IHeight, const unsigned long IWidth);
    void initialize_fData();
    void delete_fData();
};

#endif // TRACEABILITYIMAGE_H
