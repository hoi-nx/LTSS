#include<stdio.h>
#include<stdlib.h>
int nhap(int *a, int n){
	int i;
	printf("Nhap cac phan tu mang: ");
	for(i=0;i<=n-1;i++){
		printf("\nNhap phan tu mang x[%d]: ",i);
		scanf("%d",(a+i));	
	}
return 0;
}

int main(){
	int *a = (int *) malloc(10*sizeof(int));
	int *b = (int *) malloc(10*sizeof(int));
	int *c = (int *) malloc(10*sizeof(int));
	int n = 10;
	nhap(a,n);
	nhap(b,n);
	int i;
	for (i=0;i<=n-1;i++){
		c[i] = a[i] +b[i];
		printf("%d \n Mang C la: ",*(c+i));	
	}
	return 0;
}
