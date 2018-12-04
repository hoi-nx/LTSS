#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <mpi.h>

#define  m       500
#define  n       500
#define  T       100
#define  dt      0.01
#define  dx      0.1
#define  D       0.1
#define  k       10   //kich thuoc duel core
//=========================



void DisplayMatrix(float *A, int row,  int col)
{
    int i,j;
    for(i=0;i<row;i++){
        for(j=0;j<col;j++) printf("  %f",*(A+i*col+j));
        printf("\n");
    }
}

//=========================
void Write2File(float *C)
{
    FILE *result=fopen("result3.csv", "a");
    int i,j;
    
    for(i=0;i<m;i++)
    {
        for(j=0;j<n;j++)
        {
            fprintf(result, "%lf\t", *(C+i*n+j));
        }
        fprintf(result, "\n");
    }
    
    fclose(result);
}
//=========================
void KhoiTao(float *C)
{
    int i,j;
    for (  i = 0 ; i < m ; i++ )
        for ( j = 0 ; j < n ; j++ ){
            if (i>=(m/2-5)&&i<(m/2+5)&&j>=(n/2-5)&&j<(n/2+5))
                *(C+i*n+j) = 100.0;
            else
                *(C+i*n+j) = 25.0;
        
        }
}
//=========================
/*
 void FD(float *C, float *dC)
 {
 int i, j;
 float c,u,d,l,r;
 for (  i = 0 ; i < m ; i++ )
 for ( j = 0 ; j < n ; j++ )
 {
 c = *(C+i*n+j);
 u = (i==0)   ? 25 : *(C+(i-1)*n+j);
 d = (i==m-1) ? 25 : *(C+(i+1)*n+j);
 l = (j==0)   ? 25 : *(C+i*n+j-1);
 r = (j==n-1) ? 25 : *(C+i*n+j+1);
 *(dC+i*n+j) = (D/(dx*dx))*(u+d+l+r-4*c);
 }
 }*/
void FD(float *Cs, float *Cu, float *Cd, float*dCs, int ms,int rank){
    
    int i,j;
    float c,u,d,l,r;
    for(i=0;i<ms;i++)
        for(j=0;j<n;j++){
            if ((ms*rank+i>=(m/2-5)&&ms*rank+i<(m/2+5))&&(j>=(n/2-5)&&j<(n/2+5))) {
                continue;
            }
            
            c = *(Cs+i*n+j);
            u = (i==0)   ? *(Cu+j) : *(Cs+(i-1)*n+j);
            d = (i==ms-1) ? *(Cd+j) : *(Cs+(i+1)*n+j);
            l = (j==0)   ? 25 : *(Cs+i*n+j-1);
            r = (j==n-1) ? 25 : *(Cs+i*n+j+1);
            *(dCs+i*n+j) = (D/(dx*dx))*(u+d+l+r-4*c);
        }
}
//=========================

int main(int argc, char **argv)
{
    int i,j, Mc, rank, NP;
    
    //==================================
    // Khoi tao MPI
    MPI_Init(&argc,&argv);
    //==================================
    // Tinh so process: NP, gan ID cho tung process:rank
    MPI_Comm_size(MPI_COMM_WORLD,&NP);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Status stat;
    //==================================
    
    Mc=m/NP;
    
    
    
    float *C,*dC;
    C = (float *) malloc ((m*n)*sizeof(float));
    dC = (float *) malloc ((m*n)*sizeof(float));
    
    float *Cs, *dCs;
    Cs = (float *) malloc ((Mc*n)*sizeof(float));
    dCs = (float *) malloc ((Mc*n)*sizeof(float));
    
    float *Cu, *Cd;
    Cu = (float *) malloc (n*sizeof(float));
    Cd = (float *) malloc (n*sizeof(float));
    
    if(rank==0){
        KhoiTao(C);
        Write2File(C);
    }
    
    MPI_Scatter(C, Mc*n, MPI_FLOAT, Cs, Mc*n, MPI_FLOAT, 0, MPI_COMM_WORLD);
    
    int Ntime=T/dt;
    int t=0;
    while(t<Ntime){
        if(rank==0){
            for (j=0;j<n;j++) *(Cu+j) = 25;
            MPI_Send(Cs+(Mc-1)*n, n, MPI_FLOAT, rank+1, rank, MPI_COMM_WORLD);
        }else if(rank==NP-1){
            MPI_Recv(Cu, n, MPI_FLOAT, rank-1, rank-1, MPI_COMM_WORLD, &stat);
        }else{
            MPI_Send(Cs+(Mc-1)*n, n, MPI_FLOAT,rank+1, rank, MPI_COMM_WORLD);
            MPI_Recv(Cu, n, MPI_FLOAT, rank-1, rank-1, MPI_COMM_WORLD, &stat);
        }
        
        if(rank==NP-1){
            for (j=0;j<n;j++) *(Cd+j) = 25;
            MPI_Send(Cs, n, MPI_FLOAT, rank-1, rank, MPI_COMM_WORLD);
        }else if(rank==0){
            MPI_Recv(Cd, n, MPI_FLOAT, rank+1, rank+1, MPI_COMM_WORLD, &stat);
        }else{
            MPI_Send(Cs, n, MPI_FLOAT, rank-1, rank, MPI_COMM_WORLD);
            MPI_Recv(Cd, n, MPI_FLOAT, rank+1, rank+1, MPI_COMM_WORLD, &stat);
        }
        FD(Cs, Cu, Cd, dCs, Mc, rank);
        
        for (  i = 0 ; i < Mc ; i++ )
            for ( j = 0 ; j < n ; j++ )
                *(Cs+i*n+j) = *(Cs+i*n+j) + dt*(*(dCs+i*n+j));
        
        t++;
    }
    
    MPI_Gather(Cs, Mc*n, MPI_FLOAT, C, Mc*n, MPI_FLOAT, 0, MPI_COMM_WORLD);
    
    
    if(rank==0){
//Write2File(C);
    }
    
    MPI_Finalize();
    
    
    return 0;
}

