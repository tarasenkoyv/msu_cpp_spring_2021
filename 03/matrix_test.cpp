#include <cassert>
#include <iostream>
#include <sstream>

#include "matrix.hpp"

void InitializeTest();
void indexingTest();
void SumTest();
void SumSelfTest();
void SumSelfTest();
void MultiplyTest();
void EqualTest();
void OutputTest();
void MultiplySelfTest();
void OneByOneMatrixTest();
void OutOfRangeTest();

int main()
{
    InitializeTest();
    indexingTest();
    SumTest();
    SumSelfTest();
    MultiplyTest();
    MultiplySelfTest();
    EqualTest();
    OutputTest();
    OneByOneMatrixTest();
    OutOfRangeTest();

    std::cout << "Success!" << std::endl;

    return 0;
}

void InitializeTest() {
    Matrix m(1, 2);

    assert(m.rows() == 1);
    assert(m.columns() == 2);
}

void indexingTest() {
    Matrix m(2, 2);

    assert(m[1][1] == 0);

    m[1][1] = 6;
    assert(m[0][0] == 0);
    assert(m[0][1] == 0);
    assert(m[1][0] == 0);
    assert(m[1][1] == 6);
}

void SumTest() {
    Matrix m1(2, 2);
    Matrix m2(2, 2);

    m1[0][0] = 1;
    m2[0][0] = 2;
    m1[0][1] = 1;
    m2[1][0] = 2;
    Matrix m3 = m1 + m2;

    assert(m3[0][0] == 3);
    assert(m3[0][1] == 1);
    assert(m3[1][0] == 2);
    assert(m3[1][1] == 0);
}

void SumSelfTest() {
    Matrix m(2, 2);
    Matrix m1(2, 2);

    m[0][0] = 1;
    m[1][1] = 2;
    m1[0][0] = 1;

    m = m1 + m;
    assert(m[0][0] == 2);
    assert(m[0][1] == 0);
    assert(m[1][0] == 0);
    assert(m[1][1] == 2);

    m[0][0] = 1;
    m[1][1] = 2;
    m1[0][0] = 1;

    m += m1;
    assert(m[0][0] == 2);
    assert(m[0][1] == 0);
    assert(m[1][0] == 0);
    assert(m[1][1] == 2);
}

void MultiplyTest() {
    Matrix m(2, 2);

    m[0][0] = 1;
    Matrix m1 = m * 2;
    assert(m1[0][0] == 2);
    assert(m1[0][1] == 0);
    assert(m1[1][0] == 0);
    assert(m1[1][1] == 0);
}

void MultiplySelfTest() {
    Matrix m(2, 2);

    m[0][0] = 1;
    m[1][1] = 2;
    m = m * 2;
    assert(m[0][0] == 2);
    assert(m[0][1] == 0);
    assert(m[1][0] == 0);
    assert(m[1][1] == 4);

    m[0][0] = 1;
    m[1][1] = 2;
    m *= 2;
    assert(m[0][0] == 2);
    assert(m[0][1] == 0);
    assert(m[1][0] == 0);
    assert(m[1][1] == 4);
}

void EqualTest() {
    Matrix m1(2, 2);
    Matrix m2(2, 2);

    m1[0][0] = 1;
    m2[0][0] = 2;
    assert(m1 != m2);

    m2[0][0] = 1;
    assert(m1 == m2);
}

void OutputTest() {
    Matrix m(3, 3);
    m[0][0] = 1;
    m[1][1] = 1;
    m[2][2] = 1;

    std::string str_out;
    std::ostringstream strout;
    strout << m;
    assert(strout.str() == "1 0 0\n0 1 0\n0 0 1");

    assert(m[0][0] == 1);
    assert(m[0][1] == 0);
    assert(m[0][2] == 0);
    assert(m[1][0] == 0);
    assert(m[1][1] == 1);
    assert(m[1][2] == 0);
    assert(m[2][0] == 0);
    assert(m[2][1] == 0);
    assert(m[2][2] == 1);
}

void OneByOneMatrixTest() {
    Matrix m1(1, 1);
    Matrix m2(1, 1);

    assert(m1.rows() == 1);
    assert(m1.rows() == 1);

    m1[0][0] = 2;
    m2[0][0] = 1;
    m1 = m2 + m1;
    assert(m1[0][0] == 3);

    m1[0][0] = 2;
    m2[0][0] = 1;
    m1 += m2;
    assert(m1[0][0] == 3);

    m1[0][0] = 2;
    m2[0][0] = 1;
    m1 = m2 * 3;
    assert(m1[0][0] == 3);

    m1[0][0] = 2;
    m1 = m1 * 3;
    assert(m1[0][0] == 6);

    m1[0][0] = 2;
    m1 *= 2;
    assert(m1[0][0] == 4);

    m1[0][0] = 2;
    m2[0][0] = 1;
    assert(m1 != m2);

    m1[0][0] = 2;
    m2[0][0] = 2;
    assert(m1 == m2);

    m1[0][0] = 2;
    std::string str_out;
    std::ostringstream strout;
    strout << m1;
    assert(strout.str() == "2");
}

void OutOfRangeTest() {
    Matrix m(2, 2);
    std::string err;

    try {
        ProxyRow row = m[3];
    }
    catch (const std::out_of_range& e) {
        err = e.what();
    }
    assert(err == "Row index is out of range");

    try {
        m[1][2] = 3;
    }
    catch (const std::out_of_range& e) {
        err = e.what();
    }
    assert(err == "Column index is out of range");
}