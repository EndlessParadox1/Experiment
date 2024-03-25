#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>


int main() {
    int sockfd[2]; 
    pid_t pid;
    char parent_msg[] = "Hello child!";
    char child_msg[] = "Hi parent!";

    // Create a socket pair
    if (socketpair(AF_UNIX, SOCK_STREAM, 0, sockfd) == -1) {
        // domain, type, protocol(auto); 全双工
        perror("Socketpair creation failed");
        exit(1);
    }

    // Fork a child process
    pid = fork();

    if (pid == -1) {
        perror("Fork failed");
        exit(1);
    }

    if (pid == 0) {
        // Child process
        close(sockfd[1]); 

        char buffer[256];
        read(sockfd[0], buffer, 13); // Read from the socket
        printf("Child received: %s\n", buffer);

        // Send a message back to the parent
        write(sockfd[0], child_msg, sizeof(child_msg));

        close(sockfd[0]); 
    } else {
        // Parent process
        close(sockfd[0]); 

        // Send a message to the child
        write(sockfd[1], parent_msg, sizeof(parent_msg));

        char buffer[256];
        read(sockfd[1], buffer, 11); // Read from the socket
        printf("Parent received: %s\n", buffer);

        close(sockfd[1]); 
    }

    return 0;
}
