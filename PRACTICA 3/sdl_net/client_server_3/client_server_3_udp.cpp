// This file is part of the course TPV2@UCM - Samir Genaim

#include <iostream>
#include <SDL_net.h>
#include <csignal>
#include <cassert>
#include <initializer_list>
#include "netwrok_messages.h"

using namespace std;

inline void clear_screen() {
#if defined(_WIN32) || defined(_WIN64) // Windows
	system("cls");
#else
	system("clear");
#endif

}

void error() {
	cout << "Error: " << SDLNet_GetError() << endl;
	exit(1);
}

// print an IPv4
inline void print_ip(Uint32 ip, bool newline = false) {
	std::cout << (ip & 0xFF) << "." //
			<< ((ip >> 8) & 0xFF) << "." //
			<< ((ip >> 16) & 0xFF) << "." //
			<< ((ip >> 24) & 0xFF);
	if (newline)
		std::cout << std::endl;
}

// these are the classical functions for converting
// host to network integers and vice versa. Apparently
// there is no common library that works on all OS. They
// basically use SDLNet_Write and SDLNet_Read but have an
// API similar to the original ones.

inline Uint16 sdlnet_htons(Uint16 v) {
	Uint16 nv;
	SDLNet_Write16(v, &nv);
	return nv;
}

inline Uint16 sdlnet_ntohs(Uint16 nv) {
	return SDLNet_Read16(&nv);
}

// the port on which the server/client is listening, to be used by method
// shutdown to send an empty message to wake up the server/client if blocking
volatile bool done = false;
Uint16 shutDownTriggetPort = 0;

static const int MAX_PACKET_SIZE = 512; // the maximum UDP packet size
const std::size_t MAX_CLIENT = 10;

// this just sent an empty message the server/client to wake it up.
void shutdown() {

	if (done)
		return;

	cout << endl << "Shutting down  ..." << endl;

	done = true;

	if (shutDownTriggetPort != 0) {
		// just send an empty message
		UDPsocket sock = SDLNet_UDP_Open(0);
		IPaddress addr;
		SDLNet_ResolveHost(&addr, "localhost", shutDownTriggetPort);
		UDPpacket *p = SDLNet_AllocPacket(MAX_PACKET_SIZE);
		NetworkMessage *m = reinterpret_cast<NetworkMessage*>(p->data);
		m->_type = _NONE_;
		p->len = sizeof(NetworkMessage);
		p->address = addr;
		SDLNet_UDP_Send(sock, -1, p);
		SDLNet_UDP_Close(sock);
		SDLNet_FreePacket(p);
	}

}

/* Signal Handler for SIGINT */
void handlerSIGINT(int) {
	shutdown();
}

// find a server in the local network, by broadcasting a message of type _FIND_SERVER_
// and wait for a message of type _I_AM_THE_SERVER_
//
bool find_server(Uint16 minPort, Uint16 maxPort, IPaddress &hostAddress) {

	assert(minPort > 0);

	// broadcast address
	IPaddress addr;
	SDLNet_ResolveHost(&addr, "255.255.255.255", maxPort);

	// open a socket and send a _FIND_SERVER_ message
	UDPsocket sd = SDLNet_UDP_Open(0);
	UDPpacket *p = SDLNet_AllocPacket(MAX_PACKET_SIZE);
	NetworkMessage *m = reinterpret_cast<NetworkMessage*>(p->data);

	SDLNet_SocketSet socketSet = SDLNet_AllocSocketSet(1);
	SDLNet_UDP_AddSocket(socketSet, sd);

	// broadcast a message to all devices in local network!
	m->_type = _FIND_SERVER_;
	p->len = sizeof(NetworkMessage);

	// we send messages to all ports between maxPort and minPort
	bool found = false;
	Uint16 port = maxPort;
	while (!found && port >= minPort) {
		p->address.port = sdlnet_htons(port);
		SDLNet_UDP_Send(sd, -1, p);

		// check if we get a response from server in each iteration so to stop the search
		if (SDLNet_UDP_Recv(sd, p) > 0) {
			if (m->_type == _I_AM_THE_SERVER_) {
				hostAddress = p->address;
				found = true;
			}
		}
		port--;
	}

	// we wait 3sec more to a message from the server, just in case it received our message
	// in the last iterations of the loop above;
	if (!found && SDLNet_CheckSockets(socketSet, 3000)) {
		if (SDLNet_SocketReady(sd)) {
			if (SDLNet_UDP_Recv(sd, p) > 0) {
				if (m->_type == _I_AM_THE_SERVER_) {
					hostAddress = p->address;
					found = true;
				}
			}
		}
	}

	// free the packet!
	SDLNet_FreePacket(p);

	// free the socket set
	SDLNet_FreeSocketSet(socketSet);

	// close the server socket
	SDLNet_UDP_Close(sd);

	return found;

}

