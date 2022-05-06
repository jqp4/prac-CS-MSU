ssh -i ~/.ssh/id_rsa edu-cmc-sqi21-13@polus.cs.msu.ru


module load OpenMPI


xlc -qsmp=omp omp_hello.c -o run 
