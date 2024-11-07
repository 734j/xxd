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

hex_octet byte2hex(char byte) {

	return hex_octet(hex_characters[(byte >> 4) & 0x0F],
					 hex_characters[byte & 0x0F]); 
}

void offsetformat(char off_string[9], long offset) {

    memset(off_string, '\0', 9);
	long rem = 0;
	for(int i = 7 ; i >= 0 ; --i) {
		rem = offset % 16;
		offset = offset / 16;
		off_string[i] = hex_characters[rem];
	}	
}

hexadecimal_line byte2hex_octets_per_line(std::ifstream &bytestream, int octets) {

	if(octets > 256 || octets < 1) {
		return hexadecimal_line("","");
	}
	
	std::string format;
	char octetstr[3];
	char byte;
	long off = bytestream.tellg();
	char offset[9];
	offsetformat(offset, off);
	for(int i = 0 ; i < octets ; ++i) {
		if(bytestream.read(&byte, 1)) {
			byte2hex(octetstr, byte);
			format += octetstr;
		} else {
			break;
		}
	}
	
	return hexadecimal_line(format, offset);
}

std::vector<hexadecimal_line> &byte2hex_octets_per_line(std::vector<hexadecimal_line> &lines,
														std::ifstream &bytestream,
														int octets) {

	if(octets > 256 || octets < 1) {
		return lines;
	}
	
	std::string format;
	std::string octetstr(2, '0');
	std::vector<char> buffer_bytes(4096);
	long off = bytestream.tellg();
	char offset[9];
	if(!bytestream.read(&buffer_bytes[0], 4096)) {
		
	}
	
	for(auto &bb : buffer_bytes) {
		long tmpoff = off;
		for(int i = 0 ; i < octets ; ++i) {
			++tmpoff;
			octetstr[0] = hex_characters[(bb >> 4) & 0x0F];
			octetstr[1] = hex_characters[bb & 0x0F];
			format += octetstr;			
		}
		
		offsetformat(offset, off);
		lines.push_back(hexadecimal_line(format, offset));
		off = tmpoff;
	}
	
	return lines;
}

std::vector<hex_octet> byte_buffer_2_hex(std::ifstream &bytestream, size_t bufsize) {

	std::vector<char> bufbytes(bufsize);
	std::vector<hex_octet> ho;
	ho.reserve(bufsize * 2);
	if(!bytestream.read(&bufbytes[0], bufsize)) {}
	if(bytestream.eof()) {
		bufbytes.resize(bytestream.gcount());
	}
	
	for(auto &a : bufbytes) {
		ho.push_back(hex_octet(byte2hex(a)));
	}
	
	return ho;
}

int main (int argc, char **argv) {
	
	g_argv = argv[0];
	if (argc == 1) {
        return EXIT_FAILURE;
    }

	hex_characters = g_lower_hex_characters;
	std::ifstream file(argv[1], std::ios::binary);
	while(!file.eof()) {		
		auto format = byte_buffer_2_hex(file, 4096);
		for(auto &a : format) {
			std::cout << a.get_data();
		}
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
