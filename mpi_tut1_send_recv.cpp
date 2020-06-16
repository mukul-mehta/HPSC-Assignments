/*
A program that uses the standard mode send and receive of MPI Library

The program consists of two processes. Process 0 sends a large message
to the receiver. This receives the message and sends it back.

This program deadlocks if the the send and receive calls are in
the wrong order, i.e. if both proceesses first try to send, because
the message is large enough so that standard mode send does not use
buffered communication.

Compile the program with 'mpicxx -O3 mpi_tut1_send_recv.cpp -o mpi_tut1_send_recv'
Run it on two processes.
*/

#include<iostream>
#include<mpi.h>
using namespace std;

int main(int argc, char* argv[]) {
    const int K = 1024;
    const int msgsize = 256*K;  /* Messages will be of size 1 MB */
  
    int np, me, i;
    int *X, *Y;
    int tag = 42;
  
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &np);
    MPI_Comm_rank(MPI_COMM_WORLD, &me);

    /* Check that we run on exactly two processes */
    if (np != 2) {
      if (me == 0) {
        cout<<"Only 2 Processes are allowed!\n";
      }
      MPI_Finalize();	       /* Quit if there is only one process */
      return 0;
    }
  
    /* Allocate memory for large message buffers */
    X = (int *) malloc(msgsize*sizeof(int));
    Y = (int *) malloc(msgsize*sizeof(int));


    /* Initialize X and Y */
    for (i=0; i<msgsize; i++) {
      X[i] = 12345;
      Y[i] = me;
    }

    //if (me == 0) {

    cout<<"Message size is "<<msgsize*sizeof(int)<<" bytes"<<endl;
    cout<<"Process "<<me<<" sending to process 1\n"<<endl;
    
    MPI_Send(X, msgsize, MPI_INT, 1, tag, MPI_COMM_WORLD);   //send call a
    cout<<"Process "<<me<<" receiving from process 1"<<endl;
    MPI_Recv (Y, msgsize, MPI_INT, 1, tag, MPI_COMM_WORLD, &status);  //recv call b
    cout<<"Y now has the value "<<Y[0]<<endl;

  //} else { /* me == 1 */

    MPI_Recv (Y, msgsize, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);    //recv call a
    MPI_Send (Y, msgsize, MPI_INT, 0, tag, MPI_COMM_WORLD);     //send call b

 // }
    
    /*
    After removing the if-else condition the output was

    Message size is 1048576 bytes
    Process 0 sending to process 1
    Message size is 1048576 bytes
    Process 1 sending to process 1
    [hang/waiting]

    Explanation:
    The send and receive protocol used above is "blocking".
    Both Process 0 and Process 1 (running parallely) execute send call a
    Then Both process wait for send call b from the other processor, but before 
    the receiver sends the message back it has to receive first(recv call a) but 
    here recv call b is called after sender's receive call (recv call b).

    Thus we see that both process are waiting for their respective receivers
    to send back the message which will never happen because of the order conflict
    in the code.


    */

    MPI_Finalize();
    return 0;
}
