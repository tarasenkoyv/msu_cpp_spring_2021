#include "allocator.hpp"

#include <malloc.h>

#include <cassert>
#include <iostream>

Allocator::Allocator() {
	max_size_ = 0;
	occupied_size_ = 0;

	start_ptr = nullptr;
	current_ptr = nullptr;
}

Allocator::~Allocator() {
	free(start_ptr);
	start_ptr = nullptr;
	current_ptr = nullptr;
	instance_ = nullptr;
}

void Allocator::MakeAllocator(size_t max_size) {
	if (start_ptr != nullptr) {
		grow(max_size);
	}
	else {
		init(max_size);
	}
}

char* Allocator::Alloc(size_t size) {
	char* new_ptr = nullptr;

	if (occupied_size_ + size <= max_size_) {
		new_ptr = occupy(size);
	}

	return new_ptr;
}

void Allocator::Reset() {
	occupied_size_ = 0;
	current_ptr = start_ptr;
}

size_t Allocator::get_occupied_size() {
	return occupied_size_;
}

size_t Allocator::get_max_size() {
	return max_size_;
}

char* Allocator::occupy(size_t size) {
	if (size == 0) return nullptr;

	assert(occupied_size_ + size <= max_size_);

	occupied_size_ += size;

	char* new_ptr = current_ptr;
	current_ptr += size;

	return new_ptr;
}

void Allocator::init(size_t size) {
	start_ptr = static_cast<char*>(std::malloc(size));
	if (start_ptr == nullptr) {
		throw std::bad_alloc();
	}
	else {
		current_ptr = start_ptr;
		max_size_ = size;
	}
}
void Allocator::grow(size_t size) {
	if (size == 0) return;

	char* new_start_ptr;
	size_t new_size = max_size_ + size;
		
	new_start_ptr = static_cast<char*>(std::realloc(start_ptr, new_size));
	if (new_start_ptr != nullptr) {
		start_ptr = new_start_ptr;
		current_ptr = start_ptr + occupied_size_;
		max_size_ = new_size;
	}
	else {
		//std::cout << "grow: failed to allocate memory." << std::endl;
		throw std::bad_alloc();
	}
}

Allocator* Allocator::instance_ = nullptr;;

Allocator* Allocator::GetInstance()
{
	if (instance_ == nullptr) {
		instance_ = new Allocator();
	}
	return instance_;
}
