#include <iostream>
#include <fstream>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>

using namespace std;

std::mutex MUT;
std::condition_variable CV;
std::condition_variable CV_cashier;
std::condition_variable CV_checker;

std::queue<unsigned int> m_cashRequest;
unsigned int m_cashSum;
std::map<unsigned int, unsigned int> m_monetaryCount;

std::queue<unsigned int> requests;

bool CALCULATE_CASH = false;
bool AVAILABLE_CHECKER = false;

bool isEmpty() {
	return !m_cashRequest.size();
}

bool isNewRequest() {
	return (m_cashRequest.size() > 0) && AVAILABLE_CHECKER;
}

bool isGetCash() {
	return CALCULATE_CASH;
}


bool readCashDenomination() {
	std::ifstream fin("cash.dat");
	int denomination;
	int value;

	while (fin >> denomination >> value) {
		if (denomination <= 0 || value <= 0)
			return false;
		m_monetaryCount[static_cast<unsigned int>(denomination)] = static_cast<unsigned int>(value);
		m_cashSum += static_cast<unsigned int>(denomination * value);
	}
	if (!fin.eof())
		return false;
	return true;
}


bool readCash() {
	std::ifstream fin("queue.dat");
	int value;

	while (fin >> value) {
		if (value <= 0)
			return false;
		requests.push(static_cast<unsigned int>(value));
	}
	if (!fin.eof())
		return false;
	return true;
}

void runClient() {
	if (!readCash()) {
		cerr << "Parsing cash file error!\n";
		exit(1);
	}

	while (requests.size()) {
		std::unique_lock<std::mutex> lock_unique(MUT);
		//cout << "1\n";
		CV.wait(lock_unique, isEmpty);
		
		cout << "\nClient requests to get " << requests.front() << " grn" << endl;
		m_cashRequest.push(requests.front());
		requests.pop();

		AVAILABLE_CHECKER = true;
		lock_unique.unlock();
		CV_checker.notify_one();
	}
	//exit(1);
}

void runChecker() {
	while (1) {
		std::unique_lock<std::mutex> lock_unique(MUT);
		CV_checker.wait(lock_unique, isNewRequest);

		bool notifyCashier = true;
		if (m_cashRequest.front() <= m_cashSum) {
			CALCULATE_CASH = true;
			cerr << "There's enough cash left to give " << m_cashRequest.front() << " grn\n";
		}
		else {
			cerr << "There's no enough cash left to give " << m_cashRequest.front() << " grn\n";
			CALCULATE_CASH = false;
			m_cashRequest.pop();
		}
		AVAILABLE_CHECKER = false;

		lock_unique.unlock();

		if (CALCULATE_CASH)
			CV_cashier.notify_one();
		else 
			CV.notify_one();
	}
}


void getBillToSum(const unsigned int cash, unsigned int &resCash, const unsigned int currBill) {
	while (cash - resCash >= currBill) {
		auto it = m_monetaryCount.find(currBill);
		if (it != m_monetaryCount.end() && it->second) {
			resCash += it->first;
			it->second--;
			continue;
		}
		break;
	}
}
void runCashier() {
	if (!readCashDenomination()) {
		cerr << "Cannot read Cash Denomination\n";
		exit(1);
	}

	while (1) {
		std::unique_lock<std::mutex> lock_unique(MUT);
		//cout << "\n3\n";
		CV_cashier.wait(lock_unique, isGetCash);

		//main alg
		unsigned int cash = m_cashRequest.back();
		unsigned int resCash = 0;
		std::map<unsigned int, unsigned int> bufCashMap = m_monetaryCount;
		getBillToSum(cash, resCash, 100);
		getBillToSum(cash, resCash, 50);
		getBillToSum(cash, resCash, 20);
		getBillToSum(cash, resCash, 10);
		getBillToSum(cash, resCash, 5);
		getBillToSum(cash, resCash, 2);
		getBillToSum(cash, resCash, 1);

		if (cash != resCash) {
			m_monetaryCount = bufCashMap;
			cerr << "ERROR: Cannot form appropriate sum from given denominators\n";
		}
		else {
			auto oldIt = bufCashMap.begin();
			auto newIt = m_monetaryCount.begin();
			cout << "\nSuccefully received : " << resCash << " cash\n";
			cout << "Used denominator: \n";
			for (; oldIt != bufCashMap.end(), newIt != m_monetaryCount.end(); oldIt++, newIt++) {
				unsigned int diff = oldIt->second - newIt->second;
				if (diff)
					cout << newIt->first << "grn - " << diff << " times\n";
			}
			m_cashSum -= cash;
		}

		m_cashRequest.pop();
		CALCULATE_CASH = false;
		lock_unique.unlock();
		CV.notify_one();
	}
}

int main() {
	std::thread tCheck(runChecker);
	std::thread tClient(runClient);
	std::thread tCash(runCashier);

	tCash.join();
	tCheck.join();
	tClient.join();
	return 0;
}