#ifndef UNTITLED_MYSEM_H
#define UNTITLED_MYSEM_H

#include <iostream>
#include <vector>
#include <mutex>
#include <condition_variable>

using std::cout;
using std::endl;
using std::vector;
using std::mutex;

class my_sem {
private:
    int value;
    mutex lock;
    std::condition_variable cond;
public:
    explicit my_sem(int v): value(v) {};
    void sem_wait();
    void sem_post();
};

class cir_que{
private:
    int head = 0, tail = 0, size;
    int* buffer;
public:
    my_sem lock, empty, full;
    explicit cir_que(int s):
        size(s),
        lock(1),
        empty(s),
        full(0) {
        buffer = new int[s];
    };
    void put(int v);
    void get();
    ~cir_que() {
        delete[] buffer;
    }
}; // A circular cir_que with mutex and condition variables

void producer(cir_que& q, int low, int high);
void consumer(cir_que& q, int size);

#endif
