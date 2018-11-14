#include <stdio.h>
#include <mpi.h>
#define m 6
#define n 6
#define p 3

void print_matrix(int* a, int row, int col)
{
    printf("\n");
    for(int i = 0; i < row; i++)
    {
        for(int j = 0; j < col; j++)
            printf("%4d", a[i * col + j]);
        printf("\n");
    }
}
int main(int argc,char** argv)
{
int i,j,k,NP, rank, mc;
MPI_Init(NULL,NULL);
MPI_Comm_size(MPI_COMM_WORLD,&NP);
MPI_Comm_rank(MPI_COMM_WORLD,&rank);
MPI_Status thongbao;
//printf("Hello world from process %d of %d\n" ,rank , NP );

int *A, *B, *C;
A = (int *) malloc (sizeof(int)*m*n);
B = (int *) malloc (sizeof(int)*n*p);
C = (int *) malloc (sizeof(int)*m*p);
//Chia mien tinh toan
mc = m/NP;
//Khoi tao Mang A B
    if (rank==0){
        for (i=0;i<m;i++)
            for (j=0;j<n;j++)
                if(i==j){
                  *(A+i*n+j) = 1;
                }else{
                  *(A+i*n+j) = 0;
                }
                //*(A+i*n+j) = i+j;
        for (i=0;i<n;i++)
            for (j=0;j<p;j++)
                *(B+i*p+j) = 2*i+j;
        printf("Ma trận A\n");
        print_matrix(A,m,n);
        printf("\nMa trận B\n");
        print_matrix(B,n,p);
    }
   
//Gui Data den tat ca cac CPU
    int *Ac, *Cc, *Bc;
    Ac = (int *) malloc (sizeof(int)*mc*n);
    Bc = (int *) malloc (sizeof(int)*n*p);
    Cc = (int *) malloc (sizeof(int)*mc*p);
//    MPI_Scatter(A,mc*n,MPI_INT,Ac,mc*n,MPI_INT,0,MPI_COMM_WORLD);
//    MPI_Bcast(B,n*p,MPI_INT,0,MPI_COMM_WORLD);

    if (rank == 0) {
        for (i=0;i<mc;i++)
            for (j=0;j<n;j++)
                *(Ac+i*n+j) = *(A+i*n+j);
        for (i=0;i<n;i++)
            for (j=0;j<p;j++)
                *(Bc+i*p+j) = *(B+i*p+j);
        
        for (k=1;k<NP;k++){
            MPI_Send(A+k*mc*n,mc*n,MPI_INT,k,k,MPI_COMM_WORLD);
            MPI_Send(B,n*p,MPI_INT,k,k+100,MPI_COMM_WORLD);
        }

    } else {
        MPI_Recv(Ac,mc*n,MPI_INT,0,rank,MPI_COMM_WORLD,&thongbao);
        MPI_Recv(Bc,n*p,MPI_INT,0,rank+100,MPI_COMM_WORLD,&thongbao);

    }
	
//Tinh toan: Nhan ma tran
	for (i=0;i<mc;i++)
		for (k=0;k<p;k++){
			*(Cc+i*p+k) = 0;	
			for (j=0;j<n;j++)
				*(Cc+i*p+k) = *(Cc+i*p+k)+ *(Ac+i*n+j)* *(Bc+j*p+k);
		}
//Tap hop ket qua
    //Using phuong thức của MPI_Gather
//MPI_Gather(Cc,mc*p,MPI_INT,C,mc*p,MPI_INT,0,MPI_COMM_WORLD);
    if (rank != 0) {
            MPI_Send(Cc,mc*p,MPI_INT,0,rank,MPI_COMM_WORLD);
    } else {
        for (i=0;i<mc;i++)
            for (j=0;j<p;j++)
                *(C+i*p+j) = *(Cc+i*p+j);
        for (k=1;k<NP;k++)
            MPI_Recv(C+k*mc*p,mc*p,MPI_INT,k,k,MPI_COMM_WORLD,&thongbao);
    }

// In Ket Qua
if (rank==0){
    printf("Tích của 2 ma trận là C=\n");
    for (i=0;i<m;i++){
        for (j=0;j<p;j++) printf("%d\t",*(C+i*p+j));
        printf("\n");
    }
}

MPI_Finalize();
return 0;
}
 
