#pragma once
#include "cRequestQueue.h"
#include <fstream>
class cClient {


private:
	bool readCash() {
		std::ifstream fin("queue.dat");
		int value;

		while (fin >> value) {
			if (value <= 0)
				return false;
			requests.push(static_cast<unsigned int>(value));
		}
		if (!fin.eof())
			return false;
		return true;
	}

	void runClients() {

		while (requests.size()) {

		}
	}
	std::queue<unsigned int> requests;
};