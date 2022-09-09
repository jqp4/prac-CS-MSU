#!/bin/bash

rm ./lsf/task2job_*

LSF_section="003"

for matrix_size in 300 1000 3000
do
    for num_threads in 1 2 4 8
    do  
        LSF_filename="./lsf/task2_job"$LSF_section"_n"$matrix_size"_p"$num_threads".lsf"
        touch $LSF_filename

        echo "#BSUB -n $num_threads -q normal" >> $LSF_filename
        echo "#BSUB -W 00:59" >> $LSF_filename
        echo "#BSUB -o \"my_job.%J.$matrix_size."$num_threads".out\"" >> $LSF_filename
        echo "#BSUB -e \"my_job.%J.$matrix_size."$num_threads".err\"" >> $LSF_filename
        
        echo "mpiexec ./run $matrix_size" >> $LSF_filename
        echo "\tbsub < $LSF_filename"
    done
done
