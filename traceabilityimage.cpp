#include "traceabilityimage.h"

TraceabilityImage::TraceabilityImage(TraceabilityVector traceabilities) {
    _traceabilities=traceabilities;
}

TraceabilityImage::~TraceabilityImage() {
    //dtor
}

void TraceabilityImage::allocate_fData(const long IHeight, const long IWidth) {
    fData= new char** [IHeight];
    cout<<"Allocation de mémoire pour stocker l'image"<<endl;
    for(long i=0; i<IHeight; i++) {
        fData[i]= new char* [IWidth];

        for(long j=0; j<IWidth; j++) {
            fData[i][j]= new char[3];
        }
    }
    cout<<"Fin--Allocation de mémoire pour stocker l'image"<<endl;
}

void TraceabilityImage::initialize_fData(const long IHeight, const long IWidth) {
    // Init ALL Matrix
    for (long i=0; i<IHeight; i++) {
        for(long j=0; j<IWidth; j++) {
            fData[i][j][0]=(char)255;
            fData[i][j][1]=(char)255;
            fData[i][j][2]=(char)255;
        }
    }
}

void TraceabilityImage::delete_fData(const long IHeight, const long IWidth) {
    for(long i=0; i<IHeight; i++) {
        for(long j=0; j<IWidth; j++) {
            delete[] fData[i][j];
        }
        delete[] fData[i];
    }
    delete[] fData;
}

void TraceabilityImage::image(const bool cumulate=false, const string ppmOutputFileName="traceabilities.ppm") {
    const long V_RATIO=10;
    const long TIME_LINE_HEIGHT=2*V_RATIO;
    const long IHeight=(long)_traceabilities.size()*V_RATIO+TIME_LINE_HEIGHT+TIME_LINE_HEIGHT;
    const long IWidth=(long)_traceabilities.maxOutputTime();

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
    for (TraceabilityVector::iterator it = _traceabilities.begin(); it!=_traceabilities.end(); ++it) {
        //(*it).show();

        if(previousTraceability!=NULL && (*previousTraceability).getOrderName()!=(*it).getOrderName() ) {
            cursor=position;
            //trace une ligne de couleur pour marquer la séparation des OF
            /*for (long i=cursor; i<position+V_RATIO; i++) {
                for(long j=0; j<IWidth; j++) {
                    fData[i][j][0]=(char)220;  // rouge
                    fData[i][j][1]=(char)220;  // vert
                    fData[i][j][2]=(char)255;   // bleu
                }
            }*/
        }
        if(!cumulate) {
            cursor=position;
        }
        for (long i=cursor; i<position+V_RATIO; i++) {
            // input
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
        if(cumulate) {
            for (long i=cursor; i<position+V_RATIO; i++) {
                for(long j=0; j<IWidth; j++) {
                    fData[i][j][0]=fData[cursor][j][0];  // rouge
                    fData[i][j][1]=fData[cursor][j][1];  // vert
                    fData[i][j][2]=fData[cursor][j][2];   // bleu
                }
            }
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
