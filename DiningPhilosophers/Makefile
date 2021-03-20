CC = gcc
LIBS = -pthread -lncurses -lm
FILES = $(wildcard *.c) $(wildcard src/*.c)
FLAGS = -O3
OUTPUT = dining_philosophers

all:
	$(CC) $(FILES) -o $(OUTPUT) $(FLAGS) $(LIBS)

clean:
	rm $(OUTPUT)