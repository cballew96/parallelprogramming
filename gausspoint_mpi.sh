#!/bin/sh
#$ -V
#$ -cwd
#$ -S /bin/bash
#$ -N Cballew_point_mpi
#$ -o $JOB_NAME.o$JOB_ID
#$ -e $JOB_NAME.e$JOB_ID
#$ -q omni
#$ -pe mpi 72
#$ -l h_vmem=5.3G
#$ -l h_rt=48:00:00
#$ -P finals

module purge
module load intel openmpi 

echo "Testing gausspoint_mpi.exe -np 1 ..."
mpirun --machinefile machinefile.$JOB_ID -np 1 ./gausspoint_mpi.exe 2000 5
echo -e "###\n"
echo "Testing gausspoint_mpi.exe -np 4 ..."
mpirun --machinefile machinefile.$JOB_ID -np 4 ./gausspoint_mpi.exe 2000 5
echo -e "###\n"
echo "Testing gausspoint_mpi.exe -np 8 ..."
mpirun --machinefile machinefile.$JOB_ID -np 8 ./gausspoint_mpi.exe 2000 5
echo -e "###\n"
echo "Testing gausspoint_mpi.exe -np 16 ..."
mpirun --machinefile machinefile.$JOB_ID -np 16 ./gausspoint_mpi.exe 2000 5
echo -e "###\n"
echo "Testing gausspoint_mpi.exe -np 32 ..."
mpirun --machinefile machinefile.$JOB_ID -np 32 ./gausspoint_mpi.exe 2000 5
echo -e "###\n"
echo "Testing gausspoint_mpi.exe -np 64 ..."
mpirun --machinefile machinefile.$JOB_ID -np 64 ./gausspoint_mpi.exe 2000 5
echo -e "###\n"
