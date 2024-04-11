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

    int *A = (int *) malloc ((n*m)*sizeof(int));
    int *B = (int *) malloc ((m*l)*sizeof(int));
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

    MPI_Scatter(A, NS*m, MPI_INT, As, NS*m, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Bcast(B, m*l, MPI_INT, 0, MPI_COMM_WORLD);


    //Perform C = As * B
    for (int i=0; i<NS; i++) {
        for (int j=0; j<l; j++) {
            sum = 0;
            for (int k=0; k<m; k++) {
                sum += As[i*m+k] * B[k*l+j];
            }
            C[i*l+j] = sum;
        }
    }

    MPI_Gather(C, NS*l, MPI_INT, A, NS*l, MPI_INT, 0, MPI_COMM_WORLD);

    if (RANK==0) {
        printf("\nResult of matrix C: \n");
        DisplayMatrix(A, n, l);
    }

    MPI_Finalize();
}