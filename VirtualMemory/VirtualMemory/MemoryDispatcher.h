#pragma once
#include "VirtualTable.h"
#include <list>

enum {
	physicalBlockCount = 8,
};

struct RamPage {
	unsigned pageNum : 3;
};

struct LoadedPage {
	unsigned virtualPageNumber : 3;
	unsigned processorNum;
};


class MemoryDispatcher {
public:
	static MemoryDispatcher &get() {
		static MemoryDispatcher memDispatcher;
		return memDispatcher;
	}

	VirtualTable getVirtualTable(const unsigned processId) {
		if (processId < m_virtualTables.size())
			return m_virtualTables[processId];
		return VirtualTable();
	}

	unsigned getPageBlock(unsigned virtPage, unsigned m_processId);
	void printPhysicalMemory();
	void addVirtualTable(const unsigned process_id);
	void resetTablesUsage();
	bool isRamEmpty() {
		return !ramEmpty.size();
	}
private:
	MemoryDispatcher() {
		for (unsigned i = 0; i < physicalBlockCount; i++)
			ramEmpty.push_back({ i });
	};

	MemoryDispatcher(const MemoryDispatcher &&) = delete;
	
	std::vector<VirtualTable> m_virtualTables;
	std::list<RamPage> ramEmpty;
	std::list<LoadedPage> loadedVirtualPages;
};