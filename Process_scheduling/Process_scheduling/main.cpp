#include <iostream>
#include <sstream>
#include "cPriorityScheduler.h"

int main() {

	std::unique_ptr<PriorityScheduler> myScheduler(new PriorityScheduler);
	try {
		myScheduler->ParseSchedulerFile("db_in.csv");
		myScheduler->RunScheduler();
	}
	catch (PriorityScheduler::ErrorType &err) {
		std::cout << "Error: " << (int)err << std::endl;
	}
	return 0;
}