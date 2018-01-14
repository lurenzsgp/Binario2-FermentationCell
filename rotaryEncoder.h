#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <time.h>

#define  RoAPin    4
#define  RoBPin    5
#define  RoSPin    6

void initRotatory ();
int rotatoryPress();
int actionSelector (int fd);
void setTemperature (int fd, float *t);
