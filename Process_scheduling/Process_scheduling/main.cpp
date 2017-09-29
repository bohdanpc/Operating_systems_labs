#include <iostream>
#include <sstream>

int main() {
	std::stringstream ss;
	ss << "hello world, bye ";

	std::string str;
	std::getline(ss, str, ',');
	std::cout << str << std::endl;
	
	std::getline(ss, str, ',');
	std::cout << str << std::endl;

	std::string abc = "a";
	std::getline(ss, abc, ',');
	std::cout << abc << std::endl;

	return 0;
}