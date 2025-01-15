#define BUF_SIZE_2KIB 2048
#define BUF_SIZE_4KIB 4096
#define BUF_SIZE_8KIB 8144
#define BUF_SIZE_1MIB 1048576
#define DEFAULT_GROUP_SIZE 2
#define DEFAULT_COLUMN_SIZE 16
#define DEFAULT_POSTSCRIPT_COLUMN_SIZE 30

#define USAGE_LONG "A VERY USEFUL HELP MESSAGE"

#ifdef __TEST_STREAM_STDERR // Stream for debugging, pass -DDEBUG_S and then compile to turn on
#define DEBUG_STREAM(x) std::cerr x
#else
#define DEBUG_STREAM(x)
#endif

#ifndef XXD_HPP
#define XXD_HPP
#include <string>
#include <vector>
#include <unordered_set>

enum xxd_option {
    A_OPT, C_OPT, C_E_OPT, E_OPT, G_OPT, O_OPT, U_OPT, L_OPT, S_OPT,
    P_OPT, R_OPT, H_OPT, V_OPT
};

const std::vector<std::unordered_set<xxd_option>> g_option_groups{
		{A_OPT, C_OPT, C_E_OPT, E_OPT, G_OPT, O_OPT, U_OPT, L_OPT, S_OPT},
		{P_OPT, L_OPT, U_OPT, C_OPT, S_OPT},
		{P_OPT, R_OPT},
		{H_OPT},
		{V_OPT}
};

class hex_octet {
public:
	using hexo = char[3];
	hex_octet(char a, char b) {
		octet[0] = a;
		octet[1] = b;
	}

	hex_octet(hexo a) {
		octet[0] = a[0];
		octet[1] = a[1];
	}

	bool operator==(const hex_octet &other) const { return octet[0] == other.octet[0] && octet[1] == other.octet[1]; }
	const hexo &get_data() const { return octet; }
	static bool line_data_is_null(const std::vector<hex_octet> &line_data);
	
private:
	hexo octet = {'\0'};
};

bool hex_octet::line_data_is_null(const std::vector<hex_octet> &line_data) {
	const hex_octet cmp = hex_octet('0','0');
	for(const auto &a : line_data) {
		if(a == cmp) {
			return true;
		}
	}
	
	return false;
}
	
class hexadecimal_line {
public:
	hexadecimal_line() = default;
	hexadecimal_line(std::string hexadecimal, std::string offset): _hexadecimal(hexadecimal), _offset(offset) {}
	std::string &rget_hex() { return _hexadecimal; }
	std::string &rget_off() { return _offset; }
	bool is_fail() {
		if(_hexadecimal.empty() || _offset.empty()) {
			return true;
		}

		return false;
	}
private:
	std::string _hexadecimal = "";
	std::string _offset = "";
};

#endif
