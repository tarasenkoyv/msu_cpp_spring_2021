#include "bigint.hpp"

#include <cassert>
#include <iomanip> 
#include <iostream>
#include <sstream>
#include <cmath>

BigInt::BigInt() {
    is_positive_ = true;
}

BigInt::BigInt(const std::string& value) {
    std::string value_wo_sign;

    if (value.length() == 0) {
        is_positive_ = true;

        return;
    }
    else {
        is_positive_ = !(value[0] == '-');
        value_wo_sign = (value[0] == '-' or value[0] == '+') ? value.substr(1) : value;
    }

    size_t lenght = static_cast<size_t>(std::ceil((float)(value_wo_sign.length() / static_cast<float>(kDigitSize))));

    // Input string does not match format.
    assert(lenght > 0);

    for (size_t i = 0; i < lenght; i++) {
        int32_t digit;
        if (i == lenght - 1) {
            size_t count = value_wo_sign.length() - i * kDigitSize;
            digit = atoi(value_wo_sign.substr(0, count).c_str());
        }
        else {
            digit = atoi(value_wo_sign.substr(value_wo_sign.length() - (i + 1) * kDigitSize, kDigitSize).c_str());
        }
        digits_.Add(digit);
    }
    RemoveLeadingZeros();
}

BigInt::BigInt(int32_t value) : BigInt(std::to_string(value)) {}

BigInt::BigInt(const BigInt& value) {
    digits_ = value.digits_;
    is_positive_ = value.is_positive_;
}

BigInt::BigInt(BigInt&& other) {
    digits_ = std::move(other.digits_);
    is_positive_ = std::move(other.is_positive_);
}

BigInt& BigInt::operator=(const BigInt& other) {
    if (this == &other) {
        return *this;
    }

    digits_ = other.digits_;
    is_positive_ = other.is_positive_;

    return *this;
}

BigInt& BigInt::operator=(BigInt&& other) noexcept {
    if (this == &other) {
        return *this;
    }

    digits_ = std::move(other.digits_);
    is_positive_ = std::move(other.is_positive_);

    return *this;
}

const BigInt BigInt::operator-() const {
    BigInt copy(*this);
    copy.is_positive_ = !copy.is_positive_;
    return copy;
}

bool operator!=(const BigInt& left, const BigInt& right) {
    return !(left == right);
}

bool operator<=(const BigInt& left, const BigInt& right) {
    return (left < right || left == right);
}

bool operator>(const BigInt& left, const BigInt& right) {
    return !(left <= right);
}

bool operator>=(const BigInt& left, const BigInt& right) {
    return !(left < right);
}

bool operator<(const BigInt& left, const BigInt& right) {
    if (left == right) return false;
    if (!left.is_positive_) {
        if (!right.is_positive_) return ((-right) < (-left));
        else return true;
    }
    else if (!right.is_positive_) return false;
    else {
        if (left.digits_.size() != right.digits_.size()) {
            return left.digits_.size() < right.digits_.size();
        }
        else {
            for (int32_t i = left.digits_.size() - 1; i >= 0; --i) {
                if (left.digits_[i] != right.digits_[i]) return left.digits_[i] < right.digits_[i];
            }

            return false;
        }
    }
}

bool operator ==(const BigInt& left, const BigInt& right) {
    if (left.is_positive_ != right.is_positive_) return false;
    if (left.digits_.empty()) {
        if (right.digits_.empty() || (right.digits_.size() == 1 && right.digits_[0] == 0)) {
            return true;
        }
        else return false;
    }

    if (right.digits_.empty()) {
        if (left.digits_.size() == 1 && left.digits_[0] == 0) return true;
        else return false;
    }
    if (left.digits_.size() != right.digits_.size()) return false;
    for (size_t i = 0; i < left.digits_.size(); ++i) if (left.digits_[i] != right.digits_[i]) return false;

    return true;
}

const BigInt operator-(BigInt left, const BigInt& right) {
    if (!right.is_positive_) {
        return left + (-right);
    }
    else if (!left.is_positive_) {
        return -(-left + right);
    }
    else if (left < right) {
        return -(right - left);
    }

    int carry = 0;
    for (size_t i = 0; i < right.lenght() || carry != 0; ++i) {
        left.digits_[i] -= carry + (i < right.digits_.size() ? right.digits_[i] : 0);
        carry = left.digits_[i] < 0;
        if (carry != 0) {
            left.digits_[i] += BigInt::kBase;
        }
    }

    left.RemoveLeadingZeros();
    return left;
}

BigInt BigInt::operator+(const BigInt& right) const {
    BigInt left = *this;

    if (!left.is_positive_) {
        if (!right.is_positive_) {
            return -(-left + (-right));
        }
        else return right - (-left);
    }
    else if (!right.is_positive_) {
        return left - (-right);
    }

    int carry = 0;
    for (size_t i = 0; i < std::max(left.lenght(), right.lenght()) || carry; ++i) {
        if (i == left.lenght()) {
            left.digits_.Add(0);
        }
        left.digits_[i] += carry + (i < right.lenght() ? right.digits_[i] : 0);
        carry = left.digits_[i] >= kBase;
        if (carry) {
            left.digits_[i] -= kBase;
        }
    }

    return left;
}

