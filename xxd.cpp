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

void attempt_1_dumb_hex_output(const char *path) {

	FILE *file = std::fopen(path, "rb");
	int a[1];
	while(std::fread(a, 1, 1, file)) {
		std::cout << std::setw(2) << std::setfill('0') << std::hex << a[0];
	}
}

std::string byte2hex(const uint8_t byte) {

	const char* hexDigits = "0123456789ABCDEF";
    std::string hexStr(2, '0'); // Create a string of size 2 initialized with '0'
    hexStr[0] = hexDigits[(byte >> 4) & 0x0F]; // Get the first hex digit
    hexStr[1] = hexDigits[byte & 0x0F];        // Get the second hex digit
    return hexStr;
}

std::string byte2hex_2group(const uint8_t bytegroup[2]) {

	const char* hexDigits = "0123456789ABCDEF";
    std::string hexStr(4, '0'); // Create a string of size 2 initialized with '0'
	hexStr[0] = hexDigits[(bytegroup[0] >> 4) & 0x0F];
	hexStr[1] = hexDigits[bytegroup[0] & 0x0F];
	hexStr[2] = hexDigits[(bytegroup[1] >> 4) & 0x0F];
	hexStr[3] = hexDigits[bytegroup[1] & 0x0F];
	return hexStr;
}

std::string byte2hex_octets_per_line(std::ifstream &bytestream, int octets) {

	if(octets > 256 || octets < 1) {
		return "";
	}
	
	const char* hex_characters = "0123456789abcdef";
	std::string format;
	std::string octetstr(2, '0');
	uint8_t byte;
	for(int i = 0 ; i < octets ; ++i) {
		if(bytestream.read(reinterpret_cast<char*>(&byte), sizeof(byte))) {
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

	while(!file.eof()) {
		std::string format = byte2hex_octets_per_line(file, 16);
		std::cout << format << std::endl;
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
