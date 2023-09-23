#include <iostream>
#include <vector>
#include <fstream>
#include <ctime>
#include <thread>
#include "mySort.h"
//#include "mySem.h"

using std::thread;
using std::cout;
using std::vector;
using std::fstream;
using std::ios;

int main() {
    fstream fs("data.txt", ios::in);
    vector<int> nums1, nums2, nums3;
    int tmp ;
    while(fs >> tmp) {
        nums1.push_back(tmp);
        nums2.push_back(tmp);
        nums3.push_back(tmp);
    }
    fs.close();

    // Test the merge sort & quick sort of multi-threads
    clock_t st = clock();
    mSort_th(nums1, nums1, 0, nums1.size() - 1, 2);
    clock_t et = clock();
    cout << (et - st)/CLOCKS_PER_SEC * 1000<<" ms\n";
    qSort_th(nums2, 0, nums2.size() - 1, 2);
    et = clock();
    cout << (et - st)/CLOCKS_PER_SEC * 1000<<" ms\n";
    hSort(nums3);
    et = clock();
    cout << (et - st)/CLOCKS_PER_SEC * 1000<<" ms\n\n";

    // Solve the producers-consumers problem using semaphores implemented by mutex & condition variables
//    cir_que my_q(5);
//    auto p1 = thread(producer, std::ref(my_q), 1, 10);
//    auto p2 = thread(producer, std::ref(my_q), -10, -1);
//    auto c1 = thread(consumer, std::ref(my_q), 10);
//    auto c2 = thread(consumer, std::ref(my_q), 10);
//    p1.join();
//    c1.join();
//    p2.join();
//    c2.join();

    return 0;
}
