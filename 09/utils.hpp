#pragma once

#include <string>
#include <vector>

constexpr auto MAX_BUFFER_SIZE = 8 * 1024 * 1024;

class Utils {
public:
	Utils() {}
	void GenerateBinaryFile(const std::string& file_name, const uint64_t sign_number);
	bool IsSortedFile(std::string& file_name);
	static void MergeSortVectors(std::vector<uint64_t>* src, std::vector<uint64_t>* buffer, uint64_t spos, uint64_t size);
	static void PrintVectorStat(std::vector<uint64_t>* vec);
};

int Compare(const void* one, const void* two);