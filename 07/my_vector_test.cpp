#include <cassert>
#include <iostream>
#include <algorithm>
#include <sstream>

#include "my_vector.hpp"


void ConsistentTest();
void ForwardIterTest();
void ReverseIterTest();
void InitializeListTest();
void OutOfRangeTest();

int main()
{
	ConsistentTest();
	ForwardIterTest();
	ReverseIterTest();
	InitializeListTest();
	OutOfRangeTest();

	std::cout << "Success!" << std::endl;

	return 0;
}

void ConsistentTest() {
	Vector<int> my_vec;

	assert(my_vec.empty());
	assert(my_vec.size() == 0);

	my_vec.push_back(2);

	assert(!my_vec.empty());
	assert(my_vec.size() == 1);
	assert(my_vec[0] == 2);

	my_vec.pop_back();

	assert(my_vec.empty());
	assert(my_vec.size() == 0);

	my_vec.push_back(1);
	my_vec.push_back(2);
	my_vec.push_back(3);
	my_vec.push_back(4);

	assert(!my_vec.empty());
	assert(my_vec.size() == 4);
	assert(my_vec[0] == 1);
	assert(my_vec[1] == 2);
	assert(my_vec[2] == 3);
	assert(my_vec[3] == 4);

	auto rev_iter = my_vec.rbegin();
	assert(rev_iter != my_vec.rend());
	assert(*rev_iter == 4);
	++rev_iter;
	assert(rev_iter != my_vec.rend());
	assert(*rev_iter == 3);
	++rev_iter;
	assert(rev_iter != my_vec.rend());
	assert(*rev_iter == 2);
	++rev_iter;
	assert(rev_iter != my_vec.rend());
	assert(*rev_iter == 1);
	++rev_iter;
	assert(rev_iter == my_vec.rend());

	auto forward_iter = my_vec.begin();
	assert(forward_iter != my_vec.end());
	assert(*forward_iter == 1);
	++forward_iter;
	assert(forward_iter != my_vec.end());
	assert(*forward_iter == 2);
	++forward_iter;
	assert(forward_iter != my_vec.end());
	assert(*forward_iter == 3);
	++forward_iter;
	assert(forward_iter != my_vec.end());
	assert(*forward_iter == 4);
	++forward_iter;
	assert(forward_iter == my_vec.end());

	my_vec.reserve(100);
	assert(my_vec.size() == 4);
	assert(my_vec.capacity() >= 100);

	const auto capacity = my_vec.capacity();

	my_vec.resize(2);
	assert(my_vec.size() == 2);
	assert(my_vec.capacity() == capacity);
	assert(my_vec[0] == 1);
	assert(my_vec[1] == 2);

	my_vec.resize(3);
	assert(my_vec.size() == 3);
	assert(my_vec.capacity() == capacity);
	assert(my_vec[0] == 1);
	assert(my_vec[1] == 2);
	assert(my_vec[2] == 0);

	my_vec.resize(4);
	assert(my_vec.size() == 4);
	assert(my_vec.capacity() == capacity);
	assert(my_vec[0] == 1);
	assert(my_vec[1] == 2);

	my_vec.resize(0);
	assert(my_vec.size() == 0);
	assert(my_vec.begin() == my_vec.end());

	my_vec.resize(2);
	assert(my_vec.size() == 2);
}

void ForwardIterTest() {
	std::string value;
	Vector<int> my_vec = { 9, 8, 1, 3, 4, 5, 6 };
	std::for_each(my_vec.begin(), my_vec.end(), [&value](int x) { value = value + std::to_string(x) + " "; });

	assert(value == "9 8 1 3 4 5 6 ");
}

void ReverseIterTest() {
	std::string value;
	Vector<int> my_vec = { 9, 8, 1, 3, 4, 5, 6 };
	std::for_each(my_vec.rbegin(), my_vec.rend(), [&value](int x) { value = value + std::to_string(x) + " "; });

	assert(value == "6 5 4 3 1 8 9 ");
}

void InitializeListTest() {
	Vector<int> my_vec = { 9, 8, 1, 3, 4, 5, 6 };

	assert(my_vec[0] == 9);
	assert(my_vec[1] == 8);
	assert(my_vec[2] == 1);
	assert(my_vec[3] == 3);
	assert(my_vec[4] == 4);
	assert(my_vec[5] == 5);
	assert(my_vec[6] == 6);

	assert(my_vec.size() == 7);
	assert(my_vec.capacity() == 7);
	assert(!my_vec.empty());
}

void OutOfRangeTest() {
	Vector<int> my_vec = { 9, 8, 1 };
	std::string err;
	try {
		my_vec[4];
	}
	catch (const std::out_of_range& e) {
		err = e.what();
	}
	assert(err == "Index is out of range");
}