/* Zain Ul Mustafa 2017 */

/* This software is allowed for any user to copy, modify, merge, and distribute in any of his/her project.
Complete repository for this project is available on github. If you find this project anywhere except github, it
is to be noted that it is not made by me and not distributed by me. 
Please follow this link to get to the original repository of github for this project
Github shortened link (no spam and no ads): http://bit.ly/2vGkinQ
*/




/* PC 2 Arduino */

#include<iostream>
using namespace std;

#include<string>
#include<stdlib.h>
#include"SerialPort.h"

char output[MAX_DATA_LENGTH];
char incomingData[MAX_DATA_LENGTH];


// change the name of the port with the port name of your computer
// must remember that the backslashes are essential so do not remove them

char *port = "\\\\.\\COM3";

int main(){
	SerialPort arduino(port);

	if(arduino.isConnected()){
		cout<<"Connection made"<<endl<<endl;
	}
	else{
		cout<<"Error in port name"<<endl<<endl;
	}

	while(arduino.isConnected()){
		
		cout << "Enter your command: " << endl;

		string data;
		cin>>data;

		char *charArray = new char[data.size() + 1];

		// copy(first, last, output)
		copy(data.begin(), data.end(), charArray);
		charArray[data.size()] = '\n';

		// WriteSerialPort
		arduino.writeSerialPort(charArray, MAX_DATA_LENGTH);

		// ReadSerialPort
		arduino.readSerialPort(output, MAX_DATA_LENGTH);

		cout <<">> "<<output<<endl;

		delete [] charArray;
	}

	return 0;
}
