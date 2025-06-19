/**
 * @file message_sender.cpp
 * @brief The C++ version of the required message_sender.c file.
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
    if (argc != 4) {
        fprintf(stderr, "message_sender: Improper number of arguments given!\n");
        return 1;
    }

    const char* file_path = argv[1];

    char* end;
    long id_long = strtol(argv[2], &end, 10);
    if (*end != '\0' || id_long <= 0) {
        fprintf(stderr, "message_sender: Invalid channel id given!\n");
        return 1;
    }
    unsigned int id = (unsigned int) id_long;

    const char* msg = argv[3];
    size_t msg_length = strlen(msg);

    if (msg_length == 0) {
        fprintf(stderr, "message_sender: Empty message given!\n");
        return 1;
    }

    if (msg_length > 128) {
        fprintf(stderr, "message_sender: A message longer then 128 bytes given!\n");
        return 1;
    }

    int fd = open(file_path, O_RDWR);
    if (fd < 0) {
        perror("message_sender: Failed to open file!");
        return 1;
    }

    if (ioctl(fd, MSG_SLOT_CHANNEL, id) != 0) {
        perror("message_sender: ioctl failed!");
        close(fd);
        return 1;
    }

    if (write(fd, msg, msg_length) != (ssize_t) msg_length) {
        perror("message_sender: write failed!");
        close(fd);
        return 1;
    }

    close(fd);
    return 0;
}