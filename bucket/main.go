package main

import (
	"fmt"
	"math"
	"math/rand"
	"time"
)

func main() {
	rand.NewSource(time.Now().UnixNano())
	arr := make([]int, 1000)
	for i := 0; i < 1000; i++ {
		arr[i] = rand.Intn(1000) - 500
	}
	bucketSort(arr)
	fmt.Println(findKMin([]int{3, 2, 3, 1, 2, 4, 5, 5, 6}, 6))
}

func findKMin(nums []int, k int) any {
	mx, mn := findMaxMin(nums)
	bucket := make([]int, mx-mn+1)
	for _, num := range nums {
		bucket[num-mn]++
	}
	for i, num := range bucket {
		k -= num
		if k <= 0 {
			return i + mn
		}
	}
	return math.NaN()
}

func bucketSort(nums []int) {
	mx, mn := findMaxMin(nums)
	bucket := make([]int, mx-mn+1)
	for _, num := range nums {
		bucket[num-mn]++
	}
	for i, num := range bucket {
		for range num {
			fmt.Print(i+mn, " ")
		}
	}
	fmt.Println()
}

func findMaxMin(nums []int) (int, int) {
	n := len(nums)
	mx, mn := nums[0], nums[0]
	for i := 0; i < n-1; i += 2 {
		if nums[i] > nums[i+1] {
			mx = max(mx, nums[i])
			mn = min(mn, nums[i+1])
		} else {
			mx = max(mx, nums[i+1])
			mn = min(mn, nums[i])
		}
	}
	if n&1 == 1 {
		mx = max(mx, nums[n-1])
		mn = min(mn, nums[n-1])
	}
	return mx, mn
}
