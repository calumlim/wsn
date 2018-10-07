#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <unistd.h>
#include "mpi.h"

#define no_row 4
#define no_col 5
#define MSG_EXIT 1
#define MSG_SEND 2
#define MSG_TIME 3

int * countAdjacent(int arr[], int n, int maxVal, int rank, int rand_val){
  int count_arr[maxVal+1];
  static int final_arr[6];
  int found_number = -1;
  int neighbour_rank = -1;
  int rowpos = rank/no_col;
  int colpos = rank%no_col;
  memset(count_arr, 0, sizeof(count_arr));
  memset(final_arr, -1, sizeof(final_arr));

  //Count the number of occurences for each random number
  for (int i=0; i<n; i++){
    count_arr[arr[i]]++;
  }
  //Find the random number
  for (int j=0; j<n; j++){
    if (count_arr[arr[j]]>=3){
      found_number = arr[j];
    }
  }
  //If there are at least three nodes with the same random number
  if (found_number!=-1){
    for (int k=0; k<n; k++){
      if (k==0){
        if (rowpos-1>=0){
          neighbour_rank = colpos + (no_col*(rowpos-1));
          }
      } else if (k==1){
        if (rowpos+1<no_row){
          neighbour_rank = colpos + (no_col*(rowpos+1));
        }
      } else if (k==2){
        if (colpos-1>=0){
          neighbour_rank = colpos-1 + (no_col*(rowpos));
        }
      } else if (k==3){
        if (colpos+1<no_col){
          neighbour_rank = colpos+1 + (no_col*(rowpos));
        }
      }
      //Insert adjacent node's rank only if more than 0
      final_arr[k] = neighbour_rank;
      neighbour_rank = -1;
    }
    final_arr[4] = rank;
    final_arr[5] = rand_val;
    return final_arr;
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
  double start_time, end_time, time_spent;
  FILE *fp;
  fp = fopen("log.txt","w+");

  MPI_Request request[(no_row*no_col)*3];
  MPI_Status statuses[(no_row*no_col)*3];
  MPI_Comm new_comm;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_split(MPI_COMM_WORLD, rank==20, 0, &new_comm);
  start_time = MPI_Wtime();
  //This is the slave
  if (rank!=20){

    srand(clock());
    rand_val = rand()%5+1;
    nreq = 0;
    int buffer[1];
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
      MPI_Comm_rank(new_comm, &rank);
      rowpos = rank/no_col;
      colpos = rank%no_col;

      //When there are only three adjacent nodes
      if (counter==3){
        //top row case
        if (rowpos-1<0){
          MPI_Recv(&grid[rowpos][colpos][down], 1, MPI_INT, colpos+(no_col*(rowpos+1)), 0, MPI_COMM_WORLD, &statuses[nreq++]);
          MPI_Recv(&grid[rowpos][colpos][right], 1, MPI_INT, colpos+1+(no_col*rowpos), 0, MPI_COMM_WORLD, &statuses[nreq++]);
          MPI_Recv(&grid[rowpos][colpos][left], 1, MPI_INT, colpos-1+(no_col*rowpos), 0, MPI_COMM_WORLD, &statuses[nreq++]);
          end_time = MPI_Wtime();
          time_spent = end_time - start_time;
          int *adj_arr = countAdjacent(grid[rowpos][colpos], 4, 5, rank, rand_val);
          //Sending to the base station if 3 or more adjacent nodes are found
          if (adj_arr!=0){
            MPI_Send(adj_arr, 6, MPI_INT, 20, MSG_SEND, MPI_COMM_WORLD);
            MPI_Send(&time_spent, 1, MPI_DOUBLE, 20, MSG_TIME, MPI_COMM_WORLD);
          }

        //bottom row case
        } else if (rowpos+1>no_row-1){
          MPI_Recv(&grid[rowpos][colpos][top], 1, MPI_INT, colpos+(no_col*(rowpos-1)), 0, MPI_COMM_WORLD, &statuses[nreq++]);
          MPI_Recv(&grid[rowpos][colpos][right], 1, MPI_INT, colpos+1+(no_col*rowpos), 0, MPI_COMM_WORLD, &statuses[nreq++]);
          MPI_Recv(&grid[rowpos][colpos][left] , 1, MPI_INT, colpos-1+(no_col*rowpos), 0, MPI_COMM_WORLD, &statuses[nreq++]);
          end_time = MPI_Wtime();
          time_spent = end_time - start_time;
          int *adj_arr = countAdjacent(grid[rowpos][colpos],4, 5, rank, rand_val);
          //Sending to the base station if 3 or more adjacent nodes are found
          if (adj_arr!=0){
            MPI_Send(adj_arr, 6, MPI_INT, 20, MSG_SEND, MPI_COMM_WORLD);
            MPI_Send(&time_spent, 1, MPI_DOUBLE, 20, MSG_TIME, MPI_COMM_WORLD);
          }

        //left case
        } else if (colpos-1<0){
          MPI_Recv(&grid[rowpos][colpos][right], 1, MPI_INT, colpos+1+(no_col*rowpos), 0, MPI_COMM_WORLD, &statuses[nreq++]);
          MPI_Recv(&grid[rowpos][colpos][top] , 1, MPI_INT, colpos+(no_col*(rowpos-1)), 0, MPI_COMM_WORLD, &statuses[nreq++]);
          MPI_Recv(&grid[rowpos][colpos][down], 1, MPI_INT, colpos+(no_col*(rowpos+1)), 0, MPI_COMM_WORLD, &statuses[nreq++]);
          end_time = MPI_Wtime();
          time_spent = end_time - start_time;
          int *adj_arr = countAdjacent(grid[rowpos][colpos],4, 5, rank, rand_val);
          //Sending to the base station if 3 or more adjacent nodes are found
          if (adj_arr!=0){
            MPI_Send(adj_arr, 6, MPI_INT, 20, MSG_SEND, MPI_COMM_WORLD);
            MPI_Send(&time_spent, 1, MPI_DOUBLE, 20, MSG_TIME, MPI_COMM_WORLD);
          }

        //right case
        } else if (colpos+1>no_col-1){
          MPI_Recv(&grid[rowpos][colpos][left], 1, MPI_INT, colpos-1+(no_col*rowpos), 0, MPI_COMM_WORLD, &statuses[nreq++]);
          MPI_Recv(&grid[rowpos][colpos][top], 1, MPI_INT, colpos+(no_col*(rowpos-1)), 0, MPI_COMM_WORLD, &statuses[nreq++]);
          MPI_Recv(&grid[rowpos][colpos][down], 1, MPI_INT, colpos+(no_col*(rowpos+1)), 0, MPI_COMM_WORLD, &statuses[nreq++]);
          end_time = MPI_Wtime();
          time_spent = end_time - start_time;
          int *adj_arr = countAdjacent(grid[rowpos][colpos],4, 5, rank, rand_val);
          //Sending to the base station if 3 or more adjacent nodes are found
          if (adj_arr!=0){
            MPI_Send(adj_arr, 6, MPI_INT, 20, MSG_SEND, MPI_COMM_WORLD);
            MPI_Send(&time_spent, 1, MPI_DOUBLE, 20, MSG_TIME, MPI_COMM_WORLD);
          }

        }
    //When there is more than three adjacent nodes, counter = 4
    } else if (counter>3){
        MPI_Recv(&grid[rowpos][colpos][right], 1, MPI_INT, colpos+1+(no_col*rowpos), 0, MPI_COMM_WORLD, &statuses[nreq++]);
        MPI_Recv(&grid[rowpos][colpos][left], 1, MPI_INT, colpos-1+(no_col*rowpos), 0, MPI_COMM_WORLD, &statuses[nreq++]);
        MPI_Recv(&grid[rowpos][colpos][down], 1, MPI_INT, colpos+(no_col*(rowpos+1)), 0, MPI_COMM_WORLD, &statuses[nreq++]);
        MPI_Recv(&grid[rowpos][colpos][top], 1, MPI_INT, colpos+(no_col*(rowpos-1)), 0, MPI_COMM_WORLD, &statuses[nreq++]);
        end_time = MPI_Wtime();
        time_spent = end_time - start_time;
        int *adj_arr = countAdjacent(grid[rowpos][colpos],4, 5, rank, rand_val);
        //Sending to the base station if 3 or more adjacent nodes are found
        if (adj_arr!=0){
          MPI_Send(adj_arr, 6, MPI_INT, 20, MSG_SEND, MPI_COMM_WORLD);
          MPI_Send(&time_spent, 1, MPI_DOUBLE, 20, MSG_TIME, MPI_COMM_WORLD);
        }

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
    MPI_Send(buffer,0,MPI_INT, 20, MSG_EXIT, MPI_COMM_WORLD);
  //This is the master.
} else if (rank==20){
    double t_compute;
    int recv_arr[6], size, nslave, firstmsg;
    MPI_Status status;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    nslave = size -1;

    while (nslave>0){
      MPI_Recv(recv_arr, 6, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
      switch(status.MPI_TAG){
        case MSG_EXIT: nslave--;
        break;
        case MSG_SEND: fprintf(fp,"Rank: %d  Adjacent nodes: (%d,%d,%d,%d)\t with Random Number: %d\n", recv_arr[4],recv_arr[0],recv_arr[1],recv_arr[2],recv_arr[3],recv_arr[5]);
        printf("Rank: %d  Adjacent nodes: (%d,%d,%d,%d)\t with Random Number: %d\n", recv_arr[4],recv_arr[0],recv_arr[1],recv_arr[2],recv_arr[3],recv_arr[5]);
        //break;
        case MSG_TIME:
        firstmsg = status.MPI_SOURCE;
        printf("firstmsg: %d\n", firstmsg);
        MPI_Recv(&t_compute, 1, MPI_DOUBLE, recv_arr[4], MSG_TIME, MPI_COMM_WORLD, &status);
        printf("float: %f\n", t_compute);
        fprintf(fp,"Time spent: [ %f ]\n", t_compute);
        break;
      }
    }
  }
  fclose(fp);
  MPI_Finalize();
  sleep(1-time_spent);
  return 0;
}
