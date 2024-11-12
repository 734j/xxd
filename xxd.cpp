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

std::string offsetformat(long offset) {

	std::string off_string(9, '\0');
	long rem = 0;
	for(int i = 7 ; i >= 0 ; --i) {
		rem = offset % 16;
		offset = offset / 16;
		off_string[i] = hex_characters[rem];
	}

	return off_string;
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

std::ostream &final_formatted_hex_output(std::ostream &out,
										 std::vector<hex_octet> &toformat,
										 int &ri, int &rj,
										 int cols, int grpsize, long &offset) {

	// Redesign this whole function, this all sucks
	auto iter = toformat.cbegin(); // beginning of unformatted hex
	bool mid = false;
	for(;;) {
	    		
		std::string line; // create line string
		out << offsetformat(offset) << ": "; // print offset, formatted
		for(int i = ri ; i != cols ; ++i) {
			for(int j = rj ; j != grpsize ; ++j, ++iter, ++offset) {
				if(iter != toformat.cend()) {
					line += iter->hex_octet::get_data();
				}

				if(iter == toformat.cend() && (rj != grpsize || ri != cols)) {
					rj = j;
					ri = i;
					mid = true;
					break;
				} else if (iter == toformat.cend() && (rj == grpsize || ri == cols)) {
					ri = 0;
					rj = 0;
					break;
				}
			}

			if(mid) { break; }
			line += ' ';
		}

		out << line << "\n";
	}
	
	return out;
}

int main (int argc, char **argv) {
	
	g_argv = argv[0];
	if (argc == 1) {
        return EXIT_FAILURE;
    }

	hex_characters = g_lower_hex_characters;
	std::ifstream file(argv[1], std::ios::binary);
	int ri = 0;
	int rj = 0;
	while(!file.eof()) {		
		auto format = byte_buffer_2_hex(file, 4096);		
		long off = 0;
		final_formatted_hex_output(std::cout, format, ri, rj, 8, 2, off);
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
