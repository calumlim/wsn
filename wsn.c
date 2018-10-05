#include <stdio.h>
#include "mpi.h"
/*Defining the variables for number of rows and columns*/
#define no_row 4
#define no_col 5

int main(argc, argv)
int argc;
char **argv;
{
  int rank, size;
  int grid[no_row][no_col];
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_size(MPI_COMM_WORLD, &rank);
    for (int i=0; i<=no_row; i++)
      for (int j=0; j<no_col; j++){
        grid[i][j] = rank;
        printf("%d", grid[i][j]);
      }
  MPI_Finalize();
  return 0;
}
