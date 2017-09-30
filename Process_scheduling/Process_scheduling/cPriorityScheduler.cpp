#include "cPriorityScheduler.h"
#include <fstream>

PriorityScheduler::PriorityScheduler() : priorityQueues(QUEUES_COUNT, std::list<tProcess>()) {}


void PriorityScheduler::getProcName(std::stringstream &taskStream, tProcess &currProcess) {
	std::getline(taskStream, currProcess.name, ',');
	if (currProcess.name == "")
		throw ErrorType::InvalidInputFileFormat;
}


void PriorityScheduler::getArrivalTime(std::stringstream &taskStream, tProcess &currProcess) {
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
}


void PriorityScheduler::getWorkTime(std::stringstream &taskStream, tProcess &currProcess) {
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
}


void PriorityScheduler::getPriority(std::stringstream &taskStream, tProcess &currProcess) {
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
}


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
	
		getProcName(taskStream, currProcess);
		getArrivalTime(taskStream, currProcess);
		getWorkTime(taskStream, currProcess);
		getPriority(taskStream, currProcess);

		taskStream.clear();
		processesList.push_back(currProcess);
	}

	isFileRead = true;
}


void PriorityScheduler::RunScheduler() {
	if (!isFileRead)
		throw ErrorType::InFileNotRead;

	processesList.sort([](const tProcess &a, const tProcess &b) {
		return a.arrivalTime < b.arrivalTime;
	});

	int timer = 0;
	tProcess task;
	tProcess currWorkTask;
	int timeLeftToWorkOn = 0;
	bool isFree = true;

	while (true) {
		if (processesList.size())
			task = processesList.front();
		
		//check for events to add to priority Queues
		while (task.arrivalTime == timer && processesList.size()) {
			processesList.pop_front();

			if (task.priority < QUEUES_COUNT) 
				priorityQueues[task.priority].push_back(task);
			else
				priorityQueues[QUEUES_COUNT-1].push_back(task);

			if (processesList.size())
				task = processesList.front();
		}

		//choose task to work on working on task
		if (isFree) {
			for (int i = 0; i < QUEUES_COUNT; i++) {
				if (priorityQueues[i].size()) {
					currWorkTask = priorityQueues[i].front();
					priorityQueues[i].pop_front();
					isFree = false;
					break;
				}
			}
			if (isFree && processesList.size()) {
				timer++;
				continue;
			}
			else
				if (isFree)
					break;

			currWorkTask.delayTime = timer - currWorkTask.arrivalTime;
			timeLeftToWorkOn = currWorkTask.workTime;
			
			timeLeftToWorkOn--;
			timer++;
			if (timeLeftToWorkOn == 0) {
				currWorkTask.finishTime = timer;
				currWorkTask.isFinished = true;
				outputProcessesList.push_back(currWorkTask);
				isFree = true;
			}
		}
		else {
			timeLeftToWorkOn--;
			timer++;
			if (timeLeftToWorkOn == 0) {
				currWorkTask.finishTime = timer;
				currWorkTask.isFinished = true;
				outputProcessesList.push_back(currWorkTask);
				isFree = true;
			}
		}
	}

	int a;
	a = 5;
}