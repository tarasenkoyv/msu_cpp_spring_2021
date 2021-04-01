#include <iostream>
#include <cassert>

#include "allocator.hpp"

void test_1();
void test_2();
void test_3();
void test_4();
void test_5();
void test_6();

int main()
{
    test_1();
    test_2();
    test_3();
    test_4();
    test_5();
    // Testing a class destructor and singleton.
    test_6();
}

/**
* Allocation of memory in the absence of initialization,
* when the function MakeAllocator is not called.
*/
void test_1() {
    Allocator* allocator = Allocator::GetInstance();
    allocator->Reset();

    char* new_ptr = static_cast<char*>(allocator->Alloc(1));
    assert(new_ptr == nullptr);
    std::cout << "TEST 1: success." << std::endl;
}

/**
* Allocation of memory size exceeding unsigned int size.
* As a result of the test_2 call, a bad_alloc exception should be thrown.
*/
void test_2() {
    Allocator* allocator = Allocator::GetInstance();
    allocator->Reset();

    try {
        allocator->MakeAllocator(1);
        allocator->MakeAllocator(10000000000000000000);
        std::cout << "TEST 2: faild: " << std::endl;
    }
    catch (std::bad_alloc const&) {
        std::cout << "TEST 2: success:" << std::endl;

    }
}

/**
* Allocation of memory size exceeding the maximum size of the Allocator instance.
*/
void test_3() {
    Allocator* allocator = Allocator::GetInstance();
    allocator->Reset();

    allocator->MakeAllocator(5);
    char* new_ptr = static_cast<char*>(allocator->Alloc(6));
    assert(new_ptr == nullptr);
    std::cout << "TEST 3: success." << std::endl;
}
/**
* Increase memory with the MakeAllocator function.
*/
void test_4() {
    Allocator* allocator = Allocator::GetInstance();
    allocator->Reset();

    allocator->MakeAllocator(5);
    allocator->MakeAllocator(5);
    assert(allocator->get_max_size() == 10);
    std::cout << "TEST 4: success." << std::endl;
}
/**
* Testing Allocator variable values for correctness.
*/
void test_5() {
    Allocator* allocator = Allocator::GetInstance();
    allocator->Reset();

    allocator->MakeAllocator(5);
    allocator->MakeAllocator(5);
    allocator->Alloc(5);
    assert(allocator->get_occupied_size() == 5);
    assert(allocator->get_max_size() == 10);
    std::cout << "TEST 5: success." << std::endl;
}
/**
* Testing a class destructor and singleton.
*/
void test_6() {
    Allocator* allocator = Allocator::GetInstance();
    allocator->Reset();

    allocator->MakeAllocator(5);
    allocator->Alloc(5);

    delete(allocator);

    allocator = Allocator::GetInstance();
    assert(allocator->get_occupied_size() == 0);
    assert(allocator->get_max_size() == 0);

    std::cout << "TEST 6: success." << std::endl;
}
