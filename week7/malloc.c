#include <stdio.h>
#include <stdlib.h>

int main(){
    int n = 10;
    int *a = (int*)malloc(n * sizeof(int));
    printf("%p\n", (int *) &a);
    if(a == NULL){
        printf("Cap phat khong thanh cong !\n");
    }
    else{
        printf("Cap phat thanh cong !\n");
        for(int i = 0; i < n; i++){
            a[i] = 28 + i; // *(a + i) = 28 cũng được
        }
        for(int i = 0; i < n; i++){
            printf("%d ", a[i]);
        }
    }
    printf("l\n");
    printf("%d\n\n\n\n", a[8]);
    printf("l");
    free(a);
    printf("%d\n\n", a[8]);
    return 0;
}