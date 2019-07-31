// Sockets Sample Program
// Matt Foster
// July 2019

#include "server.h"

using namespace std;
using namespace tinyxml2;

void setServerAddress(int, char*[], string*, string*);

void closeSockets(int sigNum) {
	// release sockets
	close(sockfd);
	cout << "Shutting down server" << endl;
	exit(sigNum);
}
void error(const char *msg) {
    perror(msg);
    exit(1);
}


int main(int argc, char * argv[]) {

	signal(SIGINT, closeSockets);
	signal(SIGTERM, closeSockets);

	string serverIp = "127.0.0.1", serverPort = "5000";

	// Validate commandline inputs and set server IP and port
	setServerAddress(argc, argv, &serverIp, &serverPort);
	cout << "Starting Server at " << serverIp << ":" << serverPort << endl;
	// Open socket, listen for data

	int portNo = stoi(serverPort);

	runServer(2, serverIp, portNo);
	
	return 0;
}



// validates commandline input and sets IP and Port when appropriate
void setServerAddress(int argc, char * argv[], string* serverIp, string* serverPort){
	switch (argc) {
		case 1:		// no additional commandline inputs, use defaults
			break;
		case 3:		// just ip or port set, use default for the other
			if (strcmp(argv[1], "-i") == 0) {
				*serverIp = argv[2];
			} else if (strcmp(argv[1], "-p") == 0) {
				*serverPort = argv[2];
			} else {
				argumentError();
			}
			break;
		case 5:		// set input ip and port
			if (strcmp(argv[1], "-i") == 0 && (strcmp(argv[3], "-p") == 0)) {
				*serverIp = argv[2];
				*serverPort = argv[4];
			} else {
				argumentError();
			}
			break;
		default: 
			argumentError();
	}
}
