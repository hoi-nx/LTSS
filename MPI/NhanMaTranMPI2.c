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
int i,j,cpu,NP,k, rank, mc;
MPI_Init(NULL,NULL);
MPI_Comm_size(MPI_COMM_WORLD,&NP);
MPI_Comm_rank(MPI_COMM_WORLD,&rank);
MPI_Status thongbao;
//printf("Hello world from process %d of %d\n" ,rank , NP );

int A[m][n], B[n][p], C[m][p];
int Ac[1][n], Bc[n][p], Cc[1][p],Acc[1][n];
//Khoi tao Mang A B
if(rank==0){
    for (i=0;i<n;i++){
        for (j=0;j<m;j++){
           A[i][j] = i+j;
        }
    }
    for (i=0;i<m;i++){
        for (j=0;j<p;j++){
            B[i][j] = 2*i+j;
        }
    }
     //Diplay Ma tran A B
    print_matrix(A,m,n);
    printf("B------------------------------\n");
    print_matrix(B,n,p);

}
//Gửi da ta đến các CPU
if(rank==0){
    for(j=0;j<n;j++){
        Ac[0][j]=A[0][j];
    
    }
    for(i=0;i<n;i++){
        for(j=0;j<p;j++){
            Bc[i][j]=B[i][j];
        }
    }
    for(cpu=1;cpu<NP;cpu++){
        for(j=0;j<n;j++){
            Acc[0][j]=A[cpu][j];
            MPI_Send(&Acc,n,MPI_INT,cpu,cpu,MPI_COMM_WORLD);
        }
        
        MPI_Send(&B,n*p,MPI_INT,cpu,cpu+100,MPI_COMM_WORLD);
        printf("\n");
       
    }
    
}else{
     MPI_Recv(&Ac,n,MPI_INT,0,rank,MPI_COMM_WORLD,&thongbao);
    MPI_Recv(&Bc,n*p,MPI_INT,0,rank+100,MPI_COMM_WORLD,&thongbao);
}
    if(rank==0){
        for(i=0;i<n;i++){
            printf("Phần tử của véc tơ Ac %d\n",Acc[0][i]);
        }
        printf("Ma trận Bc \n");
        print_matrix(Bc,n,p);
        
    }
//Tính toán nhân Ma trận với vector
   
        for(j=0;j<p;j++)
        {
            Cc[0][j]=0;
            for(k=0;k<n;k++)
            {
                Cc[0][j]+=Ac[0][k]*Bc[k][j];
            }
        }
    print_matrix(Cc,1,p);
    
//Tổng hợp kết quả
//    if (rank==0){
//        printf("Tích của 2 ma trận là C=\n");
//       print_matrix(C,m,p);
//    }
//    
MPI_Finalize();
return 0;
}
 
