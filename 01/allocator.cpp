#include "allocator.hpp"

#include <malloc.h>

#include <cassert>
#include <iostream>

Allocator::Allocator() {
	max_size_ = 0;
	occupied_size_ = 0;

	start_ptr_ = nullptr;
	current_ptr_ = nullptr;
}

Allocator::~Allocator() {
	free(start_ptr_);
	start_ptr_ = nullptr;
	current_ptr_ = nullptr;
	instance_ = nullptr;
}

void Allocator::MakeAllocator(size_t max_size) {
	if (start_ptr_ != nullptr) {
		Grow(max_size);
	}
	else {
		Init(max_size);
	}
}

char* Allocator::Alloc(size_t size) {
	char* new_ptr = nullptr;

	if (occupied_size_ + size <= max_size_) {
		new_ptr = Occupy(size);
	}

	return new_ptr;
}

void Allocator::Reset() {
	occupied_size_ = 0;
	current_ptr_ = start_ptr_;
}

size_t Allocator::get_occupied_size() {
	return occupied_size_;
}

size_t Allocator::get_max_size() {
	return max_size_;
}

char* Allocator::Occupy(size_t size) {
	if (size == 0) return nullptr;

	assert(occupied_size_ + size <= max_size_);

	occupied_size_ += size;

	char* new_ptr = current_ptr_;
	current_ptr_ += size;

	return new_ptr;
}

void Allocator::Init(size_t size) {
	start_ptr_ = static_cast<char*>(std::malloc(size));
	if (start_ptr_ == nullptr) {
		throw std::bad_alloc();
	}
	else {
		current_ptr_ = start_ptr_;
		max_size_ = size;
	}
}
void Allocator::Grow(size_t size) {
	if (size == 0) return;

	char* new_start_ptr;
	size_t new_size = max_size_ + size;
		
	new_start_ptr = static_cast<char*>(std::realloc(start_ptr_, new_size));
	if (new_start_ptr != nullptr) {
		start_ptr_ = new_start_ptr;
		current_ptr_ = start_ptr_ + occupied_size_;
		max_size_ = new_size;
	}
	else {
		//std::cout << "Grow: failed to allocate memory." << std::endl;
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
