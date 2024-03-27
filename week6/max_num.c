#include <omp.h>
#include <stdio.h>

int main(int nargs, char **args) {
    printf("I love you very much\n");

    #pragma omp parallel
    {
        int num_threads, thread_id;
        num_threads = omp_get_num_threads();
        thread_id = omp_get_thread_num();

        printf("Hello, thread No.%d of %d threads\n\n", thread_id, num_threads);
    }
}