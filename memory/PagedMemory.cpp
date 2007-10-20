#include "PagedMemory.h"

#include <iostream>

PagedMemory::PagedMemory(Memory *engine, size_t page_size_exp):
page_size_exp(page_size_exp)
{
	this->engine = engine;
	page_size = 1 << this->page_size_exp;
}

std::pair<addr_t,size_t> PagedMemory::virtual_to_physical(addr_t virtual_addr, bool create_on_demand) {
	std::cout << " Mapping address " << std::hex << virtual_addr << std::endl;
	addr_t virtual_aligned = virtual_addr / page_size;
	addr_t offset = virtual_addr % page_size;
	addr_t page_start;
	std::map<addr_t,addr_t>::iterator it = this->page_index.find(virtual_aligned);
	if (it == this->page_index.end()) {
		std::cout << "  Page does not exist" << std::endl;
		if (create_on_demand) {
			//Page not found; insert it as requested
			page_start = page_index.size() * page_size;
			std::cout << "  Creating page at " << page_start << " ... ";
			this->page_index.insert(std::pair<addr_t,addr_t>(virtual_aligned, page_start));
			std::cout << "done" << std::endl;
		} else {
			std::cout << "  !!!Not creating page as requested" << std::endl;
			page_start = 0;
		}
	} else {
		page_start = (*it).second;
		std::cout << "  page exists, mapped to " << page_start << std::endl;
	}
	std::cout << std::dec;
	return std::pair<addr_t,size_t>(page_start + offset, (size_t)(page_size - offset));
}

std::string PagedMemory::read(addr_t addr, size_t bytes)
{
	std::string ret;
	size_t left;

	std::cout << "Read of " << bytes << " bytes at " << std::hex << addr << std::endl;
	
	//for reads spanning multiple pages, we have to perform multiple reads,
	//one for each page, and concatenate the result
	while (bytes != 0) {
		std::pair<addr_t,size_t> page_result = virtual_to_physical(addr);
		addr_t real_addr = page_result.first;
		left = page_result.second;
		std::cout << "Mapped virtual " << std::hex << addr << " to real " <<
			real_addr << "; " << std::dec << left << " bytes left" << std::endl;
		if (left > bytes)
			left = bytes;
		ret += engine->read(real_addr, left);
		//if this is last read, then "left" _was_ > bytes, so it was set to "bytes"
		//2 lines above, so bytes - left = 0, which causes the while-loop to stop
		bytes -= left;
		addr += left;
	}

	return ret;
}

void PagedMemory::write(addr_t addr, std::string data) 
{
	std::string to_write;
	size_t pos = 0;
	size_t left;
	size_t bytes = data.size();

	std::cout << "Write of " << bytes << " bytes at " << std::hex << addr << std::endl;

	//for writes spanning multiple pages, we need to perform multiple writes,
	//one for each page, using correct subsequence of bytes from "data"
	while (bytes != 0) {
		std::pair<addr_t,size_t> page_result = virtual_to_physical(addr);
		addr_t real_addr = page_result.first;
		left = page_result.second;
		std::cout << "Mapped virtual " << std::hex << addr << " to real " <<
			real_addr << "; " << std::dec << left << " bytes left" << std::endl;

		if (left > bytes)
			left = bytes;
		std::cout << "Writing " << std::dec << left << " bytes downstream ... ";
		engine->write(real_addr, data.substr(pos, pos+left));
		std::cout << "done" << std::endl;
		pos += left;
		addr += left;
		//if this is last read, then "left" _was_ > bytes, so it was set to "bytes"
		//3 lines above, so bytes - left = 0, which causes the while-loop to stop
		bytes -= left;
	}
}

