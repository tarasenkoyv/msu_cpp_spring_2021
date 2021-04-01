#include <iostream>
#include <cassert>

#include "allocator.hpp"

void AllocWithoutMakeAllocatorCall();
void AllocMemorySizeExceedingMaxSizeTest();
void ReAllocMemoryTest();
void CheckBoundaryConditionsTest();

int main()
{
    AllocWithoutMakeAllocatorCall();
    AllocMemorySizeExceedingMaxSizeTest();
    ReAllocMemoryTest();
    CheckBoundaryConditionsTest();

    std::cout << "Success!" << std:: endl;

    return 0;
}

/**
* Allocation of memory in the absence of initialization,
* when the function makeAllocator is not called.
*/
void AllocWithoutMakeAllocatorCall() {
    Allocator* allocator = new Allocator();
    
    assert(allocator->alloc(1) == nullptr);
}

/**
* Allocation of memory size exceeding the maximum size of the Allocator instance.
*/
void AllocMemorySizeExceedingMaxSizeTest() {
    Allocator* allocator = new Allocator();

    allocator->makeAllocator(5);
    assert(allocator->alloc(6) == nullptr);
}
/**
* Re-allocation memory with the makeAllocator function.
*/
void ReAllocMemoryTest() {
    Allocator* allocator = new Allocator();

    allocator->makeAllocator(5);
    assert(allocator->alloc(1) != nullptr);
    assert(allocator->get_offset() == 1);
    allocator->makeAllocator(5);
    assert(allocator->get_max_size() == 5);
    assert(allocator->get_offset() == 0);
}
/**
* Testing Allocator variable values for correctness.
*/
void CheckBoundaryConditionsTest() {
    Allocator* allocator = new Allocator();

    allocator->makeAllocator(10);
    assert(allocator->get_max_size() == 10);
    assert(allocator->get_offset() == 0);

    assert(allocator->alloc(10) != nullptr);
    assert(allocator->get_offset() == 10);

    assert(allocator->alloc(1) == nullptr);
    assert(allocator->get_offset() == 10);
    assert(allocator->get_max_size() == 10);

    allocator->reset();
    assert(allocator->get_offset() == 0);
    assert(allocator->get_max_size() == 10);
    assert(allocator->alloc(10) != nullptr);
}
