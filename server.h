// mySocket class
// Header
// Matt Foster
// July 2019

#include <iostream>
#include <string>
#include <map>

#include "tinyxml2.h"

using namespace std;
using namespace tinyxml2;

class Server {
public:
	Server();
	~Server();
	void runServer(string, int);
	void stopServer();
private:
	struct XMLStorageObject {
		string command;
		map<string, string> data;
	};

	int errFlag;
	int sockfd;

	string readFromSocket(int);
	void writeToSocket(int, string);
	string processQueue();
	void parseRows(XMLStorageObject*, XMLElement*);
	void printStruct(XMLStorageObject*);
	string createResponse(XMLStorageObject*);
	void error(const char *msg);
};
