#include <mpi.h>
#include <stdio.h>
#include <malloc.h>

int main (int argc, char *argv[]) {
    int N = 12, Np, Rank;
    MPI_Init(&argc, &argv);
    
    MPI_Comm_size(MPI_COMM_WORLD, &Np);
    MPI_Comm_rank(MPI_COMM_WORLD, &Rank);
    MPI_Status Warning;

    int i, p;

    int *A, *B, *C;
    A = (int *) malloc (N*sizeof(int));
    B = (int *) malloc (N*sizeof(int));
    C = (int *) malloc (N*sizeof(int));
    

    // Initialize Data
    if (Rank==0) {
        for (int i=0; i<N; i++) {
            *(A+i) = i;
            *(B+i) = 2*i;
        }
    }

    // Decompost Domain
    int Ns = N / Np;
    int *As, *Bs, *Cs;
    As = (int *) malloc (Ns*sizeof(int));
    Bs = (int *) malloc (Ns*sizeof(int));
    Cs = (int *) malloc (Ns*sizeof(int));

    if (Rank==0) {
        for (int i=0; i<Ns; i++) {
            *(As+i) = *(A+i);
            *(Bs+i) = *(B+i);
        }
        for (int p=1; p<Np; p++) {
            MPI_Send(
                A+p*Ns, //data
                Ns, // count
                MPI_INT, // datatype
                p, // destination
                p, //tag of A send
                MPI_COMM_WORLD //communication
            );

            MPI_Send(
                B+p*Ns, //data
                Ns, // count
                MPI_INT, // datatype
                p, //destination
                1000+p, //tag of B send
                MPI_COMM_WORLD // communication
            );
        }
    } else {
        MPI_Recv(
            As,
            Ns,
            MPI_INT,
            0,
            Rank, // tag of recieve from A
            MPI_COMM_WORLD,
            &Warning
        );

        MPI_Recv(
            Bs,
            Ns,
            MPI_INT,
            0,
            1000+Rank, // tag of recieve from B
            MPI_COMM_WORLD,
            &Warning
        );
    }

    // Calculation
    for (int i = 0; i<Ns; i++) {
        *(Cs+i) = *(As+i) + *(Bs+i);
    }

    // Gather Output
    if (Rank != 0) {
        // Send to rank 0
        MPI_Send(
            Cs,
            Ns,
            MPI_INT,
            0,
            2000 + Rank,
            MPI_COMM_WORLD
        );

    } else {
        // Assign first domain
        for (int i = 0; i<Ns; i++) {
            *(C+i) = *(Cs+i);
        }
        // Recieve other subdomain from other rank and store correctly
        for (int p=1; p<Np; p++) {
            MPI_Recv(
                C+p*Ns,
                Ns,
                MPI_INT,
                p,
                2000+p,
                MPI_COMM_WORLD,
                &Warning
            );
        }

        // Print C
        printf("\nC: ");
        for (int i=0; i<N; i++) {
            printf("%d ", *(C+i));
        }
        printf("\n\n"); 
    }

    MPI_Finalize();
    return 0;
}