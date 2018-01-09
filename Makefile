
all: startSystem

startSystem: startSystem2.o cellaferm.o rotaryEncoder.o
	gcc -o startSystem startSystem2.o cellaferm.o rotaryEncoder.o -lwiringPiDev -lwiringPi

startSystem2.o: startSystem2.c cellaferm.h
	gcc -c startSystem2.c

cellaferm.o: cellaferm.c cellaferm.h rotaryEncoder.h
	gcc -c cellaferm.c

rotatoryEncoder.o: rotatoryEncoder.c rotaryEncoder.h
	gcc -c rotatoryEncoder.c

clean:
	rm *.o
