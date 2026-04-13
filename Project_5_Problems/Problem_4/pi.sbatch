#!/bin/bash
#SBATCH --partition=oucspdn_cpu
#SBATCH --ntasks-per-node=16
#SBATCH --ntasks=16
#
#SBATCH --time=00:15:00
#
# --------------------- Change the items below here! ---------------------
#
#SBATCH --mail-user=Edward.J.Wages-1@ou.edu
#SBATCH --mail-type=ALL
#
#SBATCH --job-name=hw5prob4
#
#SBATCH --chdir=/home/oucspdn103/Wages_Edward_Project_5/Problem_4
#SBATCH --output=/home/oucspdn103/Wages_Edward_Project_5/Problem_4/_P5-4_%J_stdout.txt
#SBATCH --error=/home/oucspdn103/Wages_Edward_Project_5/Problem_4/_P5-4_%J_stderr.txt
#
#################################################

echo "Working directory:"
pwd
echo ""

module load intel/2022.2
module load OpenMPI/4.1.4-GCC-11.3.0

make clean
make

mpirun -n 2 ./pi_MPI result_2p.csv time_2p.csv
mpirun -n 4 ./pi_MPI result_4p.csv time_4p.csv
mpirun -n 8 ./pi_MPI result_8p.csv time_8p.csv
mpirun -n 16 ./pi_MPI result_16p.csv time_16p.csv
# Add more here