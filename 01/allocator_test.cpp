#include <iostream>
#include <cassert>

#include "allocator.hpp"

void Test1();
void Test2();
void Test3();
void Test4();
void Test5();
void Test6();

int main()
{
    Test1();
    Test2();
    Test3();
    Test4();
    Test5();
    // Testing a class destructor and singleton.
    Test6();

    return 0;
}

/**
* Allocation of memory in the absence of initialization,
* when the function MakeAllocator is not called.
*/
void Test1() {
    Allocator* allocator = Allocator::GetInstance();
    allocator->Reset();

    char* new_ptr = static_cast<char*>(allocator->Alloc(1));
    assert(new_ptr == nullptr);
    std::cout << "TEST 1: success." << std::endl;
}

/**
* Allocation of memory size exceeding unsigned int size.
* As a result of the Test2 call, a bad_alloc exception should be thrown.
*/
void Test2() {
    Allocator* allocator = Allocator::GetInstance();
    allocator->Reset();

    try {
        allocator->MakeAllocator(1);
        allocator->MakeAllocator(10000000000000000000);
        std::cout << "TEST 2: failed." << std::endl;
    }
    catch (std::bad_alloc const&) {
        std::cout << "TEST 2: success." << std::endl;

    }
}

/**
* Allocation of memory size exceeding the maximum size of the Allocator instance.
*/
void Test3() {
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
void Test4() {
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
void Test5() {
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
void Test6() {
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
