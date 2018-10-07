#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <unistd.h>
#include "mpi.h"

#define no_row 4
#define no_col 5

int *countAdjacent(int arr[], int n, int maxVal, int rank, int rand_val){
  int count_arr[maxVal];
  int final_arr[6];
  memset(count_arr, 0, maxVal);

  //Count the number of occurences for each random number
  for (int i=0; i<n; i++){
    count_arr[arr[i]]++;
  }
  for (int j=0; j<n; j++){
    if (count_arr[arr[j]]>=3){
      return count_arr[arr[j]];
    }
  }
  return 0;
}

int main(argc, argv)
int argc;
char **argv;
{
  int rank, size, rowpos, colpos, nreq, i, neighbour_rank, rand_val;
  int top = 0;
  int down = 1;
  int left = 2;
  int right = 3;

  int counter = 0;
  int buffer;
  int grid[no_row][no_col][4];
  MPI_Request request[(no_row*no_col)*3];
  MPI_Status statuses[(no_row*no_col)*3];

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank!=20){
    srand(clock());
    rand_val = rand()%5+1;
    nreq = 0;
    double time_spent = 0;
    clock_t begin_time = clock();
    rowpos = rank/no_col;
    colpos = rank%no_col;
    memset(grid[rowpos][colpos], 0, sizeof(grid[rowpos][colpos]));
    //left
    if (colpos-1<0){
      //top left
      if (rowpos-1<0){
        neighbour_rank = colpos+1 + (no_col*rowpos);
        MPI_Isend(&rand_val, 1, MPI_INT, neighbour_rank, 0, MPI_COMM_WORLD, &request[nreq++]);
        counter++;


        neighbour_rank = colpos + (no_col*(rowpos+1));
        MPI_Isend(&rand_val, 1, MPI_INT, neighbour_rank, 0, MPI_COMM_WORLD, &request[nreq++]);
        counter++;


      //bottom left
      } else if (rowpos+1>no_row-1){
        neighbour_rank = colpos+1 + (no_col*rowpos);
        MPI_Isend(&rand_val, 1, MPI_INT, neighbour_rank, 0, MPI_COMM_WORLD, &request[nreq++]);
        counter++;


        neighbour_rank = colpos + (no_col*(rowpos-1));
        MPI_Isend(&rand_val, 1, MPI_INT, neighbour_rank, 0, MPI_COMM_WORLD, &request[nreq++]);
        counter++;


      //has 3 adjacent nodes
      } else {
        neighbour_rank = colpos+1 + (no_col*rowpos);
        MPI_Isend(&rand_val, 1, MPI_INT, neighbour_rank, 0, MPI_COMM_WORLD, &request[nreq++]);
        counter++;


        neighbour_rank = colpos + (no_col*(rowpos-1));
        MPI_Isend(&rand_val, 1, MPI_INT, neighbour_rank, 0, MPI_COMM_WORLD, &request[nreq++]);
        counter++;


        neighbour_rank = colpos + (no_col*(rowpos+1));
        MPI_Isend(&rand_val, 1, MPI_INT, neighbour_rank, 0, MPI_COMM_WORLD, &request[nreq++]);
        counter++;


      }
    //right
    } else if (colpos+1>no_col-1){
      //top right
      if (rowpos-1<0){
        neighbour_rank = colpos-1 + (no_col*rowpos);
        MPI_Isend(&rand_val, 1, MPI_INT, neighbour_rank, 0, MPI_COMM_WORLD, &request[nreq++]);
        counter++;


        neighbour_rank = colpos + (no_col*(rowpos+1));
        MPI_Isend(&rand_val, 1, MPI_INT, neighbour_rank, 0, MPI_COMM_WORLD, &request[nreq++]);
        counter++;


      //bottom right
      } else if (rowpos+1>no_row-1){
        neighbour_rank = colpos-1 + (no_col*rowpos);
        MPI_Isend(&rand_val, 1, MPI_INT, neighbour_rank, 0, MPI_COMM_WORLD, &request[nreq++]);
        counter++;


        neighbour_rank = colpos + (no_col*(rowpos-1));
        MPI_Isend(&rand_val, 1, MPI_INT, neighbour_rank, 0, MPI_COMM_WORLD, &request[nreq++]);
        counter++;


      //has 3 adjacent nodes
      } else {
        neighbour_rank = colpos-1 + (no_col*rowpos);
        MPI_Isend(&rand_val, 1, MPI_INT, neighbour_rank, 0, MPI_COMM_WORLD, &request[nreq++]);
        counter++;


        neighbour_rank = colpos + (no_col*(rowpos-1));
        MPI_Isend(&rand_val, 1, MPI_INT, neighbour_rank, 0, MPI_COMM_WORLD, &request[nreq++]);
        counter++;


        neighbour_rank = colpos + (no_col*(rowpos+1));
        MPI_Isend(&rand_val, 1, MPI_INT, neighbour_rank, 0, MPI_COMM_WORLD, &request[nreq++]);
        counter++;


      }
    //top row
    } else if (rowpos-1<0){
      neighbour_rank = colpos + (no_col*(rowpos+1));
      MPI_Isend(&rand_val, 1, MPI_INT, neighbour_rank, 0, MPI_COMM_WORLD, &request[nreq++]);
      counter++;


      neighbour_rank = colpos+1 + (no_col*rowpos);
      MPI_Isend(&rand_val, 1, MPI_INT, neighbour_rank, 0, MPI_COMM_WORLD, &request[nreq++]);
      counter++;


      neighbour_rank = colpos-1 + (no_col*rowpos);
      MPI_Isend(&rand_val, 1, MPI_INT, neighbour_rank, 0, MPI_COMM_WORLD, &request[nreq++]);
      counter++;


    //bottom row
    } else if (rowpos+1>no_row-1){
      neighbour_rank = colpos + (no_col*(rowpos-1));
      MPI_Isend(&rand_val, 1, MPI_INT, neighbour_rank, 0, MPI_COMM_WORLD, &request[nreq++]);
      counter++;


      neighbour_rank = colpos+1 + (no_col*rowpos);
      MPI_Isend(&rand_val, 1, MPI_INT, neighbour_rank, 0, MPI_COMM_WORLD, &request[nreq++]);
      counter++;


      neighbour_rank = colpos-1 + (no_col*rowpos);
      MPI_Isend(&rand_val, 1, MPI_INT, neighbour_rank, 0, MPI_COMM_WORLD, &request[nreq++]);
      counter++;


    //has 4 adjacent nodes
    } else {
      neighbour_rank = colpos+1 + (no_col*rowpos);
      MPI_Isend(&rand_val, 1, MPI_INT, neighbour_rank, 0, MPI_COMM_WORLD, &request[nreq++]);
      counter++;


      neighbour_rank = colpos-1 + (no_col*rowpos);
      MPI_Isend(&rand_val, 1, MPI_INT, neighbour_rank, 0, MPI_COMM_WORLD, &request[nreq++]);
      counter++;


      neighbour_rank = colpos + (no_col*(rowpos+1));
      MPI_Isend(&rand_val, 1, MPI_INT, neighbour_rank, 0, MPI_COMM_WORLD, &request[nreq++]);
      counter++;


      neighbour_rank = colpos + (no_col*(rowpos-1));
      MPI_Isend(&rand_val, 1, MPI_INT, neighbour_rank, 0, MPI_COMM_WORLD, &request[nreq++]);
      counter++;

    }
    MPI_Waitall(nreq, request, statuses);
    //Receive all adjacent random values
    if (counter>=3){
      printf("rank: %d  ", rank);
      rowpos = rank/no_col;
      colpos = rank%no_col;
      //When there are only three adjacent nodes
      if (counter==3){
        //top row case
        if (rowpos-1<0){
          MPI_Recv(&grid[rowpos][colpos][down], 1, MPI_INT, colpos+(no_col*(rowpos+1)), 0, MPI_COMM_WORLD, &statuses[nreq++]);
          MPI_Recv(&grid[rowpos][colpos][right], 1, MPI_INT, colpos+1+(no_col*rowpos), 0, MPI_COMM_WORLD, &statuses[nreq++]);
          MPI_Recv(&grid[rowpos][colpos][left], 1, MPI_INT, colpos-1+(no_col*rowpos), 0, MPI_COMM_WORLD, &statuses[nreq++]);
          clock_t end_time = clock();
          time_spent = (double)(end_time - begin_time) / CLOCKS_PER_SEC;
          countAdjacent(grid[rowpos][colpos], 4, 5);
          //printf("top row: (%d,%d,%d)   |   time: %f\n", grid[rowpos][colpos][left],grid[rowpos][colpos][right],grid[rowpos][colpos][down], time_spent);

        //bottom row case
        } else if (rowpos+1>no_row-1){
          MPI_Recv(&grid[rowpos][colpos][top], 1, MPI_INT, colpos+(no_col*(rowpos-1)), 0, MPI_COMM_WORLD, &statuses[nreq++]);
          MPI_Recv(&grid[rowpos][colpos][right], 1, MPI_INT, colpos+1+(no_col*rowpos), 0, MPI_COMM_WORLD, &statuses[nreq++]);
          MPI_Recv(&grid[rowpos][colpos][left] , 1, MPI_INT, colpos-1+(no_col*rowpos), 0, MPI_COMM_WORLD, &statuses[nreq++]);
          clock_t end_time = clock();
          time_spent = (double)(end_time - begin_time) / CLOCKS_PER_SEC;
          countAdjacent(grid[rowpos][colpos],4, 5);
          //printf("bottom row: (%d,%d,%d)    | time: %f\n", grid[rowpos][colpos][left],grid[rowpos][colpos][right],grid[rowpos][colpos][top], time_spent);

        //left case
        } else if (colpos-1<0){
          MPI_Recv(&grid[rowpos][colpos][right], 1, MPI_INT, colpos+1+(no_col*rowpos), 0, MPI_COMM_WORLD, &statuses[nreq++]);
          MPI_Recv(&grid[rowpos][colpos][top] , 1, MPI_INT, colpos+(no_col*(rowpos-1)), 0, MPI_COMM_WORLD, &statuses[nreq++]);
          MPI_Recv(&grid[rowpos][colpos][down], 1, MPI_INT, colpos+(no_col*(rowpos+1)), 0, MPI_COMM_WORLD, &statuses[nreq++]);
          clock_t end_time = clock();
          time_spent = (double)(end_time - begin_time) / CLOCKS_PER_SEC;
          countAdjacent(grid[rowpos][colpos],4, 5);
          //printf("left row: (%d,%d,%d)   |   time: %f\n", grid[rowpos][colpos][top],grid[rowpos][colpos][down],grid[rowpos][colpos][right], time_spent);

        //right case
        } else if (colpos+1>no_col-1){
          MPI_Recv(&grid[rowpos][colpos][left], 1, MPI_INT, colpos-1+(no_col*rowpos), 0, MPI_COMM_WORLD, &statuses[nreq++]);
          MPI_Recv(&grid[rowpos][colpos][top], 1, MPI_INT, colpos+(no_col*(rowpos-1)), 0, MPI_COMM_WORLD, &statuses[nreq++]);
          MPI_Recv(&grid[rowpos][colpos][down], 1, MPI_INT, colpos+(no_col*(rowpos+1)), 0, MPI_COMM_WORLD, &statuses[nreq++]);
          clock_t end_time = clock();
          time_spent = (double)(end_time - begin_time) / CLOCKS_PER_SEC;
          countAdjacent(grid[rowpos][colpos],4, 5);
          //printf("right row: (%d,%d,%d)   |   time: %f\n", grid[rowpos][colpos][top],grid[rowpos][colpos][down],grid[rowpos][colpos][left], time_spent);

        }
      //When there is more than three adjacent nodes, counter = 4
    } else if (counter>3){
        MPI_Recv(&grid[rowpos][colpos][right], 1, MPI_INT, colpos+1+(no_col*rowpos), 0, MPI_COMM_WORLD, &statuses[nreq++]);
        MPI_Recv(&grid[rowpos][colpos][left], 1, MPI_INT, colpos-1+(no_col*rowpos), 0, MPI_COMM_WORLD, &statuses[nreq++]);
        MPI_Recv(&grid[rowpos][colpos][down], 1, MPI_INT, colpos+(no_col*(rowpos+1)), 0, MPI_COMM_WORLD, &statuses[nreq++]);
        MPI_Recv(&grid[rowpos][colpos][top], 1, MPI_INT, colpos+(no_col*(rowpos-1)), 0, MPI_COMM_WORLD, &statuses[nreq++]);
        clock_t end_time = clock();
        time_spent = (double)(end_time - begin_time) / CLOCKS_PER_SEC;
        countAdjacent(grid[rowpos][colpos],4, 5);
        //printf("middle row: (%d,%d,%d,%d)   |   time: %f\n", grid[rowpos][colpos][left], grid[rowpos][colpos][right],grid[rowpos][colpos][top],grid[rowpos][colpos][down], time_spent);

      }
    //Nodes with lesser than 2 adjacent nodes receiving their adjacent random numbers.
    } else {
      rowpos = rank/no_col;
      colpos = rank%no_col;
      //top left
      if (rowpos-1<0 && colpos-1<0){
        //right
        MPI_Recv(&grid[rowpos][colpos][right], 1, MPI_INT, colpos+1+(no_col*rowpos), 0, MPI_COMM_WORLD, &statuses[nreq++]);
        //bottom
        MPI_Recv(&grid[rowpos][colpos][down], 1, MPI_INT, colpos+(no_col*(rowpos+1)), 0, MPI_COMM_WORLD, &statuses[nreq++]);

      //top right
      } else if (rowpos-1<0 && colpos+1>no_col-1){
        //left
        MPI_Recv(&grid[rowpos][colpos][left], 1, MPI_INT, colpos-1+(no_col*rowpos), 0, MPI_COMM_WORLD, &statuses[nreq++]);
        //bottom
        MPI_Recv(&grid[rowpos][colpos][down], 1, MPI_INT, colpos+(no_col*(rowpos+1)), 0, MPI_COMM_WORLD, &statuses[nreq++]);

      //bottom left
      } else if (rowpos+1>no_row-1 && colpos-1<0){
        //right
        MPI_Recv(&grid[rowpos][colpos][right], 1, MPI_INT, colpos+1+(no_col*rowpos), 0, MPI_COMM_WORLD, &statuses[nreq++]);
        //top
        MPI_Recv(&grid[rowpos][colpos][top], 1, MPI_INT, colpos+(no_col*(rowpos-1)), 0, MPI_COMM_WORLD, &statuses[nreq++]);

      //bottom right
      } else if (rowpos+1>no_row-1 && colpos+1>no_col-1){
        //left
        MPI_Recv(&grid[rowpos][colpos][left], 1, MPI_INT, colpos-1+(no_col*rowpos), 0, MPI_COMM_WORLD, &statuses[nreq++]);
        //top
        MPI_Recv(&grid[rowpos][colpos][top], 1, MPI_INT, colpos+(no_col*(rowpos-1)), 0, MPI_COMM_WORLD, &statuses[nreq++]);
      }
    }
  }
  MPI_Finalize();
  return 0;
}
