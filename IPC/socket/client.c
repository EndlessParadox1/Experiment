#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "/tmp/socket"

int main() {
    int client_fd, len;
    struct sockaddr_un server_addr;
    char buffer[256];
    // Create socket
    client_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (client_fd == -1) {
        perror("socket");
        exit(1);
    }
    // Set address
    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, SOCKET_PATH);
    // Connect to server
    len = strlen(server_addr.sun_path) + sizeof(server_addr.sun_family);
    if (connect(client_fd, (struct sockaddr *)&server_addr, len) == -1) {
        perror("connect");
        exit(1);
    }
    printf("Connected to server\n");
    // Send data to server
    strcpy(buffer, "Hello, server!");
    if (send(client_fd, buffer, strlen(buffer), 0) == -1) {
        perror("send");
        exit(1);
    }
    printf("Message sent to server\n");
    // Close socket
    close(client_fd);
    return 0;
}
