#include "traceabilitytoolbox.h"


TraceabilityVector import_traceabilities_from_CSV_file(const string fileName) {
    const int MAX_ITEMS=7+1;

    ifstream inFile (fileName);
    string line;
    int linenum = 0;
    TraceabilityVector traceabilities;

    while (getline (inFile, line)) {
        linenum++;
        istringstream linestream(line);
        string item;
        int itemnum = 0;
        string items[MAX_ITEMS];
        //cout << "\nLine #" << linenum << ":" << endl;
        while (getline (linestream, item, ',')) {
            itemnum++;
            //cout << "Item #" << itemnum << ": " << item << endl;
            if(itemnum<MAX_ITEMS) { // pour éviter les segfault
                items[itemnum]=item;
            } else {
                cerr<< "il y a trops d'éléments sur la ligne" << endl;
            }
        }
        if(itemnum==MAX_ITEMS-1) {
            Traceability *t;
            t= new Traceability(
                items[1],
                items[2],
                items[3],
                items[4],
                items[5],
                items[6],
                items[7]);
            traceabilities.push_back(*t);
        }
    }
    return traceabilities;
}

void showStatistics(TraceabilityVector & traceabilities) {
    cout << "Traceability Statistics"<< endl;
    cout << "\t" << "minInputTime:   " <<traceabilities.minInputTime() << endl;
    cout << "\t" << "maxInputTime:   " <<traceabilities.maxInputTime() << endl;
    cout << endl;
    cout << "\t" << "minWorkingTime: " <<traceabilities.minWorkingTime() << endl;
    cout << "\t" << "maxWorkingTime: " <<traceabilities.maxWorkingTime() << endl;
    cout << endl;
    cout << "\t" << "minOutputTime: " <<traceabilities.minOutputTime() << endl;
    cout << "\t" << "maxOutputTime: " <<traceabilities.maxOutputTime() << endl;

    assert(traceabilities.minInputTime()>=0);
    assert(traceabilities.maxInputTime()>0);
    assert(traceabilities.minWorkingTime()>=0);
    assert(traceabilities.maxWorkingTime()>0);
    assert(traceabilities.minOutputTime()>=0);
    assert(traceabilities.maxOutputTime()>0);
}

void showGraphviz(TraceabilityVector & traceabilities) {
    cout << "-----------------------------" << endl;
    cout << traceabilities.graphvizWorkstationOriented();
    cout << "-----------------------------" << endl;
}

void make_graphvizWorkstationOriented(TraceabilityVector & traceabilities) {
    ofstream myfile;
    myfile.open ("test.gv");
    myfile << traceabilities.graphvizWorkstationOriented();
    myfile << endl;
    myfile.close();
}

//void make_image(TraceabilityVector & traceabilities, bool isCumulate=false, const string output_ppm_file="traceabilities.ppm") {
TraceabilityImage* make_image(TraceabilityVector & traceabilities, const bool isCumulate, const string output_ppm_file) {
    //traceabilities.sortByOrderName();
    //traceabilities.sortByWorkstationName();
    traceabilities.sortByWorkstationName_AND_OrderName();
    //traceabilities.image();
    TraceabilityImage* image;
    image= new TraceabilityImage(traceabilities);
    image->image(isCumulate, output_ppm_file);

    return image;
}

void show_image() {
    system("eog traceabilities.ppm");
}
