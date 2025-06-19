/**
 * @file pcc_server.c
 * @brief A Printable Characters Counting (PCC) server implementation.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <errno.h>

#define BUFFER_SIZE 1048576 /* 1MB buffer size */
#define BACKLOG 10          /* Listen queue size */
#define NUM_PRINTABLE 95    /* Number of printable characters (32-126) */

/* Global variables: */
static volatile sig_atomic_t got_sigint = 0;    /* SIGINT received flag */
static uint32_t pcc_total[NUM_PRINTABLE] = {0}; /* Count for each printable character */
static int processing_client = 0;               /* Flag for client processing status */
static int server_socket = -1;                  /* Global server socket */

/* Function declarations: */
static void handle_sigint(int);
static void handle_error(const char*);
static uint32_t count_printable(const char*, size_t, uint32_t*);
static void handle_client(int);
static void cleanup(void);

/**
 * Signal handler for SIGINT.
 * @param signum The signal number received.
 */
static void handle_sigint(int signum) { got_sigint = 1; }

/**
 * Handles error conditions by printing error message and exiting.
 * @param msg The error message to print.
 */
static void handle_error(const char* msg) {
    perror(msg);
    exit(1);
}

/**
 * Counts printable characters in a buffer and updates character statistics.
 * @param buffer The input buffer to process.
 * @param size Size of the buffer.
 * @param char_counts Array to store individual character counts.
 * @return Total number of printable characters found.
 */
static uint32_t count_printable(const char* buffer, size_t size, uint32_t* char_counts) {
    uint32_t count = 0;
    for (size_t i = 0; i < size; i++) {
        unsigned char c = buffer[i];
        if (c >= 32 && c <= 126) {
            count++;
            char_counts[c - 32]++;
        }
    }
    return count;
}

/**
 * Handles an individual client connection.
 * @param client_fd The client socket file descriptor.
 */
static void handle_client(int client_fd) {
    uint32_t temp_counts[NUM_PRINTABLE] = {0};
    uint32_t total_printable = 0;
    
    /* Read file size from client */
    uint32_t file_size_network;
    ssize_t recv_size = read(client_fd, &file_size_network, sizeof(file_size_network));
    if (recv_size != sizeof(file_size_network)) {
        if (recv_size == -1) { fprintf(stderr, "Error reading file size: %s\n", strerror(errno)); }
        else { fprintf(stderr, "Incomplete file size received\n"); }
        return;
    }
    
    uint32_t file_size = ntohl(file_size_network);
    uint32_t bytes_remaining = file_size;
    char buffer[BUFFER_SIZE];

    /* Read file content and count printable characters */
    while (bytes_remaining > 0 && !got_sigint) {
        size_t to_read = bytes_remaining < BUFFER_SIZE ? bytes_remaining : BUFFER_SIZE;
        ssize_t bytes_read = read(client_fd, buffer, to_read);
        
        if (bytes_read <= 0) {
            if (bytes_read == -1 && (errno == ETIMEDOUT || errno == ECONNRESET || errno == EPIPE)) {
                fprintf(stderr, "Connection error: %s\n", strerror(errno));
            }
            else if (bytes_read == 0) { fprintf(stderr, "Connection closed by client\n"); }
            else { fprintf(stderr, "Read error: %s\n", strerror(errno)); }
            return;
        }

        total_printable += count_printable(buffer, bytes_read, temp_counts);
        bytes_remaining -= bytes_read;
    }

    /* Send count back to client */
    uint32_t count_network = htonl(total_printable);
    if (write(client_fd, &count_network, sizeof(count_network)) != sizeof(count_network)) {
        fprintf(stderr, "Error sending count: %s\n", strerror(errno));
        return;
    }

    /* Update global statistics only if connection completed successfully */
    if (bytes_remaining == 0) {
        for (int i = 0; i < NUM_PRINTABLE; i++) { pcc_total[i] += temp_counts[i]; }
    }
}

/**
 * Performs cleanup operations when the server is shutting down.
 * Takes no arguments.
 */
static void cleanup(void) {
    if (server_socket != -1) {
        close(server_socket);
        server_socket = -1;
    }
}

/**
 * Main server function. Sets up the server socket, handles client connections,
 * and manages server shutdown.
 */
int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    /* Set up signal handler */
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = handle_sigint;
    if (sigaction(SIGINT, &sa, NULL) == -1) { handle_error("sigaction"); }

    /* Create socket */
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) { handle_error("socket"); }

    /* Set SO_REUSEADDR option */
    int opt = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
        handle_error("setsockopt");
    }

    /* Set up server address */
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(atoi(argv[1]));

    /* Bind socket */
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        handle_error("bind");
    }

    /* Listen for connections */
    if (listen(server_socket, BACKLOG) == -1) { handle_error("listen"); }

    /* Main server loop */
    while (!got_sigint) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        
        int client_fd = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);
        if (client_fd == -1) {
            if (errno == EINTR) { continue; }   /* SIGINT received during accept */
            handle_error("accept");
        }

        processing_client = 1;
        handle_client(client_fd);
        processing_client = 0;
        close(client_fd);

        if (got_sigint) { break; }
    }

    /* Print statistics when SIGINT received */
    for (int i = 0; i < NUM_PRINTABLE; i++) { printf("char '%c' : %u times\n", i + 32, pcc_total[i]); }

    cleanup();
    return 0;
}