#pragma once

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
		auto iter = rev_f_ ? Iterator(ptr_ - n) : Iterator(ptr_ + n);
		return iter;
	}

	difference_type operator-(const Iterator<T>& other) const {
		return std::distance(ptr_, other.ptr_);
	}

	Iterator operator-(size_t n) const {
		auto iter = rev_f_ ? Iterator(ptr_ + n) : Iterator(ptr_ - n);
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