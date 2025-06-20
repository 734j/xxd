#include <iostream>
#include <fstream>
#include <vector>
#include "../xxd.hpp"

int main() {

	std::ofstream as1("as-test1.ByteData", std::ios::binary);
	std::vector<char> Vas1(16,0x00);
	as1.write(Vas1.data(), Vas1.size());

	std::ofstream as2("as-test2.ByteData", std::ios::binary);
	std::vector<char> Vas2(32, 0x00);
	as2.write(Vas2.data(), Vas2.size());
	
	std::ofstream as3("as-test3.ByteData", std::ios::binary);
	std::vector<char> Vas3(48, 0x00);
	as3.write(Vas3.data(), Vas3.size());
	
	std::ofstream as4("as-test4.ByteData", std::ios::binary);
	std::vector<char> Vas4(17, 0x00);
	as4.write(Vas4.data(), Vas4.size());
	
	std::ofstream as5("as-test5.ByteData", std::ios::binary);
	std::vector<char> Vas5(16, 0x00);
	std::vector<char> Vas5_2(16, 0x31);
	as5.write(Vas5.data(), Vas5.size());
	as5.write(Vas5_2.data(), Vas5_2.size());
	
	std::ofstream as6("as-test6.ByteData", std::ios::binary);
	std::vector<char> Vas6(32, 0x00);
	std::vector<char> Vas6_2(16, 0x31);
	as6.write(Vas6.data(), Vas6.size());
	as6.write(Vas6_2.data(), Vas6_2.size());

	std::ofstream as7("as-test7.ByteData", std::ios::binary);
	std::vector<char> Vas7(48, 0x00);
	std::vector<char> Vas7_2(16, 0x31);
	as7.write(Vas7.data(), Vas7.size());
	as7.write(Vas7_2.data(), Vas7_2.size());

	std::ofstream as8("as-test8.ByteData", std::ios::binary);
	std::vector<char> Vas8(64, 0x00);
	std::vector<char> Vas8_2(16, 0x31);
	as8.write(Vas8.data(), Vas8.size());
	as8.write(Vas8_2.data(), Vas8_2.size());

	std::ofstream as9("as-test9.ByteData", std::ios::binary);
	std::vector<char> Vas9(16, 0x31);
	std::vector<char> Vas9_2(64, 0x00);
	std::vector<char> Vas9_3(16, 0x31);
	as9.write(Vas9.data(), Vas9.size());
	as9.write(Vas9_2.data(), Vas9_2.size());
	as9.write(Vas9_3.data(), Vas9_3.size());

	std::ofstream as10("as-test10.ByteData", std::ios::binary);
	std::vector<char> Vas10(16, 0x31);
	std::vector<char> Vas10_2(48, 0x00);
	std::vector<char> Vas10_3(16, 0x31);
	as10.write(Vas10.data(), Vas10.size());
	as10.write(Vas10_2.data(), Vas10_2.size());
	as10.write(Vas10_3.data(), Vas10_3.size());

	std::ofstream as11("as-test11.ByteData", std::ios::binary);
	std::vector<char> Vas11(16, 0x31);
	std::vector<char> Vas11_2(32, 0x00);
	std::vector<char> Vas11_3(16, 0x31);
	as11.write(Vas11.data(), Vas11.size());
	as11.write(Vas11_2.data(), Vas11_2.size());
	as11.write(Vas11_3.data(), Vas11_3.size());

	std::ofstream as12("as-test12.ByteData", std::ios::binary);
	std::vector<char> Vas12(16, 0x31);
	std::vector<char> Vas12_2(16, 0x00);
	std::vector<char> Vas12_3(16, 0x31);
	as12.write(Vas12.data(), Vas12.size());
	as12.write(Vas12_2.data(), Vas12_2.size());
	as12.write(Vas12_3.data(), Vas12_3.size());

	std::ofstream as13("as-test13.ByteData", std::ios::binary);
	std::vector<char> Vas13(64, 0x00);
	as13.write(Vas13.data(), Vas13.size());

	std::ofstream as14("as-test14.ByteData", std::ios::binary);
	std::vector<char> Vas14(64, 0x31);
	std::vector<char> Vas14_2(64, 0x00);
	std::vector<char> Vas14_3(64, 0x31);
	for(int i = 0 ; i < 100 ; ++i) {
		as14.write(Vas14.data(), Vas14.size());
		as14.write(Vas14_2.data(), Vas14_2.size());
		as14.write(Vas14_3.data(), Vas14_3.size());
		as14.write(Vas14.data(), Vas14.size());
		as14.write(Vas14_2.data(), Vas14_2.size());
		as14.write(Vas14_3.data(), Vas14_3.size());
	}
	
	as14.write(Vas14_2.data(), Vas14_2.size());
	std::ofstream as_random("as-test-random.ByteData", std::ios::binary);
	std::ifstream as_urandom_file("/dev/urandom", std::ios::binary);
	std::vector<char> urandom_data(BUF_SIZE_8KIB);
	as_urandom_file.read(&urandom_data[0], BUF_SIZE_8KIB);
	as_random.write(urandom_data.data(), urandom_data.size());
	return 0;
}
