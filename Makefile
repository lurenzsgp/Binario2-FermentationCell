
all: startSystem

startSystem: startSystem2.o cellaferm.o 
	gcc -o startSystem startSystem2.o cellaferm.o -lwiringPi

startSystem2.o: startSystem2.c cellaferm.h
	gcc -c startSystem2.c

cellaferm.o: cellaferm.c cellaferm.h
	gcc -c cellaferm.c
