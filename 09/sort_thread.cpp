#include <iostream>
#include <fstream>
#include <vector>

#include "sort_thread.hpp"
#include "utils.hpp"

SortThread::SortThread(const std::string& out_file, const std::string& in_file, const uint64_t number_count, const uint64_t seek, std::mutex* locker) :
	st_seek(seek),
	st_out_file(out_file),
	st_in_file(in_file),
	st_number_count(number_count),
	st_locker(locker)

{
	st_mem_size = number_count * sizeof(uint64_t);
}

void* SortThread::Run(void* obj) {
	((SortThread*)obj)->ReadBlockSortAndWrite();
	delete ((SortThread*)obj);
	return NULL;
}

void SortThread::ReadBlockSortAndWrite() {

	std::ifstream in(st_in_file, std::ios::binary | std::ios::in);

	std::vector<uint64_t> buff(st_number_count);

	in.seekg(st_seek);
	in.read(reinterpret_cast<char*>(buff.data()), st_mem_size);
	in.close();

	qsort(&buff[0], buff.size(), sizeof(uint64_t), Compare);

	st_locker->lock();

	std::ofstream out(st_out_file, std::ios::binary | std::ios::out | std::ios::in);
	out.seekp(st_seek);
	out.write(reinterpret_cast<const char*>(&buff[0]), st_mem_size);
	out.close();

	st_locker->unlock();
}

void SortThread::Print() {
	std::cout << "Seek : " << st_seek << " number count : " << st_mem_size / sizeof(uint64_t) << std::endl;
}