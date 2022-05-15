ssh -i ~/.ssh/id_rsa edu-cmc-sqi21-13@polus.cs.msu.ru

cd task1
module load OpenMPI

xlc -qsmp=omp omp_hello.c -o run 

scp -r edu-cmc-sqi21-13@polus.cs.msu.ru:~/task1/ ~/Projects/prac-CS-MSU/prac-6/task1/polus_out