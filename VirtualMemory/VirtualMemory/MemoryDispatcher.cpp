#include "MemoryDispatcher.h"
#include <iostream>
#include <algorithm>
#include <iomanip>

void MemoryDispatcher::printPhysicalMemory() {
	vector<VirtTableRecord> loadedPages;
	
	for (auto & table : m_virtualTables) {
		for (auto &record : table.getTable())
			if (record.presence)
				loadedPages.push_back(record);
	}
		std::sort(loadedPages.begin(), loadedPages.end(), [](const VirtTableRecord &record1, const VirtTableRecord &record2) {
			return record1.pageBlock < record2.pageBlock;
		});
		std::cout << "---Physical memory used---\n";
		std::cout << "Start Address | page block" << std::endl;

		unsigned long pageNum = 0;
		for (auto &record : loadedPages) {
				std::cout << std::left << std::setw(20) << (pageNum++ << 12) <<
				std::setw(15) << std::left << record.pageBlock << std::endl;
		}
}

void MemoryDispatcher::resetTablesUsage() {
	for (auto &virtualTable : m_virtualTables) 
		virtualTable.resetTableUsage();
}


void MemoryDispatcher::addVirtualTable(const unsigned process_id) {
	if (process_id >= m_virtualTables.size())
		m_virtualTables.push_back(VirtualTable());
}

unsigned MemoryDispatcher::getPageBlock(const unsigned virtPage, const unsigned processId) {
	VirtTableRecord &virtTableRecord = m_virtualTables[processId].getVirtualPageRecord(virtPage);

	if (virtTableRecord.presence) {
		virtTableRecord.usage = 1;
		return virtTableRecord.pageBlock;
	}

	if (ramEmpty.size()) {
		RamPage ramPage = ramEmpty.back();
		ramEmpty.pop_back();

		std::cout << "Virtual page " << virtPage << "of process " << processId << " binded to physical block " << ramPage.pageNum << std::endl;

		loadedVirtualPages.push_back({virtPage, processId});
		virtTableRecord.pageBlock = ramPage.pageNum;  //update TABLE for processId
		virtTableRecord.presence = 1;
		virtTableRecord.usage = 1;
		return ramPage.pageNum;
	}

	auto pageToDeleteIt = std::min_element(loadedVirtualPages.begin(), loadedVirtualPages.end(), [this, &virtPage](const LoadedPage &loadedPage1, const LoadedPage &loadedPage2)
					{
						return (m_virtualTables[loadedPage1.processorNum].getVirtualPageRecord(loadedPage1.virtualPageNumber).usage <
							m_virtualTables[loadedPage1.processorNum].getVirtualPageRecord(loadedPage1.virtualPageNumber).usage);
					});

	m_virtualTables[pageToDeleteIt->processorNum].getVirtualPageRecord(pageToDeleteIt->virtualPageNumber).presence = 0;
	m_virtualTables[pageToDeleteIt->processorNum].getVirtualPageRecord(pageToDeleteIt->virtualPageNumber).usage = 0;
	m_virtualTables[pageToDeleteIt->processorNum].getVirtualPageRecord(pageToDeleteIt->virtualPageNumber).isOnDisk = 0;
	virtTableRecord.presence = 1;
	virtTableRecord.usage = 1;
	virtTableRecord.pageBlock = m_virtualTables[pageToDeleteIt->processorNum].getVirtualPageRecord(pageToDeleteIt->virtualPageNumber).pageBlock;

	if (m_virtualTables[pageToDeleteIt->processorNum].getVirtualPageRecord(pageToDeleteIt->virtualPageNumber).usage ||
		!m_virtualTables[pageToDeleteIt->processorNum].getVirtualPageRecord(pageToDeleteIt->virtualPageNumber).isOnDisk)
	{
		std::cout << "Upload virtual page " << pageToDeleteIt->virtualPageNumber << " of processor " << pageToDeleteIt->processorNum <<
			" -> physical block " << m_virtualTables[pageToDeleteIt->processorNum].getVirtualPageRecord(pageToDeleteIt->virtualPageNumber).pageBlock <<
			" to hard disk\n";
		m_virtualTables[pageToDeleteIt->processorNum].getVirtualPageRecord(pageToDeleteIt->virtualPageNumber).isOnDisk = 1;
	}
	else
		std::cout << "Get rid off virtual page " << pageToDeleteIt->virtualPageNumber << " of processor " << pageToDeleteIt->processorNum <<
		" -> physical block " << m_virtualTables[pageToDeleteIt->processorNum].getVirtualPageRecord(pageToDeleteIt->virtualPageNumber).pageBlock << std::endl;

	if (virtTableRecord.isOnDisk)
		std::cout << "Download virtual page " << virtPage << " of process " << processId << " from disk to memory block " << virtTableRecord.pageBlock << "\n";

	pageToDeleteIt->processorNum = processId;
	pageToDeleteIt->virtualPageNumber = virtPage;

	return virtTableRecord.pageBlock;
}