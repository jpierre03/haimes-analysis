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

protected:
private:
    TraceabilityVector _traceabilities;
    char ***fData;

    void allocate_fData(const long IHeight, const long IWidth);
    void initialize_fData(const long IHeight, const long IWidth);
    void delete_fData(const long IHeight, const long IWidth);
};

#endif // TRACEABILITYIMAGE_H
