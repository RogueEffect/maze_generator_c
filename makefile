CC = gcc
FLAGS = -g -Wall -std=c99
FILES = main.c
EXE = maze

all:
	$(CC) $(FLAGS) $(FILES) -o $(EXE)
