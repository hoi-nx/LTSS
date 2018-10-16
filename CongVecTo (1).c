#include <stdio.h>
#include <malloc.h>

int main(){ 
	int i, *A, *B, *C;
	A = (int *) malloc ((10)*sizeof(int));
	B = (int *) malloc ((10)*sizeof(int));
	C = (int *) malloc ((10)*sizeof(int));

for(i=0;i<10;i++){
	*(A+i) =i;
	*(B+i) =2*i;

}

for(i=0;i<10;i++){
	*(C+i) = *(A+i)+*(B+i);
}

for(i=0;i<10;i++){
	printf("A[%d]: %d",i, *(A+i));
	printf("  | B[%d]: %d",i, *(B+i));
	printf("    |  C[%d]: %d \n",i, *(C+i));
}
free(A);free(B);free(C);
	return 0;
}
