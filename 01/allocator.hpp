#pragma once

#include <iostream>

class Allocator
{
protected:
	Allocator();
	static Allocator* instance_;
public:
	~Allocator();
	Allocator(Allocator& other) = delete;
	void operator=(const Allocator&) = delete;
	static Allocator* GetInstance();
	void MakeAllocator(size_t maxSize);
	char* Alloc(size_t size);
	void Reset();
	size_t get_occupied_size();
	size_t get_max_size();
private:
	// The total size of the memory in bytes.
	size_t max_size_;
	// The total number of occupied bytes.
	size_t occupied_size_;

	char* start_ptr;
	char* current_ptr;

	char* occupy(size_t size);
	void init(size_t size);
	void grow(size_t size);
};
