// Sockets Sample Program
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

#include "tinyxml2.h"

#define SERVER 0
#define CLIENT 1

#define PRINTSERVERINFO cout << "Starting Server at " << serverIp << ":" << serverPort << endl;

using namespace std;
using namespace tinyxml2;

struct xmlStorageObject {
	string command;
	map<string, string> data;
};

queue<XMLDocument*> workingQueue;

void argumentError();
void setServerAddress(int, char*[], string*, string*);
void runServer(int, string, int);
string readFromSocket(int);
void writeToSocket(int, string);
// int parseInput(int, string, XMLDocument&);
string processQueue();
void parseRows(xmlStorageObject*, XMLElement*);
void printStruct(xmlStorageObject*);
string createResponse(xmlStorageObject*);
void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char * argv[]) {
	string serverIp = "127.0.0.1", serverPort = "5000";

	// Validate commandline inputs and set server IP and port
	setServerAddress(argc, argv, &serverIp, &serverPort);
	PRINTSERVERINFO;
	// Open socket, listen for data

	int portNo = stoi(serverPort);
	runServer(2, serverIp, portNo);
	
	return 0;
}

void argumentError() {
	cout << "Usage: \"main -i IP -p Port\"\nOmit either or both for default values" << endl;
	exit(0);
}

void setServerAddress(int argc, char * argv[], string* serverIpPointer, string* serverPortPointer){
	switch (argc) {
		case 1:		// no additional commandline inputs, use defaults
			break;
		case 3:		// just ip or port set, use default for the other
			if (strcmp(argv[1], "-i") == 0) {
				*serverIpPointer = argv[2];
			} else if (strcmp(argv[1], "-p") == 0) {
				*serverPortPointer = argv[2];
			} else {
				argumentError();
			}
			break;
		case 5:		// set input ip and port
			if (strcmp(argv[1], "-i") == 0 && (strcmp(argv[3], "-p") == 0)) {
				*serverIpPointer = argv[2];
				*serverPortPointer = argv[4];
			} else {
				argumentError();
			}
			break;
		default: 
			argumentError();
	}
}

void runServer(int argc, string serverIp, int passedPortNo) {
	int sockfd, newsockfd, portno;
	socklen_t clilen;
	char buffer[256];
	struct sockaddr_in serv_addr, cli_addr;
	int n;
	string tempString;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		error("ERROR opening socket");
	}
	
	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = passedPortNo;
	serv_addr.sin_family = AF_INET;
	// serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_addr.s_addr = inet_addr(serverIp.c_str());
	serv_addr.sin_port = htons(portno);

	// allow reusing the address to handle server restarts
	// - default behavior is hold the resources after the process releases per TIME-WAIT TCP protocol operation
	int enable = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
		error("setsockopt(SO_REUSEADDR) failed");
	}
	// allow reusing the port to handle server restarts
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &enable, sizeof(int)) < 0) {
		error("setsockopt(SO_REUSEPORT) failed");
	}
	
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		error("ERROR on binding");
	}
	
	listen(sockfd,5);
	clilen = sizeof(cli_addr);
	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
	if (newsockfd < 0) {
		error("ERROR on accept");
	}
	
	// read from socket
	tempString = readFromSocket(newsockfd);

// determine if data is valid XML

	// parse XML
	XMLDocument *doc = new XMLDocument();
	doc->Parse(tempString.c_str());
	// queue<XMLDocument*> *workingQueue = new queue<XMLDocument*>;
	// int invalidXML;
	// invalidXML = parseInput(newsockfd, tempString, *doc);
	if (doc->ErrorID()) {
		cout << "Unknown Command" << endl;
		writeToSocket(newsockfd, "Unknown Command");
	} else {
		// pass to work queue
		workingQueue.push(doc);
	}

// work queue should parse commands and display to console along with data rows
	// processQueue();

// send response to socket
	// write to socket
	string writeThis = processQueue();
	writeToSocket(newsockfd, writeThis);


	// cleanup memory
	delete doc;
	// release socket descriptors
	close(newsockfd);
	close(sockfd);
}

string readFromSocket(int newsockfd) {
	int n, bufferSize = 256;
	char buffer[bufferSize];
	string tempString = "";
	do{
		bzero(buffer, bufferSize);
		n = read(newsockfd, buffer, bufferSize);
		tempString += string(buffer);
		if(n < 0) {
			error("ERROR reading from socket");
		}
	} while (n == bufferSize);
	return tempString;
}

void writeToSocket(int newsockfd, string output) {
	int n;
	output += "\n";
	n = write(newsockfd,output.c_str(),output.size());

	if (n < 0) {
		error("ERROR writing to socket");
	}
}

// int parseInput(int newsockfd, string input, XMLDocument& doc) {
// 	doc.Parse(input.c_str());
// 	return doc.ErrorID();
// }

string processQueue() {
	// read from workingQueue
	XMLDocument* tempDoc = workingQueue.front();
	workingQueue.pop();
	xmlStorageObject tempObj;

	// break the doc into struct
		// process input command
	XMLElement* getCommand = tempDoc->FirstChildElement()->FirstChildElement( "command" );
	if (getCommand == 0) { // valid XML but does not include a proper command
		cout << "Unknown Command" << endl;
	} else {
		tempObj.command = getCommand->GetText();
		// cout << tempObj.command << " <-- command received" << endl;
	}
		// process input data
	XMLElement* getData = tempDoc->FirstChildElement()->FirstChildElement( "data" );
	const char * first;
	XMLElement* getRow = getData->FirstChildElement( "row" );
	// getKey->QueryStringAttribute( "type", &first);
	// cout << first << endl;

	parseRows(&tempObj, getRow);

	// output to console
	printStruct(&tempObj);
	// build response string
	string responseStr = "<?xml version = '1.0' encoding = 'UTF-8'?>\n";
	responseStr += createResponse(&tempObj);
	// cleanup memory, delete the document
	// return response string
	return responseStr;
}

void parseRows(xmlStorageObject *obj, XMLElement *row) {
	if (row == NULL) {
		return;
	}

	const char *key;
	row->QueryStringAttribute( "type", &key);
	const char * val = row->GetText();
	obj->data.emplace(key, val);

	parseRows(obj, row->NextSiblingElement());
}

void printStruct(xmlStorageObject *obj) {
	cout << "Command: " << obj->command << endl;
	map<string, string>::iterator it = obj->data.begin();

	while(it != obj->data.end()) {
		cout << it->first << " : " << it->second << endl;
		it++;
	}
	cout << endl;
}

string createResponse(xmlStorageObject *obj) {
	time_t rawtime;
	struct tm * timeinfo;
	char buffer [80];

	time (&rawtime);
	timeinfo = localtime (&rawtime);
	strftime (buffer,80,"%F %X",timeinfo);
	string dt = buffer;

	string temp = 	"<response>\n\t<command>" + obj->command + "<\\command>\n" +
						"\t<status>Complete<\\status>\n" +
						"\t<date>" + dt + "<\\date>\n" +
					"<\\response>\n";

	return temp;
}

