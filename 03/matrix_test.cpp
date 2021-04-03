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
    assert(m[1][1] == 6);
}

void SumTest() {
    Matrix m1(2, 2);
    Matrix m2(2, 2);

    m1[0][0] = 1;
    m2[0][0] = 2;
    Matrix m3 = m1 + m2;

    assert(m3[0][0] == 3);
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
    assert(m[1][1] == 2);

    m[0][0] = 1;
    m[1][1] = 2;
    m1[0][0] = 1;

    m += m1;
    assert(m[0][0] == 2);
    assert(m[0][1] == 0);
    assert(m[1][1] == 2);
}

void MultiplyTest() {
    Matrix m(2, 2);

    m[0][0] = 1;
    Matrix m1 = m * 2;
    assert(m1[0][0] == 2);
    assert(m1[1][1] == 0);
}

void MultiplySelfTest() {
    Matrix m(2, 2);

    m[0][0] = 1;
    m[1][1] = 2;
    m = m * 2;
    assert(m[0][0] == 2);
    assert(m[0][1] == 0);
    assert(m[1][1] == 4);

    m[0][0] = 1;
    m[1][1] = 2;
    m *= 2;
    assert(m[0][0] == 2);
    assert(m[0][1] == 0);
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
    //std::cout << m << std::endl;

    std::string str_out;
    std::ostringstream strout;
    strout << m;
    assert(strout.str() == "1 0 0\n0 1 0\n0 0 1");
}