#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>


void signal_handler(int sig) {
    if(sig == SIGTERM) 
        exit(0); // avoid SIGINT
}

int main() {
    pid_t pid, sid;

    // Fork off the parent process
    pid = fork();
    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    }
    if (pid > 0) {
        // Exit the parent process
        exit(0);
    }

    // Change the file mode mask
    umask(0);

    // Create a new session, avoid SIGHUP
    sid = setsid();
    if (sid < 0) {
        perror("Failed to create new session");
        exit(1);
    }

    // Change the working directory to root
    if (chdir("/root/") < 0) {
        perror("Failed to change directory");
        exit(1);
    }

    // Close standard file descriptors
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    // Set up signal handlers
    signal(SIGTERM, signal_handler);
    signal(SIGINT, signal_handler);

    // Main daemon loop
    while (1) {
        // Perform daemon tasks here
        int fd = open("daemon.log", O_CREAT | O_WRONLY | O_APPEND, 0644);
        write(fd, "Daemon is running\n", 18);
        // Sleep for a while
        sleep(5);
    }

    return 0;
}
