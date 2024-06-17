// This file is part of the course TPV2@UCM - Samir Genaim

#include <iostream>
#include <string>
#include <regex>

using namespace std;

string exec(const char *command) {
	char buffer[128];
	string result = "";

	// Open pipe to file
	FILE *pipe = popen(command, "r");
	if (!pipe) {
		return "popen failed!";
	}

	// read till end of process:
	while (!feof(pipe)) {

		// use buffer to read and add to result
		if (fgets(buffer, 128, pipe) != NULL)
			result += buffer;
	}

	pclose(pipe);
	return result;
}

string getIP(std::string adapter) {

	// example string for testing
	string str(
			" bla bla \n bla bla Hamachi bla bla IPv4 bla bla   : 3.0.33.44 bla bla \n bla bla");

//	string str(exec("ifconfig")); 

	regex rexpr(
			".*" + adapter
					+ ".*IPv4.*:[ ]*([0-9]+)\\.([0-9]+)\\.([0-9]+)\\.([0-9]+).*",
			regex::extended);

	smatch sm;
	regex_match(str.cbegin(), str.cend(), sm, rexpr);

	string ip = "";
	if (sm.size() == 5) {
		ip = string(sm[1]) + "." + string(sm[2]) + "." + string(sm[3]) + "."
				+ string(sm[4]);
	}

	return ip;
}

int main() {
	auto ip = getIP("Hamachi");
	cout << "IP=" << ip << endl;
}
