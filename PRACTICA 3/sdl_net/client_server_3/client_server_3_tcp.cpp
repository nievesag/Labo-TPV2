#include <iostream>
#include <SDL_net.h>

using namespace std;

/*
 * Kinds of messages:
 *
 * M0: S->C : 0 int      // Connected
 * M1: S->C : 1          // Not possible to connect
 * M2: S->C : 2          // Please broadcast state
 * M3: S->C : 3 int      // Client disconnected
 * M4: C->C : 4 int int  // State broadcast
 *
 */

const int MAX_CLIENTS = 10;

void error() {
	cout << "Error: " << SDLNet_GetError() << endl;
	exit(1);
}

void server(int port) {
	cout << "Starting server at port " << port << endl;
	// some auxiliary variables for sending/receiving information
	char buffer[256];
	int result = 0;

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

	// We want to use non-blocking communication, the way to do this is via a socket set.
	// We add sockets to this set and then we can ask if any has some activity without blocking.
	// Non-blocking communication is the adequate one for video games!
	SDLNet_SocketSet socketSet = SDLNet_AllocSocketSet(1000);

	// add the masterSocket to the set
	SDLNet_TCP_AddSocket(socketSet, masterSocket);

	// an array for clients
	TCPsocket clients[MAX_CLIENTS];
	for (int i = 0; i < MAX_CLIENTS; i++) {
		clients[i] = nullptr;
	}

	while (true) {
		// The call to SDLNet_CheckSockets returns the number of sockets with activity
		// in socketSet. The 2nd parameter tells the method to wait up to SDL_MAX_UINT32
		// if there is no activity -- no need to put it 0 unless we really don't want to
		// block. With 0 it would consume CPU unnecessarily
		if (SDLNet_CheckSockets(socketSet, SDL_MAX_UINT32) > 0) {

			// if there is an activity in masterSocket we process it. Note that
			// before calling SDLNet_SocketReady we must have called SDLNet_CheckSockets
			if (SDLNet_SocketReady(masterSocket)) {

				// accept the connection (activity on master socket is always a connection
				// request, sending and receiving data is done via the socket returned by
				// SDLNet_TCP_Accept. This way we can serve several clients.
				TCPsocket client = SDLNet_TCP_Accept(masterSocket);

				// look for a free slot
				int j = 0;
				while (j < MAX_CLIENTS && clients[j] != nullptr)
					j++;

				// if there is a slot, add the client to the socketSet and send a connected message,
				// other say we are fully booked and close the connection
				if (j < MAX_CLIENTS) {
					cout << "Client connected, assigned id " << j << endl;
					clients[j] = client;
					SDLNet_TCP_AddSocket(socketSet, client);

					// tells the client its id (message type M0)
					buffer[0] = 0;
					*((int*) (buffer + 1)) = j;
					SDLNet_TCP_Send(client, buffer, 1 + sizeof(int));

					// tell all clients to broadcast state (message type M2)
					buffer[0] = 2;
					for (int i = 0; i < MAX_CLIENTS; i++) {
						if (clients[i] != nullptr)
							SDLNet_TCP_Send(clients[i], buffer, 1);
					}
				} else {
					// refuse connection (message type M1)
					buffer[0] = 1;
					SDLNet_TCP_Send(client, buffer, 1);
					SDLNet_TCP_Close(client);
				}
			}

			// check clients activity
			for (int i = 0; i < MAX_CLIENTS; i++) {
				if (clients[i] != nullptr && SDLNet_SocketReady(clients[i])) {
					result = SDLNet_TCP_Recv(clients[i], buffer, 255);

					// if result is zero, then the client has closed the connection
					// and if smaller than zero, then there was some error. In both
					// cases we close the connection
					if (result <= 0) {
						cout << "Client " << i << " disconnected! " << endl;
						SDLNet_TCP_Close(clients[i]);
						SDLNet_TCP_DelSocket(socketSet, clients[i]);
						clients[i] = nullptr;

						// tell all clients that 'i'disconnected (message type M3)
						buffer[0] = 3;
						*((int*) (buffer + 1)) = i;
						for (int j = 0; j < MAX_CLIENTS; j++) {
							if (clients[j] != nullptr)
								SDLNet_TCP_Send(clients[j], buffer,
										1 + sizeof(int));
						}
					} else {
						for (int j = 0; j < MAX_CLIENTS; j++) {
							if (i != j && clients[j] != nullptr)
								SDLNet_TCP_Send(clients[j], buffer, result);
						}
					}
				}
			}

		}
	}

}

