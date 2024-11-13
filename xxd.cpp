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

std::ostream &line_buffer_out(std::ostream &out, std::ifstream &bytestream, size_t bufsize, int cols, int grpsize) {

	if(cols > 256 || cols < 1) {
		bytestream.setstate(std::ios::failbit);
		return out;
	}
	
	long offset = 0;
	std::vector<hex_octet> big_hex_buffer;
	big_hex_buffer.reserve(bufsize * 2);
	auto it = big_hex_buffer.cbegin();
	while(true) {
		if(it == big_hex_buffer.cend() && !bytestream.eof()) {
			big_hex_buffer = byte_buffer_2_hex(bytestream, bufsize);
			it = big_hex_buffer.cbegin();
		}

		std::vector<hex_octet> line_data;
		for(int i = 0 ; i != cols*grpsize ; ++i, ++it) {
			if(it != big_hex_buffer.cend()) {
				line_data.push_back(*it);
			} else if (it == big_hex_buffer.cend() && !bytestream.eof()) {
				big_hex_buffer = byte_buffer_2_hex(bytestream, bufsize);
				it = big_hex_buffer.cbegin();
			} else if (it == big_hex_buffer.cend() && bytestream.eof()) {
				break;
			}
		}

		auto itld = line_data.cbegin();
		out << offsetformat(offset) << ": ";
		for(int i = 0 ; i < cols ; ++i) {
			for(int j = 0 ; j < grpsize ; ++j, ++offset) {
				if(itld != line_data.cend()) {					 
					out << itld->get_data();
					++itld;
				}
			}

			out << " ";
		}

		out << "\n";
		if(bytestream.eof() && (it == big_hex_buffer.cend())) {
			break;
		}
	}
	
	return out;
}

int main (int argc, char **argv) {
	
	g_argv = argv[0];
	if (argc == 1) {
        return EXIT_FAILURE;
    }

	hex_characters = g_lower_hex_characters;
	std::ifstream file_stream(argv[1], std::ios::binary);
	while(!file_stream.eof() && !file_stream.fail()) {
		line_buffer_out(std::cout, file_stream, 4096, 8, 2);
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
