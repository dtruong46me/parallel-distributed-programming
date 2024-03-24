#include <stdio.h>
#include <omp.h>

int main(int arc, char *argv[]) {
    #pragma omp parallel
    printf("%s\n", "Hello world");
    return 0;
}