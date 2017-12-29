#include <wiringPi.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>

#define frigo 28
#define serpentina 29

#define TMAX 13000
#define TMIN 11000
#define TMP 12000
#define button 1

int fermentazione ();
float getTemperature ();
