#include "Process.h"
#include "MemoryDispatcher.h"

unsigned Process::m_processIdCounter = 0;

PhysicalAddress Process::getPhysicalAddress(const VirtualAddress virtAddr) {
	unsigned pageBlock = MemoryDispatcher::get().getPageBlock(virtAddr.virtualPageNumber, m_processId);
	return {pageBlock, virtAddr.offset};
}

void Process::printVirtualTable() {
	VirtualTable virtTable = MemoryDispatcher::get().getVirtualTable(m_processId);

	std::cout << std::endl << "Virual table of processor " << m_processId << std::endl;
	virtTable.print();
}