BigInt BigInt::operator+(int32_t other) const {
    BigInt left = *this;
    BigInt right = other;
    return left + right;
}

BigInt BigInt::operator*(const BigInt& right) const {
    BigInt left = *this;

    BigInt result;
    result.digits_.Resize(left.digits_.size() + right.digits_.size());
    for (size_t i = 0; i < left.digits_.size(); ++i) {
        int carry = 0;
        for (size_t j = 0; j < right.digits_.size() || carry != 0; ++j) {
            int32_t cur = result.digits_[i + j] +
                left.digits_[i] * (j < right.digits_.size() ? right.digits_[j] : 0) + carry;
            result.digits_[i + j] = static_cast<int32_t>(cur % BigInt::kBase);
            carry = static_cast<int>(cur / BigInt::kBase);
        }
    }
    result.is_positive_ = left.is_positive_ == right.is_positive_;
    result.RemoveLeadingZeros();
    return result;
}

BigInt::~BigInt() {
}

size_t BigInt::lenght() const {
    return digits_.size();
}

bool BigInt::is_positive() const {
    return is_positive_;
}

bool BigInt::is_zero() const {
    return digits_.size() == 0;
}

MyVector::MyVector() {
    buffer_size_ = 0;
    buffer_ = nullptr;
    real_size_ = 0;
}

MyVector::MyVector(const MyVector& other) {
    int32_t* tmp = new int32_t[other.real_size_];
    delete[] buffer_;
    buffer_ = tmp;
    real_size_ = other.real_size_;
    buffer_size_ = real_size_;
    std::copy(other.buffer_, other.buffer_ + real_size_, buffer_);
}

MyVector::~MyVector() {
    delete[] buffer_;
}

int32_t& MyVector::GetAt(size_t index) const {
    assert(index < real_size_&& buffer_ != nullptr);
    return buffer_[index];
}

void MyVector::Grow() {
    size_t new_buffer_size = std::max(buffer_size_ * 2, kDefaultInitialSize);
    
    int32_t* new_buffer = new int32_t[new_buffer_size];
    for (size_t i = 0; i < real_size_; i++) {
        new_buffer[i] = buffer_[i];
    }
    delete[] buffer_;
    buffer_ = new_buffer;
    buffer_size_ = new_buffer_size;
}

void MyVector::Resize(size_t new_buffer_size) {
    int32_t* new_buffer = new int32_t[new_buffer_size];
    for (size_t i = 0; i < new_buffer_size; i++) {
        new_buffer[i] = 0;
    }
    delete[] buffer_;
    buffer_ = new_buffer;
    buffer_size_ = new_buffer_size;
    real_size_ = new_buffer_size;
}

void MyVector::Add(int32_t element) {
    if (real_size_ == buffer_size_) {
        Grow();
    }
    
    assert(real_size_ < buffer_size_&& buffer_ != nullptr);

    buffer_[real_size_++] = element;
}

int32_t& MyVector::operator[](size_t index) const {
    return GetAt(index);
}

MyVector& MyVector::operator=(const MyVector& other) {
    if (this == &other)
    {
        return *this;
    }

    int32_t* tmp = new int32_t[other.real_size_];
    delete[] buffer_;
    buffer_ = tmp;
    real_size_ = other.real_size_;
    buffer_size_ = real_size_;
    std::copy(other.buffer_, other.buffer_ + real_size_, buffer_);

    return *this;
}

MyVector& MyVector::operator=(MyVector&& other) noexcept {
    if (this == &other)
    {
        return *this;
    }

    buffer_ = other.buffer_;
    real_size_ = other.real_size_;
    buffer_size_ = real_size_;
    other.buffer_ = nullptr;
    other.real_size_ = 0;
    other.buffer_size_ = 0;

    return *this;
}

size_t MyVector::size() const {
    return real_size_;
}

bool MyVector::empty() const {
    return real_size_ == 0;
}

int32_t MyVector::PopBack() {
    real_size_ -= 1;
    return buffer_[real_size_];;
}

std::ostream& operator<<(std::ostream& os, const BigInt& value) {
    if (value.is_zero()) os << 0;
    else {
        if (!value.is_positive()) os << '-';

        char old_fill = os.fill('0');
        for (int32_t i = value.lenght() - 1; i >= 0; --i) {
            if (i < (static_cast<int32_t>(value.lenght()) - 1)) {
                os << std::setw(BigInt::kDigitSize) << value.digits_[i];
            }
            else {
                os << value.digits_[i];
            }
        }
        os.fill(old_fill);
    }

    return os;
}

void BigInt::RemoveLeadingZeros() {
    while (digits_.size() > 1 && digits_[lenght() - 1] == 0) {
        digits_.PopBack();
    }
    if (digits_.size() == 1 && digits_[0] == 0) is_positive_ = true;
}

BigInt::operator std::string() const {
    std::stringstream ss;
    ss << *this;
    return ss.str();
}