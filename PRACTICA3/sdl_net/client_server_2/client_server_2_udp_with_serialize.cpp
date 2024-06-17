// This file is part of the course TPV2@UCM - Samir Genaim

#include <iostream>
#include <SDL_net.h>
#include <csignal>
#include <cassert>

#include "SDLNetUtils.h"

using namespace std;

//struct Message {
//	char op;
//	int a;
//	int b;
//};

enum MsgId : Uint8 {
	_CALC, _RAND
};

struct Message {
	Uint8 id;

	_IMPL_SERIALIAZION_(id)
};

struct CalcMessage: Message {
	char op;
	Uint32 a;
	Uint32 b;

	_IMPL_SERIALIAZION_WITH_BASE_(Message,op,a,b)
};

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
bool done = false;
Uint16 shutDownTriggetPort = 0;

static const int MAX_PACKET_SIZE = 512; // the maximum UDP packet size

// this just sent an empty message the server/client to wake it up.
void shutdown() {

	if (done)
		return;

	cout << endl << "Shutting down  ..." << endl;

	done = true;

	if (shutDownTriggetPort != 0) {
		// just send an empty message
		UDPsocket sock = SDLNet_UDP_Open(0);
		IPaddress address;
		SDLNet_ResolveHost(&address, "localhost", shutDownTriggetPort);
		UDPpacket *p = SDLNet_AllocPacket(0);
		p->len = 0;
		p->address = address;
		SDLNet_UDP_Send(sock, -1, p);
		SDLNet_UDP_Close(sock);
	}
}

/* Signal Handler for SIGINT */
void handlerSIGINT(int) {
	shutdown();
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

	// this is the data sent by the client, buffer and p->data now
	// point to the same address, so we will use buffer for simplicity. The
	// reinterpret_cast is needed since it is not allowed to cast from unsigned
	// to signed chars

	cout << "The server is up and kicking ..." << endl;

	// We want to use non-blocking communication, the way to do this is via a socket set.
	// We add sockets to this set and then we can ask if any has some activity without
	// blocking. Non-blocking communication is the adequate one for video games! Since we
	// are using UDP we have only the server's socket in this set!
	// Ideally we should check for errors, but we skip it for the example ...
	SDLNet_SocketSet socketSet = SDLNet_AllocSocketSet(1);
	SDLNet_UDP_AddSocket(socketSet, sd);

	done = false;

	while (!done) {

		// The call to SDLNet_CheckSockets returns the number of sockets with activity
		// in socketSet. The 2nd parameter tells the method to wait up to 1 hour
		// if there is no activity -- no need to put it 0 unless we really don't want to
		// block (like when used in a video game). With 0 it would consume CPU unnecessarily.
		if (SDLNet_CheckSockets(socketSet, 60000) > 0) {

			// when the server wakes up due to a trigger from method shutdown_server, the
			// value of done is true so we skip the loop body
			//
			if (done)
				continue;

			// if there is an activity in serverUDPSock we process it. Note that
			// before calling SDLNet_SocketReady we must have called SDLNet_CheckSockets
			if (SDLNet_SocketReady(sd)) {

				Message m;
//				m.deserialize(p->data);

				// get the packet - returns -1, 0 or 1
				if (SDLNetUtils::deserializedReceive(m, p, sd) > 0) { // a packet received in p

					//	Message *m = reinterpret_cast<Message*>(p->data);

					if (m.id == _CALC) {
//						CalcMessage *m = reinterpret_cast<CalcMessage*>(p->data);
						CalcMessage m;
						m.deserialize(p->data);

						char op = m.op;
						Sint32 a = m.a;
						Sint32 b = m.b;

						// print the message (we assume it is a c-string, maybe better to check before!)
						cout << "Client (";
						print_ip(p->address.host);
						cout << ":" << sdlnet_ntohs(p->address.port)
								<< ") says -> " << op << " " << a << " " << b
								<< endl;

						Sint32 c = 0;
						switch (op) {
						case '+':
							c = a + b;
							break;
						case '-':
							c = a - b;
							break;
						case '*':
							c = a * b;
							break;
						default:
							break;
						}

						// send a response
						*reinterpret_cast<Sint32*>(p->data) = c;
						p->len = sizeof(Sint32); // the size of data we send goes into the packet
						SDLNet_UDP_Send(sd, -1, p);
					}
					else
					if (m.id == _RAND) {
						// send a response
						*reinterpret_cast<int*>(p->data) = rand();
						p->len = sizeof(Sint32); // the size of data we send goes into the packet
						SDLNet_UDP_Send(sd, -1, p);

					}
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
}

void client(char *host, Uint16 port) {

	// trap for CTRL-C
	signal(SIGINT, handlerSIGINT);

	// pick a socket at random port (locally)
	UDPsocket sd = SDLNet_UDP_Open(0);
	if (!sd) {
		error();
	}

	shutDownTriggetPort = sdlnet_ntohs(SDLNet_UDP_GetPeerAddress(sd, -1)->port);

	// resolve the sever name
	IPaddress srvadd;
	if (SDLNet_ResolveHost(&srvadd, host, port) < 0) {
		error();
	}

	// We allocate the data structure to be used for sending packets
	UDPpacket *p = SDLNet_AllocPacket(MAX_PACKET_SIZE);
	if (!p) {
		error();
	}

	// this is the data sent by the client, buffer and p->data now
	// point to the same address, so we will use buffer for simplicity. The
	// reinterpret_cast is needed since it is not allowed to cast from unsigned
	// to signed chars
	char *buffer = reinterpret_cast<char*>(p->data);
	memset(buffer, 1, 256);

	// socket set for non-blocking communication.
	// we use it because in UDP we might lose packets so we don't
	// want to block for receiving the server's response (since it might never arrive)
	SDLNet_SocketSet socketSet = SDLNet_AllocSocketSet(1);
	SDLNet_UDP_AddSocket(socketSet, sd);

	do {

		int option = 0;
		std::cout << "1. calc" << std::endl;
		std::cout << "2. rand" << std::endl;
		std::cin >> option;

		if (option == 1) {
			char op;
			Sint32 a;
			Sint32 b;

			cout << "Enter op num1 num2: ";
			cin >> op >> a >> b;

			// reinterpret_cast<char*>(p->data) = op;
			// reinterpret_cast<int*>(p->data+1) = a;
			// reinterpret_cast<int*>(p->data+1+sizeof(int)) = b;

//			CalcMessage *m = reinterpret_cast<CalcMessage*>(p->data);
			CalcMessage m;

			m.id = _CALC;
			m.op = op;
			m.a = a;
			m.b = b;
			SDLNetUtils::serializedSend(m, p, sd, srvadd);

		} else if (option == 2) {
			// Message *m = reinterpret_cast<Message*>(p->data);
			Message m;
			m.id = _RAND;
			SDLNetUtils::serializedSend(m, p, sd, srvadd);

		} else {
			done = true;
			continue;
		}

		// wait 3sec for the response
		if (SDLNet_CheckSockets(socketSet, 3000) > 0) {

			// when the client wakes up due to a trigger from method shutdown, the
			// value of done is true so we skip the loop body
			//
			if (done)
				continue;

			if (SDLNet_SocketReady(sd)) {
				while (SDLNet_UDP_Recv(sd, p) > 0) { // a packet received

					// we assume the message is a c-string (a bit dangerous but ...)
					cout << "Server says: "
							<< *reinterpret_cast<Sint32*>(p->data) << endl;
				}
			}
		}

	} while (!done);

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
