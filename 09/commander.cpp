#include <thread>
#include <iostream>
#include <fstream>

#include "commander.hpp"
#include "sort_thread.hpp"
#include "utils.hpp"

Commander::Commander(const uint64_t mem_size, const uint64_t max_thread_count, const std::string& unsorted_file, 
					 const std::string& sorted_file, bool verbose) :
	thread_count_(max_thread_count),
	mem_mb_size_(mem_size* MB),
	unsorted_file_(unsorted_file),
	sorted_file_(sorted_file),
	verbose_(verbose)

{
	std::ifstream in(unsorted_file_, std::ifstream::ate | std::ifstream::binary);
	file_size_ = in.tellg();
	in.close();

	mem_mb_size_ = mem_mb_size_ > file_size_ ? file_size_ : mem_mb_size_;

	while (mem_mb_size_ % (thread_count_ * sizeof(uint64_t)) != 0 && thread_count_ > 1) {
		thread_count_--;
	}

	if (thread_count_ == 1) { // if we have only one thread, memory must divide on two cause in merge place sort 
		mem_mb_size_ = mem_mb_size_ / 2;
	}

	locker_ = new std::mutex();
}

void Commander::Sort() {
	SortBlocks();

	if (!(file_size_ <= mem_mb_size_ && thread_count_ == 1)) {
		MergeSortedBlocks();
	}
}

void Commander::SortBlocks() {
	std::ofstream ofs(sorted_file_, std::ios::binary | std::ios::out);
	ofs.seekp(file_size_ - 1);
	ofs.write("", 1);
	ofs.close();

	uint64_t each_thread_mbytes = mem_mb_size_ / thread_count_;

	for (uint64_t seek = 0; seek < file_size_; seek += mem_mb_size_) {

		uint64_t seek_in_bytes = 0;
		std::vector <std::thread> threads;

		uint64_t actual_thread_count = 0;
		for (size_t i = 0; i < thread_count_ && file_size_ > seek_in_bytes + seek; i++, actual_thread_count++) {

			uint64_t number_count = each_thread_mbytes / sizeof(uint64_t);

			if (seek_in_bytes + seek + each_thread_mbytes > file_size_) {
				number_count = (file_size_ - (seek_in_bytes + seek)) / sizeof(uint64_t);
			}

			SortThread* st = new SortThread(sorted_file_, unsorted_file_, number_count, seek + seek_in_bytes, locker_);
			threads.emplace_back(SortThread::Run, (void*)st);
			
			seek_in_bytes += each_thread_mbytes;
		}
		for (size_t i = 0; i < actual_thread_count; i++) {
			threads[i].join();
		}
	}
}

void Commander::ReadBinaryFile(std::vector<uint64_t>* buffer, uint64_t bpos, uint64_t pos, uint64_t bytes, std::ifstream& io) {
	io.seekg(pos, std::ios_base::beg);
	io.read(reinterpret_cast<char*>(&(*buffer)[bpos]), bytes);
}

void Commander::WriteBinaryFile(std::vector<uint64_t>* buffer, uint64_t bpos, uint64_t size, uint64_t pos, std::ofstream& io) {
	io.seekp(pos, std::ios_base::beg);
	io.write(reinterpret_cast<const char*>(&(*buffer)[bpos]), size * sizeof(uint64_t));
	io.flush();
}

void Commander::MergeSortedBlocks() {
	uint64_t merge_block_size = mem_mb_size_ / thread_count_;
	uint64_t block_count = file_size_ / merge_block_size;

	std::ofstream out(sorted_file_, std::ios::binary | std::ios::out | std::ios::in);
	std::ifstream in(sorted_file_, std::ios::binary | std::ios::in);

	uint64_t half_size = merge_block_size / sizeof(uint64_t);

	std::vector<uint64_t>* block = new std::vector<uint64_t>(half_size + half_size);
	std::vector<uint64_t>* buffer = new std::vector<uint64_t>(half_size + half_size);

	for (uint64_t gpos = 0; gpos < (uint64_t)(block_count / 2); gpos++) {

		ReadBinaryFile(block, 0, gpos * merge_block_size, merge_block_size, in);

		for (uint64_t lpos = gpos + 1; lpos < block_count; lpos++) {
			uint64_t read_block_size = (lpos + 1) * merge_block_size > file_size_ ? file_size_ - (lpos)*merge_block_size : merge_block_size;
			ReadBinaryFile(block, half_size, lpos * merge_block_size, read_block_size, in);
			Utils::MergeSortVectors(block, buffer, half_size, half_size + read_block_size / sizeof(uint64_t));
			std::vector<uint64_t>* v = block;
			block = buffer;
			buffer = v;
			WriteBinaryFile(block, half_size, read_block_size / sizeof(uint64_t), lpos * merge_block_size, out);
		}

		WriteBinaryFile(block, 0, half_size, gpos * merge_block_size, out);

		uint64_t read_block_size = (block_count - gpos) * merge_block_size > file_size_ ? file_size_ - (block_count - 1 - gpos) * merge_block_size : merge_block_size;
		ReadBinaryFile(block, half_size, (block_count - 1 - gpos) * merge_block_size, read_block_size, in);

		for (int lpos = block_count - 2 - gpos; lpos >= 0; lpos--) {
			ReadBinaryFile(block, 0, lpos * merge_block_size, merge_block_size, in);

			Utils::MergeSortVectors(block, buffer, read_block_size / sizeof(uint64_t), half_size + read_block_size / sizeof(uint64_t));

			std::vector<uint64_t>* v = buffer;
			buffer = block;
			block = v;

			WriteBinaryFile(block, 0, half_size, lpos * merge_block_size, out);
		}
		WriteBinaryFile(block, half_size, read_block_size / sizeof(uint64_t), (block_count - 1 - gpos) * merge_block_size, out);
		if (verbose_) {
			std::cout << (int)(100 * gpos / (block_count / 2)) << "%" << std::endl;
		}
	}

	delete block;
	delete buffer;

	out.close();
	in.close();
}

Commander::~Commander() {
	delete locker_;
}