#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define MAX_CLIENTS 10
#define PORT 8888

int main() {
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    const char* message = "Hello from server";
    int opt = 1;

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        std::cerr << "Socket creation error\n";
        return -1;
    }

    // Attach socket to the port
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        std::cerr << "setsockopt error\n";
        return -1;
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind socket to address and port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        std::cerr << "Bind error\n";
        return -1;
    }

    // Listen for incoming connections
    if (listen(server_fd, MAX_CLIENTS) < 0) {
        std::cerr << "Listen error\n";
        return -1;
    }

    // Accept incoming connections and send message to each one
    while (true) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            std::cerr << "Accept error\n";
            return -1;
        }
        send(new_socket, message, strlen(message), 0);
        close(new_socket);
    }

    return 0;
}

