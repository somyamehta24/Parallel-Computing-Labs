%%cu
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <chrono>
#define MAX 100000
#include <bits/stdc++.h>
    using namespace std;
// function for random function
mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
int rand(int L, int R) {
    return uniform_int_distribution<int>(L, R)(rng);
}

void RankSortCpu(int *arr, int *n, int *rankList, int *sortedList) {
    int size = (*n);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < i; j++) {
            if (arr[i] >= arr[j])
                rankList[i] += 1;
            else
                rankList[j] += 1;
        }
    }
    for (int i = 0; i < size; i++)
        sortedList[rankList[i]] = arr[i];
}
__global__ void RankSortNewGpu(int *arr, int *n, int *rankList, int *dev_sort_list) {
    int size = (*n);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < i; j++) {
            if (arr[i] >= arr[j])
                rankList[i]++;
            else
                rankList[j]++;
        }
    }
    for (int i = 0; i < size; i++)
        dev_sort_list[rankList[i]] = arr[i];
}

int main() {
    int n = rand(1, 10000);
    // scanf("%d",n);
    int *arr = (int *)malloc(MAX * sizeof(int));
    for (int i = 0; i < n; i++) {
        arr[i] = rand() / 1e5;
    }
    int i;
    int *rankList = (int *)calloc(MAX, sizeof(int));
    int *sortedList = (int *)calloc(MAX, sizeof(int));
    printf("\tSize of vector: %d\n", n);

    printf("\tCPU\n");
    auto start = std::chrono::high_resolution_clock::now();
    RankSortCpu(arr, &n, rankList, sortedList);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    printf("Time taken (in Nano Seconds) is %d\n", duration.count());

    printf("\tGPU\n");
    int *dev_a, *dev_n, *dev_rank_list, *dev_sort_list;
    cudaMalloc((void **)&dev_n, sizeof(int));
    cudaMalloc((void **)&dev_a, MAX * sizeof(int));
    cudaMalloc((void **)&dev_rank_list, MAX * sizeof(int));
    cudaMalloc((void **)&dev_sort_list, MAX * sizeof(int));

    for (int i = 0; i < n; i++) {
        int temp = 0;
        cudaMemcpy(&dev_a[i], &arr[i], sizeof(int), cudaMemcpyHostToDevice);
        cudaMemcpy(&dev_rank_list[i], &temp, sizeof(int), cudaMemcpyHostToDevice);
        cudaMemcpy(&dev_sort_list[i], &sortedList[i], sizeof(int), cudaMemcpyHostToDevice);
    }

    cudaMemcpy(dev_n, &n, sizeof(int), cudaMemcpyHostToDevice);

    start = std::chrono::high_resolution_clock::now();
    RankSortNewGpu<<<1, 1>>>(dev_a, dev_n, dev_rank_list, dev_sort_list);
    end = std::chrono::high_resolution_clock::now();
    cudaDeviceSynchronize();
    duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    printf("Time taken is %d\n", duration.count());

    int *c = (int *)malloc(MAX * sizeof(int));
    cudaMemcpy(c, dev_sort_list, MAX * sizeof(int), cudaMemcpyDeviceToHost);

    for (i = 0; i < n; i++)
        if (c[i] != sortedList[i]) {
            printf("Error!\n");
            return 0;
        }
    printf("Arrays Same\n");
    free(arr);
    cudaFree(dev_a);
    cudaFree(dev_n);
    cudaFree(dev_rank_list);
    cudaFree(dev_sort_list);
}