void server(Uint16 port) {

	// trap for CTRL-C
	signal(SIGINT, handlerSIGINT);

	// open a UDP socket to listen at 'port' -- unlike TCP, this socket is used to
	// receive information from all clients, there is no connection in UDP!.
	UDPsocket sd = SDLNet_UDP_Open(port);
	if (!sd) {
		error();
	}

	shutDownTriggetPort = port;

	// We allocate the data structure to be used for sending/receiving packets. We could
	// use an array of packets as well (and use the corresponding sendV/recvV).
	UDPpacket *p = SDLNet_AllocPacket(MAX_PACKET_SIZE);
	if (!p) {
		error();
	}

	// since we are going to send _only_ message of type NetworkMessage, we
	// can just make aliasing between such a message and p->data, this will
	// make it easy to process the received data. Otherwise, if we define m as
	//
	//    NetworkMessage m
	//
	// to send 'm' we will have to copy the content to p->data using
	//
	//    memcpy(p->data,&m,sizeof(NetworkMessage)
	//
	// and to receive m we have to copy the context from p->data using
	//
	//    memcpy(&m,p->data,sizeof(NetworkMessage)
	//
	NetworkMessage *m = reinterpret_cast<NetworkMessage*>(p->data);

	cout << "The server is up and kicking ..." << endl;

	// We want to use non-blocking communication, the way to do this is via a socket set.
	// We add sockets to this set and then we can ask if any has some activity without
	// blocking. Non-blocking communication is the adequate one for video games! Since we
	// are using UDP we have only the server's socket in this set!
	// Ideally we should check for errors, but we skip it for the example ...
	SDLNet_SocketSet socketSet = SDLNet_AllocSocketSet(1);
	SDLNet_UDP_AddSocket(socketSet, sd);

	// array of clients. Each cell indicates if it is connected, and if it also has the
	// address of the client (we could eliminate 'connected' and use the address 0:0 to
	// indicated if the cell is occupied.
	struct ClientInfo {
		bool connected;
		IPaddress address;
	};
	ClientInfo clients[MAX_CLIENT] = { };

	done = false;
	while (!done) {

		// The call to SDLNet_CheckSockets returns the number of sockets with activity
		// in socketSet. The 2nd parameter tells the method to wait up to 1 hour
		// if there is no activity -- no need to put it 0 unless we really don't want to
		// block (like when used in a video game). With 0 it would consume CPU unnecessarily.
		if (SDLNet_CheckSockets(socketSet, SDL_MAX_UINT32) > 0) {

			// when the server wakes up due to a trigger from method shutdown_server, the
			// value of done is true so we skip the loop body
			//
			if (done)
				continue;

			// if there is an activity in serverUDPSock we process it. Note that
			// before calling SDLNet_SocketReady we must have called SDLNet_CheckSockets
			if (SDLNet_SocketReady(sd)) {

				// get the packet - returns -1, 0 or 1
				if (SDLNet_UDP_Recv(sd, p) > 0) { // a packet received in p

					// now we have a switch depending on the kind of message
					switch (m->_type) {

					// the message is a connection request
					case _CONNECTION_REQUEST_: {

						// seek a free slot
						Uint8 j = 0;
						while (j < MAX_CLIENT && clients[j].connected)
							j++;

						// if a free slot is found we will have j < MAX_CLIENT
						if (j < MAX_CLIENT) {

							// make the slot connected, and store the client's address
							clients[j].connected = true;
							clients[j].address = p->address;

							// send a message to the client indicating that the connection was accepted
							m->_id = j;
							m->_type = _CONNECTION_ACCEPTED_;
							SDLNet_UDP_Send(sd, -1, p);

							// send a message to all clients asking them to broadcast their states, since
							// we have a new client --- the new client will also receive this message
							m->_type = _BROADCAST_STATE_;
							for (auto i = 0u; i < MAX_CLIENT; i++) {
								if (clients[i].connected) {
									p->address = clients[i].address;
									SDLNet_UDP_Send(sd, -1, p);
								}
							}
						} else {

							// if not free slots, send a message to the client rejecting the connection
							m->_type = _CONNECTION_REJECTED_;
							SDLNet_UDP_Send(sd, -1, p);
						}
						break;
					}

						// the message is a client informing about disconnection
					case _DISCONNECTED_: {

						// mark the corresponding slot as free.
						auto senderId = m->_id;
						clients[senderId].connected = false;

						// tell all clients, except the sender, that the client with 'senderId' has
						// has disconnected, this is simply done by forwarding the message
						for (auto i = 0u; i < MAX_CLIENT; i++) {
							if (i != senderId && clients[i].connected) {
								p->address = clients[i].address;
								SDLNet_UDP_Send(sd, -1, p);
							}
						}
						break;
					}

						// the message is a client informing about state
					case _STATE_INFO_: {
						// forward the message to all clients except the one with id 'senderId'
						auto senderId = m->_id;
						for (auto i = 0u; i < MAX_CLIENT; i++) {
							if (i != senderId && clients[i].connected) {
								p->address = clients[i].address;
								SDLNet_UDP_Send(sd, -1, p);
							}
						}
						break;
					}

					case _FIND_SERVER_: {
						m->_type = _I_AM_THE_SERVER_;
						SDLNet_UDP_Send(sd, -1, p);
						break;
					}
					default:
						break;
					}

				}
			}
		}

	}

	// when the server shuts down, we tell all clients to shutdown
	m->_type = _SHUTDOWN_;
	for (auto i = 0u; i < MAX_CLIENT; i++) {
		if (clients[i].connected) {
			p->address = clients[i].address;
			SDLNet_UDP_Send(sd, -1, p);
		}
	}

	// free the packet!
	SDLNet_FreePacket(p);

	// free the socket set
	SDLNet_FreeSocketSet(socketSet);

	// close the server socket
	SDLNet_UDP_Close(sd);
}

