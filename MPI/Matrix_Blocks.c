#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define m 8 //ROWS
#define n 8 //COLS
#define p 4

int main(int argc, char **argv) {
    
    int NP, rank;
    // Khoi Tao MPI
    MPI_Init(NULL,NULL);
    MPI_Comm_size(MPI_COMM_WORLD,&NP);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Status thongbao;
    
    int *A, *B, *C;
    A = (int *) malloc (sizeof(int)*m*n);
    B = (int *) malloc (sizeof(int)*n*p);
    C = (int *) malloc (sizeof(int)*m*p);

    const int NPROWS=4;
    const int NPCOLS=2;
    const int BLOCKROWS = m/NPROWS;
    const int BLOCKCOLS = n/NPCOLS;
    
    if (rank==0){
        for (int i=0;i<m;i++)
            for (int j=0;j<n;j++)
                if (i==j)
                    *(A+i*n+j) = 1;
                else
                    *(A+i*n+j) = 0;
        for (int i=0;i<n;i++)
            for (int j=0;j<p;j++)
                *(B+i*p+j) = i*p+j;
    }
    
    if (NP != NPROWS*NPCOLS) {
        fprintf(stderr,"Error: number of PEs %d != %d x %d\n", p, NPROWS, NPCOLS);
        MPI_Finalize();
        exit(-1);
    }
    int *Ac, *Cc, *Bc;
    Ac = (int *) malloc (sizeof(int)*BLOCKROWS*BLOCKCOLS);
    Bc = (int *) malloc (sizeof(int)*n*p);
    Cc = (int *) malloc (sizeof(int)*BLOCKROWS*BLOCKCOLS);
    
    //for (int ii=0; ii<BLOCKROWS*BLOCKCOLS; ii++) b[ii] = 0;
    
    MPI_Datatype blocktype;
    MPI_Datatype blocktype2;
    
    MPI_Type_vector(BLOCKROWS, BLOCKCOLS, n, MPI_INT, &blocktype2);
    MPI_Type_create_resized( blocktype2, 0, sizeof(int), &blocktype);
    MPI_Type_commit(&blocktype);
    
    int *disps, *counts;
    disps = (int *) malloc (sizeof(int)*NPROWS*NPCOLS);
    counts = (int *) malloc (sizeof(int)*NPROWS*NPCOLS);
    for (int i=0; i<NPROWS; i++) {
        for (int j=0; j<NPCOLS; j++) {
            *(disps+i*NPCOLS+j) = i*n*BLOCKROWS+j*BLOCKCOLS;
            *(counts+i*NPCOLS+j) = 1;
            //printf("%3d ",(int)counts[ii*NPCOLS+jj]);
        }
    }
    
    MPI_Scatterv(A, counts, disps, blocktype, Ac, BLOCKROWS*BLOCKCOLS, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(B,n*p,MPI_INT,0,MPI_COMM_WORLD);
    
    
    //Tính toán nhân ma trận
    
    for (int i=0;i<BLOCKROWS;i++)
        for (int k=0;k<BLOCKCOLS;k++){
            *(Cc+i*p+k) = 0;
            for (int j=0;j<BLOCKCOLS;j++)
                *(Cc+i*p+k) = *(Cc+i*p+k) + *(Ac+i*n+j)* *(B+j*p+k);
        
        }
   MPI_Gatherv(Cc, BLOCKROWS*BLOCKCOLS,MPI_INT, C, counts, disps,blocktype, 0,MPI_COMM_WORLD);
    if (rank==0)
         printf("C matrix: \n");
        for (int i=0;i<m;i++){
            for (int j=0;j<p;j++) {
                 printf("%3d ",*(C+i*p+j));
            }
            printf("\n");
        }
    
    
    for (int proc=0; proc<NP; proc++) {
        if (proc == rank) {
            printf("Rank = %d\n", rank);
            if (rank == 0) {
                printf("Global matrix: \n");
                for (int ii=0; ii<m; ii++) {
                    for (int jj=0; jj<n; jj++) {
                        printf("%3d ",*(A+ii*n+jj));
                    }
                    printf("\n");
                }
            }
            printf("Local Matrix:\n");
            for (int ii=0; ii<BLOCKROWS; ii++) {
                for (int jj=0; jj<BLOCKCOLS; jj++) {
                    printf("%3d ",*(Ac+ii*BLOCKCOLS+jj));
                }
                printf("\n");
            }
            printf("\n");
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }
    
    MPI_Finalize();
    
    return 0;
}
