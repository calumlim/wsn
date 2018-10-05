#include <stdio.h>
#include "mpi.h"

int main(argc, argv)
int argc;
char **argv;
{
  int rank, size;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_size(MPI_COMM_WORLD, &rank);

  MPI_Finalize();
  return 0;
}
