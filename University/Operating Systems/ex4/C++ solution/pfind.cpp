/**
 * @file pfind.cpp
 * @brief The C++ version of the requierd pfind.c file.
 */

#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <string>
#include <vector>

int validateArgs(int argc, char* argv[]) {
    if (argc != 4) {
        perror("Invalid number of arguments given!");
        return -1;
    }

    char* end;
    int threads = (int) strtol(argv[3], &end, 10);

    if (*end != '\0' || threads <= 0) {
        perror("Invalid number of threads given!");
        return -1;
    }

    return threads;
}

int main(int argc, char* argv[]) {
    int threads = validateArgs(argc, argv);
    if (threads == -1) {
        // error in argument validation!
    }

    std::string directory = argv[1];
    std::string term = argv[2];
}