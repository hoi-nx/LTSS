//#include <stdio.h>
#include <omp.h>
int main()
{
	int n=5,m=4,p=2,i,j,k,s,id,stt,stp;
	int A[n][m], B[m][p], C[n][p];
	printf("A------------------------------\n"); 
	for (i=0;i<n;i++){
		for (j=0;j<m;j++){
			A[i][j] = i*j;
			printf("%d ",A[i][j]);
		}
		printf("\n");
	}
	printf("B------------------------------\n");
	for (i=0;i<m;i++){
		for (j=0;j<p;j++){
			B[i][j] = 2*i*j;
			printf("%d ",B[i][j]);
		}
		printf("\n");
	}
	printf("normal========================\n");
	for (i=0;i<n;i++){
		for (j=0;j<p;j++){
			s = 0;
			for (k=0;k<m;k++){
				s += A[i][k]*B[k][j];
			}
			C[i][j] = s;
			printf("%d ",C[i][j]);
		}
		printf("\n");
	}
	printf("Using openmp--------------------------\n");
	omp_set_num_threads(n);
	#pragma omp parallel private(s,id,stt,stp,j,k) 
	{
		id = omp_get_thread_num();
			for (j=id;j<p;j++){
				s = 0;
				for (k=0;k<m;k++){
					s += A[id][k]*B[k][j];
				}
				C[i][j] = s;
				printf("from thread %d C[%d][%d] = %d\n",id,id,j,C[id][j]);
			}
			printf("\n");
		
	}
	for (i=0;i<n;i++){
		for (j=0;j<p;j++){
			printf("%d ",C[i][j]);
		}
		printf("\n");
	}
	return 0;
}

