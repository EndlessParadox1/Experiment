#include "mySort.h"

using std::swap;

int mid3(vector<int>& nums, int left, int right) {
    int mid = (left + right) >> 1;
    if (nums[left] > nums[mid])
        swap(nums[left], nums[mid]);
    if (nums[left] > nums[right])
        swap(nums[left], nums[right]);
    if (nums[mid] > nums[right])
        swap(nums[mid], nums[right]);
    swap(nums[mid], nums[right-1]);
    return nums[right-1];
}

void qSort(vector<int>& nums, int left, int right) {
    if (left + 3 <= right) {
        int pivot = mid3(nums, left, right);
        int i = left, j = right - 1;
        while (1) {
            while (nums[++i] < pivot);
            while (nums[--j] > pivot);
            if (i < j)
                swap(nums[i], nums[j]);
            else
                break;
        }
        swap(nums[i], nums[right - 1]);
        qSort(nums, left, i - 1);
        qSort(nums, i + 1, right);
    } else {
        hSort(nums);
    }
}

void qSort_th(vector<int> &nums, int left, int right, int level) {
    if (left + 3 <= right) {
        int pivot = mid3(nums, left, right);
        int i = left, j = right - 1;
        while (1) {
            while (nums[++i] < pivot);
            while (nums[--j] > pivot);
            if (i < j)
                swap(nums[i], nums[j]);
            else
                break;
        }
        swap(nums[i], nums[right - 1]);
        if (level) {
            auto p1 = thread(qSort_th, std::ref(nums), left, i - 1, level - 1);
            auto p2 = thread(qSort_th, std::ref(nums), i + 1, right, level - 1);
            p1.join();
            p2.join();
        } else {
            qSort(nums, left, i - 1);
            qSort(nums, i + 1, right);
        }
    } else {
        insertSort(nums);
    }
}

void insertSort(vector<int> &nums) {
    int j, tmp;
    for (int i = 1; i < nums.size(); i++) {
        tmp = nums[i];
        for (j = i; j > 0 && nums[j-1] > tmp; j--)
            nums[j] = nums[j-1];
        nums[j] = tmp;
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
