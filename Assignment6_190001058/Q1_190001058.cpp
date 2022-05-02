%%cu
// Somya Mehta
//  190001058
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <chrono>
#include <bits/stdc++.h>
    using namespace std;
// function for random function
mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
int rand(int L, int R) {
    return uniform_int_distribution<int>(L, R)(rng);
}
// for CPU
//  finding max element
void findMax(int *arr, int *n, int *res) {
    int sz = (*n);
    (*res) = INT_MIN;
    for (int i = 0; i < sz; i++) {
        if ((*res) < arr[i]) {
            *res = arr[i];
        }
    }
}
// for GPU
__global__ void maxEle(int *arr, int *sizeOfArr, int *result) {
    int sz = (*sizeOfArr);
    (*result) = INT_MIN;
    for (int i = 0; i < sz; i++) {
        if ((*result) < arr[i]) {
            *result = arr[i];
        }
    }
}

int main() {
    // checking for random values of size of vector
    int n = rand(10000, 1000000);
    // scanf("%d",n);
    srand(time(0));
    int *arr = (int *)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        arr[i] = rand(0, 10000);
    }
    int res = INT_MIN;

    printf("\tCPU\n\n");
    auto s = std::chrono::high_resolution_clock::now();
    findMax(arr, &n, &res);
    auto e = std::chrono::high_resolution_clock::now();
    auto dur = std::chrono::duration_cast<std::chrono::microseconds>(e - s);
    int time_taken = dur.count();
    cout << "Size of the vector:" << n << "\n";
    printf("Result = %d\n", res);
    printf("Time taken is %d\n", time_taken);

    printf("\tGPU\n\n");
    int *dev_a, *dev_n, *dev_ans;
    cudaMalloc((void **)&dev_a, n * sizeof(int));
    cudaMalloc((void **)&dev_n, sizeof(int));
    cudaMalloc((void **)&dev_ans, sizeof(int));

    cudaMemcpy(dev_n, &n, sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(dev_ans, &res, sizeof(int), cudaMemcpyHostToDevice);
    for (int i = 0; i < n; i++) {
        cudaMemcpy(&dev_a[i], &arr[i], sizeof(int), cudaMemcpyHostToDevice);
    }

    s = std::chrono::high_resolution_clock::now();
    maxEle<<<1, 1>>>(dev_a, dev_n, dev_ans);
    e = std::chrono::high_resolution_clock::now();
    dur = std::chrono::duration_cast<std::chrono::microseconds>(e - s);
    time_taken = dur.count();
    cudaMemcpy(&res, dev_ans, sizeof(int), cudaMemcpyDeviceToHost);
    printf("Result = %d\n", res);
    printf("Time taken is %d\n", time_taken);
    cudaFree(dev_a);
    cudaFree(dev_n);
    cudaFree(dev_ans);
}

