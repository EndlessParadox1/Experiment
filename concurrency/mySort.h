#ifndef UNTITLED_MYSORT_H
#define UNTITLED_MYSORT_H

#include <vector>
#include <thread>

using std::vector;
using std::thread;

int part(vector<int>& nums, int low, int high);
void qSort(vector<int>& nums, int low, int high);
void qSort_th(vector<int>& nums, int low, int high, int level);

void merge(vector<int> sr, vector<int>& tr, int i, int m, int n);
void mSort(vector<int> sr, vector<int>& tr, int s, int t);
void mSort_th(vector<int> sr, vector<int>& tr, int s, int t, int level);

void heap(vector<int>& nums, int s, int m);
void hSort(vector<int>& nums);

#endif
