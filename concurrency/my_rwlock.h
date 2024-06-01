#ifndef UNTITLED_MY_RWLOCK_H
#define UNTITLED_MY_RWLOCK_H

#include <semaphore.h>

class my_rwlock {
private:
    sem_t lock{}, writeLock{};
    int readers;
public:
    my_rwlock(): readers(0) {
        sem_init(&lock, 0, 1);
        sem_init(&writeLock, 0, 1);
    }
    void acq_rlock();
    void rls_rlock();
    void acq_wlock() { sem_wait(&writeLock); }
    void rls_wlock() { sem_post(&writeLock); }
};

#endif
