/**
 * @file pcc_server.cpp
 * @brief The C++ version of the required pcc_client.c file.
 */

#include <iostream>
#include <cstdlib>
#include <sys/socket.h>
#include <csignal>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <algorithm>
#include <cstring>
#include <atomic>
#include <thread>
#include <chrono>

constexpr int FIRST_PRINTABLE = 32;
uint32_t pcc_total[95] = {0};
unsigned int port;
std::atomic<bool> processing_client{false};
std::atomic<bool> should_exit{false};

bool validArg(int argc, char* argv[], unsigned int& port) {
    if (argc != 2) {
        std::cerr << "pcc_server: Invalid number of arguments given!" << std::endl;
        return false;
    }

    char* end;
    unsigned long parsed = strtoul(argv[1], &end, 10);
    if (*end != '\0' || parsed > 65535) {
        std::cerr << "pcc_server: Invalid port!" << std::endl;
        return false;
    }
    port = static_cast<unsigned int>(parsed);

    return true;
}

bool getServerFD(int& server_fd) {
    server_fd = socket(AF_INET6, SOCK_STREAM, 0);
    if (server_fd < 0) {
        std::cerr << "pcc_server: socket error" << std::endl;
        return false;
    }

    int no = 0;
    if (setsockopt(server_fd, IPPROTO_IPV6, IPV6_V6ONLY, &no, sizeof(no)) < 0) {
        std::cerr << "pcc_server: setsockopt error" << std::endl;
        return false;
    }

    int optval = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0) {
        std::cerr << "pcc_server: setsockopt error" << std::endl;
        return false;
    }

    sockaddr_in6 addr6 = {};
    addr6.sin6_family = AF_INET6;
    addr6.sin6_addr = in6addr_any;
    addr6.sin6_port = htons(port);

    if (bind(server_fd, (sockaddr*)&addr6, sizeof(addr6)) < 0) {
        std::cerr << "pcc_server: bind error" << std::endl;
        return false;
    }

    if (listen(server_fd, 10) < 0) {
        std::cerr << "pcc_server: listen error" << std::endl;
        return false;
    }

    return true;
}

bool receiveFromClient(int server_fd) {
    while (true && !should_exit.load()) {
        sockaddr_storage client_addr;
        socklen_t addr_len = sizeof(client_addr);
        int client_fd = accept(server_fd, (sockaddr*)&client_addr, &addr_len);
        if (client_fd < 0) {
            std::cerr << "pcc_server: accept error" << std::endl;
            continue;
        }
        
        processing_client = true;

        uint32_t net_file_size;
        ssize_t recvd = recv(client_fd, &net_file_size, sizeof(net_file_size), MSG_WAITALL);
        if (recvd != sizeof(net_file_size)) {
            std::cerr << "pcc_server: failed to read file size" << std::endl;
            close(client_fd);
            goto next_client;
        }
        uint32_t file_size = ntohl(net_file_size);

        constexpr size_t BUF_SIZE = 1024 * 1024;
        char buffer[BUF_SIZE];
        uint32_t printable_count = 0;
        size_t bytes_read_total = 0;

        while (bytes_read_total < file_size) {
            size_t bytes_left = file_size - bytes_read_total;
            ssize_t chunk = recv(client_fd, buffer, std::min(BUF_SIZE, bytes_left), 0);
            if (chunk < 0) {
                if (errno == ETIMEDOUT || errno == ECONNRESET || errno == EPIPE) {
                    std::cerr << "pcc_server: TCP error - " << strerror(errno) << std::endl;
                    close(client_fd);
                    goto next_client;
                } else {
                    std::perror("pcc_server: recv error");
                    close(client_fd);
                    return false;
                }
            } else if (chunk == 0) {
                std::cerr << "pcc_server: client disconnected unexpectedly" << std::endl;
                close(client_fd);
                goto next_client;
            }

            for (ssize_t i = 0; i < chunk; ++i) {
                unsigned char c = buffer[i];
                if (c >= 32 && c <= 126) {
                    ++printable_count;
                    ++pcc_total[c - FIRST_PRINTABLE];
                }
            }

            bytes_read_total += chunk;
        }

        uint32_t net_result = htonl(printable_count);
        if (send(client_fd, &net_result, sizeof(net_result), 0) != sizeof(net_result)) {
            std::cerr << "pcc_server: failed to send result" << std::endl;
        }

        close(client_fd);
        processing_client = false;
        
        if (should_exit.load()) { 
            break; 
        }
        continue;

        next_client:
        processing_client = false;
        continue;
    }
    return true;
}

void printSummary(int) {
    should_exit = true;
    while (processing_client.load()) { std::this_thread::sleep_for(std::chrono::milliseconds(10)); }
    
    for (int i = 0; i < 95; ++i)
        { std::printf("char '%c' : %u times\n", i + FIRST_PRINTABLE, pcc_total[i]); }
    std::printf("\n");
    exit(EXIT_SUCCESS);
}

int main(int argc, char* argv[]) {
    unsigned int port;
    if (!validArg(argc, argv, port)) { return EXIT_FAILURE; }

    int server_fd;
    if (!getServerFD(server_fd)) { return EXIT_FAILURE; }

    signal(SIGINT, printSummary);

    if (!receiveFromClient(server_fd)) { return EXIT_FAILURE; }

    return EXIT_SUCCESS;
}