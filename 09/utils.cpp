#include "utils.hpp"

#include <algorithm>
#include <ctime>
#include <map>
#include <vector>
#include <iostream>
#include <fstream>


void Utils::GenerateBinaryFile(const std::string& file_name, const uint64_t sign_number) {
	std::ofstream out(file_name, std::ios::binary | std::ios::out | std::ios::in | std::ios::trunc);
	uint64_t buff_size = MAX_BUFFER_SIZE > sign_number ? sign_number : MAX_BUFFER_SIZE;

	std::vector<uint64_t> buff(buff_size);
	srand(time(0));
	uint64_t seek = 0;

	while (seek < sign_number * sizeof(uint64_t)) {

		if (seek / sizeof(uint64_t) + buff_size > sign_number) {
			buff_size = sign_number - seek / sizeof(uint64_t);
		}

		out.seekp(seek, std::ios::beg);
		for (uint64_t i = 0; i < buff_size; i++) {
			buff[i] = rand();
		}

		out.write(reinterpret_cast<const char*>(&buff[0]), buff_size * sizeof(uint64_t));
		seek += buff_size * sizeof(uint64_t);
	}

	out.close();
}

bool Utils::IsSortedFile(std::string& file_name) {
	std::ifstream f(file_name, std::ios::binary);
	f.seekg(0, f.end);
	uint64_t len = f.tellg() / sizeof(uint64_t);
	f.seekg(0, f.beg);
	uint64_t* arr = new uint64_t[len + 1];
	f.read((char*)arr, len * sizeof(uint64_t));
	bool result = std::is_sorted(arr, arr + len);
	delete[] arr;
	f.close();
	return result;
}

void  Utils::MergeSortVectors(std::vector<uint64_t>* src, std::vector<uint64_t>* buffer, uint64_t spos, uint64_t size) {
	uint64_t i = 0, j = 0, k = 0;

	while (i < spos && j < size - spos)
		(*buffer)[k++] = (*src)[i] < (*src)[j + spos] ? (*src)[i++] : (*src)[j++ + spos];

	while (i < spos)
		(*buffer)[k++] = (*src)[i++];

	while (j < size - spos)
		(*buffer)[k++] = (*src)[j++ + spos];
}

void Utils::PrintVectorStat(std::vector<uint64_t>* vec) {
	std::map<int, int> stat;

	for (uint64_t i = 0; i < vec->size(); i++) {
		stat[(*vec)[i]] += 1;
	}

	for (std::map<int, int>::iterator it = stat.begin(); it != stat.end(); ++it) {
		std::cout << "number " << it->first << " : have count " << it->second << std::endl;
	}
}

int Compare(const void* one, const void* two)
{
	return *((int*)one) - *((int*)two);
}
