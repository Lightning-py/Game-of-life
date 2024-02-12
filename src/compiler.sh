#!bin/bash

gcc -Wall -Wextra -Werror -lncurses game_of_life.c -o game_of_life 
clang-format -i *.c *.h 

cppcheck --enable=all --suppress=missingIncludeSystem *.c *.h            
echo "all right, boss!"

