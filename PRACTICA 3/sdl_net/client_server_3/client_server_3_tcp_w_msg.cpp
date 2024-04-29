#include <iostream>
#include <SDL_net.h>

#include "netwrok_messages_tcp.h"

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
	SDLNetUtils::buff_t result;
	Msg m0;
	MsgWithId m1;

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
					cout << "Client from address ";
					SDLNetUtils::print_ip(*SDLNetUtils::getSocketIP(client));
					cout << " connected, assigned id " << j << endl;
					clients[j] = client;
					SDLNet_TCP_AddSocket(socketSet, client);

					// tells the client its id (message type M0)
					m1._type = _CONNECTION_ACCEPTED_;
					m1._id = j;
					SDLNetUtils::serializedSend(m1, client);

					// tell all clients to broadcast state (message type M2)
					m0._type = _BROADCAST_STATE_;
					for (int i = 0; i < MAX_CLIENTS; i++) {
						if (clients[i] != nullptr)
							SDLNetUtils::serializedSend(m0, clients[i]);
					}
				} else {
					// refuse connection (message type M1)
					m0._type = _CONNECTION_REJECTED_;
					SDLNetUtils::serializedSend(m0, client);
					SDLNet_TCP_Close(client);
				}
			}

			// check clients activity
			for (int i = 0; i < MAX_CLIENTS; i++) {
				if (clients[i] != nullptr && SDLNet_SocketReady(clients[i])) {
					result = SDLNetUtils::receiveMsg(clients[i]);

					// if result is zero, then the client has closed the connection
					// and if smaller than zero, then there was some error. In both
					// cases we close the connection
					if (result.size <= 0) {
						cout << "Client " << i << " disconnected! " << endl;
						SDLNet_TCP_Close(clients[i]);
						SDLNet_TCP_DelSocket(socketSet, clients[i]);
						clients[i] = nullptr;

						// tell all clients that 'i'disconnected (message type M3)
						m1._type = _DISCONNECTED_;
						m1._id = i;
						for (int j = 0; j < MAX_CLIENTS; j++) {
							if (clients[j] != nullptr)
								SDLNetUtils::serializedSend(m1, clients[j]);
						}
					} else {
						for (int j = 0; j < MAX_CLIENTS; j++) {
							if (i != j && clients[j] != nullptr)
								SDLNetUtils::send(result.buffer, clients[j],
										result.size);
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
	for (int i = 0; i < MAX_CLIENTS; i++)
		states[i] = -1;

	// some auxiliary variables for sending/receiving information
	SDLNetUtils::buff_t result;

	Msg m0;
	MsgWithId m1;
	StateInfo m2;

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
	result = SDLNetUtils::receiveMsg(conn);
	if (result.size <= 0) {
		error(); // something went wrong
	} else {
		m0.deserialize(result.buffer);
		if (m0._type == _CONNECTION_ACCEPTED_) { // M0
			m1.deserialize(result.buffer);
			id = m1._id; // copy the identifier to id
		} else {
			cout << "Connection refused!" << endl;
		}
	}

	// socket set for non-blocking communication
	SDLNet_SocketSet socketSet = SDLNet_AllocSocketSet(1);
	SDLNet_TCP_AddSocket(socketSet, conn);

	// choose initial state of the client
	states[id] = rand() % 1000;
	bool stateChanegd = true;

	while (true) {
		if (SDLNet_CheckSockets(socketSet, 0) > 0) {
			if (SDLNet_SocketReady(conn)) {

				// read the first byte of the message, to know
				// of which type it is
				result = SDLNetUtils::receiveMsg(conn);
				if (result.buffer == nullptr)
					break;
				m0.deserialize(result.buffer);
				switch (m0._type) {
				case _BROADCAST_STATE_: // M2 - we need to broadcast state
					// broadcast state (message type M4)
					m2._type = _STATE_INFO_;
					m2._id = id;
					m2._state = states[id];
					SDLNetUtils::serializedSend(m2, conn);
					break;
				case _DISCONNECTED_: // M3 - a client has disconnected
					m1.deserialize(result.buffer);
					states[m1._id] = -1;
					stateChanegd = true;
					break;
				case _STATE_INFO_: // M4 - a client has sent a new state
					m2.deserialize(result.buffer);
					states[m2._id] = m2._state;
					stateChanegd = true;
					break;
				default:
					cout << "Unknown message type: " << (int) m0._type << endl;
				}

			}
		}

		// change state every now and then
		if (rand() % 10000000 < 10) {

			// change state
			states[id] = rand() % 1000;
			stateChanegd = true;

			// broadcast state (message type M4)
			m2._type = _STATE_INFO_;
			m2._id = id;
			m2._state = states[id];
			SDLNetUtils::serializedSend(m2, conn);
		}

		if (stateChanegd) {
			system("clear"); // system("CLS");
			cout << "id=" << id << endl;
			for (int i = 0; i < MAX_CLIENTS; i++) {
				if (states[i] != -1) {
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
