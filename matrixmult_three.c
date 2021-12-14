// matrix multiplication with all 3 for loops parallelized
#include <stdlib.h>
#include <omp.h>
#include <sys/time.h>
#include <stdio.h>

//max size of matrix
#define DIM 1000

int i, j, k, x, z;
int a[DIM][DIM], b[DIM][DIM], c[DIM][DIM];


int main(int argc, char **argv)
{

	double time_start, time_end;
	struct timeval tv;
	struct timezone tz;

	//test data used to make sure the calculations were correct - 3x3 matrix
	/*
	a[0][0] = 2;
	a[0][1] = 3;
	a[0][2] = 2;
	a[1][0] = 4;
	a[1][1] = 1;
	a[1][2] = 0;
	a[2][0] = 1;
	a[2][1] = 2;
	a[2][2] = 3;

	b[0][0] = 2;
	b[0][1] = 1;
	b[0][2] = 1;
	b[1][0] = 4;
	b[1][1] = 0;
	b[1][2] = 8;
	b[2][0] = 5;
	b[2][1] = 2;
	b[2][2] = 1;
	*/

	gettimeofday(&tv, &tz);
	time_start = ((double)tv.tv_sec  + (double)tv.tv_usec / 1000000.0);
	printf("Number of threads outside parallel region= %d\n",omp_get_num_threads()); //number of threads outside parallel
	//omp_set_num_threads(32);
	#pragma omp parallel shared(a,b,c) private(i,j,k)
	{	
		//#pragma omp for schedule(static)
		printf("Number of threads inside parallel region= %d\n",omp_get_num_threads()); //number of thread in parallel
		#pragma omp for collapse(3)
		for (i = 0; i < DIM; i++) {
			//printf("Number of threads= %d\n",omp_get_max_threads());
			for (j = 0; j < DIM; j++) {
				for (k = 0; k < DIM; k++) {
					c[i][j] += a[i][k] * b[k][j];

				}
			}			
		}
	}
	gettimeofday(&tv, &tz);
	time_end = ((double)tv.tv_sec  + (double)tv.tv_usec / 1000000.0);
	printf("Runtime = %f seconds\n", (time_end - time_start));

	/*for loop used to test the test data and make sure the final matrix is correct
	for(x=0;x<DIM;x++) {
		for(z=0;z<DIM;z++) {
	printf("%d ", c[x][z]);
		}
	}
	printf("\n");
	*/

} 
