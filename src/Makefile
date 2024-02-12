
all: build

build: game_of_life.c
	clang-format -i *.c
	gcc -Wall -Wextra -Werror -D_XOPEN_SOURCE=600 -lncursesw game_of_life.c -o game_of_life
