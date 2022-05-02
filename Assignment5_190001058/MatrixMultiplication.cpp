// 190001058
//  Somya Mehta
#include <bits/stdc++.h>
#include <ctime>
#include <omp.h>
using namespace std;

int numberOfRowsinA, numberOfColumnsinA;
int numOfThreads;
int numberOfRowsinB, numberOfColumnsinB;
// Initialize matrix with random values
void RandomlyInitialize(vector<vector<int>> &matrix) {
    srand(time(0));
    int row = matrix.size();
    int col = matrix[0].size();
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            matrix[i][j] = rand() % 100;
            printf("%d\t", matrix[i][j]);
        }
        printf("\n");
    }
}

void matrixMult(vector<vector<int>> &matA, vector<vector<int>> &matB, vector<vector<int>> &matC) {
#pragma omp parallel
    {
#pragma omp for collapse(2)
        for (int i = 0; i < numberOfRowsinA; i++) {
            for (int j = 0; j < numberOfColumnsinB; j++) {
                for (int k = 0; k < numberOfColumnsinA; k++) {
                    matC[i][j] += matA[i][k] * matB[k][j];
                }
            }
        }
    }
}
void OutputMatrix(vector<vector<int>> &matC) {
    printf("Output Matrix\n");
    for (int i = 0; i < numberOfRowsinA; i++) {
        for (int j = 0; j < numberOfColumnsinB; j++) {
            printf("%d \t", matC[i][j]);
        }
        printf("\n");
    }
}
int main() {
    printf("\n Enter dimensions of matrix A ");
    scanf("%d", &numberOfRowsinA);
    scanf("%d", &numberOfColumnsinA);

    vector<vector<int>> matA(numberOfRowsinA, vector<int>(numberOfColumnsinA, 0));

    numberOfRowsinB = numberOfColumnsinA;
    printf("\n Enter dimensions numberof columns in matrix B ");
    scanf("%d", &numberOfColumnsinB);

    vector<vector<int>> matB(numberOfRowsinB, vector<int>(numberOfColumnsinB, 0));
    // Randomly fill the matrices
    RandomlyInitialize(matB);
    RandomlyInitialize(matA);

    printf("\n Enter number of threads: ");
    scanf("%d", &numOfThreads);
    omp_set_num_threads(numOfThreads);

    vector<vector<int>> matC(numberOfRowsinA, vector<int>(numberOfColumnsinB, 0));
    auto start = std::chrono::high_resolution_clock::now();
    matrixMult(matA, matB, matC);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    int timeTaken = duration.count();
    OutputMatrix(matC);
    printf("\n Time taken to multiply %d X %d matrix with %d X %d matrix using %d threads = %d microseconds\n", numberOfRowsinA, numberOfColumnsinA, numberOfRowsinB, numberOfColumnsinB, numOfThreads, timeTaken);
    return 0;
}