#include <iostream>
#include <vector>
#include <array>
#include <fstream>
#include <filesystem>
#include <unistd.h>
#include <cstdio>
#include <cerrno>
#include <ctime>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <limits>
#include "xxd.hpp"

/*
  This is mainly a test to see if i can implement 'xxd'
  I will try to eventually make a real 'xxd' implementation
  but since i know very little about this domain of knowledge
  i will most likely spend a lot of time playing around with this.
 */

char *g_argv = nullptr;
const char *link_web = "open7software.se";
const char *g_lower_hex_characters = "0123456789abcdef";
const char *g_upper_hex_characters = "0123456789ABCDEF";
const char *hex_characters = g_lower_hex_characters;

inline hex_octet byte2hex(const char byte) {

	return hex_octet(hex_characters[(byte >> 4) & 0x0F],
					 hex_characters[byte & 0x0F]); 
}

std::string offsetformat(uint64_t offset) {

	std::string off_string;
	int i = 7;
	off_string = std::string(8, '\0');
	if(offset > std::numeric_limits<uint32_t>::max()) {
		off_string = std::string(16, '\0');
		i = 15;
	}
		
	uint64_t rem = 0;
	for( ; i >= 0 ; --i) {
		rem = offset % 16;
		offset = offset / 16;
		off_string[i] = hex_characters[rem];
	}

	return off_string;
}

std::vector<hex_octet> byte_buffer_2_hex(std::istream &bytestream, const size_t bufsize) {

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

std::ostream &postscript_line_buffer_out(std::ostream &out, std::istream &bytestream, const size_t bufsize, const int columns) {
	
	int cols = columns;
	if(cols == 0) {
		cols = DEFAULT_COLUMN_SIZE;
	}
	
	std::vector<hex_octet> big_hex_buffer;
	big_hex_buffer.reserve(bufsize * 2);
	auto it = big_hex_buffer.cbegin();
	while(true) {
		if(it == big_hex_buffer.cend() && !bytestream.eof()) {
			big_hex_buffer = byte_buffer_2_hex(bytestream, bufsize);
			it = big_hex_buffer.cbegin();
		}

		if(bytestream.gcount() < 1) {
			break;
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
		for(int i = 0 ; i < cols ; ++i) {
			if(itld != line_data.cend()) {					 
				out << itld->get_data();
				++itld;
			}
		}

		out << "\n";
		if(bytestream.eof() && (it == big_hex_buffer.cend())) {
			break;
		}
	}
	
	return out;
}

std::ostream &line_buffer_out(std::ostream &out, std::istream &bytestream, const size_t bufsize, const int columns, const int grpsize, const uint64_t off_start) {
	
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
	
	uint64_t offset = off_start;
	std::vector<hex_octet> big_hex_buffer;
	big_hex_buffer.reserve(bufsize * 2);
	auto it = big_hex_buffer.cbegin();
	while(true) {
		if(it == big_hex_buffer.cend() && !bytestream.eof()) {
			big_hex_buffer = byte_buffer_2_hex(bytestream, bufsize);
			it = big_hex_buffer.cbegin();
		}

		if(bytestream.gcount() < 1) {
			break;
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

uint64_t argument_validation_g(const std::string argument) {

	std::stringstream ss(argument);	
	uint64_t number = 0;
	
	ss >> number;	
	if(ss.eof()) {		
		return number;
	}

	return 0;
}

int main (int argc, char **argv) {
	
	g_argv = argv[0];
	int opt = 0;
	std::string c_arg;
	std::string g_arg;
	std::string o_arg;
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
	int h_mut = 0;
	int v_mut = 0;
	int p_mut = 0;
	int a_mut = 0;
	int o_mut = 0;
	while((opt = getopt(argc, argv, "ac:Eeg:hl:o:prs:uv")) != -1) {
		switch(opt) {
		case 'a':

			a_used = true;
			a_mut = 1;
			
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
			h_mut = 1;
			
			break;
		case 'l':

			l_used = true;
			
			break;
		case 'o':

			o_used = true;
			o_arg = optarg;
			o_mut = 1;
			
			break;
		case 'p':

			p_used = true;
			p_mut = 1;
			
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
			v_mut = 1;
			
			break;
		}
	}
	
	if(a_used || E_used || e_used || l_used || r_used || s_used) {
		return EXIT_SUCCESS;
	}

	int groupsize = DEFAULT_GROUP_SIZE;
	int columns = DEFAULT_COLUMN_SIZE;
	uint64_t off_start = 0;
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

	if(p_used) {
		columns = DEFAULT_POSTSCRIPT_COLUMN_SIZE;
	}
	
	if(o_used) {
		off_start = argument_validation_g(o_arg);
	}
	
	int count = 0;
	int index = 0;
	for(index = optind ; index < argc ; ++index, ++count) {}
	if(count > 2) {
		std::cout << "count > 2" << std::endl;
		return -1;
	}

	std::istream *is_in = &std::cin;
	std::ostream *os_out = &std::cout;
	std::ifstream in_file;
	std::ofstream out_file;
	if(count == 1) {
		for (index = optind ; index < argc ; ++index) {
			in_file.open(argv[index], std::ios::binary);
			if(!in_file.is_open()) {
				std::perror(g_argv);
				return EXIT_FAILURE;
			}
			
			is_in = &in_file;
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
		
		in_file.open(argv[in], std::ios::binary);
		if(!in_file.is_open()) {
			std::perror(g_argv);
			return EXIT_FAILURE;
		}
		
		out_file.open(argv[out]);
		if(!out_file.is_open()) {
			std::perror(g_argv);
			in_file.close();
			return EXIT_FAILURE;
		}
		
		is_in = &in_file;
		os_out = &out_file;
	}

	while(!is_in->eof() && !is_in->fail()) {
		if(p_used) {
			postscript_line_buffer_out(*os_out, *is_in, BUF_SIZE_8KIB, columns);
		} else {
			line_buffer_out(*os_out, *is_in, BUF_SIZE_8KIB, columns, groupsize, off_start);
		}
		
		in_file.close();
		out_file.close();
	}

	return 0;
}
