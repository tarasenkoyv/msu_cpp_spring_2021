#pragma once

#include <iostream>
#include <sstream>

class MyVector {
public:
	MyVector();
	MyVector(const MyVector& other);
	~MyVector();

	MyVector& operator=(const MyVector& other);
	MyVector& operator=(MyVector&& other) noexcept;
	int32_t& operator[](size_t index) const;

	int32_t& GetAt(size_t index) const;
	void Add(int32_t elemet);
	void Resize(size_t new_buffer_size);
	int32_t PopBack();
	size_t size() const;
	bool empty() const;

	const size_t kDefaultInitialSize = 100;
private:
	int32_t* buffer_;
	size_t buffer_size_;
	size_t real_size_;

	void Grow();
};

class BigInt {
public:
	BigInt();
	
	BigInt(const std::string& value);
	BigInt(const BigInt& value);
	BigInt(int32_t value);
	BigInt(BigInt&& value);

	BigInt& operator=(const BigInt& other);
	BigInt& operator=(BigInt&& other) noexcept;

	BigInt operator+(const BigInt& other) const;
	BigInt operator+(int32_t other) const;
	BigInt operator*(const BigInt& right) const;
	const BigInt operator-() const;
	friend const BigInt operator-(BigInt left, const BigInt& right);
	friend std::ostream& operator<<(std::ostream& ostream, const BigInt& value);
	operator std::string() const;
	friend bool operator!=(const BigInt& left, const BigInt& right);
	friend bool operator<=(const BigInt& left, const BigInt& right);
	friend bool operator>(const BigInt& left, const BigInt& right);
	friend bool operator>=(const BigInt& left, const BigInt& right);
	friend bool operator<(const BigInt& left, const BigInt& right);
	friend bool operator ==(const BigInt& left, const BigInt& right);

	~BigInt();

	size_t lenght() const;
	bool is_positive() const;
	bool is_zero() const;
public:
	static const int kBase = 10000;
	static const int kDigitSize = 4;
private:
	bool is_positive_ = true;
	MyVector digits_;
	void RemoveLeadingZeros();
};