CC = gcc
CFLAGS = -Wall -g

all: main

clean:
	rm -f *.o main 

main: main.c
	$(CC) $(CFLAGS) main.c -o main