// mySocket class
// Header
// Matt Foster
// July 2019

#include <iostream>
#include <string>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h> 
#include <netinet/in.h>
#include <arpa/inet.h>
#include <queue>
#include <map>
#include <ctime>
#include <csignal>

#include "tinyxml2.h"

using namespace std;

class server {
public: 
	void runServer(int, string, int);
	void stopServer();
private:
	struct XMLStorageObject {
		string command;
		map<string, string> data;
	};

	queue<XMLDocument*> workingQueue;
	int errFlag = 0, sockfd;

	void argumentError();
	string readFromSocket(int);
	void writeToSocket(int, string);
	string processQueue();
	void parseRows(XMLStorageObject*, XMLElement*);
	void printStruct(XMLStorageObject*);
	string createResponse(XMLStorageObject*);
}
