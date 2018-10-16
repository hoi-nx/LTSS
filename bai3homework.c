#include <stdio.h>
#include <omp.h>
#include <malloc.h>
int main ()
{
	printf("Ex A\n");
	int i, id;
	int N = 10, m = 2;
	int *A = malloc(10*sizeof(int));
	int *B = malloc(10*sizeof(int));
	int *C = malloc(10*sizeof(int));

	omp_set_num_threads(m);
	#pragma omp parallel for \
		           private(id)
	for (i=0; i < N; i++){
		id = omp_get_thread_num();
		*(A+i) = i;
		*(B+i) = 2*i;
		*(C+i)=*(A+i)+ *(B+i);
		printf("From thread %d,A[%d] = %d, B[%d] = %d, C[%d] = %d\n"
				, id, i, *(A+i), i, *(B+i), i, *(C+i));
	}
	printf("--------------------------------------------------------\n");
	printf("Ex B\n");
	int sum =0;
	int demSum=0;
	#pragma omp parallel for reduction(+:sum) \
		            private(id)
	for(i=0;i<N;i++){
		sum +=*(A+i);
		demSum++;
	}
	printf("Sum A = %d ,Thoi gian = %d \n",sum,demSum);
	printf("--------------------------------------------------------\n");
	printf("Ex C\n");
	printf("Search max in C don't use to OMP\n");
	int dem1 = 0;
	int max1 = C[0];
	for(i=0;i<N;i++){
		if(*(C+i)>max1){
			max1=*(C+i);
			dem1++;
		}
	}
	printf("Max cua mang C bang = %d \n",max1);
	printf("Thoi gian tìm max = %d \n",dem1);
	printf("--------------------------------------------------------\n");
	printf("Search max using OMP\n");
	int max2 = C[0];
	int dem2 = 0;
	omp_set_num_threads(5);
	#pragma omp parallel for   \
		                private(id)
	for (i=0;i<N;i++){
		id=omp_get_thread_num();
		#pragma omp critical
		if(*(C+i)>max2){
			max2 = *(C+i);
			dem2++;
			printf("From thread %d, C[%d]=%d,maxC=%d,dem=%d\n"
					,id,i,*(C+i),max2,dem2);
		}   
	}
	printf("Max cua mang C bang (OMP) = %d \n",max2);
	printf("Thoi gian tìm max OMP = %d \n",dem2);
	free(A), free(B), free(C);
return 0;
}
