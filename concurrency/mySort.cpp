#include "mySort.h"

int part(vector<int> &nums, int low, int high) {
    int temp = nums[low], key = nums[low];
    while(low < high) {
        while(low < high && nums[high] >= key) {
            high--;
            nums[low] = nums[high];
        }
        while(low < high && nums[low] <= key) {
            low++;
            nums[high] = nums[low];
        }
    }
    nums[low] = temp;
    return low;
}

void qSort(vector<int> &nums, int low, int high) {
    if (low < high) {
        int loc = part(nums, low, high);
        qSort(nums, low, loc);
        qSort(nums, loc + 1, high);
    }
}

void qSort_th(vector<int> &nums, int low, int high, int level) {
    if (low < high) {
        int loc = part(nums, low, high);
        if(level) {
            auto p1 = thread(qSort_th, std::ref(nums), low, loc, level - 1);
            auto p2 = thread(qSort_th, std::ref(nums), loc + 1, high, level - 1);
            p1.join();
            p2.join();
        } else {
            qSort(nums, low, loc);
            qSort(nums, loc + 1, high);
        }
    }
}

void merge(vector<int> sr, vector<int> &tr, int i, int m, int n) {
    int j, k;
    for(j = m+1, k = i; i <= m && j <= n; k++) {
        if(sr[i] < sr[j])
            tr[k] = sr[i++];
        else
            tr[k] = sr[j++];
    }
    while(i <= m)
        tr[k++] = sr[i++];
    while(j <= n)
        tr[k++] = sr[j++];
}

void mSort(vector<int> sr, vector<int> &tr, int s, int t) {
    if(s == t)
        tr[s] = sr[s];
    else {
        int m = (s + t)/2;
        mSort(sr, tr, s, m);
        mSort(sr, tr, m+1, t);
        merge(tr, tr, s, m, t);
    }
}

void mSort_th(vector<int> sr, vector<int> &tr, int s, int t, int level) {
    if(s == t)
        tr[s] = sr[s];
    else {
        int m = (s + t)/2;
        if(level) {
            auto p1 = thread(mSort_th, sr, std::ref(tr), s, m, level - 1);
            auto p2 = thread(mSort_th, sr, std::ref(tr), m + 1, t, level - 1);
            p1.join();
            p2.join();
        } else {
            mSort(sr, tr, s, m);
            mSort(sr, tr, m+1, t);
        }
        merge(tr, tr, s, m, t);
    }
}

void heap(vector<int> &nums, int s, int m) {
    int temp = nums[s];
    for(int j = s * 2 + 1; j <= m; j = j * 2 + 1) {
        if(j < m && nums[j] < nums[j + 1])
            j++;
        if(temp >= nums[j])
            break;
        nums[s] = nums[j];
        s = j;
    }
    nums[s] = temp;
}

void hSort(vector<int> &nums) {
    for(int i = nums.size()/2 - 1; i >= 0; i--)
        heap(nums, i, nums.size() - 1);
    for(int i = nums.size() - 1; i > 0; i--) {
        int temp = nums[0];
        nums[0] = nums[i];
        nums[i] = temp;
        heap(nums, 0, i - 1);
    }
}