void client(char *host, Uint16 port) {

	// trap for CTRL-C
	signal(SIGINT, handlerSIGINT);

	// resolve the sever name
	IPaddress srvadd;
	if (host == nullptr) { // if host name is null we try to find the server in a local network
		Uint16 maxPort;
		Uint16 minPort;
		if (port == 0) {
			maxPort = 49151;
			minPort = 1024;
		} else {
			maxPort = port;
			minPort = port;
		}
		if (!find_server(minPort, maxPort, srvadd)) {
			if (srvadd.host == 0) {
				std::cout << "Couldn't find the server!" << std::endl;
				return;
			}
		}
	} else if (SDLNet_ResolveHost(&srvadd, host, port) < 0) {
		error();
	}

	// pick a socket at random port (locally)
	UDPsocket sd = SDLNet_UDP_Open(0);
	if (!sd) {
		error();
	}

	// this is the port of the socket, so we can use it in shutdown
	shutDownTriggetPort = sdlnet_ntohs(SDLNet_UDP_GetPeerAddress(sd, -1)->port);

	// We allocate the data structure to be used for sending packets
	UDPpacket *p = SDLNet_AllocPacket(MAX_PACKET_SIZE);
	if (!p) {
		error();
	}

	// since we are going to send _only_ message of type NetworkMessage, we
	// can just make aliasing between such a message and p->data, this will
	// make it easy to process the received data. Otherwise, if we define m as
	//
	//    NetworkMessage m
	//
	// to send 'm' we will have to copy the content to p->data using
	//
	//    memcpy(p->data,&m,sizeof(NetworkMessage)
	//
	// and to receive m we have to copy the context from p->data using
	//
	//    memcpy(&m,p->data,sizeof(NetworkMessage)
	//
	NetworkMessage *m = reinterpret_cast<NetworkMessage*>(p->data);

	// socket set for non-blocking communication.
	// we use it because in UDP we might lose packets so we don't
	// want to block for receiving the server's response (since it might never arrive)
	SDLNet_SocketSet socketSet = SDLNet_AllocSocketSet(1);
	SDLNet_UDP_AddSocket(socketSet, sd);

	// array that includes the states of all client. If client[i]=0, then no
	// client with id i is connected
	Uint32 states[MAX_CLIENT] = { };
	Uint8 myId; // the local use id

	// request to connect
	m->_type = _CONNECTION_REQUEST_;
	p->len = sizeof(NetworkMessage);
	p->address = srvadd;
	SDLNet_UDP_Send(sd, -1, p);

	// wait 3sec for the response, if we don't get we stop (since done will be 'true')
	done = true;
	if (SDLNet_CheckSockets(socketSet, 3000) > 0) {
		if (SDLNet_SocketReady(sd)) {
			if (SDLNet_UDP_Recv(sd, p) > 0) {
				switch (m->_type) {
				case _CONNECTION_ACCEPTED_:
					myId = m->_id;
					states[myId] = 1 + (rand() % 100);
					p->address = srvadd;
					m->_type = _STATE_INFO_;
					SDLNet_UDP_Send(sd, -1, p);
					done = false;
					break;
				case _CONNECTION_REJECTED_:
					std::cout << "Connection rejected" << std::endl;
					break;
				}
			}
		}
	}

	// aux variable so we print the state only when there is change
	bool changed = true;

	// aux variable to force an interval of at least 5sec between state change
	Uint32 refreshRate = 1000;
	Uint32 lastUpdate = 0;

	while (!done) {
		// wait 3sec for the response
		if (SDLNet_CheckSockets(socketSet, refreshRate) > 0) {
			if (SDLNet_SocketReady(sd)) {
				if (SDLNet_UDP_Recv(sd, p) > 0) {
					switch (m->_type) {

					// a client sending a state
					case _STATE_INFO_:
						states[m->_id] = m->_state;
						changed = true;
						break;

						// the server is asking to broadcast the state
					case _BROADCAST_STATE_:
						p->address = srvadd;
						m->_type = _STATE_INFO_;
						m->_id = myId;
						m->_state = states[myId];
						SDLNet_UDP_Send(sd, -1, p);
						break;

						// a client informing about disconnection
					case _DISCONNECTED_:
						states[m->_id] = 0;
						changed = true;
						break;

						// server asking to shutdown
					case _SHUTDOWN_:
						done = true;
					}
				}
			}
		}

		// change the state every now and then, and send it to everyone
		if (!done && SDL_GetTicks() - lastUpdate > refreshRate
				&& rand() % 10 < 5) {
			states[myId] = 1 + (rand() % 100);
			p->address = srvadd;
			m->_type = _STATE_INFO_;
			m->_id = myId;
			m->_state = states[myId];
			SDLNet_UDP_Send(sd, -1, p);
			changed = true;
			lastUpdate = SDL_GetTicks();
		}

		// if any of the states where changed, print them all again
		if (!done && changed) {
			clear_screen();
			cout << "id=" << static_cast<int>(myId) << endl;
			for (auto i = 0u; i < MAX_CLIENT; i++) {
				if (states[i] != 0) {
					cout << i << ":" << states[i] << " ";
				}
			}
			cout << endl;
			changed = false;
		}

	}

	// if the client did not exit due to a shutdown message, we inform
	// everyone that the client has disconnected
	if (m->_type != _SHUTDOWN_) {
		p->address = srvadd;
		m->_type = _DISCONNECTED_;
		m->_id = myId;
		SDLNet_UDP_Send(sd, -1, p);
	}

	// free the packet!
	SDLNet_FreePacket(p);

	// free the socket set
	SDLNet_FreeSocketSet(socketSet);

	// close the client socket
	SDLNet_UDP_Close(sd);
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
	} else if (argc == 3 && strcmp(argv[1], "client") == 0) {
		client(nullptr, static_cast<Uint16>(atoi(argv[2]))); // start in client mode
	} else if (argc == 2 && strcmp(argv[1], "client") == 0) {
		client(nullptr, 0); // start in client mode
	} else {
		cout << "Usage: " << endl;
		cout << "  " << argv[0] << " server port " << endl;
		cout << "  " << argv[0] << " client host port " << endl;
		cout << "  " << argv[0] << " client port " << endl;
		cout << "  " << argv[0] << " client  " << endl;
		cout << endl;
		cout << "Example:" << endl;
		cout << "  " << argv[0] << " server 2000" << endl;
		cout << "  " << argv[0] << " client localhost 2000" << endl;
	}

	// finalize SDLNet
	SDLNet_Quit();

	return 0;
}
