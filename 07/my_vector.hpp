#pragma once

template <class T>
class Allocator {

public:
	Allocator() {}
	~Allocator() {}
	using value_type = T;
	using pointer = T*;

	pointer allocate(size_t count) {
		if (count == 0) {
			return nullptr;
		}
		return new value_type[count];
	}

	void construct(pointer ptr, const value_type& val) {
		new (ptr) value_type(val);
	}

	void deallocate(pointer ptr) {
		delete[] ptr;
	}

	void destruct(pointer ptr, size_t count) {

		for (size_t i = 0; i < count; ++i) {
			ptr[i].~value_type();
		}
	}

	size_t max_size() const noexcept {
		return std::numeric_limits<size_t>::max();
	}
};

template <class T>
class Iterator {
private:
	T* ptr_;
	bool rev_f_;
public:
	using difference_type = std::ptrdiff_t;
	using reference = T&;
	explicit Iterator(T* ptr, bool rev = false) : ptr_(ptr), rev_f_(rev) {}

	Iterator& operator++() {
		rev_f_ ? --ptr_ : ++ptr_;
		return *this;
	}

	Iterator operator+(size_t n) const {
		auto iter = rev_f_ ?  Iterator(ptr_ - n) : Iterator(ptr_ + n);
		return iter;
	}

	difference_type operator-(const Iterator<T>& other) const {
		return std::distance(ptr_, other.ptr_);
	}

	Iterator operator-(size_t n) const {
		auto iter =  rev_f_ ? Iterator(ptr_ + n) : Iterator(ptr_ - n);
		return iter;
	}

	Iterator& operator--() {
		rev_f_ ? ptr_++ : ptr_--;
		return *this;
	}

	bool operator==(const Iterator<T>& other) const {
		return ptr_ == other.ptr_;
	}

	bool operator!=(const Iterator<T>& other) const {
		return !(*this == other);
	}

	reference operator*() const {
		return *ptr_;
	}

};

template <class T, class Alloc = Allocator<T>>
class Vector {
private:
	size_t size_;
	Alloc alloc_;
	T* ptr_;
	size_t capacity_;
public:
	using iterator = Iterator<T>;

	Vector() : size_(0), ptr_(alloc_.allocate(0)), capacity_(0) {}

	Vector(size_t size) : size_(size), ptr_(alloc_.allocate(size)), capacity_(size) {}

	Vector(const std::initializer_list<T>& list) : Vector(list.size()) {
		size_t count = 0;
		for (auto& element : list) {
			ptr_[count] = element;
			++count;
		}
	}

	~Vector() {
		clear();
		alloc_.deallocate(ptr_);
	}

	void resize(size_t new_size) {
		if (new_size < size_) {
			alloc_.destruct(ptr_ + new_size, size_ - new_size);
			size_ = new_size;
		}
		else
			if (new_size > size_) {
				if (new_size > capacity_)
					reserve(new_size);
				for (size_t i = size_; i < new_size; ++i)
					alloc_.construct(ptr_ + i, T());
				size_ = new_size;
			}
	}

	void reserve(size_t new_cap) {
		if (new_cap > capacity_) {
			T* new_ptr = alloc_.allocate(new_cap);
			for (size_t i = 0; i < size_; ++i)
				alloc_.construct(new_ptr + i, std::move(ptr_[i]));
			alloc_.destruct(ptr_, size_);
			alloc_.deallocate(ptr_);
			ptr_ = new_ptr;
			capacity_ = new_cap;
		}
	}

	void clear() {
		alloc_.destruct(ptr_, size_);
		size_ = 0;
	}

	void push_back(const T& val) {
		if (capacity_ == size_) {
			reserve(size_ + 1);
		}
		alloc_.construct(ptr_ + size_, val);
		size_++;
	}

	void pop_back() {
		if (size_ > 0) {
			size_--;
			alloc_.destruct(ptr_ + size_, 1);
		}
	}

	const T& operator[](size_t i) const {
		if (i >= size_) {
			throw std::out_of_range("Index is out of range");
		}
		return ptr_[i];
	}

	T& operator[](size_t i) {
		if (i >= size_) {
			throw std::out_of_range("Index is out of range");
		}
		return ptr_[i];
	}

	size_t size() const {
		return size_;
	}

	size_t capacity() const {
		return capacity_;
	}

	bool empty() const {
		return (size_ == 0);
	}

	iterator begin() {
		return Iterator<T>(ptr_);
	}

	iterator end() {
		return Iterator<T>(ptr_ + size_);
	}

	iterator rbegin() {
		return Iterator<T>(ptr_ + size_ - 1, true);

	}

	iterator rend() {
		return Iterator<T>(ptr_ - 1, true);
	}
};

