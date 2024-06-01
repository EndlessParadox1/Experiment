#include "mySem.h"

void my_sem::sem_wait() {
    std::unique_lock<mutex> tmp(lock);
    while (value <= 0)
        cond.wait(tmp);
    value--;
}

void my_sem::sem_post() {
    std::unique_lock<mutex> tmp(lock);
    value++;
    cond.notify_one();
}

void cir_que::put(int v) {
    *(buffer + tail) = v;
    tail = (tail + 1) % size;
    cout << "put " << v << endl;
}

void cir_que::get() {
    int tmp = *(buffer + head);
    head = (head + 1) % size;
    cout << "get " << tmp <<endl;
}

void producer(cir_que& q, int low, int high) {
    for (int i = low; i <= high; i++) {
        q.empty.sem_wait();
        q.lock.sem_wait();
        q.put(i);
        q.lock.sem_post();
        q.full.sem_post();
    }
}

void consumer(cir_que& q, int size) {
    while (size--) {
        q.full.sem_wait();
        q.lock.sem_wait();
        q.get();
        q.lock.sem_post();
        q.empty.sem_post();
    }
}
