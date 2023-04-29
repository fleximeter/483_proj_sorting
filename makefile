CC = gcc
MCC = mpicc
CPP = g++
CFLAGSGDB = -ggdb -Wall -lm
CFLAGS = -Wall -lm
EXE = driver

all: driver

driver: driver.c
	$(CC) $(CFLAGSGDB) driver.c -o $(EXE)

clean:
	rm -f *.o
	rm -f *.gch
	rm -f driver

val:
	valgrind --track-origins=yes -s ./$(EXE)
