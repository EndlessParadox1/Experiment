#ifndef CONCURRENT_MYSORT_H
#define CONCURRENT_MYSORT_H

#include <pthread.h>

typedef struct {
    int low;
    int high;
    int level; // 2^level threads will be used
    int* nums;
} args_q;
int part(int, int, int*);
void qSort(int, int, int*);
void* qSort_th(void*);

typedef struct {
    int *sr, *tr;
    int s, t, level;
} args_m;
void merge(const int*, int*, int, int, int);
void mSort(int*, int*, int, int);
void* mSort_th(void*);

#endif
