#include <iostream>
#include <sstream>
#include "cPriorityScheduler.h"

int main() {

	std::unique_ptr<PriorityScheduler> myScheduler(new PriorityScheduler);
	try {
		constexpr auto inFileName = "db_in.csv";
		constexpr auto outFileName = "db_out.csv";
		constexpr auto outExlFileName = "db_exl.csv";

		myScheduler->ParseSchedulerFile(inFileName);
		std::cout << "Input file \'" << inFileName << "\' has been parsed successfully..." << std::endl << std::endl;

		myScheduler->RunScheduler();
		std::cout << "Scheduler has finished emulation successfully..." << std::endl << std::endl;

		myScheduler->BuildExlResFile(outExlFileName);
		std::cout << "Result information has been written to file \'" << outFileName << "\'" << std::endl << std::endl;

		myScheduler->BuildResFile(outFileName);
		std::cout << "Result information to build chart has been written to file \'"  << outExlFileName << "\'" << std::endl << std::endl;

	}
	catch (PriorityScheduler::ErrorType &err) {
		std::cout << "Error: " << (int)err << std::endl;
	}
	return 0;
}