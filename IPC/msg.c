#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <string.h>


// Define the message structure
struct Message {
    long mtype; 
    // `0` for the first, `> 0` for the first of such type, when using in msgrcv
    char mtext[256];
};

int main() {
    key_t key;
    int msgid;
    struct Message message;
    // Generate a unique key for the message queue
    key = ftok(".", 1);
    if (key == -1) {
        perror("ftok");
        exit(1);
    }

    // Create a message queue
    msgid = msgget(key, 0644 | IPC_CREAT); 
    // same key and only privilege when open a msg; key can be IPC_PRIVATE if single
    if (msgid == -1) {
        perror("msgget");
        exit(1);
    }

    // Send a message
    message.mtype = 1; // message type (can be any positive number)
    strcpy(message.mtext, "Hello from Process A!");
    if (msgsnd(msgid, &message, sizeof(message.mtext), 0) == -1) {
        perror("msgsnd");
        exit(1);
    }

    printf("Process A sent: %s\n", message.mtext);

    // Receive a message
    if (msgrcv(msgid, &message, sizeof(message.mtext), 1, 0) == -1) {
        perror("msgrcv");
        exit(1);
    }

    printf("Process B received: %s\n", message.mtext);

    // Remove the message queue
    if (msgctl(msgid, IPC_RMID, NULL) == -1) {
        perror("msgctl");
        exit(1);
    }

    return 0;
}
