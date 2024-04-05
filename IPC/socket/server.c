#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "/tmp/socket"

int main() {
    int server_fd, client_fd, len;
    struct sockaddr_un server_addr, client_addr;
    char buffer[256];
    // Create socket
    server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket");
        exit(1);
    }
    // Set address
    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, SOCKET_PATH);
    // Bind socket to the address
    len = strlen(server_addr.sun_path) + sizeof(server_addr.sun_family);
    if (bind(server_fd, (struct sockaddr *)&server_addr, len) == -1) {
        perror("bind");
        exit(1);
    }
    // Listen for incoming connections
    if (listen(server_fd, 5) == -1) {
        // buffer 5 requests
        perror("listen");
        exit(1);
    }
    printf("Server is listening...\n");
    // Accept incoming connection
    len = sizeof(client_addr);
    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &len);
    if (client_fd == -1) {
        perror("accept");
        exit(1);
    }
    printf("Client connected\n");
    // Receive data from client
    if (recv(client_fd, buffer, sizeof(buffer), 0) == -1) {
        perror("recv");
        exit(1);
    }
    printf("Received message from client: %s\n", buffer);
    // Close sockets
    close(client_fd);
    close(server_fd);
    // Clean up socket file
    unlink(SOCKET_PATH);
    return 0;
}
