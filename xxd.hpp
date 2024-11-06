#define BUF_SIZE_2 2048
#define BUF_SIZE_4 4096
#define BUF_SIZE_1MIB 1048576

#ifndef XXD_HPP
#define XXD_HPP
#include <string>
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
