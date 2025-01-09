#include <iostream>
#include <vector>
#include <array>
#include <fstream>
#include <filesystem>
#include <unistd.h>
#include <cstdio>
#include <ctime>
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
const char *link_web = "open7software.se";
const char *hex_characters = nullptr;
const char *g_lower_hex_characters = "0123456789abcdef";
const char *g_upper_hex_characters = "0123456789ABCDEF";

inline hex_octet byte2hex(const char byte) {

	return hex_octet(hex_characters[(byte >> 4) & 0x0F],
					 hex_characters[byte & 0x0F]); 
}

std::string offsetformat(long offset) {

	std::string off_string(8, '\0');
	long rem = 0;
	for(int i = 7 ; i >= 0 ; --i) {
		rem = offset % 16;
		offset = offset / 16;
		off_string[i] = hex_characters[rem];
	}

	return off_string;
}

std::vector<hex_octet> byte_buffer_2_hex(std::ifstream &bytestream, const size_t bufsize) {

	std::vector<char> bufbytes(bufsize);
	std::vector<hex_octet> ho;
	ho.reserve(bufsize * 2);
	if(!bytestream.read(&bufbytes[0], bufsize)) {}
	if(bytestream.eof()) {
		bufbytes.resize(bytestream.gcount());
	}
	
	for(auto &a : bufbytes) {
		ho.push_back(byte2hex(a));
	}
	
	return ho;
}

std::ostream &line_buffer_out(std::ostream &out, std::ifstream &bytestream, const size_t bufsize, const int columns, const int grpsize) {

	int grpsz = grpsize;
	int cols = columns;
	const char *spacer = " ";
	const char *nospacer = "";
	const char *ptr_spacer = spacer;
	if(grpsize == 0) {
		ptr_spacer = nospacer;
		grpsz = DEFAULT_GROUP_SIZE;
	}		

	if(cols == 0) {
		cols = DEFAULT_COLUMN_SIZE;
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
		for(int i = 0 ; i != cols ; ++i, ++it) {			
			if(it != big_hex_buffer.cend()) {
				line_data.push_back(*it);				
			} else if (it == big_hex_buffer.cend() && !bytestream.eof()) {
				big_hex_buffer = byte_buffer_2_hex(bytestream, bufsize);
				it = big_hex_buffer.cbegin();
				line_data.push_back(*it);
			} else if (it == big_hex_buffer.cend() && bytestream.eof()) {
				break;
			}
		}

		auto itld = line_data.cbegin();
		out << offsetformat(offset) << ": ";
		for(int i = 0 ; i < cols ; ++i) {
			for(int j = 0 ; j < grpsz ; ++j) {
				if(itld != line_data.cend()) {					 
					out << itld->get_data();
					++itld;
					++offset;
				}
			}
			
			out << ptr_spacer;
		}

		out << "\n";
		if(bytestream.eof() && (it == big_hex_buffer.cend())) {
			break;
		}
	}
	
	return out;
}

int argument_validation_g(const std::string argument) {

	std::stringstream ss(argument);	
	int64_t number = 0;
	
	ss >> number;	
	if(ss.eof()) {		
		return number;
	}

	return -1;
}

int main (int argc, char **argv) {
	
	g_argv = argv[0];
	if (argc == 1) {
        return EXIT_FAILURE;
    }	

	bool a_used = false;
	bool c_used = false;
	bool E_used = false;
	bool e_used = false;
	bool g_used = false;
	bool h_used = false;
	bool l_used = false;
	bool o_used = false;
	bool p_used = false;
	bool r_used = false;
	bool s_used = false;
	bool u_used = false;
	bool v_used = false;
	int opt = 0;
	std::string c_arg;
	std::string g_arg;
	while((opt = getopt(argc, argv, "ac:Eeg:hl:o:prs:uv")) != -1) {
		switch(opt) {
		case 'a':

			a_used = true;
			
			break;
		case 'c':

			c_used = true;
			c_arg = optarg;
			
			break;
		case 'E':

			E_used = true;
			
			break;
		case 'e':

			e_used = true;
			
			break;
		case 'g':

			g_used = true;
			g_arg = optarg;
			
			break;
		case 'h':

			h_used = true;
			
			break;
		case 'l':

			l_used = true;
			
			break;
		case 'o':

			o_used = true;
			
			break;
		case 'p':

			p_used = true;
			
			break;
		case 'r':

			r_used = true;
			
			break;
		case 's':

			s_used = true;
			
			break;
		case 'u':

			u_used = true;
			
			break;
		case 'v':

			v_used = true;
			
			break;
		}
	}

	if(a_used || E_used || e_used || l_used || o_used || p_used || r_used || s_used) {
		return EXIT_SUCCESS;
	}

	int groupsize = DEFAULT_GROUP_SIZE;
	int columns = DEFAULT_COLUMN_SIZE;	
	if(v_used) {
		std::cout << g_argv << "   " << "2025" << "   " << link_web << "   " << std::endl;
		return EXIT_SUCCESS;
	}

	if(h_used) {
		std::cout << "A VERY USEFUL HELP MESSAGE" << std::endl;
		return EXIT_SUCCESS;
	}
	
	if(u_used) {
		hex_characters = g_upper_hex_characters;
	} else {
		hex_characters = g_lower_hex_characters;
	}

	if(c_used) {
		int av = argument_validation_g(c_arg);		
		if(av > 256 || av < 0) {
			std::cout << g_argv << ": invalid number of columns (max. 256). " << std::endl;
			return EXIT_FAILURE;
		}

		columns = av;
	}
	
	if(g_used) {
		int av = argument_validation_g(g_arg);
		if(av < 0) {
			std::cout << g_argv << ": invalid group size. " << std::endl;
			return EXIT_FAILURE;
		}
		
		groupsize = av;
	}
	
	int count = 0;
	int index = 0;
	for(index = optind ; index < argc ; ++index, ++count) {} // get number of files given
	if(count > 2) {
		std::cout << "count > 2" << std::endl;
		return -1;
	}

	if(count == 1) {
		for (index = optind ; index < argc ; ++index) {		
			std::ifstream file_stream(argv[index], std::ios::binary);
			while(!file_stream.eof() && !file_stream.fail()) {
				
				line_buffer_out(std::cout, file_stream, BUF_SIZE_8KIB, columns, groupsize);
			}
		}
	}

	if(count == 2) {

		int in = 0;
		int out = 0;
		int ccount = 1;
		for (index = optind ; index < argc ; ++index, ++ccount) {
			switch(ccount) {
			case 1:
				in = index;
				break;
			case 2:
				out = index;
				break;
			}
		}

		std::ifstream file_stream(argv[in], std::ios::binary);
		std::ofstream ofile(argv[out]);
			while(!file_stream.eof() && !file_stream.fail()) {				
				line_buffer_out(ofile, file_stream, BUF_SIZE_8KIB, columns, groupsize);
			}
	}

	
	return 0;
}
