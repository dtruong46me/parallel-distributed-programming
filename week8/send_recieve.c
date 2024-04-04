#include <mpi.h>
#include <stdio.h>
#include <malloc.h>

int main (int argc, char *argv[]) {
    MPI_Init(&argc, &argv);
    int Nr_of_Process, ID_of_Process;
    MPI_Comm_size(MPI_COMM_WORLD, &Nr_of_Process);
    MPI_Comm_rank(MPI_COMM_WORLD, &ID_of_Process);
    MPI_Status Warning;
    int i;

    if (ID_of_Process==0) {
        int *A;
        A = (int *) malloc (10*sizeof(int));
        for (int i=0; i<10; i++) {
            *(A+i) = i;
        }
        int dest = 1;
        MPI_Send(
            A, //data to send
            10, // count
            MPI_INT, // datatype
            dest, //destination
            2024, //tag
            MPI_COMM_WORLD //communicator
        );
        printf("A: ");
        for (int i=0; i<10; i++) {
            printf("%d ", *(A+i));
        }
        printf("\nSend from processor %d to processor %d\n", ID_of_Process, dest);
    } else if (ID_of_Process==1) {
        int *B, *C;
        B = (int *) malloc (10*sizeof(int));
        C = (int *) malloc (10*sizeof(int));
        int source = 0;
        MPI_Recv(
            B, // store
            10, // count
            MPI_INT, // datatype
            source, // source
            2024, // tag
            MPI_COMM_WORLD, // communicator
            &Warning
        );

        for (int i=0; i<10; i++) {
            *(C+i) = 2 * *(B+i);
        }

        printf("C: ");
        for (int i=0; i<10; i++) {
            printf("%d ", *(C+i));
        }
        printf("\nRecieve from processor %d to processor %d\n", ID_of_Process, source);

    }

    MPI_Finalize();
    return 0;
}