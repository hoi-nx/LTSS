#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <mpi.h>

#define  m       20
#define  Time    10
#define  dt      0.01
#define  dx      0.1
#define  D       0.1
//=========================
void DisplayArray(float *T, int size)
{
    int i;
    for(i=0;i<size;i++)
        printf("  %.2f",*(T+i));
    printf("\n");
    
}
//=========================
void Write2File(float *T, int size)
{
    FILE *result=fopen("result2.txt", "a");
    int i;
    
    for(i=0;i<size;i++)
    {
        fprintf(result, "%lf", *(T+i));
        fprintf(result, "\n");
    }
    
    fclose(result);
}
//=========================
void KhoiTao(float *T)
{
    int i,j;
    for (  i = 0 ; i < m ; i++ )
        *(T+i) = 25.0;
}
//=========================
void DHB2(float *Ts, float Tl, float Tr, float *dTs ,int ms,int id,int NP) {
    int i;
    float c, l, r;
    for (i = 0 ; i < ms ; i++ ) {
        // ignore last point
        //        if (id == NP - 1){
        //            if (i == ms -1){
        //                continue;
        //            }
        //        }
        c = *(Ts + i);
        //Tl =100 Tr=25
        l = (i == 0)        ? Tl   : *(Ts + i - 1);
        r = (i == ms - 1)   ? Tr   : *(Ts + i + 1);
        *(dTs + i) = D * (l - 2 * c + r) / (dx * dx);
    }
}

//=========================
int main(int argc, char *argv[])
{
    int i, j,NTime;
    float t; t=0;
    int NP,rank,mc; MPI_Status stat;
    float *C,*Cs,*Ts,*dTs;
    float Tl,Tr;
    
    C   = (float *) malloc ((m)*sizeof(float));
    
    MPI_Init(NULL,NULL);
    MPI_Comm_size(MPI_COMM_WORLD,&NP);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    //==================================
    // Kích thước miền tính toán là m
    //Mỗi CPU tính toán trên miền có kích thước là mc = N / NP với NP là số lượng CPU
    
    mc = m/NP;
    // Cap phat bo nho cho cac ma tran con
    Cs  = (float *) malloc ((mc)*sizeof(float));
    
    //mang tinh đạo hàm
    dTs = (float *) malloc ((mc)*sizeof(float));
    if(rank==0){
        KhoiTao(C);
        DisplayArray(C,m);
    }
    
    MPI_Scatter(C,mc,MPI_FLOAT,Cs,mc,MPI_FLOAT,0,MPI_COMM_WORLD);
    
    printf( "Ma tran Cs tai core %d is:\n", rank);
    DisplayArray(Cs, mc);
    
    NTime=Time/dt;
    int t;
    
    for(t=0;t<NTime;t++){
        if (rank == 0){
            Tl = 100;
            MPI_Send (Cs + mc - 1, 1, MPI_FLOAT, rank + 1, rank, MPI_COMM_WORLD);
        } else if (rank == NP - 1) {
            MPI_Recv (&Tl, 1, MPI_FLOAT, rank - 1, rank - 1, MPI_COMM_WORLD, &stat);
        } else {
            MPI_Send (Cs + mc - 1, 1, MPI_FLOAT, rank + 1, rank, MPI_COMM_WORLD);
            MPI_Recv (&Tl, 1, MPI_FLOAT, rank - 1, rank - 1, MPI_COMM_WORLD, &stat);
            
        }
        //=================================
        if (rank == NP - 1){
            Tr = 25;
            MPI_Send (Cs, 1, MPI_FLOAT, rank - 1, rank, MPI_COMM_WORLD);
        } else if (rank == 0) {
            MPI_Recv (&Tr, m, MPI_FLOAT, rank + 1, rank + 1, MPI_COMM_WORLD, &stat);
        } else {
            MPI_Send (Cs, 1, MPI_FLOAT, rank - 1, rank, MPI_COMM_WORLD);
            MPI_Recv (&Tr, 1, MPI_FLOAT, rank + 1, rank + 1, MPI_COMM_WORLD, &stat);
            
        }
        
        // printf( "Ma tran Cs tai core %d is:\n", rank);
        //DisplayArray(Cs, 1);
        //printf("\n");
        //Tính đạo hàm
        DHB2(Cs,Tl,Tr,dTs,mc,rank,NP);
        ///
        for (  i = 0 ; i < mc ; i++ )
            *(Cs+i) = *(Cs+i) + *(dTs+i)*dt*D;
    
    
    }
    
    
    
    
//    while (t<=Time) {
//       
//    }
    MPI_Gather(Cs,mc,MPI_FLOAT,C,mc,MPI_FLOAT,0,MPI_COMM_WORLD);
    if(rank==0){
        printf( "Ma tran C:\n");
        DisplayArray(C,m);
        printf("\n");
    }
    
    
    
    
    
    
    MPI_Finalize();
    
    return 0;
}