void client(char *host, int port) {

	srand(time(NULL));

	int id = -1;
	int states[MAX_CLIENTS];
	for(int i=0; i<MAX_CLIENTS; i++)
		states[i] = -1;

	// some auxiliary variables for sending/receiving information
	char buffer[256];
	int result = 0;

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

	// Read the first byte of the server's message
	result = SDLNet_TCP_Recv(conn, buffer, 1);
	if (result < 0) {
		error(); // something went wrong
	} else if (result == 1 && buffer[0] == 0) { // M0
		SDLNet_TCP_Recv(conn, buffer, sizeof(int)); // read the identifier
		id = *((int*)buffer); // copy the identifier to id
	} else {
		cout << "Connection refused!" << endl;
	}

	// socket set for non-blocking communication
	SDLNet_SocketSet socketSet = SDLNet_AllocSocketSet(1);
	SDLNet_TCP_AddSocket(socketSet, conn);

	// choose initial state of the client
	states[id] = rand() % 1000;
	bool stateChanegd = true;


	while (true) {
		if (SDLNet_CheckSockets(socketSet, 0) > 0 ) {
			if ( SDLNet_SocketReady(conn)) {

				// read the first byte of the message, to know
				// of which type it is
				if ( SDLNet_TCP_Recv(conn, buffer, 1) <= 0)
					break;

				switch ( buffer[0] ) {
				case 2: // M2 - we need to broadcast state
					// broadcast state (message type M4)
					buffer[0] = 4;
					*((int*)(buffer+1)) = id;
					*((int*)(buffer+1+sizeof(int))) = states[id];
					SDLNet_TCP_Send(conn, buffer, 1+2*sizeof(int));
					break;
				case 3: // M3 - a client has disconnected
					SDLNet_TCP_Recv(conn, buffer, sizeof(int));
					states[*((int*) buffer)] = -1;
					stateChanegd = true;
					break;
				case 4: // M4 - a client has sent a new state
					SDLNet_TCP_Recv(conn, buffer, 2*sizeof(int));
					states[*((int*) buffer)] = *((int*) (buffer+sizeof(int)));
					stateChanegd = true;
					break;
				default:
					cout << "Unknown message type: " << (int)buffer[0] << endl;
				}

			}
		}


		// change state every now and then
		if ( rand() % 1000000 < 10 ) {

			// change state
			states[id] = rand() % 1000;
			stateChanegd = true;

			// broadcast state (message type M4)
			buffer[0] = 4;
			*((int*)(buffer+1)) = id;
			*((int*)(buffer+1+sizeof(int))) = states[id];
			SDLNet_TCP_Send(conn, buffer, 1+2*sizeof(int));
		}

		if ( stateChanegd ) {
			system("clear"); // system("CLS");
			cout << "id=" << id << endl;
			for(int i=0; i<MAX_CLIENTS; i++) {
				if ( states[i] != -1 ) {
					cout << i << ":" << states[i] << " ";
				}
			}
			cout << endl;
			stateChanegd = false;
		}
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
		server(atoi(argv[2])); // start in server mode
	} else if (argc == 4 && strcmp(argv[1], "client") == 0) {
		client(argv[2], atoi(argv[3])); // start in client mode
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
