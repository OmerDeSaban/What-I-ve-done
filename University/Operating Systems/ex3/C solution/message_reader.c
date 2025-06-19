/**
 * @file message_reader.c
 * @brief A user-space program to read a message from a message slot.
 *
 * This program opens a message_slot device file, sets its message channel using ioctl, reads a message
 * from it, prints the message to the standard output, and then closes the device.
 */

#include <fcntl.h>          /* open */
#include <unistd.h>         /* read */
#include <sys/ioctl.h>      /* ioctl */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "message_slot.h"

/**
 * @brief Reads a message from a message slot.
 * @param argc The number of given user CMD arguments.
 * @param argv The given user CMD arguments.
 * @return SUCCESS, or nothing on error (program exits).
 */
int main(int argc, char **argv) {
    int fd;
    unsigned long channel_id;
    char buffer[MAXIMAL_BUFFER];
    ssize_t bytes_read;

    /* Validate the number of arguments: */
    if (argc != 3) {
        fprintf(stderr, "Error: Invalid number of arguments.\n");
        fprintf(stderr, "Usage: %s <message_slot_file_path> <channel_id>.\n", argv[0]);
        exit(1);
    }

    /* Convert channel_id from string to unsigned long: */
    channel_id = strtoul(argv[2], NULL, 10);
    if (errno == ERANGE) {
        perror("Error: Channel ID conversion failed.");
        exit(1);
    }

    /* Open the message slot device file: */
    fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        perror("Error: Could not open message slot device file.");
        exit(1);
    }

    /* Set the channel id using ioctl: */
    if (ioctl(fd, MSG_SLOT_CHANNEL, channel_id) < 0) {
        perror("Error: IOCTL failed.");
        close(fd);
        exit(1);
    }

    /* Read the message: */
    bytes_read = read(fd, buffer, MAXIMAL_BUFFER);
    if (bytes_read < 0) {
        perror("Error: Read failed.");
        close(fd);
        exit(1);
    }

    /* Write the message to standard output: */
    if (write(STDOUT_FILENO, buffer, bytes_read) != bytes_read) {
        perror("Error: Write to standard output failed.");
        close(fd);
        exit(1);
    }

    close(fd);      /* Close the device */
    return 0;
}
