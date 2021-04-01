#include "allocator.hpp"

#include <cassert>
#include <iostream>

Allocator::Allocator() {
	max_size_ = 0;
	offset_ = 0;
	start_ptr_ = nullptr;
}

Allocator::~Allocator() {
	delete[] start_ptr_;
}

void Allocator::makeAllocator(size_t max_size) {
	if (max_size == 0) return;

	if (start_ptr_ != nullptr) {
		delete[] start_ptr_;
		init(max_size);
	}
	else {
		init(max_size);
	}
}

char* Allocator::alloc(size_t size) {
	if (size == 0) return nullptr;
	if (offset_ + size > max_size_) return nullptr;

	auto old_offset = offset_;
	offset_ += size;

	return start_ptr_ + old_offset;
}

void Allocator::reset() {
	offset_ = 0;
}

size_t Allocator::get_offset() {
	return offset_;
}

size_t Allocator::get_max_size() {
	return max_size_;
}

void Allocator::init(size_t size) {
	start_ptr_ = new char[size];
	max_size_ = size;
	offset_ = 0;
}
