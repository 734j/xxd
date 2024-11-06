#include <iostream>
#include <vector>
#include <array>
#include <fstream>
#include <filesystem>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include "xxd.hpp"

/*
  This is mainly a test to see if i can implement 'xxd'
  I will try to eventually make a real 'xxd' implementation
  but since i know very little about this domain of knowledge
  i will most likely spend a lot of time playing around with this.
 */

char *g_argv = nullptr;
const char *hex_characters = nullptr;
const char *g_lower_hex_characters = "0123456789abcdef";
const char *g_upper_hex_characters = "0123456789ABCDEF";

std::string offsetformat(long offset) {

	char off_string[9] = {'0','0','0','0','0','0','0','0','\0'};
	long rem = 0;
	for(int i = 7 ; i >= 0 ; --i) {
		rem = offset % 16;
		offset = offset / 16;
		off_string[i] = hex_characters[rem];
	}
	
	return std::string(off_string);
}

hexadecimal_line byte2hex_octets_per_line(std::ifstream &bytestream, int octets) {

	if(octets > 256 || octets < 1) {
		return hexadecimal_line("","");
	}
	
	std::string format;
	std::string octetstr(2, '0');
	char byte;
	auto off = bytestream.tellg();
	std::string offset = offsetformat(off);
	for(int i = 0 ; i < octets ; ++i) {
		if(bytestream.read(&byte, 1)) {
			octetstr[0] = hex_characters[(byte >> 4) & 0x0F];
			octetstr[1] = hex_characters[byte & 0x0F];
			format += octetstr;		
		} else {
			break;
		}
	}
	
	return hexadecimal_line(format, offset);
}

hexadecimal_line byte2hex_octets_per_line_stdc(FILE *bytestream, int octets) {

	if(octets > 255 || octets < 1) {
		return hexadecimal_line("","");
	}

	char off_string[9] = {'0','0','0','0','0','0','0','0','\0'};
	auto offset = std::ftell(bytestream);
	long rem = 0;
	for(int i = 7 ; i >= 0 ; --i) {
		rem = offset % 16;
		offset = offset / 16;
		off_string[i] = hex_characters[rem];
	}

	char byte;
	std::string octetstr(2,'0');
	std::string format;
	for(int i = 0 ; i < octets ; ++i) {
		if(fread(&byte, 1, 1, bytestream)) {
			octetstr[0] = hex_characters[(byte >> 4) & 0x0F];
			octetstr[1] = hex_characters[byte & 0x0F];
			format += octetstr;
		} else {
			break;
		}
	}
	
	return hexadecimal_line(format,std::string(off_string));
}

int main (int argc, char **argv) {
	
	g_argv = argv[0];
	if (argc == 1) {
        return EXIT_FAILURE;
    }

	hex_characters = g_lower_hex_characters;
	
	std::ifstream file(argv[1], std::ios::binary);
	//FILE *file = fopen(argv[1], "rb");
	hexadecimal_line format;
	while(!file.eof()) {
		format = byte2hex_octets_per_line(file, 16);	   
		std::cout << format.rget_off() << ": " << format.rget_hex() << "\n";		
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
