// This file is part of the course TPV2@UCM - Samir Genaim

#include <iostream>
#include <SDL2_net/SDL_net.h>

using namespace std;

void error() {
	cout << "Error: " << SDLNet_GetError() << endl;
	exit(1);
}

void client(char* host, int port) {

	// some auxiliary variables for sending/receiving information
	char buffer[256];
	int result = 0;

	// a variable that represents the address of the server we want to connect to
	IPaddress ip;

	// fill in the address in 'ip'
	if (SDLNet_ResolveHost(&ip, host, port) < 0) {
		error();
	}

	// establish the connection with the WEB server
	TCPsocket conn = SDLNet_TCP_Open(&ip);
	if (!conn) {
		error();
	}

	// request the main page from the WEB server
	result = SDLNet_TCP_Send(conn, "GET /\n\n", 7);
	if (result != 7) {
		error(); // if Send returns a number less than the bytes we have sent then something went wrong
	}

	// now we receive the server's response
	while (true) {

		// receive at most 255 chars into buffer (the call is blocking).
		// We first put the buffer to 0, we need this to identify the end
		// of the received message (sine we assume it is a string)
		bzero(buffer, 256);
		result = SDLNet_TCP_Recv(conn, buffer, 255);
		if (result < 0) {
			error(); // something went wrong
		} else if (result == 0) {
			break; // the server closed the connection, so we exit the loop
		} else {
			cout << buffer;
		}

	}

	// close the connection
	SDLNet_TCP_Close(conn);
}

int main(int argc, char** argv) {

	// initialize SDLNet
	if (SDLNet_Init() < 0) {
		error();
	}

	// we should receive two arguments in the command-line, i.e., 'argc' is 3
	// because we get the program name in argv[0] and then the parameters, etc.
	if ( argc == 3 ) {
		client(argv[1], atoi(argv[2])); // atoi converts a sting to an integer
	} else {
		cout << "Usage: " << endl;
		cout << "  " << argv[0] << " host port " << endl;
		cout << endl;
		cout << "Example:" << endl;
		cout << "  " << argv[0] << " www.ucm.es 80" << endl;
	}

	// finalize SDLNet
	SDLNet_Quit();

	return 0;
}
