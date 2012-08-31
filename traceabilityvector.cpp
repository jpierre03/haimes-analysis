#include "traceabilityvector.h"

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

//public:
TraceabilityVector::TraceabilityVector() {
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

TraceabilityVector::~TraceabilityVector() {
    cout << "TraceabilityVector::~TraceabilityVector destructor" << endl;
}


void TraceabilityVector::show(void) {
    for (TraceabilityVector::iterator it = begin(); it!=end(); ++it) {
        (*it).show();
    }
}

void TraceabilityVector::showWorkstationName(void) {
    for (TraceabilityVector::iterator it = begin(); it!=end(); ++it) {
        cout << (*it).getWorkstationName() << endl;
    }
}

void TraceabilityVector::push_back (Traceability &traceability ) {
    assert(traceability.getWorkstationName().size()>0);

    vector<Traceability>::push_back(traceability);
    makeStatistics(traceability);
}

double TraceabilityVector::minInputTime() {
    return _minInputTime;
}

double TraceabilityVector::maxInputTime() {
    return _maxInputTime;
}

double TraceabilityVector::minOutputTime() {
    return _minOutputTime;
}

double TraceabilityVector::maxOutputTime() {
    return _maxOutputTime;
}

double TraceabilityVector::minWorkingTime() {
    return _minWorkingTime;
}

double TraceabilityVector::maxWorkingTime() {
    return _maxWorkingTime;
}

void TraceabilityVector::sortByOrderName() {
    stable_sort(begin(), end(), [](Traceability a, Traceability b) {
        return a.getOrderName() > b.getOrderName();
    });

}

void TraceabilityVector::sortByWorkstationName() {
    stable_sort(begin(), end(), [](Traceability a, Traceability b) {
        return a.getWorkstationName() > b.getWorkstationName();
    });
}

void TraceabilityVector::sortByWorkstationName_AND_OrderName() {
    stable_sort(begin(), end(), [](Traceability a, Traceability b) {
        return !(a.getOrderName() <= b.getOrderName()) && (a.getStartWorkingTime() >= b.getEndWorkingTime());
    });
}

string TraceabilityVector::graphvizWorkstationOriented(void) {
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

void TraceabilityVector::image(const bool cumulate=false, const string ppmOutputFileName="traceabilities.ppm") {
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
                fData[i][j][0]=(char)250;  // rouge
                fData[i][j][1]=(char)155;  // vert
                fData[i][j][2]=(char)20;   // bleu
            }
            // working
            for(long j=(long)(*it).getStartWorkingTime(); j<(long)(*it).getEndWorkingTime(); j++) {
                fData[i][j][0]=(char)0;
                fData[i][j][1]=(char)255; // vert
                fData[i][j][2]=(char)0;
            }
            // output
            for(long j=(long)(*it).getEndWorkingTime(); j<(long)(*it).getOutputTime(); j++) {
                fData[i][j][0]=(char)255;
                fData[i][j][1]=(char)0;
                fData[i][j][2]=(char)0;
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


//private:
void TraceabilityVector::makeStatistics(Traceability t) {
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
void TraceabilityVector::setMinInputTime(const double value) {
    if(_minInputTime > value) {
        _minInputTime=value;
    }
}
void TraceabilityVector::setMaxInputTime(const double value) {
    if(_maxInputTime < value) {
        _maxInputTime=value;
    }
}
void TraceabilityVector::setMinOutputTime(const double value) {
    if(_minOutputTime > value) {
        _minOutputTime=value;
    }
}
void TraceabilityVector::setMaxOutputTime(const double value) {
    if(_maxOutputTime < value) {
        _maxOutputTime=value;
    }
}
void TraceabilityVector::setMinWorkingTime(const double value) {
    if(_minWorkingTime > value) {
        _minWorkingTime=value;
    }
}
void TraceabilityVector::setMaxWorkingTime(const double value) {
    if(_maxWorkingTime < value) {
        _maxWorkingTime=value;
    }
}

void TraceabilityVector::allocate_fData(const long IHeight, const long IWidth) {
    fData= new char** [IHeight];
    for(long i=0; i<IHeight; i++) {
        fData[i]= new char* [IWidth];

        for(long j=0; j<IWidth; j++) {
            fData[i][j]= new char[3];
        }
    }
}
void TraceabilityVector::initialize_fData(const long IHeight, const long IWidth) {
    // Init ALL Matrix
    for (long i=0; i<IHeight; i++) {
        for(long j=0; j<IWidth; j++) {
            fData[i][j][0]=(char)255;
            fData[i][j][1]=(char)255;
            fData[i][j][2]=(char)255;
        }
    }
}
void TraceabilityVector::delete_fData(const long IHeight, const long IWidth) {
    for(long i=0; i<IHeight; i++) {
        for(long j=0; j<IWidth; j++) {
            delete[] fData[i][j];
        }
        delete[] fData[i];
    }
    delete[] fData;
}
