#include <iostream>
#include <fstream>
//#include <windows.h>

using namespace std;


int main()
{
	const int IHeight=255;
	const int IWidth=255;

	char fData[IHeight][IWidth][3];

	int tmp=0;
	for (int i=0;i<IWidth;i++){
		for(int j=0;j<IHeight;j++){
			tmp++;
			fData[i][j][0]=(i)%255;
			fData[i][j][1]=(j)%255;
			fData[i][j][2]=(tmp)%(255/2);
		}
	}

	ofstream output("d.ppm", ios::binary|ios::out);
	if(!output){
		cout << "unable to open the output file "<< "d.ppm" << endl;
	}
	else{
		output << "P6"<< endl <<"# foreground "<<endl;
		//output << itoa(IWidth, strtemp, 10);
		output << IWidth;
		output << " ";
		//output << itoa(IHeight, strtemp, 10);
		output << IHeight;
		output << endl;
		//output << itoa(255, strtemp, 10) << endl;
		output << 255 << endl;
		output.write( (char *)fData, IHeight*IWidth*3);
		output.close();

	};//end of else

}
