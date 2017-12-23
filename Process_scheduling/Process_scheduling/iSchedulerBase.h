/* file iSchedulerBase.h
 * created: 29.09.17
 * author: Levunets
 */
#pragma once
#include <string>
#include <fstream>
#include <queue>

template <class processType>
class SchedulerBase {
public:
	virtual void ParseSchedulerFile(const std::string &inFileName) = 0;
	virtual void BuildResFile(const std::string &outFileName) = 0;
	virtual void BuildExlResFile(const std::string &outFileName) = 0;
	virtual void RunScheduler() = 0;
protected:
	std::list<processType> processesList;
	std::list<processType> outputProcessesList;
};
