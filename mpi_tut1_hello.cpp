#include <iostream>
#include <mpi.h>
using namespace std;

int main (int argc, char *argv[])
{
    int rank, size;
    // Initialize MPI 
    MPI_Init (&argc, &argv);

    // Get Rank and Size from MPI
    MPI_Comm_rank (MPI_COMM_WORLD, &rank); /* get current process id */
    MPI_Comm_size (MPI_COMM_WORLD, &size); /* get number of processes */

    if (rank == 0)
      cout<<"Hello world! Process "<<rank<<" of "<<size<<endl;
    else
      cout<<"Hello world from process "<<rank<<endl;
  
    MPI_Finalize();
    return 0;
}
