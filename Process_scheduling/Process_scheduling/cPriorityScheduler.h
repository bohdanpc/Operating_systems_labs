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
		InFileNotRead,
		InvalidInputFileFormat,
		InvalidArrivalTimeFormat,
		ValueOutOfRange,

		OUTFILE_WRITE_FAIL,
	};

	virtual void ParseSchedulerFile(const std::string &inFileName);//��� ��������������� ������� ����};
																	  //� ��������� ���� ��� �� ������� � �����
	virtual void BuildSchedulerResFile(const std::string &outFileName) {};
	virtual void RunScheduler();		//�������� �� ���� � ���� ��������
										//�������� ��������� ������� ���� ������� �� ������� ����� ���������
										//   ... std::unique_ptr<std::queue<tProcess> > queue1(new std::queue); ???
										//���... priorityQUeues.push_back(std::move(queue1));
private:
	void getProcName(std::stringstream &taskStream, tProcess &currProcess);
	void getArrivalTime(std::stringstream &taskStream, tProcess &currProcess);
	void getWorkTime(std::stringstream &taskStream, tProcess &currProcess);
	void getPriority(std::stringstream &taskStream, tProcess &currProcess);

	//����� ����������� �������� ������� �� ������� ����� ���������
	std::vector<std::list<tProcess> > priorityQueues;
	
	bool isFileRead;
};