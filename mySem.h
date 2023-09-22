#ifndef CONCURRENT_MYSEM_H
#define CONCURRENT_MYSEM_H

#include <pthread.h>
#include <stdio.h>

typedef struct {
    int value;
    pthread_mutex_t lock;
    pthread_cond_t cond;
} my_sem;
void sem_init(my_sem*, int);
void sem_wait(my_sem*);
void sem_post(my_sem*);


typedef struct {
    int head, tail, size, *buffer;
    my_sem lock, empty, full;
} queue; // A circular queue with mutex and condition variables
typedef struct {
    int low, high;
    queue* q;
} args;
void put(int, queue*);
int get(queue*);
void* producer(void*);
void* consumer(void*);

#endif
