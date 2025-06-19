/**
 * @file thread_safe_queue.h
 * @brief A header file containing all declarations for the ThreadSafeQueue class.
 * This class will be used to implement the needed FIFO queue for the assignment.
 */

#pragma once

#include <queue>
#include <string>
#include <mutex>
#include <condition_variable>
#include <optional>

class ThreadSafeQueue {
    private:
        std::queue<std::string> queue_;
        mutable std::mutex mutex_;
        std::condition_variable condition_;
    
    public:
        /* --------------------------------------- CONSTRUCTORS -------------------------------------- */

        /** Default constructor. */
        ThreadSafeQueue() = default;
        /** Constructs a thread safe FIFO queue with the given queue. */
        ThreadSafeQueue(const std::queue<std::string>& my_queue)
            : queue_(my_queue), mutex_(), condition_() { }
        /** Default deconstructor. */
        ~ThreadSafeQueue() = default;

        /* ------------------------------------- OTHER FUNCTIONS ------------------------------------- */
        
        /** Adds a new directory to the queue's end. */
        void push(const std::string& directory);
        /** Removes the queue's first directory. */
        std::optional<std::string> pop();
};