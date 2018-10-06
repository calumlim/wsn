#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mpi.h"

/*Defining the variables for number of rows and columns*/
#define no_row 4
#define no_col 5

int main(argc, argv)
int argc;
char **argv;
{
  int rank, size, grid_rowno, grid_colno;
  int grid[no_row][no_col][2];
  int base_station[no_row][no_col][2] = {-1};
  MPI_Request request[no_row*no_col];
  int nreq;
  MPI_Status statuses[no_row*no_col];
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);


  /*Calculating the 2D grid positions for each rank*/
  if (rank!=20){
    nreq = 0;
    srand(clock());
    grid_rowno = rank/no_col;
    grid_colno = rank%no_col;
    //grid[grid_rowno][grid_colno][0] = rank;
    printf("rank: %d  | [%d, %d]\n", rank, grid[grid_rowno][grid_colno][0], grid[grid_rowno][grid_colno][1]);
    /*Left boundary case*/
    if (grid_colno-1<0){
      /*top left case*/
      if (grid_rowno-1<0){
        if (grid[grid_rowno][grid_colno+1][1]==0){
          int rand_val = rand()%5+1;
          int neighbour_rank = grid_colno+1 + (no_col*grid_rowno);
          int buffer[2] = {neighbour_rank, rand_val};
          MPI_Isend(buffer, 2, MPI_INT, neighbour_rank, 0, MPI_COMM_WORLD, &request[rank]);
          MPI_Irecv(grid[grid_rowno][grid_colno], 2, MPI_INT, neighbour_rank, 0, MPI_COMM_WORLD, &request[rank]);
        }
        if (grid[grid_rowno+1][grid_colno][1]==0){
          int rand_val = rand()%5+1;
          int neighbour_rank = grid_colno + (no_col*grid_rowno+1);
          int buffer[2] = {neighbour_rank, rand_val};
          MPI_Isend(buffer, 2, MPI_INT, neighbour_rank, 0, MPI_COMM_WORLD, &request[rank]);
          MPI_Irecv(grid[grid_rowno][grid_colno], 2, MPI_INT, neighbour_rank, 0, MPI_COMM_WORLD, &request[rank]);
        }
      /*bottom left case*/
      } else if (grid_rowno+1>no_row-1){
        if (grid[grid_rowno][grid_colno+1][1]==0){
          int rand_val = rand()%5+1;
          int neighbour_rank = grid_colno+1 + (no_col*grid_rowno);
          int buffer[2] = {neighbour_rank, rand_val};
          MPI_Isend(buffer, 2, MPI_INT, neighbour_rank, 0, MPI_COMM_WORLD, &request[rank]);
          MPI_Irecv(grid[grid_rowno][grid_colno], 2, MPI_INT, neighbour_rank, 0, MPI_COMM_WORLD, &request[rank]);
        }
        if (grid[grid_rowno-1][grid_colno][1]==0){
          int rand_val = rand()%5+1;
          int neighbour_rank = grid_colno + (no_col*grid_rowno-1);
          int buffer[2] = {neighbour_rank, rand_val};
          MPI_Isend(buffer, 2, MPI_INT, neighbour_rank, 0, MPI_COMM_WORLD, &request[rank]);
          MPI_Irecv(grid[grid_rowno][grid_colno], 2, MPI_INT, neighbour_rank, 0, MPI_COMM_WORLD, &request[rank]);
        }
      /*has at least 3 adjacent nodes*/
      } else {
        /*top row, same col*/
        if (grid[grid_rowno-1][grid_colno][1]==0){
          int rand_val = rand()%5+1;
          int neighbour_rank = grid_colno + (no_col*grid_rowno-1);
          int buffer[2] = {neighbour_rank, rand_val};
          MPI_Isend(buffer, 2, MPI_INT, neighbour_rank, 0, MPI_COMM_WORLD, &request[rank]);
          MPI_Irecv(grid[grid_rowno][grid_colno], 2, MPI_INT, neighbour_rank, 0, MPI_COMM_WORLD, &request[rank]);
        }
        /*bottom row, same col*/
        if (grid[grid_rowno+1][grid_colno][1]==0){
          int rand_val = rand()%5+1;
          int neighbour_rank = grid_colno + (no_col*grid_rowno+1);
          int buffer[2] = {neighbour_rank, rand_val};
          MPI_Isend(buffer, 2, MPI_INT, neighbour_rank, 0, MPI_COMM_WORLD, &request[rank]);
          MPI_Irecv(grid[grid_rowno][grid_colno], 2, MPI_INT, neighbour_rank, 0, MPI_COMM_WORLD, &request[rank]);
        }
        /*same row, right col*/
        if (grid[grid_rowno][grid_colno+1][1]==0){
          int rand_val = rand()%5+1;
          int neighbour_rank = grid_colno+1 + (no_col*grid_rowno);
          int buffer[2] = {neighbour_rank, rand_val};
          MPI_Isend(buffer, 2, MPI_INT, neighbour_rank, 0, MPI_COMM_WORLD, &request[rank]);
          MPI_Irecv(grid[grid_rowno][grid_colno], 2, MPI_INT, neighbour_rank, 0, MPI_COMM_WORLD, &request[rank]);
        }
      }
    /*Right boundary case*/
    } else if (grid_colno+1>no_col-1){
      /*top right case*/
      if (grid_rowno-1<0){
        if (grid[grid_rowno][grid_colno-1][1]==0){
          int rand_val = rand()%5+1;
          int neighbour_rank = grid_colno-1 + (no_col*grid_rowno);
          int buffer[2] = {neighbour_rank, rand_val};
          MPI_Isend(buffer, 2, MPI_INT, neighbour_rank, 0, MPI_COMM_WORLD, &request[rank]);
          MPI_Irecv(grid[grid_rowno][grid_colno], 2, MPI_INT, neighbour_rank, 0, MPI_COMM_WORLD, &request[rank]);
        }
        if (grid[grid_rowno+1][grid_colno][1]==0){
          int rand_val = rand()%5+1;
          int neighbour_rank = grid_colno + (no_col*grid_rowno+1);
          int buffer[2] = {neighbour_rank, rand_val};
          MPI_Isend(buffer, 2, MPI_INT, neighbour_rank, 0, MPI_COMM_WORLD, &request[rank]);
          MPI_Irecv(grid[grid_rowno][grid_colno], 2, MPI_INT, neighbour_rank, 0, MPI_COMM_WORLD, &request[rank]);
        }
      /*bottom right case*/
      } else if (grid_rowno+1>no_row-1){
        if (grid[grid_rowno][grid_colno-1][1]==0){
          int rand_val = rand()%5+1;
          int neighbour_rank = grid_colno + (no_col*grid_rowno+1);
          int buffer[2] = {neighbour_rank, rand_val};
          MPI_Isend(buffer, 2, MPI_INT, neighbour_rank, 0, MPI_COMM_WORLD, &request[rank]);
          MPI_Irecv(grid[grid_rowno][grid_colno], 2, MPI_INT, neighbour_rank, 0, MPI_COMM_WORLD, &request[rank]);
        }
        if (grid[grid_rowno-1][grid_colno][1]==0){
          int rand_val = rand()%5+1;
          int neighbour_rank = grid_colno + (no_col*grid_rowno-1);
          int buffer[2] = {neighbour_rank, rand_val};
          MPI_Isend(buffer, 2, MPI_INT, neighbour_rank, 0, MPI_COMM_WORLD, &request[rank]);
          MPI_Irecv(grid[grid_rowno][grid_colno], 2, MPI_INT, neighbour_rank, 0, MPI_COMM_WORLD, &request[rank]);
        }
      /*has at least 3 adjacent nodes*/
      } else {
        /*top row, same col*/
        if (grid[grid_rowno-1][grid_colno][1]==0){
          int rand_val = rand()%5+1;
          int neighbour_rank = grid_colno + (no_col*grid_rowno-1);
          int buffer[2] = {neighbour_rank, rand_val};
          MPI_Isend(buffer, 2, MPI_INT, neighbour_rank, 0, MPI_COMM_WORLD, &request[rank]);
          MPI_Irecv(grid[grid_rowno][grid_colno], 2, MPI_INT, neighbour_rank, 0, MPI_COMM_WORLD, &request[rank]);
        }
        /*bottom row, same col*/
        if (grid[grid_rowno+1][grid_colno][1]==0){
          int rand_val = rand()%5+1;
          int neighbour_rank = grid_colno + (no_col*grid_rowno+1);
          int buffer[2] = {neighbour_rank, rand_val};
          MPI_Isend(buffer, 2, MPI_INT, neighbour_rank, 0, MPI_COMM_WORLD, &request[rank]);
          MPI_Irecv(grid[grid_rowno][grid_colno], 2, MPI_INT, neighbour_rank, 0, MPI_COMM_WORLD, &request[rank]);
        }
        /*same row, left col*/
        if (grid[grid_rowno][grid_colno-1][1]==0){
          int rand_val = rand()%5+1;
          int neighbour_rank = grid_colno-1 + (no_col*grid_rowno);
          int buffer[2] = {neighbour_rank, rand_val};
          MPI_Isend(buffer, 2, MPI_INT, neighbour_rank, 0, MPI_COMM_WORLD, &request[rank]);
          MPI_Irecv(grid[grid_rowno][grid_colno], 2, MPI_INT, neighbour_rank, 0, MPI_COMM_WORLD, &request[rank]);
        }
      }
    /*Top row boundary case*/
    } else if (grid_rowno-1<0){
      /*left col*/
      if (grid[grid_rowno][grid_colno-1][1]==0){
        int rand_val = rand()%5+1;
        int neighbour_rank = grid_colno-1 + (no_col*grid_rowno);
        int buffer[2] = {neighbour_rank, rand_val};
        MPI_Isend(buffer, 2, MPI_INT, neighbour_rank, 0, MPI_COMM_WORLD, &request[rank]);
        MPI_Irecv(grid[grid_rowno][grid_colno], 2, MPI_INT, neighbour_rank, 0, MPI_COMM_WORLD, &request[rank]);
      }
      /*right col*/
      if (grid[grid_rowno][grid_colno+1][1]==0){
        int rand_val = rand()%5+1;
        int neighbour_rank = grid_colno+1 + (no_col*grid_rowno);
        int buffer[2] = {neighbour_rank, rand_val};
        MPI_Isend(buffer, 2, MPI_INT, neighbour_rank, 0, MPI_COMM_WORLD, &request[rank]);
        MPI_Irecv(grid[grid_rowno][grid_colno], 2, MPI_INT, neighbour_rank, 0, MPI_COMM_WORLD, &request[rank]);
      }
      /*bottom row, same col*/
      if (grid[grid_rowno+1][grid_colno][1]==0){
        int rand_val = rand()%5+1;
        int neighbour_rank = grid_colno + (no_col*grid_rowno+1);
        int buffer[2] = {neighbour_rank, rand_val};
        MPI_Isend(buffer, 2, MPI_INT, neighbour_rank, 0, MPI_COMM_WORLD, &request[rank]);
        MPI_Irecv(grid[grid_rowno][grid_colno], 2, MPI_INT, neighbour_rank, 0, MPI_COMM_WORLD, &request[rank]);
      }
    /*Bottom row boundary case*/
    } else if (grid_rowno+1>no_row-1){
      /*left col*/
      if (grid[grid_rowno][grid_colno-1][1]==0){
        int rand_val = rand()%5+1;
        int neighbour_rank = grid_colno-1 + (no_col*grid_rowno);
        int buffer[2] = {neighbour_rank, rand_val};
        MPI_Isend(buffer, 2, MPI_INT, neighbour_rank, 0, MPI_COMM_WORLD, &request[rank]);
        MPI_Irecv(grid[grid_rowno][grid_colno], 2, MPI_INT, neighbour_rank, 0, MPI_COMM_WORLD, &request[rank]);
      }
      /*right col*/
      if (grid[grid_rowno][grid_colno+1][1]==0){
        int rand_val = rand()%5+1;
        int neighbour_rank = grid_colno+1 + (no_col*grid_rowno);
        int buffer[2] = {neighbour_rank, rand_val};
        MPI_Isend(buffer, 2, MPI_INT, neighbour_rank, 0, MPI_COMM_WORLD, &request[rank]);
        MPI_Irecv(grid[grid_rowno][grid_colno], 2, MPI_INT, neighbour_rank, 0, MPI_COMM_WORLD, &request[rank]);
      }
      /*top row, same col*/
      if (grid[grid_rowno-1][grid_colno][1]==0){
        int rand_val = rand()%5+1;
        int neighbour_rank = grid_colno + (no_col*grid_rowno-1);
        int buffer[2] = {neighbour_rank, rand_val};
        MPI_Isend(buffer, 2, MPI_INT, neighbour_rank, 0, MPI_COMM_WORLD, &request[rank]);
        MPI_Irecv(grid[grid_rowno][grid_colno], 2, MPI_INT, neighbour_rank, 0, MPI_COMM_WORLD, &request[rank]);
      }
    /*has at least three adjacent nodes*/
    } else {
      /*top*/
      if (grid[grid_rowno-1][grid_colno][1]==0){
        int rand_val = rand()%5+1;
        int neighbour_rank = grid_colno + (no_col*grid_rowno-1);
        int buffer[2] = {neighbour_rank, rand_val};
        MPI_Isend(buffer, 2, MPI_INT, neighbour_rank, 0, MPI_COMM_WORLD, &request[rank]);
        MPI_Irecv(grid[grid_rowno][grid_colno], 2, MPI_INT, neighbour_rank, 0, MPI_COMM_WORLD, &request[rank]);
      }
      /*bottom*/
      if (grid[grid_rowno+1][grid_colno][1]==0){
        int rand_val = rand()%5+1;
        int neighbour_rank = grid_colno + (no_col*grid_rowno+1);
        int buffer[2] = {neighbour_rank, rand_val};
        MPI_Isend(buffer, 2, MPI_INT, neighbour_rank, 0, MPI_COMM_WORLD, &request[rank]);
        MPI_Irecv(grid[grid_rowno][grid_colno], 2, MPI_INT, neighbour_rank, 0, MPI_COMM_WORLD, &request[rank]);
      }
      /*left*/
      if (grid[grid_rowno][grid_colno-1][1]==0){
        int rand_val = rand()%5+1;
        int neighbour_rank = grid_colno-1 + (no_col*grid_rowno);
        int buffer[2] = {neighbour_rank, rand_val};
        MPI_Isend(buffer, 2, MPI_INT, neighbour_rank, 0, MPI_COMM_WORLD, &request[rank]);
        MPI_Irecv(grid[grid_rowno][grid_colno], 2, MPI_INT, neighbour_rank, 0, MPI_COMM_WORLD, &request[rank]);
      }
      /*right*/
      if (grid[grid_rowno][grid_colno+1][1]==0){
        int rand_val = rand()%5+1;
        int neighbour_rank = grid_colno+1 + (no_col*grid_rowno);
        int buffer[2] = {neighbour_rank, rand_val};
        MPI_Isend(buffer, 2, MPI_INT, neighbour_rank, 0, MPI_COMM_WORLD, &request[rank]);
        MPI_Irecv(grid[grid_rowno][grid_colno], 2, MPI_INT, neighbour_rank, 0, MPI_COMM_WORLD, &request[rank]);
      }
    }
    MPI_Waitall(nreq, request, statuses);

    /*
    //Left boundary case
    if (grid_colno-1<0){
      //top left case
      if (grid_rowno-1<0){
        if (grid[grid_rowno][grid_colno+1][1]==-1){
          grid[grid_rowno][grid_colno+1][1] = rand()%5;
        }
        if (grid[grid_rowno+1][grid_colno][1]==-1){
          grid[grid_rowno+1][grid_colno][1] = rand()%5;
        }
      //bottom left case
      } else if (grid_rowno+1>no_row-1){
        if (grid[grid_rowno][grid_colno+1][1]==-1){
          grid[grid_rowno][grid_colno+1][1] = rand()%5;
        }
        if (grid[grid_rowno-1][grid_colno][1]==-1){
          grid[grid_rowno-1][grid_colno][1] = rand()%5;
        }
      //has at least 3 adjacent nodes
      } else {
        //top row, same col
        if (grid[grid_rowno-1][grid_colno][1]==-1){
          grid[grid_rowno-1][grid_colno][1] = rand()%5;
        }
        //bottom row, same col
        if (grid[grid_rowno+1][grid_colno][1]==-1){
          grid[grid_rowno+1][grid_colno][1] = rand()%5;
        }
        //same row, right col
        if (grid[grid_rowno][grid_colno+1][1]==-1){
          grid[grid_rowno][grid_colno+1][1] = rand()%5;
        }
      }
    //Right boundary case
    } else if (grid_colno+1>no_col-1){
      //top right case
      if (grid_rowno-1<0){
        if (grid[grid_rowno][grid_colno-1][1]==-1){
          grid[grid_rowno][grid_colno-1][1] = rand()%5;
        }
        if (grid[grid_rowno+1][grid_colno][1]==-1){
          grid[grid_rowno+1][grid_colno][1] = rand()%5;
        }
      //bottom right case
      } else if (grid_rowno+1>no_row-1){
        if (grid[grid_rowno][grid_colno-1][1]==-1){
          grid[grid_rowno][grid_colno-1][1] = rand()%5;
        }
        if (grid[grid_rowno-1][grid_colno][1]==-1){
          grid[grid_rowno-1][grid_colno][1] = rand()%5;
        }
      //has at least 3 adjacent nodes
      } else {
        //top row, same col
        if (grid[grid_rowno-1][grid_colno][1]==-1){
          grid[grid_rowno-1][grid_colno][1] = rand()%5;
        }
        //bottom row, same col
        if (grid[grid_rowno+1][grid_colno][1]==-1){
          grid[grid_rowno+1][grid_colno][1] = rand()%5;
        }
        //same row, left col
        if (grid[grid_rowno][grid_colno-1][1]==-1){
          grid[grid_rowno][grid_colno-1][1] = rand()%5;
        }
      }
    //Top row boundary case
    } else if (grid_rowno-1<0){
      //left col
      if (grid[grid_rowno][grid_colno-1][1]==-1){
        grid[grid_rowno][grid_colno-1][1] = rand()%5;
      }
      //right col
      if (grid[grid_rowno][grid_colno+1][1]==-1){
        grid[grid_rowno][grid_colno+1][1] = rand()%5;
      }
      //bottom row, same col
      if (grid[grid_rowno+1][grid_colno][1]==-1){
        grid[grid_rowno+1][grid_colno][1] = rand()%5;
      }
    //Bottom row boundary case
    } else if (grid_rowno+1>no_row-1){
      //left col
      if (grid[grid_rowno][grid_colno-1][1]==-1){
        grid[grid_rowno][grid_colno-1][1] = rand()%5;
      }
      //right col
      if (grid[grid_rowno][grid_colno+1][1]==-1){
        grid[grid_rowno][grid_colno+1][1] = rand()%5;
      }
      //top row, same col
      if (grid[grid_rowno-1][grid_colno][1]==-1){
        grid[grid_rowno-1][grid_colno][1] = rand()%5;
      }
    //has at least three adjacent nodes
    } else {
      //top
      if (grid[grid_rowno-1][grid_colno][1]==-1){
        grid[grid_rowno-1][grid_colno][1] = rand()%5;
      }
      //bottom
      if (grid[grid_rowno+1][grid_colno][1]==-1){
        grid[grid_rowno+1][grid_colno][1] = rand()%5;
      }
      //left
      if (grid[grid_rowno][grid_colno-1][1]==-1){
        grid[grid_rowno][grid_colno-1][1] = rand()%5;
      }
      //right
      if (grid[grid_rowno][grid_colno+1][1]==-1){
        grid[grid_rowno][grid_colno+1][1] = rand()%5;
      }
    }*/
  }
  MPI_Finalize();
  return 0;
  /*printf("%d random no: %d\n", grid[grid_rowno][grid_colno][0], grid[grid_rowno][grid_colno][1]);*/
}
