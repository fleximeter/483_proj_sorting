# Note: For production and testing speed, DO NOT compile with GDB flags!

CC = gcc
MCC = mpicc
CFLAGSGDB = -ggdb -Wall -lm
CFLAGS = -Wall -lm
EXE = driver

all: driver parallel_driver

driver: driver.c
	$(CC) $(CFLAGSGDB) driver.c -o $(EXE)

parallel_driver: parallel_driver.c
	$(MCC) $(CFLAGSGDB) parallel_driver.c -o parallel_driver

clean:
	rm -f *.o
	rm -f *.gch
	rm -f driver
	rm -f parallel_driver

val:
	valgrind --track-origins=yes -s ./$(EXE)
