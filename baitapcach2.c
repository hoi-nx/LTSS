#include <stdio.h>
#include <omp.h>
#include <malloc.h>
int main ()
{
	printf("Ex A\n");
	int i, id, stt, stp;
	int N = 10, nt = 2;
	int m = N/nt;
	int *A = malloc(10*sizeof(int));
	int *B = malloc(10*sizeof(int));
	int *C = malloc(10*sizeof(int));
	for (i = 0; i<N;i++){
		*(A+i) = i;
		*(B+i) = 2*i;
		printf("A[%d] = %d, B[%d] = %d \n", i, *(A+i), i, *(B+i));
	}
	omp_set_num_threads(nt);
	#pragma omp parallel private(id,i,stt,stp)
	{
		id = omp_get_thread_num();
		stt = id*m; stp = id*m+m;
		for (i=stt; i < stp; i++){
			*(C+i)=*(A+i)+ *(B+i);
			printf("From thread %d, C[%d] = %d\n", id, i, *(C+i));
		}
	}
	free(A);free(B);free(C);
	return 0;
}
