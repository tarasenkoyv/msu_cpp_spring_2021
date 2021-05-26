template<class T, class Alloc>
Vector<T, Alloc>::Vector() : size_(0), ptr_(alloc_.allocate(0)), capacity_(0) {}

template<class T, class Alloc>
Vector<T, Alloc>::Vector(size_t count) : size_(0), ptr_(alloc_.allocate(count)), capacity_(count) {}

template<class T, class Alloc>
Vector<T, Alloc>::Vector(const std::initializer_list<T>& list) : Vector(list.size()) {
	size_ = 0;
	for (auto& element : list) {
		ptr_[size_] = element;
		++size_;
	}
}

template<class T, class Alloc>
Vector<T, Alloc>::~Vector() {
	clear();
	alloc_.deallocate(ptr_);
}

template<class T, class Alloc>
void Vector<T, Alloc>::reserve(size_t new_cap) {
	if (new_cap > capacity_) {
		T* new_ptr = alloc_.allocate(new_cap);
		for (size_t i = 0; i < size_; ++i) {
			alloc_.construct(new_ptr + i, std::move(ptr_[i]));
		}
		alloc_.destruct(ptr_, size_);
		alloc_.deallocate(ptr_);
		ptr_ = new_ptr;
		capacity_ = new_cap;
	}
}

template<class T, class Alloc>
void Vector<T, Alloc>::resize(size_t new_size)
{
	if (new_size < size_) {
		alloc_.destruct(ptr_ + new_size, size_ - new_size);
		size_ = new_size;
	}
	else
		if (new_size > size_) {
			if (new_size > capacity_) {
				reserve(new_size);
			}
			for (size_t i = size_; i < new_size; ++i) {
				alloc_.construct(ptr_ + i, T());
			}
			size_ = new_size;
		}
}

template<class T, class Alloc>
template <class... Args>
T& Vector<T, Alloc>::emplace_back(Args&&... args) {
	if (capacity_ == size_) {
		reserve(size_ + 1);
	}
	alloc_.construct(ptr_ + size_, std::forward<Args>(args)...);
	auto& ref = *(ptr_ + size_);
	size_++;
	return ref;
}

template<class T, class Alloc>
void Vector<T, Alloc>::clear() {
	alloc_.destruct(ptr_, size_);
	size_ = 0;
}

template<class T, class Alloc>
void Vector<T, Alloc>::push_back(const T& val) {
	if (capacity_ == size_) {
		reserve(size_ + 1);
	}
	alloc_.construct(ptr_ + size_, val);
	size_++;
}

template<class T, class Alloc>
void Vector<T, Alloc>::pop_back() {
	if (size_ > 0) {
		size_--;
		alloc_.destruct(ptr_ + size_, 1);
	}
}

template<class T, class Alloc>
const T& Vector<T, Alloc>::operator[](size_t i) const {
	if (i >= size_) {
		throw std::out_of_range("Index is out of range");
	}
	return ptr_[i];
}

template<class T, class Alloc>
T& Vector<T, Alloc>::operator[](size_t i) {
	if (i >= size_) {
		throw std::out_of_range("Index is out of range");
	}
	return ptr_[i];
}

template<class T, class Alloc>
size_t Vector<T, Alloc>::size() const {
	return size_;
}

template<class T, class Alloc>
size_t Vector<T, Alloc>::capacity() const {
	return capacity_;
}

template<class T, class Alloc>
bool Vector<T, Alloc>::empty() const {
	return (size_ == 0);
}

template<class T, class Alloc>
Iterator<T> Vector<T, Alloc>::begin() {
	return Iterator<T>(ptr_);
}

template<class T, class Alloc>
Iterator<T> Vector<T, Alloc>::end() {
	return Iterator<T>(ptr_ + size_);
}

template<class T, class Alloc>
Iterator<T> Vector<T, Alloc>::rbegin() {
	return Iterator<T>(ptr_ + size_ - 1, true);

}

template<class T, class Alloc>
Iterator<T> Vector<T, Alloc>::rend() {
	return Iterator<T>(ptr_ - 1, true);
}