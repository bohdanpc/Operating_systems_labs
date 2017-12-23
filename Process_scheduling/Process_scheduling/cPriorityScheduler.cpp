#include "cPriorityScheduler.h"
#include <fstream>
#include <iomanip>

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
		if (field[0] == '#')
			continue;

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

/*
* analyse all queues and choose task with the highest possible priority
* @return:
*   TRUE if task was selected and FALSE if there's no tasks in queues
*/
bool PriorityScheduler::chooseWorkTask(tProcess &currWorkTask) {
	for (int i = 0; i < QUEUES_COUNT; i++) 
		if (priorityQueues[i].size()) {
			currWorkTask = priorityQueues[i].front();
			priorityQueues[i].pop_front();
			return false;
		}
	return true;
}


bool PriorityScheduler::calculateOneTick(tProcess &workProcess, int &timer, int &workTimeLeft) {
	workTimeLeft--;
	timer++;
	if (workTimeLeft == 0) {
		workProcess.finishTime = timer;
		workProcess.isFinished = true;
		outputProcessesList.push_back(workProcess);
		return true;
	}
	return false;
}


void PriorityScheduler::processIncomeTasks(const int &timer) {
	tProcess task;

	if (processesList.size())
		task = processesList.front();
	else return;

	//check for events to add to priority Queues
	while (task.arrivalTime == timer) {
		processesList.pop_front();

		if (task.priority < QUEUES_COUNT)
			priorityQueues[task.priority].push_back(task);
		else
			priorityQueues[QUEUES_COUNT - 1].push_back(task);

		if (processesList.size())
			task = processesList.front();
		else break;
	}
}


void PriorityScheduler::RunScheduler() {
	if (!isFileRead)
		throw ErrorType::InFileNotRead;

	processesList.sort([](const tProcess &a, const tProcess &b) {
		return a.arrivalTime < b.arrivalTime;
	});

	int timer = 0;
	tProcess currWorkTask;
	int workTimeLeft = 0;
	bool isFree = true;

	while (true) {
		processIncomeTasks(timer);

		//choose task to work on working on task
		if (isFree) {
			isFree = chooseWorkTask(currWorkTask);
			if (isFree && processesList.size()) {
				timer++;
				continue;
			}
			else
				if (isFree)
					break;

			currWorkTask.delayTime = timer - currWorkTask.arrivalTime;
			workTimeLeft = currWorkTask.workTime;
			
			isFree = calculateOneTick(currWorkTask, timer, workTimeLeft);
		}
		else 
			isFree = calculateOneTick(currWorkTask, timer, workTimeLeft);
	}
}


void PriorityScheduler::BuildExlResFile(const std::string &outFileName) {
	std::ofstream fout(outFileName);

	if (!fout) {
		throw ErrorType::CannotFindOutFile;
	}

	fout << "#Process name(Priority), Arrival Time, Delay Time, Working Time" << std::endl;
	for (auto &task : outputProcessesList) {
		fout << task.name << "(" << task.priority << "), "
			<< task.arrivalTime << ", " << task.delayTime << ", "
			<< task.workTime << std::endl;
	}
}


void PriorityScheduler::BuildResFile(const std::string &outFileName) {
	std::ofstream fout(outFileName);

	if (!fout) {
		throw ErrorType::CannotFindOutFile;
	}

	fout << std::setw(15) << "#Process name(Priority)," <<
			std::setw(15) << "Arrival Time, " << 
			std::setw(15) << "Delay Time, " <<
			std::setw(15) << "Working Time, " <<
			std::setw(15) << "Finish Time, " << std::endl;

	for (auto &task : outputProcessesList) {
		fout << std::setw(15) << task.name << "(" << task.priority << "), "
			<< std::setw(15) << task.arrivalTime << ", " << std::setw(15) << task.delayTime << ", "
			<< std::setw(15) <<task.workTime << ", " << std::setw(15) << task.finishTime << std::endl;
	}
}
