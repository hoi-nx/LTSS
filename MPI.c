#include <stdio.h>
#include <mpi.h>
#include <unistd.h> 
int main(int args, char **arrgv){
	int rank,size;
	MPI_Init(&args,&arrgv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
    printf('Hello :rank %d,word :%d \n',rank,size);
	//MPI_Finalize();
	return 0;
}
