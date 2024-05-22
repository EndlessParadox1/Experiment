#include <iostream>
#include <vector>
#include <fstream>
#include <thread>
#include <chrono>
#include "mySort.h"
#include "mySem.h"

using namespace std::chrono;
using std::thread;
using std::cout;
using std::vector;
using std::fstream;
using std::ios;

int main() {
    fstream fs("data.txt", ios::in);
    vector<int> nums1, nums2, nums3, nums4, nums5;
    int tmp ;
    while(fs >> tmp) {
        nums1.push_back(tmp);
        nums2.push_back(tmp);
        nums3.push_back(tmp);
        nums4.push_back(tmp);
        nums5.push_back(tmp);
    }
    fs.close();

    // Test sort algorithms.
    auto start = high_resolution_clock::now();
    mSort(nums1, nums1, 0, nums1.size() - 1);
    auto end = high_resolution_clock::now();
    cout << duration_cast<nanoseconds>(end - start).count() <<" ns\n";

    start = high_resolution_clock::now();
    qSort(nums2, 0, nums2.size() - 1);
    end = high_resolution_clock::now();
    cout << duration_cast<nanoseconds>(end - start).count() <<" ns\n";

    start = high_resolution_clock::now();
    qSort_th(nums3, 0, nums3.size() - 1, 2);
    end = high_resolution_clock::now();
    cout << duration_cast<nanoseconds>(end - start).count() <<" ns\n";

    start = high_resolution_clock::now();
    qSort_th(nums4, 0, nums4.size() - 1, 3);
    end = high_resolution_clock::now();
    cout << duration_cast<nanoseconds>(end - start).count() <<" ns\n";

    start = high_resolution_clock::now();
    hSort(nums5);
    end = high_resolution_clock::now();
    cout << duration_cast<nanoseconds>(end - start).count() <<" ns\n";

    // Solve the producers-consumers problem using semaphores implemented by mutex & condition variables.
    cir_que my_q(5);
    auto p1 = thread(producer, std::ref(my_q), 1, 10);
    auto p2 = thread(producer, std::ref(my_q), -10, -1);
    auto c1 = thread(consumer, std::ref(my_q), 10);
    auto c2 = thread(consumer, std::ref(my_q), 10);
    p1.join();
    c1.join();
    p2.join();
    c2.join();
    return 0;
}
