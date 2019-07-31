// mySocket class
// Implementation
// Matt Foster
// July 2019
#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h> 
#include <netinet/in.h>
#include <arpa/inet.h>
#include <queue>
#include <ctime>


#include "server.h"

using namespace std;
using namespace tinyxml2;

queue<XMLDocument*> workingQueue;

Server::Server() {
	errFlag = 0;
}

Server::~Server() {

}

/*
	Server method that sets passed IP and port then opens socket and listens for response
	- parses the XML and determines if it is valid
	- adds valid XML to work queue
	- processes work queue to send command and data to console
	- sends response XML to client
	- releases memory and closes sockets
*/
void Server::runServer(string serverIp, int passedPortNo) {
	int clientSockFD, portno;
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

	while(1) {
		// cout << "listening for clients" << endl;
		listen(sockfd,5);
		// cout << "client attempting to connect" << endl;
		clilen = sizeof(cli_addr);
		// cout << "running accept method" << endl;
		clientSockFD = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
		if (clientSockFD < 0) {
			error("ERROR on accept");
		}
		// read from socket
		tempString = readFromSocket(clientSockFD);
		// determine if data is valid XML
	
		// parse XML
		XMLDocument *doc = new XMLDocument();
		doc->Parse(tempString.c_str());
		if (doc->ErrorID()) {
			cout << "Unknown Command" << endl;
			writeToSocket(clientSockFD, "Unknown Command");
			errFlag = 1;
		} else {
			// pass to work queue
			workingQueue.push(doc);
		}
	
		// work queue should parse commands and display to console along with data rows
		// send response to socket
		// write to socket
		if(!errFlag){
			string writeThis = processQueue();
			writeToSocket(clientSockFD, writeThis);
		}
	
		// cleanup memory
		delete doc;
		// close client socket
		close(clientSockFD);
	}

	// release socket descriptors
	close(sockfd);
}

void Server::stopServer() {
	close(sockfd);
}


// reads from socket clientSockFD and returns results as a string
string Server::readFromSocket(int inputNewsockfd) {
	int n, bufferSize = 256;
	char buffer[bufferSize];
	string tempString = "";
	do{
		bzero(buffer, bufferSize);
		n = read(inputNewsockfd, buffer, bufferSize);
		tempString += string(buffer);
		if(n < 0) {
			error("ERROR reading from socket");
		}
	} while (n == bufferSize);
	return tempString;
}

// writes output to the socket with clientSockFD
void Server::writeToSocket(int inputNewsockfd, string output) {
	int n;
	output += "\n";
	n = write(inputNewsockfd,output.c_str(),output.size());

	if (n < 0) {
		error("ERROR writing to socket");
	}
}

// method to process the workingQueue, errors if command or data tag missing
string Server::processQueue() {
	// read from workingQueue
	XMLDocument* tempDoc = workingQueue.front();
	workingQueue.pop();
	XMLStorageObject tempObj;

	// break the doc into struct
		// process input command
	XMLElement* getCommand = tempDoc->FirstChildElement()->FirstChildElement( "command" );
	if (getCommand == 0) { // valid XML but does not include a proper command
		cout << "Unknown Command" << endl;
		errFlag = 1;
		return "Unknown Command";
	} else {
		tempObj.command = getCommand->GetText();
	}
		// process input data
	XMLElement* getData = tempDoc->FirstChildElement()->FirstChildElement( "data" );
	if (getData == 0) {
		cout << "Invalid XML: Data tag missing" << endl;
		errFlag = 1;
		return "Invalid XML: Data tag missing";
	}
	const char * first;
	XMLElement* getRow = getData->FirstChildElement( "row" );

	parseRows(&tempObj, getRow);

	// output to console
	printStruct(&tempObj);
	// build response string
	string responseStr = "<?xml version = '1.0' encoding = 'UTF-8'?>\n";
	responseStr += createResponse(&tempObj);
	// return response string
	return responseStr;
}

// method to parse rows of XMLElement storing key:value pair in XMLStorageObject map
void Server::parseRows(XMLStorageObject *obj, XMLElement *row) {
	if (row == NULL) {
		return;
	}

	const char *key;
	row->QueryStringAttribute( "type", &key);
	const char * val = row->GetText();
	obj->data.emplace(key, val);

	parseRows(obj, row->NextSiblingElement());
}

// method to print XMLStorageObject to console
void Server::printStruct(XMLStorageObject *obj) {
	cout << "Command: " << obj->command << endl;

	cout << "Data:" << endl;
	for(map<string, string>::iterator it = obj->data.begin(); it != obj->data.end(); it++) {
		cout << "\t" << it->first << " : " << it->second << endl;
	}

	cout << endl;
}

// method to create response xml string from XMLStorageObject
string Server::createResponse(XMLStorageObject *obj) {
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

void Server::error(const char *msg) {
    perror(msg);
    exit(1);
}
