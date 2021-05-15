#include <cassert>
#include <iostream>
#include <typeinfo>
#include <string>

#include "format.hpp"

void IncorrectFormatTest();
void InvalidArgumentTest();
void FormatTest();
void TooMuchArgsTest();
void EmptyStringTest();
void StringWithoutFormatTest();
void SingleArgTest();
void RepeatArgTest();
void BracketsWithoutDigitTest();
void ExtraBracketsTest();
void NotEnoughArgsTest();

int main()
{
	IncorrectFormatTest();
	InvalidArgumentTest();
	FormatTest();
	TooMuchArgsTest();
	EmptyStringTest();
	StringWithoutFormatTest();
	SingleArgTest();
	RepeatArgTest();
	BracketsWithoutDigitTest();
	ExtraBracketsTest();
	NotEnoughArgsTest();

	std::cout << "Success!" << std::endl;

	return 0;
}

void NotEnoughArgsTest() {
	try {
		format("{0} {1} {100}", 1, 2);
	}
	catch (const std::exception& e) {
		assert(typeid(e) == typeid(too_few_args));
	}
}

void BracketsWithoutDigitTest() {
	try {
		format("{} {0} {1} {test}", 1, 2);
	}
	catch (const std::exception& e) {
		assert(typeid(e) == typeid(bad_format_string));
	}
}

void ExtraBracketsTest() {
	try {
		format("{0}{ {1}{0}}", 1, 2);
	}
	catch (const std::exception& e) {
		assert(typeid(e) == typeid(bad_format_string));
	}

	try {
		format("{0}{", 1);
	}
	catch (const std::exception& e) {
		assert(typeid(e) == typeid(bad_format_string));
	}

	try {
		format("{0}}", 1);
	}
	catch (const std::exception& e) {
		assert(typeid(e) == typeid(bad_format_string));
	}
}

void RepeatArgTest() {
	assert(format("{0}{0}{0}{0}", 1) == "1111");
	assert(format("{0}={1}{1}{1}{1}", 1111, 1) == "1111=1111");
	
	std::string str_test = "";
	std::string str_res = "";
	for (size_t i = 0; i < 20; ++i) {
		str_test += " {2}";
		str_res += " 32";
	}
	str_test = str_test + "{0} test test {1}";
	str_res = str_res + "2 test test 2";
	std::string str_out_res = format(str_test.c_str(), 2, 2, 32);
	assert(str_out_res == str_res);
}

void SingleArgTest() {
	assert(format("{0}", 1) == "1");
	assert(format("{0}  ", "test") == "test  ");

	try {
		format("{1}     ", 1);
	}
	catch (const std::exception& e) {
		assert(typeid(e) == typeid(too_few_args));
	}
}

void EmptyStringTest() {
	assert(format("") == "");
}

void StringWithoutFormatTest() {
	assert(format("string without formatting") == "string without formatting");
}

void TooMuchArgsTest() {
	try {
		format("{0}", 4, 5);
	}
	catch (const std::exception& e) {
		assert(typeid(e) == typeid(too_much_args));
	}
}

void IncorrectFormatTest() {
	try {
		format("{ {0} {1}", 4, 5);
	}
	catch (const std::exception& e) {
		assert(typeid(e) == typeid(bad_format_string));
	}

	try {
		format("{-1} any text {1} {0}", 4, 5);
	}
	catch (const std::exception &e) {
		assert(typeid(e) == typeid(bad_format_string));
		assert(std::string(e.what()) == "Incorrect format: pos = 1");
	}

	try {
		format("{0} any text {} {0}", 4);
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
		std::string text = "{1" + std::to_string(std::numeric_limits<size_t>::max()) + "} } test {1}";
		format(text.c_str(), 4, "test");
	}
	catch (const std::exception& e) {
		assert(typeid(e) == typeid(bad_format_string));
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


	try {
		std::string text = "{" + std::to_string(std::numeric_limits<size_t>::max()) + "} } test {1}";
		format(text.c_str(), 4, "test");
	}
	catch (const std::exception& e) {
		assert(typeid(e) == typeid(too_few_args));
	}
}

void FormatTest() {
	std::string res = format("Test {0} {1} test {1} {2}", 4, "test", 5);
	assert(res == "Test 4 test test test 5");
}