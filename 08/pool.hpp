#pragma once

#include <thread>
#include <future>
#include <queue>
#include <vector>
#include <functional>

class ThreadPool
{
private:
    // Indicates that a destructor is running
    bool is_destructing_;
    std::condition_variable condition_;
    std::mutex mutex_;
    std::queue <std::function<void()>> tasks_;
    std::vector <std::thread> threads_;



public:
    explicit ThreadPool(size_t poolSize) : is_destructing_(false) {
        for (size_t i = 0; i < poolSize; ++i) {
            threads_.emplace_back(
                [this]() {
                    while (!is_destructing_) {
                        std::unique_lock <std::mutex> lock(mutex_);
                        while (!is_destructing_ && tasks_.empty()) {
                            // Unlock the mutex
                            // and go to sleep until it receives a notification.
                            condition_.wait(lock);
                        }
                        
                        if (is_destructing_ ) break;
                        auto task = tasks_.front();
                        tasks_.pop();
                        lock.unlock();
                        task();
                    }
                }
            );
        }
    }


    ~ThreadPool() {
        is_destructing_ = true;
        condition_.notify_all();
        for (auto& t : threads_) {
            t.join();
        }
    }


    template <class Func, class... Args>
    auto exec(Func func, Args... args) -> std::future<decltype(func(args...))> {
        auto task = std::make_shared<std::packaged_task<decltype(func(args...))()>>
            (std::bind(func, args...));
        std::lock_guard<std::mutex> lock(mutex_);
        tasks_.push([task]() { (*task)(); });
        condition_.notify_one();

        return task->get_future();
    }
};