// lesson-10.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <string>
#include <cassert>

#include "commander.hpp"
#include "utils.hpp"

uint64_t mem_size = 8;
uint64_t max_thread_count = 2;
std::string unsorted_file = "input.bin";
std::string sorted_file = "output.bin";

void GenerateBinaryFile();
void SortTest();

int main()
{
	SortTest();

    std::cout << "Success!" << std::endl;
}

void SortTest() {

	GenerateBinaryFile();
	
	Commander c(mem_size, max_thread_count, unsorted_file, sorted_file, false);
	c.Sort();

	Utils f = Utils();
	assert(f.IsSortedFile(sorted_file));
}

void GenerateBinaryFile() {
	try {
		Utils f = Utils();
		f.GenerateBinaryFile(unsorted_file, 5 * MB);
	}
	catch (std::exception& exc) {
		std::cout << exc.what() << std::endl;
	}
}
