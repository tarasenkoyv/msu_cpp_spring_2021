#pragma once

#include "allocator.hpp"
#include "iterator.hpp"

template <class T, class Alloc = Allocator<T>>
class Vector {
private:
	size_t size_;
	Alloc alloc_;
	T* ptr_;
	size_t capacity_;
public:
	using iterator = Iterator<T>;

	Vector();
	Vector(size_t count);
	Vector(const std::initializer_list<T>& list);
	~Vector();

	void resize(size_t new_size);
	void reserve(size_t new_cap);
	void clear();
	void push_back(const T& val);
	void pop_back();

	template <class... Args>
	T& emplace_back(Args&&... args);

	const T& operator[](size_t i) const;
	T& operator[](size_t i);

	size_t size() const;
	size_t capacity() const;
	bool empty() const;

	iterator begin();
	iterator end();
	iterator rbegin();
	iterator rend();
};

#include "my_vector.tpp"

