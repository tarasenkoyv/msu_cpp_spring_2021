#include "matrix.hpp"

#include <cassert>
#include <iostream>
#include <sstream>

Matrix::Matrix(size_t rows, size_t cols) {
	// The number of columns must be greater than zero.
	assert(cols > 0);
	// The number of rows must be greater than zero.
	assert(rows > 0);

	cols_ = cols;
	rows_ = rows;

	data_ = new ProxyRow[rows_];
	for (size_t i = 0; i < rows_; i++) {
		data_[i] = *(new ProxyRow(cols_));
	}
}

Matrix::Matrix(const Matrix& other) {
	cols_ = other.cols_;
	rows_ = other.rows_;

	data_ = new ProxyRow[rows_];
	for (size_t i = 0; i < rows_; i++) {
		data_[i] = *(new ProxyRow(other[i]));
	}
}

Matrix& Matrix::operator=(const Matrix& other) {
	cols_ = other.cols_;
	rows_ = other.rows_;

	data_ = new ProxyRow[rows_];
	for (size_t i = 0; i < rows_; i++) {
		data_[i] = *(new ProxyRow(other[i]));
	}

	return *this;
}

Matrix::~Matrix() {
	delete[] data_;
}

Matrix Matrix::operator+(const Matrix& other) const {
	Matrix tmp(rows_, cols_);

	for (size_t i = 0; i < rows_; i++) {
		for (size_t j = 0; j < cols_; j++) {
			tmp[i][j] = (*this)[i][j] + other[i][j];
		}
	}
	return tmp;
}

Matrix& Matrix::operator+=(const Matrix& other) {
	for (size_t i = 0; i < rows_; i++) {
		for (size_t j = 0; j < cols_; j++) {
			(*this)[i][j] = (*this)[i][j] + other[i][j];
		}
	}
	return (*this);
}

Matrix Matrix::operator*(int x) const {
	Matrix tmp(rows_, cols_);
	for (size_t i = 0; i < rows_; i++) {
		for (size_t j = 0; j < cols_; j++) {
			tmp[i][j] = (*this)[i][j] * x;
		}
	}
	return tmp;
}

Matrix& Matrix::operator*=(int x) {
	for (size_t i = 0; i < rows_; i++) {
		for (size_t j = 0; j < cols_; j++) {
			(*this)[i][j] = (*this)[i][j] * x;
		}
	}
	return (*this);
}

bool Matrix::operator!=(const Matrix& other) const {
	return !(*this == other);
}

bool Matrix::operator==(const Matrix& other) const {
	for (size_t i = 0; i < rows_; i++) {
		for (size_t j = 0; j < cols_; j++) {
			if (data_[i][j] != other[i][j]) {
				return false;
			}
		}
	}
	return true;
}

const ProxyRow& Matrix::operator[](size_t row_idx) const {
	if (row_idx >= rows_) {
		throw std::out_of_range("Row index is out of range");
	}
	return data_[row_idx];
}

std::ostream& operator<<(std::ostream& ostream, const Matrix& matrix) {
	for (size_t i = 0; i < matrix.rows_; i++) {
		for (size_t j = 0; j < matrix.cols_; j++) {
			ostream << matrix.data_[i][j];
			if (j < matrix.cols_ - 1) {
				ostream << " ";
			}
		}
		if (i < matrix.rows_ - 1) {
			ostream << std::endl;
		}
	}
	return ostream;
}

std::ostringstream& operator<<(std::ostringstream& ostream, const Matrix& matrix)
{
	for (size_t i = 0; i < matrix.rows_; i++) {
		for (size_t j = 0; j < matrix.cols_; j++) {
			ostream << matrix.data_[i][j];
			if (j < matrix.cols_ - 1) {
				ostream << " ";
			}
		}
		if (i < matrix.rows_ - 1) {
			ostream << std::endl;
		}
	}
	return ostream;
}

size_t Matrix::columns() const {
	return cols_;
}

size_t Matrix::rows() const {
	return rows_;
}

ProxyRow::ProxyRow(size_t cols) {
	// The number of columns must be greater than zero.
	assert(cols > 0);

	cols_ = cols;

	data_ = new int[cols_];
	for (size_t i = 0; i < cols_; i++) {
		data_[i] = 0;
	}
}

ProxyRow::ProxyRow(const ProxyRow& other) {
	cols_ = other.cols_;

	data_ = new int[cols_];
	for (size_t i = 0; i < cols_; i++) {
		data_[i] = other[i];
	}
}

ProxyRow::~ProxyRow() {
	delete[] data_;
}

int& ProxyRow::operator[](size_t col_idx) const {
	if (col_idx >= cols_) {
		throw std::out_of_range("Column index is out of range");
	}
	return data_[col_idx];
}

size_t ProxyRow::columns() const {
	return cols_;
}