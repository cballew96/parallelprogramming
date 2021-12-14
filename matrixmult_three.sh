#$ -S /bin/bash
#$ -N Cballew_matrixmult_three_job
#$ -o $JOB_NAME.o$JOB_ID
#$ -e $JOB_NAME.e$JOB_ID
#$ -q omni
#$ -pe sm 36
#$ -l h_vmem=5.3G
#$ -l h_rt=48:00:00
#$ -P quanah

echo "Testing matrixmult_three.exe ..."
OMP_NUM_THREADS=1 ./matrixmult_three.exe
OMP_NUM_THREADS=2 ./matrixmult_three.exe
OMP_NUM_THREADS=4 ./matrixmult_three.exe
OMP_NUM_THREADS=8 ./matrixmult_three.exe
OMP_NUM_THREADS=16 ./matrixmult_three.exe
OMP_NUM_THREADS=32 ./matrixmult_three.exe
echo -e "###\n"
