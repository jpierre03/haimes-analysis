#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <limits>


using namespace std;

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
inline std::string to_string (const T& t)
{
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

            cout <<  ws << endl;

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
    void setMinInputTime(double value) {
        if(_minInputTime > value) {
            _minInputTime=value;
        }
    }
    void setMaxInputTime(double value) {
        if(_maxInputTime < value) {
            _maxInputTime=value;
        }
    }
    void setMinOutputTime(double value) {
        if(_minOutputTime > value) {
            _minOutputTime=value;
        }
    }
    void setMaxOutputTime(double value) {
        if(_maxOutputTime < value) {
            _maxOutputTime=value;
        }
    }
    void setMinWorkingTime(double value) {
        if(_minWorkingTime > value) {
            _minWorkingTime=value;
        }
    }
    void setMaxWorkingTime(double value) {
        if(_maxWorkingTime < value) {
            _maxWorkingTime=value;
        }
    }
};


int main () {
    const int MAX_ITEMS=7+1;

    ifstream inFile ("/home/jpierre03/resultsSimulationTraceability.csv");
    string line;
    int linenum = 0;
    TraceabilityVector traceabilities;

    while (getline (inFile, line)) {
        linenum++;
        //cout << "\nLine #" << linenum << ":" << endl;
        istringstream linestream(line);
        string item;
        int itemnum = 0;
        string items[MAX_ITEMS];
        while (getline (linestream, item, ',')) {
            itemnum++;
            //cout << "Item #" << itemnum << ": " << item << endl;
            if(itemnum<MAX_ITEMS) {
                items[itemnum]=item;
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
            //t.show();
            traceabilities.push_back(t);
        }

    }

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

    cout << "-----------------------------" << endl;
    cout << traceabilities.graphvizWorkstationOriented();
    cout << "-----------------------------" << endl;

    ofstream myfile;
    myfile.open ("/home/jpierre03/test.gv");
    myfile << traceabilities.graphvizWorkstationOriented();
    myfile.close();

    return 0;
}
