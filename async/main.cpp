#include <iostream>
#include <future>
#include <mutex>
#include <algorithm>
#include <numeric>
#include <vector>

using namespace std;

mutex m;

struct Test{
    void foo(int i, const string& s) {
        lock_guard<mutex> lg(m);
        cout << s << ' ' << i << endl;
    }
    void bar(const string& s) {
        lock_guard<mutex> lg(m);
        cout << s << endl;
    }
    int operator()(int i) {
        lock_guard<mutex> lg(m);
        cout << i << endl;
        return i + 10;
    }
};

template<typename RandomIt>
int parallel_sum(RandomIt begin, RandomIt end) {
    auto len = end - begin;
    if (len < 1000)
        return accumulate(begin, end, 0);
    RandomIt mid = begin + len / 2;
    auto handle = async(std::launch::async, parallel_sum<RandomIt>, mid, end);
    int sum = parallel_sum(begin, mid);
    return sum + handle.get();
}

void acm(vector<int>::iterator begin, vector<int>::iterator end, promise<int> pro) {
    int sum = accumulate(begin, end, 0);
    pro.set_value(sum);
}

int main() {
    vector<int> v1(10000, 1);
    cout << parallel_sum(v1.begin(), v1.end()) << endl;
    Test t;
    async(&Test::foo, &t, 42, "Hello");
    auto a2 = async(std::launch::deferred, &Test::bar, t, "Hello");
    auto a3 = async(std::launch::async, Test(), 42);
    a2.wait();
    cout << a3.get() << endl;

    vector<int> v2{1, 2, 3, 4, 5};
    promise<int> pro;
    auto handle = pro.get_future();
    auto p1 = thread(acm, v2.begin(), v2.end(), std::move(pro));
    cout << handle.get() << endl;
    p1.join();
    return 0;
}
