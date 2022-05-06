#!/bin/bash

# rm ./lsf/OMP_job_*

for matrix_size in 300 1000 3000 6000
do
    for omp_num_threads in 1 2 4 8 16 32
    do  
        LSF_filename="./lsf/OMP_job_"$matrix_size"_"$omp_num_threads".lsf"
        touch $LSF_filename

        omp_num_cores=$(((omp_num_threads/8)+1))
        echo "#BSUB -n $omp_num_cores" >> $LSF_filename

        echo "#BSUB -W 00:15" >> $LSF_filename
        echo "#BSUB -o \"my_job.%J.$matrix_size."$omp_num_threads".out\"" >> $LSF_filename
        echo "#BSUB -e \"my_job.%J.$matrix_size."$omp_num_threads".err\"" >> $LSF_filename
        echo "#BSUB -R \"span[hosts=1]\"" >> $LSF_filename

        echo "OMP_NUM_THREADS=$omp_num_threads" >> $LSF_filename
        echo "./parallel_algorithm $matrix_size $omp_num_threads slae_$matrix_size" >> $LSF_filename
        echo "bsub < $LSF_filename"
    done
done
