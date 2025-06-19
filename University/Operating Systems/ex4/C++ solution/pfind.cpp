/**
 * @file pfind.cpp
 * @brief The C++ version of the required pfind.c file.
 */

#include "thread_safe_queue.h"
#include <thread>
#include <vector>
#include <dirent.h>
#include <iostream>
#include <cerrno>
#include <cstring>
#include <sys/stat.h>
#include <unistd.h>
#include <atomic>
#include <chrono>
#include <cmath>

ThreadSafeQueue directory_queue;
std::atomic<int> files_found{0};
std::atomic<bool> error_occurred{false};
std::atomic<bool> start_searching{false};
std::condition_variable start_cv;
std::mutex start_mutex;
std::atomic<int> active_threads{0};

int validateArgs(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Invalid number of arguments given!" << std::endl;
        return -1;
    }

    char* end;
    int num_threads = (int) strtol(argv[3], &end, 10);

    if (*end != '\0' || num_threads <= 0) {
        std::cerr << "Invalid number of threads given!" << std::endl;
        return -1;
    }

    return num_threads;
}

void process_directory(const std::string& directory_path, const std::string& term) {
    DIR* dir = opendir(directory_path.c_str());

    if (dir == NULL) {
        std::cerr << "Error opening directory " << directory_path << ": " << strerror(errno) << std::endl;
        error_occurred = true;
        return;
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) { continue; }

        std::string full_path = directory_path + "/" + entry->d_name;

        struct stat file_stat;
        if (stat(full_path.c_str(), &file_stat) == -1) {
            error_occurred = true;
            continue;
        }

        if (S_ISDIR(file_stat.st_mode)) {
            if (access(full_path.c_str(), R_OK | X_OK) == 0) { directory_queue.push(full_path); }
            else { std::printf("Directory %s: Permission denied.\n", full_path.c_str()); }
        }
        else if (S_ISREG(file_stat.st_mode)) {
            if (strstr(entry->d_name, term.c_str()) != nullptr) {
                std::printf("%s\n", full_path.c_str());
                files_found++;
            }
        }
    }
    
    closedir(dir);
}

void searchingThread(const std::string& term) {
    {
        std::unique_lock<std::mutex> lock(start_mutex);
        start_cv.wait(lock, []{ return start_searching.load(); });
    }

    while (true) {
        auto curr_directory = directory_queue.pop();
        if (!curr_directory.has_value()) { 
            std::this_thread::sleep_for(std::chrono::milliseconds(pow(2, 6)));
            curr_directory = directory_queue.pop();
            if (!curr_directory.has_value()) { break;  }
        }
        
        process_directory(curr_directory.value(), term);
    }
}

int main(int argc, char* argv[]) {
    int num_threads = validateArgs(argc, argv);
    if (num_threads == -1) { exit(1); }

    std::string root_directory = argv[1];
    directory_queue.push(root_directory);
    std::string term = argv[2];
    
    std::vector<std::thread> searching_threads;
    for (int i = 0; i < num_threads; i++) { searching_threads.emplace_back(searchingThread, term); }
    {
        std::lock_guard<std::mutex> lock(start_mutex);
        start_searching = true;
    }
    start_cv.notify_all();
    
    for (auto& thread : searching_threads) { thread.join(); }
    
    std::printf("Done searching, found %d files\n", files_found.load());
    
    return error_occurred.load() ? 1 : 0;
}