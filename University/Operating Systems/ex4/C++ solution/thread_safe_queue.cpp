/**
 * @file thread_safe_queue.cpp
 * @brief A file containing all implementations for the functions declared in the thread_safe_queue.h file.
 */

#include "thread_safe_queue.h"
#include <stdexcept>
#include <cmath>
#include <chrono>

void ThreadSafeQueue::push(const std::string& directory) {
    {
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.push(directory);
    }
    condition_.notify_one();
}

std::optional<std::string> ThreadSafeQueue::pop() {
    std::unique_lock<std::mutex> lock(mutex_);
    
    if (condition_.wait_for(lock, std::chrono::milliseconds(pow(2, 10)), 
                           [this]() { return !queue_.empty(); })) {
        std::string result = queue_.front();
        queue_.pop();
        return result;
    }
    
    return std::nullopt;
}