#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <mpi.h>

#define  n 1000


//=========================
int main(int argc, char *argv[])
{
    int a[n], b[n], myrank;
    int NP,rank,mc; MPI_Status stat;
    MPI_Init(NULL,NULL);
    MPI_Comm_size(MPI_COMM_WORLD,&NP);
    MPI_Comm_rank(MPI_COMM_WORLD,&myrank);
    if (myrank == 0) {
        MPI_Send(a, n, MPI_INT, 1, 1, MPI_COMM_WORLD);
        MPI_Send(b, n, MPI_INT, 1, 2, MPI_COMM_WORLD);
    }
    else if (myrank == 1) {
        //dead lock vif thằng gửi A chưa xong mà nhận B
        MPI_Recv(b, n, MPI_INT, 0, 2, MPI_COMM_WORLD,&stat);
        MPI_Recv(a, n, MPI_INT, 0, 1, MPI_COMM_WORLD,&stat);
    }
    
    
    
    
    MPI_Finalize();
    
    return 0;
}



