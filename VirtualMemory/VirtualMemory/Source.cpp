#include "VirtualTable.h"
#include "Process.h"
#include <iostream>
#include "MemoryDispatcher.h"

int main() {
	Process proc1;
	Process proc2;

	char choice;
	int counter = 0;
	while (true) {
		std::cout << "\n1 - Show virtual table\n";
		std::cout << "2 - Transfer virtual memory to physical\n";
		std::cout << "3 - Randomly add processes virtual pages to memory\n";
		std::cout << "4 - Print physical memory structure\n";
		std::cout << "Enter: ";
		unsigned procId;
		std::cin >> choice;
		switch (choice) {
		case '1':
			std::cout << "Enter proc number:";
			std::cin >> procId;
			switch (procId) {
			case 0:
				proc1.printVirtualTable();
				break;
			case 1:
				proc2.printVirtualTable();
				break;
			default:
				std::cout << "Invalid processor id entered\n";
				break;
			}
			break;
		case '2':
			unsigned address;
			std::cout << "Enter proc number:";
			std::cin >> procId;
			std::cout << "\nEnter address: ";
			std::cin >> address;
			PhysicalAddress addr1;
			switch (procId) {
			case 0:
				addr1 = proc1.getPhysicalAddress({(address >> 12), (address & 0x0FFF)});
				break;
			case 1:
				addr1 = proc2.getPhysicalAddress({ (address >> 12), (address & 0x0FFF)});
				break;
			default:
				std::cout << "Invalid processor id entered\n";
			}
			std::cout << "Physical memory: " << ((addr1.BlockNumber << 12) + addr1.offset) << std::endl;
			break;
		case '3':
			while (!MemoryDispatcher::get().isRamEmpty()) {
				unsigned addr = std::rand() % 32768;
				int procId = std::rand() % 2;
				if (procId == 0)
					proc1.getPhysicalAddress({ (addr >> 12), (addr & 0x0FFF) });
				else
					proc2.getPhysicalAddress({ (addr >> 12), (addr & 0x0FFF) });
			}
			break;
		case '4':
			MemoryDispatcher::get().printPhysicalMemory();
			break;
		default:
			break;
		}
		counter++;
		if (counter % 5 == 0)
			MemoryDispatcher::get().resetTablesUsage();
	}

	PhysicalAddress addr1 = proc1.getPhysicalAddress({ 3, 25 });
	PhysicalAddress addr2 = proc2.getPhysicalAddress({ 4, 10 });

	std::cout << addr1.BlockNumber << ": " << addr1.offset << std::endl;
	std::cout << addr2.BlockNumber << ": " << addr2.offset << std::endl;

	proc1.printVirtualTable();
	proc2.printVirtualTable();
	return 0;
}