#pragma once 

#include <mutex>
#include <vector>

constexpr auto MB = 1024 * 1024;

class Commander {

private:
    // Actual thread count
    uint64_t thread_count_;

    // Memory size in bytes
    uint64_t mem_mb_size_;

    // Unsorted file name
    std::string unsorted_file_;

    // Sorted file name
    std::string sorted_file_;

    // Mutex to lock unlock sorted file write
    std::mutex* locker_;

    // Sorted and unsorted files size's in bytes
    uint64_t file_size_;

    bool verbose_;
public:
    Commander(const uint64_t mem_size, const uint64_t max_thread_count, const std::string& unsorted_file, const std::string& sorted_file, bool verbose);

    void Sort();

    void SortBlocks();

    void MergeSortedBlocks();

    static void ReadBinaryFile(std::vector<uint64_t>* buffer, const uint64_t bpos, const uint64_t pos, const uint64_t bytes, std::ifstream& io);

    static void WriteBinaryFile(std::vector<uint64_t>* buffer, const uint64_t bpos, const uint64_t size, const uint64_t pos, std::ofstream& io);

    ~Commander();
};