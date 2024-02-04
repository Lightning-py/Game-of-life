#!bin/bash

path=$1
gcc -Wall -Wextra -Werror -lncurses $path
clang-format -i $path
clang-format -n $path

cppcheck --enable=all --suppress=missingIncludeSystem $path            
echo "all right, boss!"

