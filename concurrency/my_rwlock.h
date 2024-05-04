#ifndef UNTITLED_MY_RWLOCK_H
#define UNTITLED_MY_RWLOCK_H

#include <semaphore.h>

class [[maybe_unused]] my_rwlock {
private:
    sem_t lock{}, writeLock{};
    int readers;
public:
    my_rwlock(): readers(0) {
        sem_init(&lock, 0, 1);
        sem_init(&writeLock, 0, 1);
    }

    [[maybe_unused]] void acq_rlock();

    [[maybe_unused]] void rls_rlock();

    [[maybe_unused]] void acq_wlock() {sem_wait(&writeLock);}

    [[maybe_unused]] void rls_wlock() {sem_post(&writeLock);}
};

#endif
