
all: build

build: game_of_life.c
	clang-format -i *.c
	gcc -Wall -Wextra -Werror -lncurses game_of_life.c -o game_of_life