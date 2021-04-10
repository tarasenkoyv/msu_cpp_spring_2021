#include <cassert>
#include <iostream>
#include <sstream>

#include "bigint.hpp"

void InitializeTest();
void SumTest();
void MultiplyTest();
void SubTest();
void OutputTest();
void InEqualityTest();

int main() {

	InitializeTest();
	SumTest();
	MultiplyTest();
	SubTest();
	OutputTest();
	InEqualityTest();

	std::cout << "Success!" << std::endl;

	return 0;
}

void InitializeTest() {
	BigInt z;
	assert(z.lenght() == 0);
	assert(z.is_positive());
	assert(z.is_zero());

	BigInt b1("+123456778787878787878787878787908");
	assert(b1.lenght() == 9);
	assert(b1.is_positive());

	BigInt b2(-238389389);
	assert(b2.lenght() == 3);
	assert(!b2.is_positive());

	BigInt d;
	d = std::move(b1);
	assert(d.lenght() == 9);
	assert(d.is_positive());
	assert(b1.lenght() == 0);
	assert(b1.is_zero());

	BigInt d1 = std::move(d);
	assert(d1.lenght() == 9);
	assert(d1.is_positive());
	assert(d.lenght() == 0);
	assert(d.is_zero());

	BigInt a = -1;
	assert(a.lenght() == 1);
	assert(!a.is_positive());
}

void SumTest() {
	BigInt a = 1;
	BigInt b("+123456789");
	BigInt c = b + 123456789;
	assert(std::string(c) == "246913578");

	BigInt d;
	d = std::move(c);
	a = d + b;
	assert(std::string(a) == "370370367");
}

void SubTest() {
	BigInt b("+123456789");
	BigInt c = b - 6789;
	assert(std::string(c) == "123450000");

	BigInt c2 = b - 123456790;
	assert(std::string(c2) == "-1");
}

void MultiplyTest() {
	BigInt a = 2;
	BigInt b("1234567890");
	BigInt c = a * b + 2;
	assert(std::string(c) == "2469135782");

	a = -2;
	c = a * b + 2;
	assert(std::string(c) == "-2469135778");

	c = b * 0 + 2;
	assert(std::string(c) == "2");

	c = b * 5 + 2;
	assert(std::string(c) == "6172839452");

	c = b * 5 - 450 - BigInt("6172839000");
	assert(std::string(c) == "0");

	size_t n = 20;
	BigInt factorial = 1;
	for (size_t i = 1; i <= n; i++) {
		factorial = factorial * i;
	}
	assert(std::string(factorial) == "2432902008176640000");
}

void OutputTest() {
	BigInt b("-1234567890");
	std::string str_out;
	std::ostringstream strout;
	strout << b;
	assert(strout.str() == "-1234567890");
}

void InEqualityTest() {
	BigInt b("-1234567890");
	BigInt a("-1234567891");
	assert(a < b);

	a = a + 1;
	assert(a == b);
	assert(a < -b);

}