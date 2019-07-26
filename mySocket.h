// mySocket class
// Header
// Matt Foster
// July 2019

#include <iostream>
#include <string>

using namespace std;

class mySocket {
public:
	mySocket(int);
	void printTest();
private:
	int socketType;
	string client = "I am a client.", server = "I am a server.";

};