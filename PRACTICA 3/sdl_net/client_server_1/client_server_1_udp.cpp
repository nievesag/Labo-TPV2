// This file is part of the course TPV2@UCM - Samir Genaim

#include <iostream>
#include <SDL_net.h>
#include <csignal>
#include <cassert>


using namespace std;



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
	char *buffer = reinterpret_cast<char*>(p->data);

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

				// get the packet - returns -1, 0 or 1
				if (SDLNet_UDP_Recv(sd, p) > 0) { // a packet received in p

					// print the message (we assume it is a c-string, maybe better to check before!)
					cout << "Client (";
					print_ip(p->address.host);
					cout << ":" << sdlnet_ntohs(p->address.port) << ") says -> "
							<< buffer << endl;

					// send a response
					memcpy(buffer, "Received!", 10); // we copy 10 to include the zero at the end
					p->len = 10; // the size of data we send goes into the packet
					SDLNet_UDP_Send(sd, -1, p);
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

	cout << "Enter a message: ";
	cin.getline(buffer, 255);

	// the message is already filled in p->data, but cin.getline above.
	// so we just store the length of the message and the address (the address
	// could be done only once before the loop since we communicate with only one
	// server -- but it is a bit risk since in UDP we might receive  unexpected
	// messages as well!
	//
	// the one we add to the length is for the \0 char at the end of the c-string
	//
	p->len = static_cast<int>(strlen(buffer)) + 1;
	p->address = srvadd;

	// send the message, -1 means we don't want to use channels
	SDLNet_UDP_Send(sd, -1, p);

	// wait 3sec for the response
	if (SDLNet_CheckSockets(socketSet, 3000) > 0) {

		// when the client wakes up due to a trigger from method shutdown, the
		// value of done is true so we skip the loop body
		//

		if (SDLNet_SocketReady(sd)) {
			while (SDLNet_UDP_Recv(sd, p) > 0) { // a packet received

				// we assume the message is a c-string (a bit dangerous but ...)
				cout << "Server says: " << buffer << endl;
			}
		}
	} else {
		cout << "The server did not reply within 3sec, I give up!" << endl;

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
