/* This file is only for reference. It cannot be compiled successfully, 
 * because m_set_procs(), m_get_numprocs() is not supported. Please 
 * write your own parallel version (Pthread, OpenMP, or MPI verion). For 
 * instance, you should use pthread_create() and pthread_join() to 
 * write a Pthread version, and use MPI initilization and communication
 * functions to write a MPI version.
 */

/* Demostration code - Gaussian elimination without pivoting.
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/types.h>
#include <sys/times.h>
#include <sys/time.h>
#include <limits.h>
#include <mpi.h>



/* Program Parameters */
#define MAXN 2000  /* Max value of N */
#define TAG 10
int N;  /* Matrix size */
int myrank, numnodes;
double startTime, endTime;

/* Matrices and vectors */
double A[MAXN][MAXN], B[MAXN], X[MAXN];
/* A * X = B, solve for X */
  


/* Prototype */
void gauss();  /* The function you will provide.
		* It is this routine that is timed.
		* It is called only on the parent.
		*/

/* returns a seed for srand based on the time */
unsigned int time_seed() {
  struct timeval t;
  struct timezone tzdummy;

  gettimeofday(&t, &tzdummy);
  return (unsigned int)(t.tv_usec);
}

/* Set the program parameters from the command-line arguments */
void parameters(int argc, char **argv) {
    int seed = 0;  /* Random seed */
  
  /* Read command-line arguments */
  srand(time_seed());  /* Randomize */
  
  if(argc != 3) {
	printf("Usage: <matrix_dimension> [random seed]\n");
	exit(0);
  }

  if(argc == 3) {
	N = atoi(argv[1]);
	seed = atoi(argv[2]);
        printf("\nMatrix dimensions = %i\nRandom seed = %i\n", N, seed);
	srand(seed);
  }
  if(N < 1 || N > MAXN) {
	printf("N = %i is out of range.\n" , N);
	exit(0);
  }

}

/* Initialize A and B (and X to 0.0s) */
void initialize_inputs() {
  int row, col;

  printf("\nInitializing...\n");
  
  for (col = 0; col < N; col++) {
    for (row = 0; row < N; row++) {
      A[row][col] = rand() / 32768.0;
    }
    B[col] = rand() / 32768.0;
    X[col] = 0.0;
    /*
    A[0][0] = 9.0;
    A[0][1] = 3.0;
    A[0][2] = 4.0;
    A[1][0] = 4.0;
    A[1][1] = 3.0;
    A[1][2] = 4.0;
    A[2][0] = 1.0;
    A[2][1] = 1.0;
    A[2][2] = 1.0;
    B[0] = 7.0;
    B[1] = 8.0;
    B[2] = 3.0;
    */
  }

}

/* Print input matrices */
void print_inputs() {
  int row, col;

  if (N < 10) {
    printf("\nA =\n\t");
    for (row = 0; row < N; row++) {
      for (col = 0; col < N; col++) {
	printf("%5.2f%s", A[row][col], (col < N-1) ? ", " : ";\n\t");
      }
    }
    printf("\nB = [");
    for (col = 0; col < N; col++) {
      printf("%5.2f%s", B[col], (col < N-1) ? "; " : "]\n");
    }
  }
}

void print_X() {
  int row;

  if (N < 10) {
    printf("\nX = [");
    for (row = 0; row < N; row++) {
      printf("%5.2f%s", X[row], (row < N-1) ? "; " : "]\n");
    }
  }
}

int main(int argc, char **argv) {
  
  MPI_Init(&argc,&argv);
  
  MPI_Comm_rank(MPI_COMM_WORLD,&myrank);
  MPI_Comm_size(MPI_COMM_WORLD,&numnodes);

  //printf("I am %d of %d\n", myrank, numnodes);


  /* Process program parameters */
  if(myrank==0) {
	parameters(argc, argv);
  }
  /* Initialize A and B */
  if(myrank==0) {
	initialize_inputs();
  }

  /* Print input matrices */
  if(myrank==0) {
	print_inputs();
  }
  
  /* Gaussian Elimination */
  gauss();

  
  /* Display output */
  if(myrank==0) {
	print_X();
  }
  

  MPI_Finalize();
}

