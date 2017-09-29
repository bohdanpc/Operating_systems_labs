/* file cPriorityScheduler.h
* created: 29.09.17
* author: Levunets
*/
#pragma once
#include <vector>
#include <queue>
#include <string>
#include <memory>

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

enum class ErrorType {
	CannotOpenInFile,
	InvalidInputFileFormat,
	InvalidArrivalTimeFormat,
	ValueOutOfRange,
	OUTFILE_WRITE_FAIL,
};

class PriorityScheduler : public SchedulerBase<tProcess> {
public:
	virtual void ParseSchedulerFile(const std::string &inFileName);//тут визначатиметься кількість черг};
																	  //і читаються данні про всі процеси з файла
	virtual void BuildSchedulerResFile(const std::string &outFileName) {};
	virtual void RunScheduler() {};
private:

	//черги створюються динамічно залежно від кількості різних пріоритетів
	std::vector<std::unique_ptr<std::queue<tProcess> > > priorityQueues;
	bool isFileRead;
};