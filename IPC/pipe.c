#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    int pipefd[2];
    char buffer[100];
    pid_t pid;

    // Create the pipe
    if (pipe(pipefd) == -1) {
        // 半双工, 0 only for read, and 1 only for write
        perror("pipe");
        return 1;
    }

    pid = fork();
    if (pid == -1) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {  // Child process
        close(pipefd[1]);  // Close write end

        read(pipefd[0], buffer, 18);
        printf("Child process received: %s\n", buffer);

        close(pipefd[0]);
    } else {  // Parent process
        close(pipefd[0]);  // Close read end

        write(pipefd[1], "Hello from parent", 18);

        close(pipefd[1]);
    }

    return 0;
}
