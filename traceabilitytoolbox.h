#ifndef TRACEABILITYTOOLBOX_H
#define TRACEABILITYTOOLBOX_H

#include "traceabilityvector.h"
#include "traceabilityimage.h"

TraceabilityVector import_traceabilities_from_CSV_file(const string fileName);
void showStatistics(TraceabilityVector & traceabilities);
void showGraphviz(TraceabilityVector & traceabilities);
void make_graphvizWorkstationOriented(TraceabilityVector & traceabilities);
void make_image(TraceabilityVector & traceabilities, const bool isCumulate=false, const string output_ppm_file="traceabilities.ppm");
void show_image();

#endif // TRACEABILITYTOOLBOX_H
