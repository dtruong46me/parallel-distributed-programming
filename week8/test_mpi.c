#include <mpi.h>
#include <stdio.h>

int main (int argc, char *argv[]) {
    MPI_Init(&argc, &argv);
    int Nr_of_Process, ID_of_Process;
    MPI_Comm_size(MPI_COMM_WORLD, &Nr_of_Process);
    MPI_Comm_rank(MPI_COMM_WORLD, &ID_of_Process);

    printf("Hi from rank %d in total %d processors\n", ID_of_Process, Nr_of_Process);
    MPI_Finalize();
    return 0;
}