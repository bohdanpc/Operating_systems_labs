#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "cRequestQueue.h"

class cAtm {
public:
	cAtm() {
		if (!readCash())
			std::cerr << "Invalid input file\n";
		else {

		}
	}

private:
	
	
	std::condition_variable requestReceived;




	void runChecker() {

	}

	void runCashier() {

	}

	std::thread atmChecker;
	std::thread atmCashier;
};