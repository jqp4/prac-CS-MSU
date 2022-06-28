c_files=~/Projects/prac-CS-MSU/prac-6/task2/*.c

# pragma format
# https://stackoverflow.com/questions/31353748/how-could-i-indent-c-pragma-using-clang-format
# https://github.com/MedicineYeh/p-clang-format

# Replace "#pragma omp" by "//#pragma omp"
sed -i '' 's/#pragma omp/\/\/#pragma omp/g' $c_files
# Do format
clang-format -i -style='{BasedOnStyle: LLVM, IndentWidth: 2, ColumnLimit: 200, AllowShortIfStatementsOnASingleLine: AllIfsAndElse}' $c_files
# Replace "// *#pragma omp" by "#pragma omp"
sed -i '' 's/\/\/ *#pragma omp/#pragma omp/g' $c_files


# transport all files
# scp -r  ~/Projects/prac-CS-MSU/prac-6/task1 edu-cmc-sqi21-13@polus.cs.msu.ru:~/
