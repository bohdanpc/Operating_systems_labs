#include "cPriorityScheduler.h"
#include <fstream>
#include <sstream>

void PriorityScheduler::ParseSchedulerFile(const std::string &inFileName) {
	std::ifstream fin(inFileName);

	if (!fin) {
		throw ErrorType::CannotOpenInFile;
	}

	std::string field;
	std::stringstream taskStream;

	tProcess currProcess;
	while (std::getline(fin, field, '\n')) {
		taskStream << field;
		
		//make function
		std::getline(taskStream, currProcess.name, ',');
		if (currProcess.name == "")
			throw ErrorType::InvalidInputFileFormat;

		//make function
		std::string sArrivalTime;
		std::getline(taskStream, sArrivalTime, ',');
		try {
			currProcess.arrivalTime = std::stoi(sArrivalTime);
		}
		catch (std::invalid_argument &) {
			throw ErrorType::InvalidArrivalTimeFormat;
		}
		catch (std::out_of_range &) {
			throw ErrorType::ValueOutOfRange;
		}

		//make function
		std::string sWorkTime;
		std::getline(taskStream, sWorkTime, ',');
		try {
			currProcess.workTime = std::stoi(sWorkTime);
		}
		catch (std::invalid_argument &) {
			throw ErrorType::InvalidArrivalTimeFormat;
		}
		catch (std::out_of_range &) {
			throw ErrorType::ValueOutOfRange;
		}

		//make function
		//getPriority(stringstream &taskStream, tProcess &currProcess);
		std::string sPriority;
		std::getline(taskStream, sPriority, ',');
		try {
			currProcess.priority = std::stoi(sPriority);
		}
		catch (std::invalid_argument &) {
			throw ErrorType::InvalidArrivalTimeFormat;
		}
		catch (std::out_of_range &) {
			throw ErrorType::ValueOutOfRange;
		}

		processesList.emplace(currProcess);
	}

	isFileRead = true;
}