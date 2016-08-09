#include <wiringPi.h>
#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include "cellaferm.h"

#define LedPin 0
#define ButtonPin 1


int main  (void)
{
        if(wiringPiSetup() == -1) {
                printf("setup wiringPi failed !");
                return 1;
        }

        pinMode(LedPin, OUTPUT);
        pinMode(ButtonPin, INPUT);

        pullUpDnControl(ButtonPin, PUD_UP);  //pull up to 3.3V,make GPIO1 a stable level

        digitalWrite(LedPin, LOW); // led on Show sytem ready
        printf("StartSystem ready\n");

        while(1) {
		if(digitalRead(ButtonPin) == 0){ //indicate that button has pressed down
			delay(1500);
			digitalWrite(LedPin, HIGH); // led off cella running
			printf("cella avviata\n");
			printf("cella running\n");
			
			fermentazione();

			digitalWrite(LedPin, LOW); // led on system ready
			printf("cella stop\n");
			printf("system ready\n");
		}
        }
}
