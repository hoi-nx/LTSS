#include <stdio.h>
#include <omp.h>
#include <malloc.h>
int main()
{
	int i,id,N = 10;
	int *A = malloc(10*sizeof(int));
	
	for (i=0;i<N;i++){
		*(A+i) = i;
		*(A+2) = 1000;
		printf("A[%d] = %d \n",i,*(A+i));
	}
	int nt = N;id = 0;
	int max = *(A+N-1);
	do {
		if (nt%2==1){
			if (*(A+nt-2)>*(A+nt-1)) max = *(A+nt-2); 
			nt = nt/2;
		} else nt = nt/2;
		omp_set_num_threads(nt);
		#pragma omp parallel private(id)
		{
			id = omp_get_thread_num();
			if (*(A+id)>*(A+id+nt)) max = *(A+id);
		}
	}
	while (nt>1);
	printf("Max la: %d\n",max);	
	free(A);
	return 0;
}
