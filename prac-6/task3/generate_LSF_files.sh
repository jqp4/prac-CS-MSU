#!/bin/bash

# $1 - omp_num_cores
# $2 - omp_num_threads 
# $3 - size

lll() {
    LSF_filename="./lsf/OMP_job_"$3"_c"$1"_t"$2".lsf"
    touch $LSF_filename

    echo "#BSUB -W 00:15" >> $LSF_filename
    echo "#BSUB -o \"my_job.%J.$3.c$1.t"$2".out\"" >> $LSF_filename
    echo "#BSUB -e \"my_job.%J.$3.c$1.t"$2".err\"" >> $LSF_filename
    echo "#BSUB -R \"affinity[core($1)]\"" >> $LSF_filename

    echo "OMP_NUM_THREADS=$2" >> $LSF_filename
    echo "/polusfs/lsf/openmp/launchOpenMP.py ./main $3 $2 " >> $LSF_filename
    echo "\tbsub < $LSF_filename"
}



rm ./lsf/OMP_job_*

for size in 10000 100000 1000000
do
    # for omp_num_threads in 1 2 4 8 16 32
    # do  
    #     omp_num_cores=$(((omp_num_threads/8)+2))
    #     lll $omp_num_cores $omp_num_threads $size
    # done

    # lll 1 1 $size
    lll 2 1 $size
    lll 2 2 $size
    lll 2 4 $size
    lll 4 4 $size
    lll 4 8 $size
    lll 8 8 $size
    lll 8 16 $size
    lll 16 16 $size
done
