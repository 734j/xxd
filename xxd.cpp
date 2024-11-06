#include <iostream>
#include <vector>
#include <fstream>
#include <filesystem>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <iomanip>
#include "xxd.hpp"

/*
  This is mainly a test to see if i can implement 'xxd'
  I will try to eventually make a real 'xxd' implementation
  but since i know very little about this domain of knowledge
  i will most likely spend a lot of time playing around with this.
 */

char *g_argv = nullptr;

std::string byte2hex_octets_per_line(std::ifstream &bytestream, int octets, std::string &offset) {

	if(octets > 256 || octets < 1) {
		return "";
	}
	
	const char* hex_characters = "0123456789abcdef";
	std::string format;
	std::string octetstr(2, '0');
	char byte;
	auto off = bytestream.tellg();
	std::stringstream offset_stream;
    offset_stream << std::hex << std::setw(8) << std::setfill('0') << off;
    offset = offset_stream.str();
	for(int i = 0 ; i < octets ; ++i) {
		if(bytestream.read(&byte, 1)) {
			octetstr[0] = hex_characters[(byte >> 4) & 0x0F];
			octetstr[1] = hex_characters[byte & 0x0F];
			format += octetstr;		
		} else {
			break;
		}
	}
	
	return format;
}

int main (int argc, char **argv) {
	
	g_argv = argv[0];
	if (argc == 1) {
        return EXIT_FAILURE;
    }

	std::ifstream file(argv[1], std::ios::binary);
	std::string offset;
	std::string format;
	while(!file.eof()) {
		format = byte2hex_octets_per_line(file, 16, offset);
		std::cout << offset << ": " << format << "\n";
	}
	/*
	int opt = 0;
	while((opt = getopt(argc, argv, "acEeg:hl:o:prs:uv")) != -1) {

		switch(opt) {
		case 'v':
			
			break;
		}
	}
	*/
	
	/*
	int index = 0;
	for (index = optind ; index < argc ; index++) {
		
	}
	*/
	return 0;
}
