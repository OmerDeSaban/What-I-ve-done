/**
 * @file pcc_client.c
 * @brief A Printable Characters Counting (PCC) client implementation.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>

#define BUFFER_SIZE 1048576 /* 1MB buffer size */

/* Function declarations: */
static void handle_error(const char*);
static void send_file(int, int, off_t);
static uint32_t receive_count(int);

/**
 * Handles error conditions by printing error message and exiting.
 * @param msg The error message to print.
 */
static void handle_error(const char* msg) {
    perror(msg);
    exit(1);
}

/**
 * Sends file contents to the server over a socket connection.
 * @param sockfd The socket file descriptor.
 * @param fd The file descriptor of the file to send.
 * @param file_size The size of the file in bytes.
 */
static void send_file(int sockfd, int fd, off_t file_size) {
    char buffer[BUFFER_SIZE];
    size_t total_sent = 0;
    ssize_t bytes_read;

    /* Send file size to server (in network byte order) */
    uint32_t size_to_send = htonl((uint32_t)file_size);
    if (write(sockfd, &size_to_send, sizeof(size_to_send)) != sizeof(size_to_send)) {
        handle_error("write");
    }

    /* Send file contents */
    while ((bytes_read = read(fd, buffer, BUFFER_SIZE)) > 0) {
        ssize_t bytes_sent = 0;
        while (bytes_sent < bytes_read) {
            ssize_t sent = write(sockfd, buffer + bytes_sent, bytes_read - bytes_sent);
            if (sent == -1) {
                if (errno == EPIPE || errno == ECONNRESET || errno == ETIMEDOUT) {
                    fprintf(stderr, "Connection error: %s\n", strerror(errno));
                    exit(1);
                }
                handle_error("write");
            }
            bytes_sent += sent;
        }
        total_sent += bytes_sent;
    }

    if (bytes_read == -1) { handle_error("read"); }
}

/**
 * Receives the count of printable characters from the server.
 * @param sockfd The socket file descriptor.
 * @return The count received from the server.
 */
static uint32_t receive_count(int sockfd) {
    uint32_t count_network;
    ssize_t recv_size = read(sockfd, &count_network, sizeof(count_network));
    
    if (recv_size == -1) { handle_error("read"); }
    else if (recv_size == 0) {
        fprintf(stderr, "Connection closed by server\n");
        exit(1);
    }

    return ntohl(count_network);
}

/**
 * Main client function. Connects to the server, sends a file,
 * and receives the count of printable characters.
 */
int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <server_ip> <server_port> <file_path>\n", argv[0]);
        exit(1);
    }

    /* Parse command line arguments */
    const char* server_ip = argv[1];
    uint16_t server_port = (uint16_t)atoi(argv[2]);
    const char* file_path = argv[3];

    /* Open the file */
    int fd = open(file_path, O_RDONLY);
    if (fd == -1) { handle_error("open"); }

    /* Get file size */
    off_t file_size = lseek(fd, 0, SEEK_END);
    if (file_size == -1) { handle_error("lseek"); }
    if (lseek(fd, 0, SEEK_SET) == -1) { handle_error("lseek"); }

    /* Create socket */
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) { handle_error("socket"); }

    /* Set up server address */
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0) { handle_error("inet_pton"); }

    /* Connect to server */
    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        handle_error("connect");
    }

    /* Send file to server */
    send_file(sockfd, fd, file_size);

    /* Receive and print count of printable characters */
    uint32_t count = receive_count(sockfd);
    printf("# of printable characters: %u\n", count);

    /* Clean up */
    close(sockfd);
    close(fd);

    return 0;
}