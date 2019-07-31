// Sockets Sample Program
// Matt Foster
// July 2019
#include <csignal>
#include "server.h"

using namespace std;
using namespace tinyxml2;

Server myServer;
void setServerAddress(int, char*[], string*, string*);

// displayes correct commandline usage and exits application
void argumentError() {
	cout << "Usage: \"main -i IP -p Port\"\nOmit either or both for default values" << endl;
	exit(1);
}
void closeSockets(int sigNum) {
	// release sockets
	myServer.stopServer();
	cout << "Shutting down server" << endl;
	exit(sigNum);
}


int main(int argc, char * argv[]) {

	signal(SIGINT, closeSockets);
	signal(SIGTERM, closeSockets);

	string serverIp = "127.0.0.1", serverPort = "5000";

	// Validate commandline inputs and set server IP and port
	setServerAddress(argc, argv, &serverIp, &serverPort);
	cout << "Starting Server at " << serverIp << ":" << serverPort << endl;

	int portNo = stoi(serverPort);
	
	myServer.runServer(serverIp, portNo);
	
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
