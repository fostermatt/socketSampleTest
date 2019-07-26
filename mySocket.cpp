// mySocket class
// Implementation
// Matt Foster
// July 2019

#include <iostream>
#include <string>
#include "mySocket.h"

using namespace std;

mySocket::mySocket(int id) {
	socketType = id;
}

void mySocket::printTest(){
	if (socketType == 0) {
		cout << server << endl;
	} else {
		cout << client << endl;
	}
}
