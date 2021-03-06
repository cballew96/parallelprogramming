/* 
Gaussian elimination on a matrix using mpi
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
#define MAXN 4000  /* Max value of N */
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
  int elements_per_proc;
  double *sub_gauss;
  double *x_buffer, *output;
  

  
  
  MPI_Barrier(MPI_COMM_WORLD);  //barrier to let all processes catch up

  if(myrank==0) {
	startTime = MPI_Wtime();
	printf("Starting time for gaussian elimination\n");
  }
  //printf("My rank is: %d\n", myrank); //print ranks of processes
  //MPI_Bcast(&A[0],N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  //MPI_Bcast(&B[0], 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  
  elements_per_proc = N/numnodes;
  sub_gauss = (double *) malloc (elements_per_proc * N* sizeof(double));
  //working with mpi_scatter - processes get hung on gathering the data and never reach back substitution
  //MPI_Scatter(&A[norm], elements_per_proc, MPI_DOUBLE, sub_gauss, elements_per_proc, MPI_DOUBLE, 0, MPI_COMM_WORLD); //gives no errors
  //MPI_Scatter(&A[0], elements_per_proc * N, MPI_DOUBLE, sub_gauss, elements_per_proc * N, MPI_DOUBLE, 0, MPI_COMM_WORLD); 
   

  /* Gaussian elimination */
  for (norm = 0; norm < N - 1; norm++) {
  	//MPI_Scatter(&A[norm], elements_per_proc * N, MPI_DOUBLE, sub_gauss, elements_per_proc * N, MPI_DOUBLE, 0, MPI_COMM_WORLD); 
	//MPI_Scatter(&A[norm], elements_per_proc, MPI_DOUBLE, sub_gauss, elements_per_proc, MPI_DOUBLE, 0, MPI_COMM_WORLD); 
	MPI_Bcast(&A[norm][0], N, MPI_DOUBLE, 0, MPI_COMM_WORLD); //broadcast A and B matrices from master to all processes
	MPI_Bcast(&B[norm], 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    for (row = norm + 1; row < N; row++) {
      multiplier = A[row][norm] / A[norm][norm];
      for (col = norm; col < N; col++) {
	A[row][col] -= A[norm][col] * multiplier;
      }
      B[row] -= B[norm] * multiplier;
    }
  }
  
  printf("My rank is: %d \n", myrank);
  output = (double *) malloc (N * sizeof(double));
  //MPI_Gather(x_buffer, elements_per_proc, MPI_DOUBLE, output, elements_per_proc, MPI_DOUBLE, 0, MPI_COMM_WORLD);


  /* (Diagonal elements are not normalized to 1.  This is treated in back
   * substitution.)
   */

MPI_Barrier(MPI_COMM_WORLD); //wait for all processes to catch up






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
  if(myrank==0) { 
	endTime = MPI_Wtime();
	printf("Time is: %f\n", endTime-startTime);
  }
  
}
