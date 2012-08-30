#include <algorithm>
#include <limits>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <vector>

using namespace std;
void replaceAll(std::string& str, const std::string& from, const std::string& to);


void replaceAll(std::string& str, const std::string& from, const std::string& to) {
    if(from.empty())
        return;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}


template <class T>
inline std::string to_string (const T& t) {
    std::stringstream ss;
    ss << t;
    return ss.str();
}


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

class Traceability {
public:
    Traceability(string orderName="DefaultOrderName",
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
    virtual ~Traceability() {}
    void show(void) {
        cout << "Traceability: (" << this << ")"<< endl;
        cout << "\torderName:\t\t" <<  _orderName<< endl;
        cout << "\tmanufacturingStep:\t" <<  _manufacturingStep<< endl;
        cout << "\tworkstationName:\t" <<  _workstationName<< endl;
        cout << "\tinputTime:\t\t" <<  _inputTime<< endl;
        cout << "\tstartWorkingTime:\t" <<  _startWorkingTime<< endl;
        cout << "\tendWorkingTime:\t\t" <<  _endWorkingTime<< endl;
        cout << "\toutputTime:\t\t" <<  _outputTime<< endl;
    }

    string getWorkstationName(void) {
        return _workstationName;
    }

    double getInputTime(void) {
        return _inputTime;
    }

    double getOutputTime(void) {
        return _outputTime;
    }

    double getStartWorkingTime(void) {
        return _startWorkingTime;
    }

    double getEndWorkingTime(void) {
        return _endWorkingTime;
    }

    string getOrderName(void) {
        return _orderName;
    }

private:
    string _orderName;
    string _manufacturingStep;
    string _workstationName;
    double _inputTime;
    double _startWorkingTime;
    double _endWorkingTime;
    double _outputTime;
};

class TraceabilityVector  : public vector<Traceability> {
public:
    TraceabilityVector() {
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

    void show(void) {
        for (TraceabilityVector::iterator it = begin(); it!=end(); ++it) {
            (*it).show();
        }
    }

    void showWorkstationName(void) {
        for (TraceabilityVector::iterator it = begin(); it!=end(); ++it) {
            cout << (*it).getWorkstationName() << endl;
        }
    }

    virtual void push_back ( Traceability traceability ) {
        vector<Traceability>::push_back(traceability);
        makeStatistics(traceability);
    }

    double minInputTime() {
        return _minInputTime;
    }

    double maxInputTime() {
        return _maxInputTime;
    }

    double minOutputTime() {
        return _minOutputTime;
    }

    double maxOutputTime() {
        return _maxOutputTime;
    }

    double minWorkingTime() {
        return _minWorkingTime;
    }

    double maxWorkingTime() {
        return _maxWorkingTime;
    }

    void sortByOrderName() {
        stable_sort(begin(), end(), [](Traceability a, Traceability b) {
            return a.getOrderName() > b.getOrderName();
        });

    }

    void sortByWorkstationName() {
        stable_sort(begin(), end(), [](Traceability a, Traceability b) {
            return a.getWorkstationName() > b.getWorkstationName();
        });
    }

    void sortByWorkstationName_AND_OrderName() {
        stable_sort(begin(), end(), [](Traceability a, Traceability b) {
            return !(a.getOrderName() <= b.getOrderName()) && (a.getStartWorkingTime() >= b.getEndWorkingTime());
        });
    }

    string graphvizWorkstationOriented(void) {
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

    void image(bool cumulate=false, string ppmOutputFileName="traceabilities.ppm") {
        const long V_RATIO=10;
        const long TIME_LINE_HEIGHT=2*V_RATIO;
        const long IHeight=(long)vector<Traceability>::size()*V_RATIO+TIME_LINE_HEIGHT+TIME_LINE_HEIGHT;
        const long IWidth=(long)maxOutputTime();

        //char fData[IHeight][IWidth][3];

        allocate_fData(IHeight, IWidth);
        initialize_fData(IHeight, IWidth);

        // Time Line -- TOP
        for (long i=0; i<TIME_LINE_HEIGHT; i++) {
            for(long j=0; j<IWidth; j++) {
                int color=0;
                long position=j;
                position=(int)(j/V_RATIO)%2;
                color=position*255;
                fData[i][j][0]=fData[i][j][1]=fData[i][j][2]=color;
            }
        }
        // Time Line -- Botom
        for (long i=IHeight-TIME_LINE_HEIGHT; i<IHeight; i++) {
            for(long j=0; j<IWidth; j++) {
                int color=0;
                long position=j;
                position=(int)(j/V_RATIO)%2;
                color=position*255;
                fData[i][j][0]=fData[i][j][1]=fData[i][j][2]=color;
            }
        }
        // Time Line --CROSS
        for (long i=TIME_LINE_HEIGHT; i<IHeight-TIME_LINE_HEIGHT; i++) {
            for(long j=0; j<IWidth; j++) {
                int color=0;
                long position=j;
                position=(int)(j/V_RATIO)%2;
                color=position*(255-240)+240;
                fData[i][j][0]=fData[i][j][1]=fData[i][j][2]=color;
            }
        }

        long position=TIME_LINE_HEIGHT; // on continue juste après la time line
        long cursor=position;
        // Add traceability informations
        Traceability *previousTraceability=NULL;
        for (TraceabilityVector::iterator it = begin(); it!=end(); ++it) {
            //(*it).show();

            if(previousTraceability!=NULL && (*previousTraceability).getOrderName()!=(*it).getOrderName() ) {
                cursor=position;
            }
            if(!cumulate) {
                cursor=position;
            }
            // input
            for (long i=cursor; i<position+V_RATIO; i++) {
                for(long j=(long)(*it).getInputTime(); j<(long)(*it).getStartWorkingTime(); j++) {
                    fData[i][j][0]=250;  // rouge
                    fData[i][j][1]=155;  // vert
                    fData[i][j][2]=20;   // bleu
                }
                // working
                for(long j=(long)(*it).getStartWorkingTime(); j<(long)(*it).getEndWorkingTime(); j++) {
                    fData[i][j][0]=0;
                    fData[i][j][1]=255; // vert
                    fData[i][j][2]=0;
                }
                // output
                for(long j=(long)(*it).getEndWorkingTime(); j<(long)(*it).getOutputTime(); j++) {
                    fData[i][j][0]=255;
                    fData[i][j][1]=0;
                    fData[i][j][2]=0;
                }
                previousTraceability=&(*it);
            }
            position+=V_RATIO;
        }

        ofstream output(ppmOutputFileName, ios::binary|ios::out);
        if(!output) {
            cout << "unable to open the output file "<< "d.ppm" << endl;
        } else {
            output << "P6"<< endl <<"# foreground "<<endl;
            //output << itoa(IWidth, strtemp, 10);
            output << IWidth;
            output << " ";
            //output << itoa(IHeight, strtemp, 10);
            output << IHeight;
            output << endl;
            //output << itoa(255, strtemp, 10) << endl;
            output << 255 << endl;
            //output.write( (char *)fData, IHeight*IWidth*3);
            for (long i=0; i<IHeight; i++) {
                for(long j=0; j<IWidth; j++) {
                    output.write( &fData[i][j][0], sizeof(fData[i][j][0]));
                    output.write( &fData[i][j][1], sizeof(fData[i][j][1]));
                    output.write( &fData[i][j][2], sizeof(fData[i][j][2]));
                }
            }
            output.close();
        }
        delete_fData(IHeight, IWidth);
    }


private:
    double _minInputTime;
    double _maxInputTime;
    double _minOutputTime;
    double _maxOutputTime;
    double _minWorkingTime;
    double _maxWorkingTime;

    void makeStatistics(Traceability t) {
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
    void setMinInputTime(const double value) {
        if(_minInputTime > value) {
            _minInputTime=value;
        }
    }
    void setMaxInputTime(const double value) {
        if(_maxInputTime < value) {
            _maxInputTime=value;
        }
    }
    void setMinOutputTime(const double value) {
        if(_minOutputTime > value) {
            _minOutputTime=value;
        }
    }
    void setMaxOutputTime(const double value) {
        if(_maxOutputTime < value) {
            _maxOutputTime=value;
        }
    }
    void setMinWorkingTime(const double value) {
        if(_minWorkingTime > value) {
            _minWorkingTime=value;
        }
    }
    void setMaxWorkingTime(const double value) {
        if(_maxWorkingTime < value) {
            _maxWorkingTime=value;
        }
    }

    char ***fData; // used in image
    void allocate_fData(long IHeight, long IWidth) {
        fData= new char** [IHeight];
        for(long i=0; i<IHeight; i++) {
            fData[i]= new char* [IWidth];

            for(long j=0; j<IWidth; j++) {
                fData[i][j]= new char[3];
            }
        }
    }
    void initialize_fData(long IHeight, long IWidth) {
        // Init ALL Matrix
        for (long i=0; i<IHeight; i++) {
            for(long j=0; j<IWidth; j++) {
                fData[i][j][0]=255;
                fData[i][j][1]=255;
                fData[i][j][2]=255;
            }
        }
    }
    void delete_fData(long IHeight, long IWidth) {
        for(long i=0; i<IHeight; i++) {
            for(long j=0; j<IWidth; j++) {
                delete[] fData[i][j];
            }
            delete[] fData[i];
        }
        delete[] fData;
    }

};

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
            Traceability t(
                items[1],
                items[2],
                items[3],
                items[4],
                items[5],
                items[6],
                items[7]);
            traceabilities.push_back(t);
        }
    }
    return traceabilities;
}

int main () {
    const string INPUT_CSV_FILE="resultsSimulationTraceability.csv";
    //const string INPUT_CSV_FILE="/home/jpierre03/GIT-depot/dev-haimes/resultsSimulationTraceability.csv";
    const string OUTPUT_PPM_FILE="traceabilities.ppm";

    TraceabilityVector traceabilities=import_traceabilities_from_CSV_file(INPUT_CSV_FILE);


    //traceabilities.show();
    //traceabilities.showWorkstationName();
    cout << "minInputTime: " <<traceabilities.minInputTime() << endl;
    cout << "maxInputTime: " <<traceabilities.maxInputTime() << endl;
    cout << endl;
    cout << "minWorkingTime: " <<traceabilities.minWorkingTime() << endl;
    cout << "maxWorkingTime: " <<traceabilities.maxWorkingTime() << endl;
    cout << endl;
    cout << "minOutputTime: " <<traceabilities.minOutputTime() << endl;
    cout << "maxOutputTime: " <<traceabilities.maxOutputTime() << endl;
    cout << endl;

    /*
    cout << "-----------------------------" << endl;
    cout << traceabilities.graphvizWorkstationOriented();
    cout << "-----------------------------" << endl;

    ofstream myfile;
    myfile.open ("test.gv");
    myfile << traceabilities.graphvizWorkstationOriented();
    myfile << endl;
    myfile.close();
    */

    //traceabilities.sortByOrderName();
    //traceabilities.sortByWorkstationName();
    traceabilities.sortByWorkstationName_AND_OrderName();
    //traceabilities.image();
    traceabilities.image(true, OUTPUT_PPM_FILE);

    system("eog traceabilities.ppm");

    return 0;
}
