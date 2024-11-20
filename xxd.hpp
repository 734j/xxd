#define BUF_SIZE_2KIB 2048
#define BUF_SIZE_4KIB 4096
#define BUF_SIZE_8KIB 8144
#define BUF_SIZE_1MIB 1048576
#define DEFAULT_GROUP_SIZE 2
#define DEFAULT_COLUMN_SIZE 16

#ifndef XXD_HPP
#define XXD_HPP
#include <string>

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
	
	const hexo &get_data() const { return octet; }
private:
	hexo octet = {'\0'};
};

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
