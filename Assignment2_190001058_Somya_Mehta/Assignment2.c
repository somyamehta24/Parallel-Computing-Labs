//190001058
//Somya Mehta
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int npes;
// here we merge the array with 2 pointers
int* merge(int* finalarr, int* a1, int *a2, int size1, int size2){
    int i=0, j=0, l=0;
    while(i < size1 && j < size2){
        if(a1[i] < a2[j]){
            finalarr[l] = a1[i];
            l++;
            i++;
        }
        else{
            finalarr[l] = a2[j];
            l++;
            j++;
        }
    }
    while(i < size1){
        finalarr[l] = a1[i];
        l++;
        i++;
    }
    while(j < size2){
        finalarr[l] = a2[j];
        l++;
        j++;
    }
    return finalarr;
}

int comparator (const void * x, const void * y) {
   return ( *(int*)x - *(int*)y );
}

int main(int argc, char **argv) {
    int myRank, n = 0, nelements = 0;

    MPI_Status status;
    MPI_Init (&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    MPI_Comm_size(MPI_COMM_WORLD, &npes);

    if(myRank == 0){
        // Input of number of elements
        printf("Enter Size of Array: ");
        scanf("%d", &n);
        nelements = n / npes;
    }
    int temp[n];

    // Broadcast to all processes
    MPI_Bcast(&nelements, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Remaining elements sent to last process
    if(myRank == 0){
        int last = n % npes;
        MPI_Send(&last, 1, MPI_INT, npes-1, 0, MPI_COMM_WORLD);
    }
    if(myRank == npes - 1){
        int extra;
        MPI_Recv(&extra, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        nelements += extra;
    }

    // Generating Random Data
    int arr[nelements];

    int pid= myRank;
    srand(pid + 1);     // To make different arrays for different processes
    for(int i=0; i<nelements; i++){
        arr[i] = rand()%10000 + 1;
    }

    // the data is being sorted
    qsort(arr, nelements, sizeof(int), comparator);

    // Storing the lengths of arrays to be used for Gatherv
    int *recvlen = NULL;

    // if the base process is there then this much npes length we will recieve
    if(myRank == 0) recvlen = malloc(npes * sizeof(int));
    MPI_Gather(&nelements, 1, MPI_INT, recvlen, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Make array of displacements so that the recieve length of the processes 
    // can be stored  to be used in Gatherv later
    int *disp = NULL;
   
    if(myRank == 0){
        disp = malloc(npes * sizeof(int));
        disp[0] = 0;
        for(int i=1; i < npes; i++) disp[i] = disp[i-1] + recvlen[i-1];
    }

    // Gather all sorted sub-lists in temp array of process 0
    MPI_Gatherv(arr, nelements, MPI_INT, temp, recvlen, disp, MPI_INT, 0, MPI_COMM_WORLD);

    // Print the sorted lists
    if(myRank == 0){
        for(int i=0; i<npes; i++){
            int pid = i;
            int from = i * nelements, to = (i+1)*nelements;
            printf("Process no: %d \n Sorted Array: ", pid);
            if(pid == npes - 1) to = n;
            for(int j=from; j<to; j++) printf("%d ", temp[j]);
            printf("\n");
        }
    }

    // Merge Children's arrays
    int children1 = 2*myRank + 1, children2 = 2*myRank + 2;
    int parent = (myRank - 1) / 2;
    int childrenSize1 = 0, childrenSize2 = 0;
    int *childrenArray1 = NULL, *childrenArray2 = NULL;
    
    if(children2 < npes){
        MPI_Recv(&childrenSize2, 1, MPI_INT, children2, 0, MPI_COMM_WORLD, &status);
        childrenArray2 = malloc(childrenSize2 * sizeof(int));
        MPI_Recv(childrenArray2, childrenSize2, MPI_INT, children2, 52, MPI_COMM_WORLD, &status);
    }
    if(children1 < npes){
        MPI_Recv(&childrenSize1, 1, MPI_INT, children1, 0, MPI_COMM_WORLD, &status);
        childrenArray1 = malloc(childrenSize1 * sizeof(int));
        MPI_Recv(childrenArray1, childrenSize1, MPI_INT, children1, 52, MPI_COMM_WORLD, &status);
    }
    int* finalarr = NULL;
    if(childrenSize1 + childrenSize2 > 0){
        finalarr = malloc((childrenSize1 + childrenSize2) * sizeof(int));
        finalarr = merge(finalarr, childrenArray1, childrenArray2, childrenSize1, childrenSize2);
    }

    // Merge the children's array with current process' array
    int *finalArray = malloc((childrenSize1 + childrenSize2 + nelements) * sizeof(int));
    finalArray = merge(finalArray, finalarr, arr, childrenSize1+childrenSize2, nelements);

    // Send final array to parent array
    if(myRank != 0){
        int totalSize = childrenSize1 + childrenSize2 + nelements;
        MPI_Send(&totalSize, 1, MPI_INT, parent, 0, MPI_COMM_WORLD);
        MPI_Send(finalArray, totalSize, MPI_INT, parent, 52, MPI_COMM_WORLD);
    }

    // stop process 0 until others complete
    MPI_Barrier(MPI_COMM_WORLD);

    // Print the final result in the process  0
    if(myRank == 0){
        printf("\n Final Merged Array: ");
        for(int i=0; i<n; i++) printf("%d ", finalArray[i]);
        printf("\n");
    }

    MPI_Finalize();
    return 0;
}

