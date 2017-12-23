/* file cPriorityScheduler.h
* created: 29.09.17
* author: Levunets
*/
#pragma once
#include <vector>
#include <list>
#include <string>
#include <memory>
#include <sstream>

#include "iSchedulerBase.h"

struct tProcess {
	std::string name;
	int arrivalTime;
	int workTime;
	int delayTime;
	int finishTime = 0;
	bool isFinished = false;
	int priority;
};


class PriorityScheduler : public SchedulerBase<tProcess> {
public:
	PriorityScheduler();
	~PriorityScheduler() {};
	enum {
		QUEUES_COUNT = 3,
	};

	enum class ErrorType {
		CannotOpenInFile,
		CannotFindOutFile,
		InFileNotRead,
		InvalidInputFileFormat,
		InvalidArrivalTimeFormat,
		ValueOutOfRange,
		OUTFILE_WRITE_FAIL,
	};

	virtual void ParseSchedulerFile(const std::string &inFileName);
	virtual void BuildResFile(const std::string &outFileName);
	virtual void BuildExlResFile(const std::string &outFileName);
	virtual void RunScheduler();	
private:
	void getProcName(std::stringstream &taskStream, tProcess &currProcess);
	void getArrivalTime(std::stringstream &taskStream, tProcess &currProcess);
	void getWorkTime(std::stringstream &taskStream, tProcess &currProcess);
	void getPriority(std::stringstream &taskStream, tProcess &currProcess);
	bool calculateOneTick(tProcess &workProcess, int &timer, int &workTimeLeft);
	void processIncomeTasks(const int &timer);	//add tasks from GENERAL list to queues if their's time has come
	bool chooseWorkTask(tProcess &currWorkTask);
	std::vector<std::list<tProcess> > priorityQueues;
	
	bool isFileRead;
};