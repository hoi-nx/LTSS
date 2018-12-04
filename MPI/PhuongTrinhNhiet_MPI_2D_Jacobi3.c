#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
#define  m       50
#define  n       50
#define  T       10
#define  dt      0.01
#define  dx      0.1
#define  D       0.1
//==================================
void DisplayMatrix(float *A, int row,  int col)
{
    int i,j;
    for(i=0;i<row;i++){
        for(j=0;j<col;j++) printf("  %f",*(A+i*col+j));
        printf("\n");
    }
}
//==================================
void Write2File(float *C,char str[])
{
    FILE *result=fopen(str, "a");
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
//==================================
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
//==================================
void FD(float *Cs,float *Cu,float *Cd,float *xNew,int ms,int rank,double maxdiffnorm)
{
    int i, j;
    float c,u,d,l,r;
    for (  i = 0 ; i < ms ; i++ )
        for ( j = 0 ; j < n ; j++ )
        {
            if ((ms*rank+i>=(m/2-5)&&ms*rank+i<(m/2+5))&&(j>=(n/2-5)&&j<(n/2+5))||(ms*rank+i==0 && (j>=0 &&j<n))||
                (ms*rank+i==m-1&&(j>=0 &&j<n))) {
                continue;
            }
            c = *(Cs+i*n+j);
            u = (i==0)    ? *(Cu+j)     : *(Cs+(i-1)*n+j);
            d = (i==ms-1) ? *(Cd+j)     : *(Cs+(i+1)*n+j);
            l = (j==0)    ? 25 : *(Cs+i*n+j-1);
            r = (j==n-1)  ? 25 : *(Cs+i*n+j+1);
            *(xNew+i*n+j) = (u+d+l+r)/4.0;
            maxdiffnorm= MAX(maxdiffnorm,abs(*(xNew+i*n+j)- *(Cs+i*n+j)));
        }
}
//==================================
//==================================
main(int argc, char *argv[])
{
    int i, j;
    float t; t=0;
    int NP,rank,mc,itcnt;
    MPI_Status status;
    double t1,t2;
    float c,u,d,l,r;
    //
    double maxdiffnorm, gdiffnorm;
    float *C,*dC,*Cs,*xNew;
    float *Cu,*Cd;
    C   = (float *) malloc ((m*n)*sizeof(float));
    dC  = (float *) malloc ((m*n)*sizeof(float));
    // Khoi tao MPI
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&NP);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    //==================================
    mc = m/NP;
    Cs = (float *) malloc ((mc*n)*sizeof(float));
    
    xNew = (float *) malloc ((mc*n)*sizeof(float));
    //
    Cu = (float *) malloc (n*sizeof(float));
    Cd = (float *) malloc (n*sizeof(float));
    //==================================
    if (rank==0) {
        KhoiTao(C);
        Write2File(C,"init.csv");
    }
    
    t1 = MPI_Wtime();
    MPI_Scatter(C,mc*n,MPI_FLOAT,Cs,mc*n,MPI_FLOAT,0,MPI_COMM_WORLD);
    //==================================
    do
    {
        if (rank==0){
            for (j=0;j<n;j++) *(Cu+j) = 25;//*(Cs+0*n+j);
            MPI_Send(Cs+(mc-1)*n,n,MPI_FLOAT,rank+1,rank,MPI_COMM_WORLD);
        } else if (rank==NP-1) {
            MPI_Recv(Cu,n,MPI_FLOAT,rank-1,rank-1,MPI_COMM_WORLD,&status);
        } else {
            MPI_Send(Cs+(mc-1)*n,n,MPI_FLOAT,rank+1,rank,MPI_COMM_WORLD);
            MPI_Recv(Cu,n,MPI_FLOAT,rank-1,rank-1,MPI_COMM_WORLD,&status);
        }
        if (rank==NP-1){
            for (j=0;j<n;j++) *(Cd+j) = 25; //*(Cs+(mc-1)*n+j);
            MPI_Send(Cs,n,MPI_FLOAT,rank-1,rank,MPI_COMM_WORLD);
        } else if (rank==0) {
            MPI_Recv(Cd,n,MPI_FLOAT,rank+1,rank+1,MPI_COMM_WORLD,&status);
        } else {
            MPI_Send(Cs,n,MPI_FLOAT,rank-1,rank,MPI_COMM_WORLD);
            MPI_Recv(Cd,n,MPI_FLOAT,rank+1,rank+1,MPI_COMM_WORLD,&status);
        }
        
         itcnt=itcnt+1;
        //FD(Cs,Cu,Cd,xNew,mc,rank,maxdiffnorm);
        
        maxdiffnorm=0.0;
        
        //================
        for (  i = 0 ; i < mc ; i++ )
            for ( j = 0 ; j < n ; j++ )
            {
                if ((mc*rank+i>=(m/2-5)&&mc*rank+i<(m/2+5))&&(j>=(n/2-5)&&j<(n/2+5))||(mc*rank+i==0 && (j>=0 &&j<n))||
                    (mc*rank+i==m-1&&(j>=0 &&j<n))) {
                    //*(xNew+i*n+j)=0;
                }
                c = *(Cs+i*n+j);
                u = (i==0)    ? *(Cu+j)     : *(Cs+(i-1)*n+j);
                d = (i==mc-1) ? *(Cd+j)     : *(Cs+(i+1)*n+j);
                l = (j==0)    ? 25 : *(Cs+i*n+j-1);
                r = (j==n-1)  ? 25 : *(Cs+i*n+j+1);
                *(xNew+i*n+j) = (u+d+l+r)/4.0;
                maxdiffnorm += (*(xNew+i*n+j)- *(Cs+i*n+j))*(*(xNew+i*n+j)- *(Cs+i*n+j));
            }
        
        MPI_Allreduce( &maxdiffnorm, &gdiffnorm, 1, MPI_DOUBLE, MPI_MAX,MPI_COMM_WORLD );
        gdiffnorm = sqrt( gdiffnorm );
        if (rank == 0) printf( "At iteration %d, diff is %e\n", itcnt,gdiffnorm );
        //
        for (  i = 0 ; i < mc ; i++ )
            for ( j = 0 ; j < n ; j++ )
                *(Cs+i*n+j) = *(xNew+i*n+j);
        
       
    }while(gdiffnorm>1.0e-2&& itcnt<100);
    
    //==================================
    MPI_Gather(Cs,mc*n,MPI_FLOAT,C,mc*n,MPI_FLOAT,0,MPI_COMM_WORLD);
    t2 = MPI_Wtime();
    printf ("\t time:%f\n",(t2 - t1));
    if (rank==0)
    {
        // printf( "Ma tran C:\n");
        //DisplayMatrix(C, m, n);
        Write2File(C,"result2.csv");
    }
    
    MPI_Finalize();
    return 0;
}


