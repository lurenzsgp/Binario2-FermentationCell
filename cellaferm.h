#include <wiringPi.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include "rotaryEncoder.h"

#define frigo 28
#define serpentina 29

void initCell ();
int fermentazione (int load, int fd);
float getTemperature ();
void cooler ();
void heater();
float setCellTemperature (int load, int fd);
