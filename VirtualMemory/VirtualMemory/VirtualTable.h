#pragma once
#include <vector>
#include <exception>
#include <iostream>
#include <iomanip>

using std::vector;

struct VirtTableRecord {
	VirtTableRecord() : presence(0), usage(0), isOnDisk(0), pageBlock(0) {};
	unsigned presence : 1;
	unsigned usage : 1;
	unsigned isOnDisk : 1;
	unsigned pageBlock : 3;
};


class VirtualTable {
public:
	VirtualTable() : pageTable(VirtualTableSize) {};
	void resetTableUsage() {
		for (auto &record : pageTable) 
			record.usage = 0;
	}
	static const int VirtualTableSize = 8;

	VirtTableRecord &getVirtualPageRecord(const unsigned pageNumber) {
		if (pageNumber < VirtualTableSize)
			return pageTable[pageNumber];
		throw std::exception("Invalid page number to convert\n");
	}

	void print() {
		std::cout << "Start Address | Page num | page block | presence bit | usage bit" << std::endl;
		int pageNum = 0;
		for (auto &page : pageTable) {
			unsigned long number = pageNum << 12;
			std::cout << std::left << std::setw(20) << number << 
				std::setw(10) << std::left << pageNum++ << 
				std::setw(15) << std::left << page.pageBlock <<
				std::setw(15) << std::left << page.presence << 
				std::setw(15) << std::left << page.usage << std::endl;
		}
	}
	vector<VirtTableRecord> &getTable() {
		return pageTable;
	}
private:
	vector<VirtTableRecord> pageTable;
};