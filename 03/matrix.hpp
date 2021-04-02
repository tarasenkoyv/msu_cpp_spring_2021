#pragma once

#include <cassert>
#include <iostream>
#include <sstream>

class ProxyRow {
public:
	ProxyRow() = default;
	ProxyRow(const ProxyRow& other);
	ProxyRow(size_t cols);
	~ProxyRow();

	int& operator[](size_t col_idx) const;

	size_t columns() const;
private:
	size_t cols_;
	int* data_;
};

class Matrix {
public:
	Matrix(size_t rows, size_t cols);
	Matrix(const Matrix& other);
	~Matrix();

	Matrix operator+(const Matrix& other) const;
	bool operator==(const Matrix& other) const;
	bool operator!=(const Matrix& other) const;
	Matrix operator*(int x) const;
	friend std::ostream& operator<<(std::ostream& ostream, const Matrix& matrix);
	friend std::ostringstream& operator<<(std::ostringstream& ostream, const Matrix& matrix);

	const ProxyRow& operator[](size_t row_idx) const;

	size_t columns() const;
	size_t rows() const;

private:
	size_t cols_;
	size_t rows_;

	ProxyRow* data_;
};