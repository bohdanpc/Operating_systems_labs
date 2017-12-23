#include "TreeAvl.h"
#include <string>
#include <iostream>

void printHeader() {
	std::cout << "1 - Print tree\n";
	std::cout << "2 - Add identifier\n";
	std::cout << "3 - Delete identifier\n";
	std::cout << "4 - Find identifier\n";
}


std::string readString() {
	std::cout << "Enter identifier: ";
	std::string identifier;
	std::cin >> identifier;
	return identifier.substr(0, TreeAvl<std::string>::eIdMaxLen);
	std::cout << std::endl;
}


int main() {
	TreeAvl<std::string> myTree;

	unsigned char ch;
	while (true) {
		std::string id;
		printHeader();
		std::cin >> ch; std::cin.clear();
		switch (ch) {
		case '1': myTree.printTree();
			break;
		case '2':
			id = readString();
			myTree.insert(id);
			break;
		case '3':
			id = readString();
			myTree.remove(id);
			break;
		case '4':
			id = readString();
			if (myTree.findKey(id))
				myTree.printLastKeyWay();
			else
				std::cout << "The key wasn't found\n";
			break;
		default:
			std::cout << "Invalid choice";
		}
		std::cout << std::endl << std::endl;
	}

	return 0;
}