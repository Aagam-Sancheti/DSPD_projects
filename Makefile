all: driver.o driver2.o
	gcc driver.o driver2.o

driver.o: driver.c
	gcc -c driver.c

driver2.o: driver2.c
	gcc -c driver2.c
