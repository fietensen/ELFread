CC=gcc
CFLAGS=-O3 -Wall
PROJECT=ELFread
FILES=./main.c ./elf.c

all:
	$(CC) $(CFLAGS) $(FILES) -o $(PROJECT)

clean:
	rm $(PROJECT)
