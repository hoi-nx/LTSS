#include <stdio.h>
#include <omp.h>
#include <malloc.h>
int main()
{
	int i,id,N = 10, m= 5,sum = 0,dem =0;
	int *A = malloc(10*sizeof(int));
	
	for (i=0;i<N;i++){
		*(A+i) = i;
		printf("A[%d] = %d \n",i,*(A+i));
	}
	//#pragma omp parallel for reduction(+:sum)
	//for(i=0;i<N;i++){
	//	sum +=*(A+i);
	//}
	int nt = N;id = 0;
	do {
		if (nt%2==1){
			*(A+nt-2) = *(A+nt-2) + *(A+nt-1);
			nt = nt/2;
		} else nt = nt/2;
		omp_set_num_threads(nt);
		#pragma omp parallel private(id)
		{
			id = omp_get_thread_num();
			*(A+id) +=*(A+id+nt);
			dem+=1;
			printf("Tinh tong tu thread %d = %d\n",id,*(A+id));
		}
	}
	while (nt>1);
	printf("Tong la: %d\n",*(A+0));	
	return 0;
}
