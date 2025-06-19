/**
 * @file pcc_client.cpp
 * @brief The C++ version of the required pcc_client.c file.
 */

#include <iostream>
#include <string>
#include <arpa/inet.h>
#include <filesystem>
#include <fcntl.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <limits>
#include <cstdlib>

constexpr size_t BUF_SIZE = 1024 * 1024;

bool validArgs(int argc, char* argv[], int& family, sockaddr_storage& addr, unsigned int& port) {
    if (argc != 4) {
        std::cerr << "pcc_client: Invalid number of arguments given!" << std::endl;
        return false;
    }

    // What type of IP is argv[1], and is it even a valid IP check:
    if (inet_pton(AF_INET, argv[1], &((sockaddr_in*)&addr)->sin_addr) == 1) {
        family = AF_INET;
        sockaddr_in* a = (sockaddr_in*)&addr;
        a->sin_family = AF_INET;
    }
    else if (inet_pton(AF_INET6, argv[1], &((sockaddr_in6*)&addr)->sin6_addr) == 1) {
        family = AF_INET6;
        sockaddr_in6* a = (sockaddr_in6*)&addr;
        a->sin6_family = AF_INET6;
    }
    else {
        std::cerr << "pcc_client: Invalid IP address format!" << std::endl;
        return false;
    }
    
    // Is argv[2] a valid server's port:
    char* end;
    unsigned long parsed = strtoul(argv[2], &end, 10);
    if (*end != '\0' || parsed > 65535) {
        std::cerr << "pcc_client: Invalid port!" << std::endl;
        return false;
    }
    port = static_cast<unsigned int>(parsed);

    // Setting the port number in the appropriate socket address structure
    if (family == AF_INET) { ((sockaddr_in*)&addr)->sin_port = htons(port); }
    else { ((sockaddr_in6*)&addr)->sin6_port = htons(port); }


    // Is argv[3] an existing file's path:
    if (!(std::filesystem::exists(argv[3]) && std::filesystem::is_regular_file(argv[3]))) {
        std::cerr << "pcc_client: Invalid file path!" << std::endl;
        return false;
    }

    return true;
}

bool openFile(const char* file_path, int& file_fd, uint32_t& file_size, uint32_t& net_file_size) {
    file_fd = open(file_path, O_RDONLY);
    if (file_fd < 0) {
        std::cerr << "pcc_client: Failed to open file!" << std::endl;
        return false;
    }

    auto file_size_check = std::filesystem::file_size(file_path);
    if (file_size_check > UINT32_MAX) {
        std::cerr << "pcc_client: Given file is too large!" << std::endl;
        close(file_fd);
        return false;
    }
    file_size = static_cast<uint32_t>(file_size_check);
    net_file_size = htonl(file_size);

    return true;
}

void closeAll(int file_fd, int sockfd) {
    close(file_fd);
    close(sockfd);
}

bool connectTCP(int& family, sockaddr_storage& server_addr, int file_fd, int& sockfd,
    socklen_t& addrlen) {
        sockfd = socket(family, SOCK_STREAM, 0);
        if (sockfd < 0) {
            std::cerr << "pcc_client: Socket creation failed!\n" << std::endl;
            close(file_fd);
            return false;
        }

        addrlen = (family == AF_INET) ? sizeof(sockaddr_in) : sizeof(sockaddr_in6);

        if (connect(sockfd, (sockaddr*)&server_addr, addrlen) < 0) {
            std::cerr << "pcc_client: Connection failed!\n" << std::endl;
            closeAll(file_fd, sockfd);
            return false;
        }

        return true;
}

bool sendToServer(int sockfd, uint32_t net_file_size, int file_fd, char buffer[],
    ssize_t& bytes_read) {
        if (send(sockfd, &net_file_size, sizeof(net_file_size), 0) != sizeof(net_file_size)) {
            std::cerr << "pcc_client: Failed to send file size!" << std::endl;
            closeAll(file_fd, sockfd);
            return false;
        }

        while ((bytes_read = read(file_fd, buffer, BUF_SIZE)) > 0) {
            ssize_t total_sent = 0;
            while (total_sent < bytes_read) {
                ssize_t bytes_sent = send(sockfd, buffer + total_sent, bytes_read - total_sent, 0);
                if (bytes_sent < 0) {
                    std::perror("pcc_client: Failed to send file content");
                    closeAll(file_fd, sockfd);
                    return false;
                }
                total_sent += bytes_sent;
            }
        }

        if (bytes_read < 0) {
            std::perror("pcc_client: Failed to read file");
            closeAll(file_fd, sockfd);
            return false;
        }

        return true;
}

bool receiveFromServer(uint32_t& net_result, ssize_t& recvd, int file_fd, int sockfd, 
    uint32_t& printable_count) {
        recvd = recv(sockfd, &net_result, sizeof(net_result), MSG_WAITALL);
        if (recvd != sizeof(net_result)) {
            std::perror("pcc_client: Failed to receive printable character count");
            closeAll(file_fd, sockfd);
            return false;
        }

        printable_count = ntohl(net_result);
        std::printf("# of printable characters: %u\n", printable_count);
        return true;
}

int main(int argc, char* argv[]) {
    sockaddr_storage server_addr;
    int family;
    unsigned int port;
    if (!validArgs(argc, argv, family, server_addr, port)) { return EXIT_FAILURE; }

    int file_fd;
    uint32_t file_size;
    uint32_t net_file_size;
    if (!openFile(argv[3], file_fd, file_size, net_file_size)) { return EXIT_FAILURE; }

    int sockfd;
    socklen_t addrlen;
    if (!connectTCP(family, server_addr, file_fd, sockfd, addrlen)) { return EXIT_FAILURE; }

    char buffer[BUF_SIZE];
    ssize_t bytes_read;
    if (!sendToServer(sockfd, net_file_size, file_fd, buffer, bytes_read)) { return EXIT_FAILURE; }

    uint32_t net_result;
    ssize_t recvd;
    uint32_t printable_count;
    if (!receiveFromServer(net_result, recvd, file_fd, sockfd, printable_count)) { return EXIT_FAILURE; }

    closeAll(file_fd, sockfd);
    return EXIT_SUCCESS;
}