#!/bin/sh
#$ -V
#$ -cwd
#$ -S /bin/bash
#$ -N Cballew_collect_mpi
#$ -o $JOB_NAME.o$JOB_ID
#$ -e $JOB_NAME.e$JOB_ID
#$ -q omni
#$ -pe mpi 72
#$ -l h_vmem=5.3G
#$ -l h_rt=48:00:00
#$ -P quanah

module purge
module load intel openmpi 

echo "Testing gausscollect_mpi.exe -np 1 ..."
mpirun --machinefile machinefile.$JOB_ID -np 1 ./gausscollect_mpi.exe 2000 5
echo -e "###\n"
echo "Testing gausscollect_mpi.exe -np 2 ..."
mpirun --machinefile machinefile.$JOB_ID -np 2 ./gausscollect_mpi.exe 2000 5
echo -e "###\n"
echo "Testing gausscollect_mpi.exe -np 4 ..."
mpirun --machinefile machinefile.$JOB_ID -np 4 ./gausscollect_mpi.exe 2000 5
echo -e "###\n"
echo "Testing gausscollect_mpi.exe -np 8 ..."
mpirun --machinefile machinefile.$JOB_ID -np 8 ./gausscollect_mpi.exe 2000 5
echo -e "###\n"
echo "Testing gausscollect_mpi.exe -np 16 ..."
mpirun --machinefile machinefile.$JOB_ID -np 16 ./gausscollect_mpi.exe 2000 5
echo -e "###\n"
echo "Testing gausscollect_mpi.exe -np 32 ..."
mpirun --machinefile machinefile.$JOB_ID -np 32 ./gausscollect_mpi.exe 2000 5
echo -e "###\n"
echo "Testing gausscollect_mpi.exe -np 64 ..."
mpirun --machinefile machinefile.$JOB_ID -np 64 ./gausscollect_mpi.exe 2000 5
echo -e "###\n"
