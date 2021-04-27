#include <cassert>
#include <iostream>
#include <typeinfo>
#include <string>

#include "format.hpp"

void IncorrectFormatTest();
void InvalidArgumentTest();
void FormatTest();

int main()
{
	IncorrectFormatTest();
	InvalidArgumentTest();
	FormatTest();

	std::cout << "Success!" << std::endl;

	return 0;
}

void IncorrectFormatTest() {
	try {
		format("{-1} any text {1} {0}", 4, 5);
	}
	catch (const std::exception &e) {
		assert(typeid(e) == typeid(bad_format_string));
		assert(std::string(e.what()) == "Incorrect format: pos = 1");
	}

	try {
		format("{0} any text {} {0}", 4, "test");
	}
	catch (const std::exception& e) {
		assert(typeid(e) == typeid(bad_format_string));
		assert(std::string(e.what()) == "Incorrect format: pos = 14");
	}

	try {
		format("{0} } test {1}", 4, "test");
	}
	catch (const std::exception& e) {
		assert(typeid(e) == typeid(bad_format_string));
		assert(std::string(e.what()) == "Incorrect format: pos = 4");
	}

	try {
		std::string text = "{1" + std::to_string(UINT64_MAX) + "} } test {1}";
		format(text.c_str(), 4, "test");
	}
	catch (const std::exception& e) {
		assert(typeid(e) == typeid(bad_format_string));
		assert(std::string(e.what()) == "Incorrect format: pos = 22");
	}
}

void InvalidArgumentTest() {
	try {
		format("{0} {3} test {1}", 4, "test");
	}
	catch (const std::exception& e) {
		assert(typeid(e) == typeid(too_few_args));
		assert(std::string(e.what()) == "Fewer arguments than specified in the formatted string: number = 3");
	}
}

void FormatTest() {
	std::string res = format("Test {0} {1} test {1} {2}", 4, "test", 5);
	assert(res == "Test 4 test test test 5");

	assert(format("Test ", 4, "test", 5) == "Test ");

	assert(format("", 4, "test", 5) == "");
}