
all: startSystem

startSystem: startSystem2.o cellaferm.o rotatoryEncoder.o
	gcc -o startSystem startSystem2.o cellaferm.o rotatoryEncoder.o -lwiringPi

startSystem2.o: startSystem2.c cellaferm.h
	gcc -c startSystem2.c

cellaferm.o: cellaferm.c cellaferm.h rotatoryEncoder.h
	gcc -c cellaferm.c

rotatoryEncoder.o: rotatoryEncoder.c rotatoryEncoder.h
	gcc -c rotatoryEncoder.c

clean:
	rm *.o
