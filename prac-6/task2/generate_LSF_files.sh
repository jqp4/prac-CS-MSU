#!/bin/bash

rm ./lsf/task_2_job_*

for matrix_size in 300 1000
do
    for num_threads in 1 2 4 8
    do  
        LSF_filename="./lsf/task_2_job_"$matrix_size"_"$num_threads".lsf"
        touch $LSF_filename

        # num_cores=$(((num_threads/8)+1))
        echo "#BSUB -n $num_threads -q short" >> $LSF_filename

        echo "#BSUB -W 00:15" >> $LSF_filename
        echo "#BSUB -o \"my_job.%J.$matrix_size."$num_threads".out\"" >> $LSF_filename
        echo "#BSUB -e \"my_job.%J.$matrix_size."$num_threads".err\"" >> $LSF_filename
        # echo "#BSUB -R \"span[hosts=1]\"" >> $LSF_filename
        # echo "#BSUB -R \"affinity[core($num_cores)]\"" >> $LSF_filename
        # echo "OMP_NUM_THREADS=$num_threads" >> $LSF_filename

        # echo "./parallel_algorithm $matrix_size $num_threads slae_$matrix_size" >> $LSF_filename
        # echo "mpirun ./parallel_algorithm 700 slae"
        echo "mpiexec ./main $matrix_size" >> $LSF_filename
        echo "\tbsub < $LSF_filename"
    done
done
