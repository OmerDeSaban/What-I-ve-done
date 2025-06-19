/**
 * @file message_sender.c
 * @brief A user-space program to send a message to a message slot.
 *
 * This program opens a message_slot device file, sets its message channel using ioctl, writes a message
 * to it, and then closes the device.
 */

#include <fcntl.h>          /* open */
#include <unistd.h>         /* write */
#include <sys/ioctl.h>      /* ioctl */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "message_slot.h"

/**
 * @brief Sends the user's message to a message slot.
 * @param argc The number of given user CMD arguments.
 * @param argv The given user CMD arguments.
 * @return SUCCESS, or nothing on error (program exits).
 */
int main(int argc, char **argv) {
    int fd;
    unsigned long channel_id;
    ssize_t bytes_written;
    size_t message_len;

    /* Validate the number of arguments: */
    if (argc != 4) {
        fprintf(stderr, "Error: Invalid number of arguments.\n");
        fprintf(stderr, "Usage: %s <message_slot_file_path> <channel_id> <message>.\n", argv[0]);
        exit(1);
    }

    /* Convert channel_id from string to unsigned long: */
    channel_id = strtoul(argv[2], NULL, 10);
    if (errno == ERANGE) {
        perror("Error: Channel ID conversion failed.");
        exit(1);
    }

    /* Open the message slot device file: */
    fd = open(argv[1], O_WRONLY);
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

    /* Validate the message's length: */
    message_len = strlen(argv[3]);
    if (message_len > MAXIMAL_BUFFER) {
        fprintf(stderr, "Error: Message too long.\n");
        close(fd);
        exit(1);
    }

    /* Write the message: */
    bytes_written = write(fd, argv[3], message_len);
    if (bytes_written < 0 || (size_t)bytes_written != message_len) {
        perror("Error: Write failed.");
        close(fd);
        exit(1);
    }

    close(fd);      /* Close the device */
    return 0;
}
