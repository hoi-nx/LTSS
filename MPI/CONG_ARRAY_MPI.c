#include <stdio.h>
//#include <malloc.h>
#include <mpi.h>


int main(int argc, char **argv){
    
    int i,rank,size,Mc;
    MPI_Init(&args,&arrgv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    
    MPI_Status stat;
    //Bước 1
    if (rank==0){
        int *A, *B, *C;
        A = (int *) malloc ((10)*sizeof(int));
        B = (int *) malloc ((10)*sizeof(int));
        C = (int *) malloc ((10)*sizeof(int));
        for(i=0;i<10;i++){
            *(A+i) =i;
            *(B+i) =2*i;
            
        }
    }
    
    //Bước 2
    int Mc=10/size;
    
    int *Ac, *Bc, *Cc;
    Ac = (int *) malloc ((Mc)*sizeof(int));
    Bc = (int *) malloc ((Mc)*sizeof(int));
    Cc = (int *) malloc ((Mc)*sizeof(int));
    //Bước 3
    if(rank==0){
        for(i=0;i<Mc;i++){
            *(Ac+i)=*(A+i);
            *(Bc+i)=*(B+i);
        
        }
        for (cpu=1; cpu<size; cpu++) {
            MPI_Send(A+Mc*cpu,Mc_MPI_INT,cpu,cpu,MPI_COMM_WORLD);
        }
    
    }else{
        
        MPI_Recv(Ac,Mc,MPI_INT,0,rank,MPI_COMM_WORLD,&yes);
    
    
    }
    //Bước 4
    for(i=0;i<10;i++){
        *(C+i) = *(A+i)+*(B+i);
    }
    
    for(i=0;i<10;i++){
        printf("A[%d]: %d",i, *(A+i));
        printf("  | B[%d]: %d",i, *(B+i));
        printf("    |  C[%d]: %d \n",i, *(C+i));
    }
    free(A);free(B);free(C);
    return 0;
}

