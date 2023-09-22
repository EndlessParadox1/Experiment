#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>
#include "mySort.h"
#include "mySem.h"

int main() {
    const int size = 500000;
    int* nums1 = malloc(sizeof(int) * size), *nums2 = malloc(sizeof(int) * size);
    FILE* fp = fopen("data.txt", "r");
    fscanf(fp, "%d", nums1);
    fclose(fp);
    for(int i = 0; i < size; i++) {
        nums2[i] = nums1[i];
    }

    // Test the merge sort of multi-threads
    args_m tmp1_ = {nums1, nums1, 0, size, 1};
    SYSTEMTIME st;
    GetSystemTime(&st);
    pthread_t p1_;
    pthread_create(&p1_, NULL, mSort_th, &tmp1_);
    pthread_join(p1_, NULL);
    SYSTEMTIME et;
    GetSystemTime(&et);
    printf("mSort_th: %d ms\n", et.wMilliseconds - st.wMilliseconds);

    // Test the merge sort of multi-threads
    args_q tmp2_ = {0, size - 1, 1, nums1};
    long long st_ = time(0);
    pthread_t p2_;
    pthread_create(&p2_, NULL, qSort_th, &tmp2_);
    pthread_join(p2_, NULL);
    long long et_ = time(0);
    printf("qSort_th: %lld s\n", et_ - st_);

    // Solve the producers-consumers problem using semaphores implemented by mutex & condition variables
    queue my_q = {0, 0, 5, malloc(sizeof(int) * 5)};
    sem_init(&my_q.lock, 1);
    sem_init(&my_q.empty, my_q.size);
    sem_init(&my_q.full, 0);

    pthread_t p1, p2, c1, c2;
    args tmp1 = {1, 10, &my_q};
    pthread_create(&p1, NULL, producer, &tmp1);
    args tmp2 = {-10, -1, &my_q};
    pthread_create(&p2, NULL, producer, &tmp2);
    args tmp3 = {1, 10, &my_q};
    pthread_create(&c1, NULL, consumer, &tmp3);
    pthread_create(&c2, NULL, consumer, &tmp3);
    pthread_join(p1, NULL);
    pthread_join(c1, NULL);
    pthread_join(c2, NULL);
    pthread_join(p2, NULL);

    return 0;
}
