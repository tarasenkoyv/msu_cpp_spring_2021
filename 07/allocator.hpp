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

	template <class... Args>
	void construct(pointer ptr, Args&&... args) {
		new (ptr) value_type(std::forward<Args>(args)...);
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