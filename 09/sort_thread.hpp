#pragma once

#include <mutex>

class SortThread {
private:
	// Block size of thread
	uint64_t st_mem_size;
	// Position of the memory where we write block
	uint64_t st_seek;

	std::string st_out_file;
	std::string st_in_file;

	uint64_t st_number_count;
	std::mutex* st_locker;
public:

	SortThread(const std::string& out_file, const std::string& in_file, const uint64_t number_count, const uint64_t seek, std::mutex* locker);

	static void* Run(void* obj);
	void ReadBlockSortAndWrite();
	void Print();
};