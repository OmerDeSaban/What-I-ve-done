/**
 * @file message_reader.cpp
 * @brief The C++ version of the required message_reader.c file.
 * 
 * Yes, I know a Linux kernel and C++ don't mix well, but I just wanted to practice some C++, as I
 * already did this assignment in C in the past.
 * Please regard this as a "if you translate this to C, this would be my implementation".
 */

#include "message_slot.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "message_reader: Improper number of arguments given!\n");
        return 1;
    }

    const char* file_path = argv[1];

    char* end;
    long id_long = strtol(argv[2], &end, 10);
    if (*end != '\0' || id_long <= 0) {
        fprintf(stderr, "message_reader: Invalid channel id given!\n");
        return 1;
    }
    unsigned int id = (unsigned int) id_long;

    int fd = open(file_path, O_RDWR);
    if (fd < 0) {
        perror("message_reader: Failed to open file!");
        return 1;
    }

    if (ioctl(fd, MSG_SLOT_CHANNEL, id) != 0) {
        perror("message_reader: ioctl failed!");
        close(fd);
        return 1;
    }

    char buffer[128];

    ssize_t bytes_read = read(fd, buffer, sizeof(buffer));
    if (bytes_read < 0) {
        perror("message_reader: read failed!");
        close(fd);
        return 1;
    }

    if (write(STDOUT_FILENO, buffer, bytes_read) != bytes_read) {
        perror("message_reader: write to stdout failed!");
        close(fd);
        return 1;
    }

    close(fd);
    return 0;
}