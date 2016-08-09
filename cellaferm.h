#include <wiringPi.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>

#define frigo 28
#define serpentina 29

#define TMAX 19500
#define TMIN 16500
#define TMP 18000
#define button 1

int fermentazione ();
float getTemperature ();
