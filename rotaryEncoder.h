#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <time.h>

#define  RoAPin    0
#define  RoBPin    1
#define  RoSPin    2

void initRotatory ();
int rotatoryPress();
int actionSelector (int fd);
void setTemperature (int fd, float *t);