/* ------------------ Above Was Provided --------------------- */

/****** You will replace this routine with your own parallel version *******/
/* Provided global variables are MAXN, N, procs, A[][], B[], and X[],
 * defined in the beginning of this code.  X[] is initialized to zeros.
 */
void gauss() {
  int norm, row, col;  /* Normalization row, and zeroing
  			* element row and col */
  double startTime, endTime;
  float multiplier;
  MPI_Request req;
  int i, n, numElements, offset, stripSize;
  
  
  MPI_Barrier(MPI_COMM_WORLD);  //barrier to let all processes catch up

  if(myrank==0) {		//start the clock with master process
	startTime = MPI_Wtime();
	printf("Starting time for gaussian elimination\n");
  }
  //printf("My rank is: %d\n", myrank); //print ranks of processes
  /*
  stripSize = N / numnodes;
  if(myrank==0) {
	offset = stripSize;
	numElements = stripSize * N;
	for(i=1; i<numnodes; i++) {
		MPI_Send(&A[offset], numElements, MPI_DOUBLE, i, TAG, MPI_COMM_WORLD);
		offset += stripSize;
	}
  } else if (myrank==1) {
	 MPI_Recv(&A[0], stripSize*N, MPI_DOUBLE, 0, TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	    printf("Process 1 received number %d from process 0\n", A[offset]);

  }
  
  */
  //MPI_Bcast(B[0], N*N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  /* Gaussian elimination */
stripSize = N / numnodes;
 
 for (norm = 0; norm<N-1; norm++) {
  if(myrank==0) {
	for (n=1; n<numnodes; n++) {
		for (i=n+norm+1; i<N; i+=numnodes) {		 		
			MPI_Isend(&A[i], stripSize, MPI_DOUBLE, n, TAG, MPI_COMM_WORLD, &req); //sending part of matrix A from process 0
			MPI_Wait(&req, MPI_STATUS_IGNORE);			       //to destination n (rest of processes)
			MPI_Isend(&B[i], 1, MPI_DOUBLE, n, TAG, MPI_COMM_WORLD, &req); //sending matrix B from process 0
			MPI_Wait(&req, MPI_STATUS_IGNORE);			       //to destination n (rest of processes)
		}
	}
  }
  else {    /*gaussian elimination*/
	for(row=norm+1+myrank; row<N; row+=numnodes) {
		MPI_Recv(&A[row], stripSize, MPI_DOUBLE, 0, TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE); //each process receives the message from process 0 - matrix A
		MPI_Recv(&B[row], 1, MPI_DOUBLE, 0, TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE); //each process receives the message from process 0 - matrix B	
		multiplier = A[row][norm] / A[norm][norm];
		for(col=norm; col<N; col++) {
			A[i][col] -= A[norm][col] * multiplier;
		}
		B[i] -= B[norm] * multiplier;
  	}
  }
 }
 /* 
  for (norm = 0; norm < N-1; norm++) {
    for (row = norm + 1; row < N; row++) {
      multiplier = A[row][norm] / A[norm][norm];
      for (col = norm; col < N; col++) {
	A[row][col] -= A[norm][col] * multiplier;
      }
      B[row] -= B[norm] * multiplier;
    }
  }
 */



  /*(Diagonal elements are not normalized to 1.  This is treated in back
   * substitution.)
   */

MPI_Barrier(MPI_COMM_WORLD);		//letting processes catch up before back substitution






  /* Back substitution */
 if(myrank==0) {
  for (row = N - 1; row >= 0; row--) {
    X[row] = B[row];
    for (col = N-1; col > row; col--) {
      X[row] -= A[row][col] * X[col];
    }
    X[row] /= A[row][row];
  }
 } 
  if(myrank==0) { 		//stop the clock with master process
	endTime = MPI_Wtime();
	printf("Time is: %f\n", endTime-startTime);
  }
  
}
