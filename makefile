CC = gcc
CFLAGS = -Wall -g

all: main merging_avg sorting_avg 

clean:
	rm -f *.o main merging_avg sorting_avg 

main: main.c
	$(CC) $(CFLAGS) main.c -o main

merging_avg: merging_avg.c
	$(CC) $(CFLAGS) merging_avg.c -o merging_avg

sorting_avg: sorting_avg.c
	$(CC) $(CFLAGS) sorting_avg.c -o sorting_avg