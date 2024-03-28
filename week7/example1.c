#include <omp.h>
#include <stdio.h>

int main() {
    omp_set_num_threads(4);
    int tid, numt;
    printf("Processor count: %d\n", omp_get_num_procs());
    printf("Number of threads: %d\n", omp_get_num_threads());

    #pragma omp parallel private(tid) shared(numt)
    {
        tid = omp_get_thread_num();
        
        if (tid==0) {
            numt = omp_get_num_threads();
        }
        
        for (int j=0; j<100; j++) {
            printf("Thread ID: %d of %d\n", tid, numt);
        }
    }
}