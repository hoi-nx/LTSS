#include <stdio.h>
#include <malloc.h>
#include <omp.h>
int f(int *a,int *b,int *c,int x){
	int i;
	for (i=0;i<x;i++){
		*(c+i) = *(a+i)+*(b+i);
	}
	return *(c+i);
}
int main(){ 
	int i, n, m, ind, *A, *B, *C;
	n = 10; m = 2;
	A = (int *) malloc ((n)*sizeof(int));
	B = (int *) malloc ((n)*sizeof(int));
	C = (int *) malloc ((n)*sizeof(int));

	for(i=0;i<n;i++){
		*(A+i) =i;
		*(B+i) =2*i;
	}

	//omp_set_num_threads(m);
	//#pragma omp parallel private(ind)
	{	
		printf("___________\n");
		for(i=0;i<n/m;i++){
			ind = i*m;
			f(A+ind, B+ind, C+ind,n/m);
			printf("C[%d]=%d+%d=%d \n", ind, *(A+ind), *(B+ind), 	*(C+ind));			
		}
	}
	free(A);free(B);free(C);
return 0;
}

