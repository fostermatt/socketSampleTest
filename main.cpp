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

#include "mySocket.h"

#define SERVER 0
#define CLIENT 1

#define PRINTSERVERINFO cout << "Server Address: " << serverIp << ":" << serverPort << endl;

using namespace std;

void argumentError();
void setServerAddress(int, char*[], string*, string*);
void runServer(int, int);
void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char * argv[]) {
	string serverIp = "-1000";
	string serverPort = "-1000";

	// Validate commandline inputs and set server IP and port
	setServerAddress(argc, argv, &serverIp, &serverPort);
	
	// Open socket, listen for data
	// mySocket thisConnection(SERVER);
	// thisConnection.printTest();

	int portNo = stoi(serverPort);
	runServer(2, portNo);

	// determine if data is valid XML
		// pass to work queue to process valid XML
		// "Unknown Command" if invalid XML

	// work queue should parse commands and display to console along with data rows

	// send response to socket

	
	return 0;
}

void argumentError() {
	cout << "Usage: \"main -i IP -p Port\"\nOmit either or both for default values" << endl;
	exit(0);
}

void setServerAddress(int argc, char * argv[], string* serverIp, string* serverPort){
	if(argc == 1) {				// no address or port entered
		// default IP and Port will be set
	} else if (argc == 3) {		// just an address or port entered
		if (strcmp(argv[1], "-i") == 0) {
			*serverIp = argv[2];
		} else if (strcmp(argv[1], "-p") == 0) {
			*serverPort = argv[2];
		} else {
			argumentError();
		}
		
	} else if (argc == 5) { 	// both are entered
		if (strcmp(argv[1], "-i") == 0 && (strcmp(argv[3], "-p") == 0)) {
			*serverIp = argv[2];
			*serverPort = argv[4];
		} else {
			argumentError();
		}
	} else {					// improper command line usage
		argumentError();
	}
	// Set default values if not updated
	if (*serverIp == "-1000") {
		*serverIp = "127.0.0.1";
	}
	if (*serverPort == "-1000") {
		*serverPort = "5000";
	}
}

void runServer(int argc, int passedPortNo) {
	int sockfd, newsockfd, portno;
	socklen_t clilen;
	char buffer[256];
	struct sockaddr_in serv_addr, cli_addr;
	int n;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		error("ERROR opening socket");
	}
	
	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = passedPortNo;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);

	// allow reusing the address to handle server restarts
	// - default behavior is hold the resources after the process releases per TIME-WAIT TCP protocol operation
	int enable = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
		error("setsockopt(SO_REUSEADDR) failed");
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
	bzero(buffer,256);
	n = read(newsockfd,buffer,255);
	if (n < 0) {
		error("ERROR reading from socket");
	}
	// printf("Here is the message: %s\n",buffer);
	cout << buffer << endl;
	n = write(newsockfd,"yass queen\n",12);

	if (n < 0) {
		error("ERROR writing to socket");
	}
	close(newsockfd);
	close(sockfd);
}














