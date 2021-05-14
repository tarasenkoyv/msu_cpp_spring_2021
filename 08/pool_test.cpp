#include <iostream>
#include <cassert>
#include <chrono>
#include <thread>
#include <functional>
#include "pool.hpp"

int foo();
int boo(int i);

void ExecutionTest();
void VariousFunctionsTest();

int main() {
    ExecutionTest();
    VariousFunctionsTest();

    std::cout << "Success!" << std::endl;

    return 0;
}

void ExecutionTest() {
    ThreadPool pool(8);
    auto t1 = pool.exec(foo);
    for (int i = 0; i < 100; ++i) {
        assert(pool.exec([i]() { return i; }).get() == i);
    }

    assert(t1.get() == 0);
}

void VariousFunctionsTest() {
    ThreadPool pool(8);
    auto t1 = pool.exec(foo);
    auto t2 = pool.exec(boo, 2);
    for (int i = 0; i < 100; ++i) {
        auto val = pool.exec([i](int j) { return i * j; }, 2 * i).get();
        assert(val == 2 * i * i);
    }

    assert(t1.get() == 0);
    assert(t2.get() == 2);
}

int foo() {
    std::this_thread::sleep_for(std::chrono::seconds(2));
    return 0;
}

int boo(int i) {
    std::this_thread::sleep_for(std::chrono::seconds(2));
    return i;
}