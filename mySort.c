#include "mySort.h"

int part(int low, int high, int* nums) {
    int temp = *(nums + low), key = *(nums + low);
    while(low < high) {
        while(low < high && *(nums + high) >= key) {
            high --;
            *(nums + low) = *(nums + high);
        }
        while(low < high && *(nums + low) <= key) {
            low ++;
            *(nums + high) = *(nums + low);
        }
    }
    *(nums + low) = temp;
    return low;
}

void qSort(int low, int high, int* nums) {
    if (low < high) {
        int loc = part(low, high, nums);
        qSort(low, loc, nums);
        qSort(loc + 1, high, nums);
    }
}

void* qSort_th(void* args_q_) {
    args_q* tmp = (args_q*)args_q_;
    int low = tmp->low, high = tmp->high, level = tmp->level;
    int* nums = tmp->nums;
    if (low < high) {
        int loc = part(low, high, nums);
        if(level) {
            pthread_t p1, p2;
            args_q tmp1 = {low, loc, level - 1, nums};
            pthread_create(&p1, NULL, qSort_th, &tmp1);
            args_q tmp2 = {loc + 1, high, level - 1, nums};
            pthread_create(&p2, NULL, qSort_th, &tmp2);
            pthread_join(p1, NULL);
            pthread_join(p2, NULL);
        } else {
            qSort(low, loc, nums);
            qSort(loc + 1, high, nums);
        }
    }
    return NULL;
}


void merge(const int* sr, int* tr, int i, int m, int n) {
    int j, k;
    for(j = m+1,k = i; i <= m && j <= n; k++) {
        if(*(sr + i) < *(sr + j))
            *(tr + k) = *(sr + i++);
        else
            *(tr + k) = *(sr + j++);
    }
    while(i <= m)
        *(tr + k++) = *(sr + i++);
    while(j <= n)
        *(tr + k++) = *(sr + j++);
}

void mSort(int* sr, int* tr, int s, int t) {
    if(s == t)
        *(tr + s) = *(sr + s);
    else {
        int m = (s+t)/2;
        mSort(sr, tr, s, m);
        mSort(sr, tr, m+1, t);
        merge(tr, tr, s, m, t);
    }
}

void* mSort_th(void* args_) {
    args_m* tmp = (args_m*)args_;
    int s = tmp->s, t = tmp->t, level = tmp->level;
    int *sr = tmp->sr, *tr = tmp->tr;
    if(s == t)
        *(tr + s) = *(sr + s);
    else {
        int m = (s+t)/2;
        if(level) {
            pthread_t p1, p2;
            args_m tmp1 = {sr, tr, s, m, level - 1};
            pthread_create(&p1, NULL, mSort_th, &tmp1);
            args_m tmp2 = {sr, tr, m + 1, t, level - 1};
            pthread_create(&p2, NULL, mSort_th, &tmp2);
            pthread_join(p1, NULL);
            pthread_join(p2, NULL);
        } else {
            mSort(sr, tr, s, m);
            mSort(sr, tr, m+1, t);
        }
        merge(tr, tr, s, m, t);
    }
    return NULL;
}
