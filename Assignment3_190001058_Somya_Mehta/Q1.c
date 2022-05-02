// Name: Somya Mehta
// Roll No: 190001058

#include <stdio.h>
#include <stdlib.h>
#include <mpich/mpi.h>

// Function to check if given number is prime

int is_prime(int n) {
    if (n == 1) return 0;
    int flag = 1;
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            flag = 0;
            break;
        }
    }
    return flag;
}
// Compute number of primes for each processor
int NumberOfPrime(int myRank, int n_elements_per_process) {
    int left = myRank * n_elements_per_process + 1;
    int right = left + n_elements_per_process;
    int counter = 0;
    for (int i = left; i < right; i++) {
        if (is_prime(i)) counter++;
    }
    return counter;
}
int main(int argc, char** argv) {
    int myRank, noOfProcessors;
    int n, n_elements_per_process;

    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    MPI_Comm_size(MPI_COMM_WORLD, &noOfProcessors);

    if (myRank == 0) {
        printf("\n Enter the upper limit: ");
        scanf("%d", &n);
        n_elements_per_process = n / noOfProcessors;
    }

    // Broadcast to other processors
    MPI_Bcast(&n_elements_per_process, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Sright remaining numbers to last processor
    if (myRank == 0) {
        int remaining = n % noOfProcessors;
        MPI_Send(&remaining, 1, MPI_INT, noOfProcessors - 1, 0, MPI_COMM_WORLD);
    }
    if (myRank == noOfProcessors - 1) {
        int extra;
        MPI_Recv(&extra, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        n_elements_per_process += extra;
    }

    // Create array in processor 0 to receive all answers
    int* recvlen = NULL;
    int counter = NumberOfPrime(myRank, n_elements_per_process);
    if (myRank == 0) recvlen = (int*)malloc(noOfProcessors * sizeof(int));
    MPI_Gather(&counter, 1, MPI_INT, recvlen, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (myRank == 0) {
        int res = 0;
        for (int i = 0; i < noOfProcessors; i++) res += recvlen[i];
        printf("\n Number of prime numbers in [1, %d] are %d \n", n, res);
    }

    MPI_Finalize();

    return 0;
}