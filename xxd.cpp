#include <iostream>
#include <vector>
#include <array>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <unistd.h>
#include <cstdio>
#include <cerrno>
#include <ctime>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <limits>
#include <unordered_set>
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

inline std::vector<hex_octet>::const_iterator request_hex_data(std::vector<hex_octet> &hex_buffer, std::istream &bytestream, const size_t bufsize) {

	hex_buffer = byte_buffer_2_hex(bytestream, bufsize);	
	return hex_buffer.cbegin();
}

std::ostream &postscript_line_data_out(std::ostream &out, const std::vector<hex_octet> &line_data, const int cols, const char *spacer) {

	auto itld = line_data.cbegin();
		for(int i = 0 ; i < cols ; ++i) {
			if(itld != line_data.cend()) {					 
				out << itld->get_data();
				++itld;
			}
		}

		out << spacer;
		return out;
}

std::ostream &line_data_out(std::ostream &out, const std::vector<hex_octet> &line_data, const int cols, const int grpsz, const char *spacer, uint64_t &offset) {

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
			
			out << spacer;
		}

		out << "\n";
		return out;
}
/*
  0000 0000 1 Print
  1000 0000 0 Print
  
  0000 0000 1 Print
  0000 0000 2 No print
  1000 0000 0 if count was 2 then print 0000 0000 first and current
  
  0000 0000 1 Print
  0000 0000 2 No print
  0000 0000 3 No print ( == 3 then just dont print anything until line_data_is_null == false)
  0000 0000 4 No print
  0000 0000 5 No print
  ..... and so on
  1000 0000 42 Print like normal
  
  Something like this maybe...
*/

std::ostream &postscript_line_buffer_out(std::ostream &out, std::istream &bytestream, const size_t bufsize, const int columns) {
	
	int cols = columns;
	const char *newline = "\n";
	const char *nothing = "";
	const char *ptr_spacer = newline;
	if(cols == 0) {
		cols = DEFAULT_POSTSCRIPT_COLUMN_SIZE;
		ptr_spacer = nothing;
	}
	
	std::vector<hex_octet> big_hex_buffer;
	big_hex_buffer.reserve(bufsize * 2);
	auto it = big_hex_buffer.cbegin();
	while(true) {		
		std::vector<hex_octet> line_data;
		for(int i = 0 ; i != cols ; ++i, ++it) {			
			if(it != big_hex_buffer.cend()) {
				line_data.push_back(*it);				
			} else if (it == big_hex_buffer.cend() && !bytestream.eof()) {
				it = request_hex_data(big_hex_buffer, bytestream, bufsize);
				if(bytestream.gcount() < 1) {
					return out;
				}
				
				line_data.push_back(*it);
			} else if (it == big_hex_buffer.cend() && bytestream.eof()) {
				break;
			}
		}

		postscript_line_data_out(out, line_data, cols, ptr_spacer);
		if(bytestream.eof() && (it == big_hex_buffer.cend())) {
			break;
		}
	}

	if(columns == 0) {
		out << std::endl;
	}
	
	return out;
}

std::ostream &line_buffer_out(std::ostream &out, std::istream &bytestream, const size_t bufsize, const int columns, const int grpsize, const uint64_t off_start, const bool autoskip) {
	
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
		std::vector<hex_octet> line_data;
		for(int i = 0 ; i != cols ; ++i, ++it) {			
			if(it != big_hex_buffer.cend()) {
				line_data.push_back(*it);				
			} else if (it == big_hex_buffer.cend() && !bytestream.eof()) {				
				it = request_hex_data(big_hex_buffer, bytestream, bufsize);
				if(bytestream.gcount() < 1) {
					return out;
				}
				
				line_data.push_back(*it);
			} else if (it == big_hex_buffer.cend() && bytestream.eof()) {
				break;
			}
		}

		if(autoskip == true) {
			autoskip_line_data_out(out, line_data, cols, grpsz, ptr_spacer, offset);
		} else {
			line_data_out(out, line_data, cols, grpsz, ptr_spacer, offset);
		}
		
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

bool check_for_conflict(const xxd_option opt1, const xxd_option opt2) {

    for (const auto &group : g_option_groups) {
        if (group.count(opt1) && group.count(opt2)) {
            return false;
        }
    }
	
    return true;
}

int main (int argc, char **argv) {
	
	g_argv = argv[0];
	int opt = 0;
	std::unordered_set<xxd_option> used_opts;
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
	while((opt = getopt(argc, argv, "ac:Eeg:hl:o:prs:uv")) != -1) {
		xxd_option current_opt;
		switch(opt) {
		case 'a':

			a_used = true;
			current_opt = A_OPT;

			break;
		case 'c':

			c_used = true;
			c_arg = optarg;
			current_opt = C_OPT;
			
			break;
		case 'E':

			E_used = true;
			current_opt = C_E_OPT;
			
			break;
		case 'e':

			e_used = true;
			current_opt = E_OPT;
			
			break;
		case 'g':

			g_used = true;
			g_arg = optarg;
			current_opt = G_OPT;
			
			break;
		case 'h':

			h_used = true;
			current_opt = H_OPT;
			
			break;
		case 'l':

			l_used = true;
			current_opt = L_OPT;
			
			break;
		case 'o':

			o_used = true;
			o_arg = optarg;
			current_opt = O_OPT;
			
			break;
		case 'p':

			p_used = true;
			current_opt = P_OPT;
			
			break;
		case 'r':

			r_used = true;
			current_opt = R_OPT;
			
			break;
		case 's':

			s_used = true;
			current_opt = S_OPT;
			
			break;
		case 'u':

			u_used = true;
			current_opt = U_OPT;
			
			break;
		case 'v':

			v_used = true;
			current_opt = V_OPT;
			
			break;
		default:

			std::cerr << USAGE_LONG << std::endl;
			return EXIT_FAILURE;
			
			break;
		}

		for (const auto &used_opt : used_opts) {
            if (check_for_conflict(used_opt, current_opt)) {
                std::cerr << USAGE_LONG << std::endl;
                return EXIT_FAILURE;
            }
        }

		used_opts.insert(current_opt);		
	}

		
	if(E_used || e_used || l_used || r_used || s_used) {
		return EXIT_SUCCESS;
	}
	
	int groupsize = DEFAULT_GROUP_SIZE;
	int columns = DEFAULT_COLUMN_SIZE;
	uint64_t off_start = 0;
	bool autoskip = false;
	if(v_used) {
		std::cout << g_argv << "   " << "2025" << "   " << link_web << "   " << std::endl;
		return EXIT_SUCCESS;
	}

	if(h_used) {
		std::cout << USAGE_LONG << std::endl;
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
		if(c_used) {} else {
			columns = DEFAULT_POSTSCRIPT_COLUMN_SIZE;
		}
	}
	
	if(o_used) {
		off_start = argument_validation_g(o_arg);
	}

	if(a_used) {
		autoskip = true;
	}
	
	int count = 0;
	int index = 0;
	for(index = optind ; index < argc ; ++index, ++count) {}
	if(count > 2) {
		std::cerr << USAGE_LONG << std::endl;
		return EXIT_FAILURE;
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
			postscript_line_buffer_out(*os_out, *is_in, BUF_SIZE_8KIB, columns) << std::ends;
		} else {
			line_buffer_out(*os_out, *is_in, BUF_SIZE_8KIB, columns, groupsize, off_start, autoskip) << std::ends;
		}
		
		in_file.close();
		out_file.close();
	}

	return 0;
}
