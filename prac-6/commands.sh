ssh -i ~/.ssh/id_rsa edu-cmc-sqi21-13@polus.cs.msu.ru

cd task2

cd _scratch/research/task2

module load OpenMPI
module load SpectrumMPI


xlc -qsmp=omp omp_hello.c -o run 

scp -r edu-cmc-sqi21-13@polus.cs.msu.ru:~/task1/ ~/Projects/prac-CS-MSU/prac-6/task1/polus_out

# polus compile xlc/xlc++ http://hpc.cmc.msu.ru/node/241
# для этих задач надо выбирать потокобезопаснубю версию компилятора


clear; printf "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"; cat my_job.*.100000.*.out






module load slurm
module load openmpi
make compile

sbatch -n32 impi ./run 300

mpisubmit.pl -p 1 --stdout mpi-1-300.out --stderr mpi-1-300.err -w 00:10 mpiexec ./run 300