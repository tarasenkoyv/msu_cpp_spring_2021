#include <iostream>
#include <cassert>
#include <chrono>
#include <thread>

#include "pool.hpp"

int foo();

void ExecutionTest();

int main() {
    ExecutionTest();

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

int foo() {
    std::this_thread::sleep_for(std::chrono::seconds(2));
    return 0;
}