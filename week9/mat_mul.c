#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
// size of maxtrix
#define  n       8
#define  m       8
#define  l       4
//==================================
void DisplayMatrix(int *A, int row,  int col){
  int i,j;
  for(i=0;i<row;i++){
    for(j=0;j<col;j++) printf("%d\t",*(A+i*col+j));
    printf("\n");
  }
}
//==================================
int main()
{
  int i, j, k, sum = 0.0;
  int *A, *B, *C;
  A = (int *) malloc ((n*m)*sizeof(int));
  B = (int *) malloc ((m*l)*sizeof(int));
  C = (int *) malloc ((n*l)*sizeof(int));
// Input data (A, B)
   for (  i = 0 ; i < n ; i++ )
    for ( j = 0 ; j < m ; j++ ){
      if (i==j) 
        *(A+i*m+j) = 2.0;
      else
        *(A+i*m+j) = 1.0;
    }
    printf("The matrix A:\n");
    DisplayMatrix(A, n, m);
   for (  i = 0 ; i < m ; i++ )
    for ( j = 0 ; j < l ; j++ ){
      *(B+i*l+j) = 1.0*(i*l+j);
    }
    printf("The matrix B\n");
    DisplayMatrix(B, m, l);
  
 // Multiply Matrix: A x B = C
  for ( i = 0 ; i < n ; i++ )
    {
      for ( j = 0 ; j < l ; j++ )
      {
        for ( k = 0 ; k < m ; k++ )
        {
          sum = sum + (*(A + i*m + k)) * (*(B + k*l + j));
        } 
        *(C + i*l + j) = sum;
        sum = 0.0;
      }
    }
// Print Result
   printf( "Ma tran C:\n");
   DisplayMatrix(C, n, l);
  return 0;
}