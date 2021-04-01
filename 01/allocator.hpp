#pragma once

#include <iostream>

class Allocator
{
public:
	Allocator();
	~Allocator();
	void makeAllocator(size_t maxSize);
	char* alloc(size_t size);
	void reset();
	size_t get_offset();
	size_t get_max_size();
private:
	// The total size of the memory in bytes.
	size_t max_size_;
	// offset from the beginning, the place where the not yet occupied memory is located.
	ptrdiff_t offset_;
	char* start_ptr_;

	void init(size_t size);
};
