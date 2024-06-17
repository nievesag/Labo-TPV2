// This file is part of the course TPV2@UCM - Samir Genaim

#include <iostream>
#include <SDL_net.h>
#include <SDL_stdinc.h>
#include <csignal>
#include <cstring>

using namespace std;

void error() {
	cout << "Error: " << SDLNet_GetError() << endl;
	exit(1);
}

// the port on which the server is listening, to be used by method
// shutdown to send an empty message to wake up the server if blocking
bool done = false;
Uint16 shutDownTriggetPort = 0;

// this just sent an empty message the server/client to wake it up.
void shutdown() {

	if (done)
		return;

	cout << endl << "Shutting down  ..." << endl;

	done = true;

	if (shutDownTriggetPort != 0) {
		IPaddress ip;
		SDLNet_ResolveHost(&ip, "localhost", shutDownTriggetPort);
		TCPsocket conn = SDLNet_TCP_Open(&ip);
		SDLNet_TCP_Close(conn);
	}

}

/* Signal Handler for SIGINT */
void handlerSIGINT(int) {
	shutdown();
}

void server(Uint16 port) {
	cout << "Starting server at port " << port << endl;

	// trap for CTRL-C
	signal(SIGINT, handlerSIGINT);

	// some auxiliary variables for sending/receiving information
	char buffer[256];
	int result = 0;
	memset(buffer, 0, 256);

	// a variable that represents the address -- in this case only the port
	IPaddress ip;

	// fill in the address in 'ip' -- note that the 2nd parameter is 'nullptr'
	// which means that we want to use 'ip' to start a server
	if (SDLNet_ResolveHost(&ip, nullptr, port) < 0) {
		error();
	}

	// Since the host in 'ip' is 0 (we provided 'nullptr' above), SDLNet_TCP_Open starts
	// a server listening at the port specified in 'ip', and returns a socket for listening
	// to connection requests
	TCPsocket masterSocket = SDLNet_TCP_Open(&ip);
	if (!masterSocket) {
		error();
	}

	shutDownTriggetPort = port;

	cout << "The server is up and kicking ..." << endl;

	// We want to use non-blocking communication, the way to do this is via a socket set.
	// We add sockets to this set and then we can ask if any has some activity without blocking.
	// Non-blocking communication is the adequate one for video games!
	SDLNet_SocketSet socketSet = SDLNet_AllocSocketSet(1000);

	// add the masterSocket to the set
	SDLNet_TCP_AddSocket(socketSet, masterSocket);

	done = false;
	while (!done) {
		// The call to SDLNet_CheckSockets returns the number of sockets with activity
		// in socketSet. The 2nd parameter tells the method to wait up 1 hour
		// if there is no activity -- no need to put it 0 unless we really don't want to
		// block (like when used in a video game). With 0 it would consume CPU
		// unnecessarily.
		if (SDLNet_CheckSockets(socketSet, 60000) > 0) {

			if (done)
				continue;

			// if there is an activity in masterSocket we process it. Note that
			// before calling SDLNet_SocketReady we must have called SDLNet_CheckSockets
			if (SDLNet_SocketReady(masterSocket)) {

				// accept the connection (activity on master socket is always a connection
				// request, sending and receiving data is done via the socket returned by
				// SDLNet_TCP_Accept.
				TCPsocket client = SDLNet_TCP_Accept(masterSocket);

				cout << "A client has connected, waiting for a message ..."
						<< endl;

				// receive a message from the client, we assume it is a c-string of at
				// most 255 chars.
				result = SDLNet_TCP_Recv(client, buffer, 255);
				if (result > 0) { // if no error

					// print the user message
					cout << "Client says: " << buffer << endl; // this is why we used memset before!

					// - send back the 9 chars c-string "Received!", plus one for \0
					// - normally we should check errors afterward sending, but we skip here
					//   for simplicity
					SDLNet_TCP_Send(client, "Received!", 11);
				}

				// close the socket, in a later example we will see how to keep it open, etc
				SDLNet_TCP_Close(client);

			}
		}
	}

	// free the socket set
	SDLNet_FreeSocketSet(socketSet);

	// close the server's master socket
	SDLNet_TCP_Close(masterSocket);

}

void client(const char *host, Uint16 port) {

	// some auxiliary variables for sending/receiving information
	char buffer[256];
	int result = 0;
	memset(buffer, 0, 256);

	// a variable that represents the address of the server we want to connect to
	IPaddress ip;

	// fill in the address in 'ip'
	if (SDLNet_ResolveHost(&ip, host, port) < 0) {
		error();
	}

	// establish the connection with the server
	TCPsocket conn = SDLNet_TCP_Open(&ip);
	if (!conn) {
		error();
	}

	// - ask the user for a message
	cout << "Enter a message: ";
	cin.getline(buffer, 255);

	// - send the message to the server
	// - the +1 for \0 at the end of the c-string

	int size = strlen(buffer) + 1;
	result = SDLNet_TCP_Send(conn, buffer, size);
	if (result != size) {
		error(); // if send returns a number less than the bytes we have sent then something went wrong
	}

	// - wait for the server's response
	// - we assume it is a c-string
	result = SDLNet_TCP_Recv(conn, buffer, 255);
	if (result < 0) {
		error(); // something went wrong
	} else if (result == 0) {
		cout << "The server closed the connection without any message!" << endl;
	} else {
		cout << buffer << endl;
	}

	// close the connection
	SDLNet_TCP_Close(conn);
}

int main(int argc, char **argv) {

	// initialize SDLNet
	if (SDLNet_Init() < 0) {
		error();
	}

	if (argc == 3 && strcmp(argv[1], "server") == 0) {
		server(static_cast<Uint16>(atoi(argv[2]))); // start in server mode
	} else if (argc == 4 && strcmp(argv[1], "client") == 0) {
		client(argv[2], static_cast<Uint16>(atoi(argv[3]))); // start in client mode
	} else {
		cout << "Usage: " << endl;
		cout << "  " << argv[0] << " client host port " << endl;
		cout << "  " << argv[0] << " server port " << endl;
		cout << endl;
		cout << "Example:" << endl;
		cout << "  " << argv[0] << " server 2000" << endl;
		cout << "  " << argv[0] << " client localhost 2000" << endl;
	}

	// finalize SDLNet
	SDLNet_Quit();

	return 0;
}
