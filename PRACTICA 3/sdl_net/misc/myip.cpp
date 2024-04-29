// This file is part of the course TPV2@UCM - Samir Genaim

#include <iostream>
#include <vector>
#include <sstream>
#include <string>

#include <SDL_net.h>

bool isNumber(const std::string &s) {
	return !s.empty() && std::all_of(s.begin(), s.end(), ::isdigit);
}

bool isValidIP(const std::string &ip) {
	int c = 0;
	auto i = 0;

	auto pos = ip.find(".");
	while (pos != std::string::npos) {
		auto num = ip.substr(i, pos - i);
		c++;

		if (c > 4 || !isNumber(num) || stoi(num) > 255 || stoi(num) < 0)
			return false;

		i = ++pos;
		pos = ip.find(".", pos);
	}

	return true;
}

void error() {
	std::cout << "Error: " << SDLNet_GetError() << std::endl;
	exit(1);
}

std::string myip() {
	const char *host = "ipinfo.io";
	int port = 80;

	const int bufferSize = 256;
	char buffer[bufferSize];
	int result = 0;

	IPaddress ip;
	if (SDLNet_ResolveHost(&ip, host, port) < 0) {
		error();
	}

	TCPsocket conn = SDLNet_TCP_Open(&ip);
	if (!conn) {
		error();
	}

	const char *s = "GET /ip HTTP/1.0\nHost: ipinfo.io\n\n";
	result = SDLNet_TCP_Send(conn, s, std::strlen(s));
	if (result != std::strlen(s)) {
		error();
	}

	// get the response, we assume it is at most bufferSize-1 chars
	int read = 0;
	bzero(buffer, bufferSize);
	while (read < 255) {
		result = SDLNet_TCP_Recv(conn, buffer + read, bufferSize - 1);
		if (result < 0) {
			error();
		} else if (result == 0) {
			break;
		} else {
			read = read + result;
		}
	}

	SDLNet_TCP_Close(conn);

	// The IP is the last token of the response
	std::vector<std::string> tokens;
	std::stringstream iss(buffer);
	for (std::string s; iss >> s;) {
		tokens.push_back(s);
	}

	if (isValidIP(tokens.back()))
		return tokens.back();
	else
		throw "Failed! The response was: \n\n" + std::string(buffer);
}

int main(int argc, char **argv) {

	// initialize SDLNet
	if (SDLNet_Init() < 0) {
		error();
	}

	try {
		auto ip = myip();
		std::cout << "You IP is: " << ip << std::endl;
	} catch (std::string &e) {
		std::cerr << e << std::endl;
	}

	// finalize SDLNet
	SDLNet_Quit();

	return 0;
}
