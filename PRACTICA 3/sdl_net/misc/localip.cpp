// This file is part of the course TPV2@UCM - Samir Genaim

#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <iostream>

int main() {
	char host[256];
	char *IP;
	struct hostent *host_entry;
	gethostname(host, sizeof(host));
	host_entry = gethostbyname(host);
	std::cout << "Hostname: " << host << std::endl;
	for (int i = 0; i < host_entry->h_length; i++) {
		IP = inet_ntoa(*((struct in_addr*) host_entry->h_addr_list[i]));
		std::cout << "IP: " << IP << std::endl;
	}
}
