#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <malloc.h>

#define n 8
#define m 8
#define l 4

void DisplayMatrix (int *A, int row, int col) {
    for (int i=0; i<row; i++) {
        for (int j=0; j<col; j++) {
            printf("%d\t", *(A+i*col+j));
        }
        printf("\n");
    }
}

void main(int argc, char *argv[]) {
    int i, j, k, sum=0.0;
    int RANK, NP;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &NP);
    MPI_Comm_rank(MPI_COMM_WORLD, &RANK);

    int NS = n / NP;

    int *As = (int *) malloc ((NS*m)*sizeof(int));
    int *Bs = (int *) malloc ((m*l)*sizeof(int));
    int *C = (int *) malloc ((NS*l)*sizeof(int));

    if (RANK==0) {
        
        // Input Matrix A
        for (int i=0; i<n; i++) {
            for (int j=0; j<m; j++) {
                if (i==j) {
                    *(A+i*m+j) = 2.0;
                } else {
                    *(A+i*m+j) = 1.0;
                }
            }
        }
        printf("\nA:\n"); DisplayMatrix(A, n, m);

        // Input Matrix B
        for (int i=0; i<m; i++) {
            for (int j=0; j<l; j++) {
                *(B+i*l+j) = 1.0 * (i*l+j);
            }
        }
        printf("\nB:\n"); DisplayMatrix(B, m, l);
    }


    

    MPI_Finalize();
}