# Note: For production and testing speed, DO NOT compile with GDB flags!
# Adapted from Jeff Martin's HW4 makefile

CC = gcc
MCC = mpicc
CFLAGSGDB = -ggdb -Wall -lm
CFLAGS = -Wall -lm
EXE = driver

all: driver parallel_driver svp_driver

driver: driver.c
	$(CC) $(CFLAGSGDB) driver.c -o $(EXE)

parallel_driver: parallel_driver.c
	$(MCC) $(CFLAGSGDB) parallel_driver.c -o parallel_driver

svp_driver: svp_driver.c
	$(MCC) $(CFLAGSGDB) svp_driver.c -o svp_driver

clean:
	rm -f *.o
	rm -f *.gch
	rm -f driver
	rm -f parallel_driver
	rm -f svp_driver

val:
	valgrind --track-origins=yes -s ./$(EXE)
