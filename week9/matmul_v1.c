#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <mpi.h>

#define m 8
#define n 8
#define p 4

void DisplayMatrix(int *A, int row, int col) {
    int i, j;
    for (i = 0; i < row; i++) {
        for (j = 0; j < col; j++) 
            printf("%d\t", *(A + i * col + j));
        printf("\n");
    }
}

int main(int argc, char *argv[]) {
    int NP, rank, offset, mc;
    int i, j, k, sum = 0.0;
    MPI_Status status;

    int *A, *B, *C, *Ac, *Bc, *Cc;
    A = (int *)malloc((m * n) * sizeof(int));
    B = (int *)malloc((n * p) * sizeof(int));
    C = (int *)malloc((m * p) * sizeof(int));

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &NP);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        // Initialize matrix A as an identity matrix
        for (i = 0; i < m; i++) {
            for (j = 0; j < n; j++) {
                if (i == j)
                    *(A + i * n + j) = 1;
                else
                    *(A + i * n + j) = 0;
            }
        }
        printf("The matrix A:\n");
        DisplayMatrix(A, m, n);

        // Initialize matrix B with values
        for (i = 0; i < n; i++) {
            for (j = 0; j < p; j++) {
                *(B + i * p + j) = i * p + j;
            }
        }
        printf("The matrix B\n");
        DisplayMatrix(B, n, p);
    }

    mc = m / NP; // Number of rows each process will handle

    Ac = (int *)malloc((mc * n) * sizeof(int));
    Bc = (int *)malloc((n * p) * sizeof(int));
    Cc = (int *)malloc((mc * p) * sizeof(int));

    if (rank == 0) {
        // Send parts of matrix A to other ranks
        for (i = 0; i < mc; i++) {
            for (j = 0; j < n; j++) {
                *(Ac + i * n + j) = *(A + i * n + j);
            }
        }
        for (i = 1; i < NP; i++)
            MPI_Send(A + i * mc * n, mc * n, MPI_INT, i, i, MPI_COMM_WORLD);
    } else {
        MPI_Recv(Ac, mc * n, MPI_INT, 0, rank, MPI_COMM_WORLD, &status);
    }

    // Send matrix B to all ranks
    if (rank == 0) {
        for (i = 0; i < n; i++) {
            for (j = 0; j < p; j++) {
                *(Bc + i * p + j) = *(B + i * p + j);
            }
        }
        for (i = 1; i < NP; i++)
            MPI_Send(B, n * p, MPI_INT, i, i, MPI_COMM_WORLD);
    } else {
        MPI_Recv(Bc, n * p, MPI_INT, 0, rank, MPI_COMM_WORLD, &status);
    }

    // Matrix multiplication: Ac x Bc = Cc
    for (i = 0; i < mc; i++) {
        for (j = 0; j < p; j++) {
            for (k = 0; k < n; k++) {
                sum += *(Ac + i * n + k) * *(Bc + k * p + j);
            }
            *(Cc + i * p + j) = sum;
            sum = 0;
        }
    }

    // Gather results to rank 0
    if (rank != 0) {
        MPI_Send(Cc, mc * p, MPI_INT, 0, rank, MPI_COMM_WORLD);
    } else {
        for (i = 0; i < mc; i++) {
            for (j = 0; j < p; j++) {
                *(C + i * p + j) = *(Cc + i * p + j);
            }
        }
        for (i = 1; i < NP; i++)
            MPI_Recv(C + i * mc * p, mc * p, MPI_INT, i, i, MPI_COMM_WORLD, &status);
    }

    // Display the resulting matrix C at rank 0
    if (rank == 0) {
        printf("Matrix C:\n");
        DisplayMatrix(C, m, p);
    }

    MPI_Finalize();
    return 0;
}