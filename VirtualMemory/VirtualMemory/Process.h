#pragma once
#include "VirtualTable.h"
#include "MemoryDispatcher.h"

enum {
	VirtualAddressSpace = 64,
	offsetSize = 12,
};

struct VirtualAddress {
	unsigned virtualPageNumber : 3;
	unsigned offset : offsetSize;
};

struct PhysicalAddress {
	unsigned BlockNumber : 3;
	unsigned offset : offsetSize;
};

class Process {
public:
	Process() : m_processId(m_processIdCounter++) {
		MemoryDispatcher::get().addVirtualTable(m_processId);
	};
	PhysicalAddress getPhysicalAddress(const VirtualAddress virtAddr);
	void printVirtualTable();

private:
	static unsigned m_processIdCounter;
	unsigned m_processId;
};