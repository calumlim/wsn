//Name  : Calum Lim Sheng En
//ID    : 27372537
//Title : FIT3143 - Assignment 2: Part A
//Date  : 08/10/2018

/**
OpenMPI must be installed before being able to run this program!
To run this program:
MacOS:
1) mpicc -o wsn wsn.c
2) mpirun --mca shmem posix --oversubscribe -np 21 wsn
Linux:
1) mpicc -o wsn wsn.c
2) mpirun -np 21 wsn
===============================================================
The program will produce a log file containing all relavant
information in the directory that this program is in.

!IMPORTANT: The adjacent node section within the log file
can be read as such: (top, down, left, right), and the integer
-1 can be represented as a non-existent node.
===============================================================
**/

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
#define MSG_COUNT 4

//function to count the number of adjacent nodes with the same random value, returns an array
//of the neighbour nodes, the reference node, as well as the random value.
//If the neighbour node is out of bounds, it is denoted by the integer of -1
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

//Main driver function to run the MPI program
int main(argc, argv)
int argc;
char **argv;
{
  //Initialization of local variables
  int rank, size, rowpos, colpos, nreq, i, neighbour_rank, rand_val;
  int top = 0;
  int down = 1;
  int left = 2;
  int right = 3;

  int no_of_event = 0;
  int counter = 0;
  int grid[no_row][no_col][4];
  double start_time, end_time, time_spent;

  //open file for writing
  FILE *fp;
  fp = fopen("log.txt","w+");

  //Initialization of MPI variables
  MPI_Request request[(no_row*no_col)*3];
  MPI_Status statuses[(no_row*no_col)*3];
  MPI_Comm new_comm;

  //Initialization of MPI Commands
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_split(MPI_COMM_WORLD, rank==20, 0, &new_comm);

  //A while loop to continiously loop through each rank 1000 times
  int iterate_count = 1000;
  while (iterate_count>0){

  //This is the slave function, if the rank is not 20: base station
  if (rank!=20){
    start_time = MPI_Wtime(); //Record the start time
    srand(clock());
    rand_val = rand()%5+1;
    nreq = 0;
    int buffer[1];
    rowpos = rank/no_col;
    colpos = rank%no_col;
    memset(grid[rowpos][colpos], 0, sizeof(grid[rowpos][colpos]));

    //Using non-blocking send to send the random numbers to its adjacent nodes while considering the boundaries.
    //left boundary case
    if (colpos-1<0){
      //top left boundary case
      if (rowpos-1<0){
        neighbour_rank = colpos+1 + (no_col*rowpos);
        MPI_Isend(&rand_val, 1, MPI_INT, neighbour_rank, 0, MPI_COMM_WORLD, &request[nreq++]);
        counter++;


        neighbour_rank = colpos + (no_col*(rowpos+1));
        MPI_Isend(&rand_val, 1, MPI_INT, neighbour_rank, 0, MPI_COMM_WORLD, &request[nreq++]);
        counter++;


      //bottom left boundary case
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
    //right boundary case
    } else if (colpos+1>no_col-1){
      //top right boundary case
      if (rowpos-1<0){
        neighbour_rank = colpos-1 + (no_col*rowpos);
        MPI_Isend(&rand_val, 1, MPI_INT, neighbour_rank, 0, MPI_COMM_WORLD, &request[nreq++]);
        counter++;


        neighbour_rank = colpos + (no_col*(rowpos+1));
        MPI_Isend(&rand_val, 1, MPI_INT, neighbour_rank, 0, MPI_COMM_WORLD, &request[nreq++]);
        counter++;


      //bottom right boundary case
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
    //top row boundary case
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


    //bottom row boundary case
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

    //Receive all adjacent random values if there are more than 2 random values sent
    if (counter>=3){

      MPI_Comm_rank(new_comm, &rank);
      //Computing the 2D positions for the row and column in the grid
      rowpos = rank/no_col;
      colpos = rank%no_col;

      //When there are only three adjacent nodes
      if (counter==3){
        //top row case, receiving the adjacent random values
        if (rowpos-1<0){
          MPI_Recv(&grid[rowpos][colpos][down], 1, MPI_INT, colpos+(no_col*(rowpos+1)), 0, MPI_COMM_WORLD, &statuses[nreq++]);
          MPI_Recv(&grid[rowpos][colpos][right], 1, MPI_INT, colpos+1+(no_col*rowpos), 0, MPI_COMM_WORLD, &statuses[nreq++]);
          MPI_Recv(&grid[rowpos][colpos][left], 1, MPI_INT, colpos-1+(no_col*rowpos), 0, MPI_COMM_WORLD, &statuses[nreq++]);
          end_time = MPI_Wtime(); //Record end time and compute the difference
          time_spent = end_time - start_time;
          int *adj_arr = countAdjacent(grid[rowpos][colpos], 4, 5, rank, rand_val);
          //Sending to the base station if 3 or more adjacent nodes are found
          if (adj_arr!=0){
            MPI_Send(adj_arr, 6, MPI_INT, 20, MSG_SEND, MPI_COMM_WORLD);
            MPI_Send(&time_spent, 1, MPI_DOUBLE, 20, MSG_TIME, MPI_COMM_WORLD);
            MPI_Send(&nreq, 1, MPI_INT, 20, MSG_COUNT, MPI_COMM_WORLD);
          }

        //bottom row case receiving the adjacent random values
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
            MPI_Send(&nreq, 1, MPI_INT, 20, MSG_COUNT, MPI_COMM_WORLD);
          }

        //left case receiving the adjacent random values
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
            MPI_Send(&nreq, 1, MPI_INT, 20, MSG_COUNT, MPI_COMM_WORLD);
          }

        //right case receiving the adjacent random values
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
            MPI_Send(&nreq, 1, MPI_INT, 20, MSG_COUNT, MPI_COMM_WORLD);
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
          MPI_Send(&nreq, 1, MPI_INT, 20, MSG_COUNT, MPI_COMM_WORLD);
        }

      }
    //Nodes with lesser than 2 adjacent nodes receiving their adjacent random numbers, this is to prevent the program from being stuck.
    } else {
      rowpos = rank/no_col;
      colpos = rank%no_col;
      //top left case
      if (rowpos-1<0 && colpos-1<0){
        //right
        MPI_Recv(&grid[rowpos][colpos][right], 1, MPI_INT, colpos+1+(no_col*rowpos), 0, MPI_COMM_WORLD, &statuses[nreq++]);
        //bottom
        MPI_Recv(&grid[rowpos][colpos][down], 1, MPI_INT, colpos+(no_col*(rowpos+1)), 0, MPI_COMM_WORLD, &statuses[nreq++]);

      //top right case
      } else if (rowpos-1<0 && colpos+1>no_col-1){
        //left
        MPI_Recv(&grid[rowpos][colpos][left], 1, MPI_INT, colpos-1+(no_col*rowpos), 0, MPI_COMM_WORLD, &statuses[nreq++]);
        //bottom
        MPI_Recv(&grid[rowpos][colpos][down], 1, MPI_INT, colpos+(no_col*(rowpos+1)), 0, MPI_COMM_WORLD, &statuses[nreq++]);

      //bottom left case
      } else if (rowpos+1>no_row-1 && colpos-1<0){
        //right
        MPI_Recv(&grid[rowpos][colpos][right], 1, MPI_INT, colpos+1+(no_col*rowpos), 0, MPI_COMM_WORLD, &statuses[nreq++]);
        //top
        MPI_Recv(&grid[rowpos][colpos][top], 1, MPI_INT, colpos+(no_col*(rowpos-1)), 0, MPI_COMM_WORLD, &statuses[nreq++]);

      //bottom right case
      } else if (rowpos+1>no_row-1 && colpos+1>no_col-1){
        //left
        MPI_Recv(&grid[rowpos][colpos][left], 1, MPI_INT, colpos-1+(no_col*rowpos), 0, MPI_COMM_WORLD, &statuses[nreq++]);
        //top
        MPI_Recv(&grid[rowpos][colpos][top], 1, MPI_INT, colpos+(no_col*(rowpos-1)), 0, MPI_COMM_WORLD, &statuses[nreq++]);
      }
    }
    MPI_Send(buffer,0,MPI_INT, 20, MSG_EXIT, MPI_COMM_WORLD);

  //This is the master, which is also rank 20, which also denotes the base station.
} else if (rank==20){
    double t_compute;
    int recv_arr[6], size, nslave, no_msg;
    MPI_Status status;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    nslave = size -1;

    //Getting the messages from any source if available
    while (nslave>0){
      MPI_Recv(recv_arr, 6, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
      //Case Basis
      switch(status.MPI_TAG){
        case MSG_EXIT: nslave--;
        break;
        case MSG_SEND:
        //Writing the information of the reference node, adjacent nodes, rank, and random number to a txt file.
        fprintf(fp,"==========================================================\n");
        printf("==========================================================\n");
        fprintf(fp,"Reference Node: %d  \nAdjacent Nodes: (%d,%d,%d,%d) \nRandom Number: %d\n", recv_arr[4],recv_arr[0],recv_arr[1],recv_arr[2],recv_arr[3],recv_arr[5]);
        printf("Reference Node: %d  \nAdjacent Nodes: (%d,%d,%d,%d) \nRandom Number: %d\n", recv_arr[4],recv_arr[0],recv_arr[1],recv_arr[2],recv_arr[3],recv_arr[5]);

        //Receive the time stamp of when the event occured
        MPI_Recv(&t_compute, 1, MPI_DOUBLE, recv_arr[4], MSG_TIME, MPI_COMM_WORLD, &status);
        printf("Time Spent: [ %fs ]\n", t_compute);
        fprintf(fp,"Time Spent: [ %fs ]\n", t_compute);

        //Receive the number of requests which can be used as the number of messages exchanged and the number of nodes which activated the event
        MPI_Recv(&no_msg, 1, MPI_INT, recv_arr[4], MSG_COUNT, MPI_COMM_WORLD, &status);
        printf("Messages Exchanged: %d\n", (no_msg+6)/2);
        fprintf(fp,"Messages Exchanged: %d\n", (no_msg+6)/2);
        printf("No. of Nodes which triggered the event: %d\n", no_msg/2);
        fprintf(fp,"No. of Nodes which triggered the event: %d\n", no_msg/2);

        //Increment the number of events by 1
        no_of_event++;
        fprintf(fp,"Total no. of events that have occurred: %d\n\n", no_of_event);
        printf("Total no. of events that have occurred: %d\n\n", no_of_event);
        break;
      }
    }
  }
  iterate_count--;
  counter = 0;
  neighbour_rank = 0;
  sleep(0.99-time_spent);
  }
  fclose(fp);
  MPI_Finalize();
  return 0;
}
