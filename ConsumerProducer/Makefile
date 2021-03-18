CC = gcc
LIBS = -pthread -lncurses -lm
FILES = $(wildcard *.c) $(wildcard src/*.c)
FLAGS = -O3
OUTPUT = consumer_producer

all:
	$(CC) $(FILES) -o $(OUTPUT) $(FLAGS) $(LIBS)