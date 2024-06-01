#include "my_rwlock.h"

void my_rwlock::acq_rlock() {
    sem_wait(&lock);
    if (++readers == 1)
        sem_wait(&writeLock);
    sem_post(&lock);
}

void my_rwlock::rls_rlock() {
    sem_wait(&lock);
    if (--readers == 0)
        sem_post(&writeLock);
    sem_post(&lock);
}
