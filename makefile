# Note: For production and testing speed, DO NOT compile with GDB flags!
# Adapted from Jeff Martin's HW4 makefile

CC = gcc
MCC = mpicc
CFLAGSGDB = -ggdb -Wall -lm
CFLAGS = -Wall -lm
EXE = driver

all: driver parallel_driver svp_driver svp_driver_strong_scale

driver: driver.c
	$(CC) $(CFLAGSGDB) driver.c -o $(EXE)

parallel_driver: parallel_driver.c
	$(MCC) $(CFLAGSGDB) parallel_driver.c -o parallel_driver

svp_driver: svp_driver.c
	$(MCC) $(CFLAGSGDB) svp_driver.c -o svp_driver

svp_driver_strong_scale: svp_driver_strong_scale.c
	$(MCC) $(CFLAGSGDB) svp_driver_strong_scale.c -o svp_driver_strong_scale

clean:
	rm -f *.o *.gch driver parallel_driver svp_driver svp_driver_strong_scale

val:
	valgrind --track-origins=yes -s ./$(EXE)
