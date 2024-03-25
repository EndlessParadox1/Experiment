#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <string.h>
#include <wait.h>


int main() {
    key_t key = ftok(".", 2);
    int shmid = shmget(key, 1024, IPC_CREAT | 0666);
    char *shmaddr = (char *)shmat(shmid, NULL, 0); // attach to the shared mem; `0` for WR

    pid_t pid = fork();
    if(pid == -1) {
        perror("fork error");
        exit(1);
    } 

    if (pid == 0) {
        strcpy(shmaddr, "Hello from shared memory");
    } else {
        waitpid(pid, NULL, 0);
        printf("Message written to shared memory: %s\n", shmaddr);
        shmdt(shmaddr); // detach
        if(shmctl(shmid, IPC_RMID, NULL) == -1) {
            perror("shmctl error");
            exit(1);
        }
    }

    return 0;
}
