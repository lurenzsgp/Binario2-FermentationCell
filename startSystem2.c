#include <wiringPi.h>
#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include "cellaferm.h"


int main  (void)
{
    int fd;
    if(wiringPiSetup() == -1) {
        printf("setup wiringPi failed !");
        return 1;
    }
    
    fd = lcdInit(2,16,4, 2,3, 6,5,4,1,0,0,0,0);
    if (fd == -1){
		printf("lcdInit 1 failed\n") ;
		return 1;
	}

    initRotatory(); 
    initCell();

    lcdClear(fd);
    // show system ready on display
    printf("StartSystem ready\n");

    // Interface loop
    while (1) 
    {
        //menu
        action = actionSelector(fd);
        lcdClear(fd);
        switch (action)
        {
            case 1 :
                printf("Start fermentation");
                lcdPosition(fd, 0, 0);
                lcdPuts(fd, "     Start");
                lcdPosition(fd,0,1);
                lcdPuts(fd, "  Fermentation");
                delay(3000);
                fermentazione(0);
                break;
            case 2 :
                printf("Cold on");
                lcdPosition(fd, 0, 0);
                lcdPuts(fd, "   Fridge ON");
                lcdPosition(fd,0,1);
                lcdPuts(fd, "Need a Cold one");
                delay(3000);
                cooler();
            case 3 :
                printf("Heat on");
                lcdPosition(fd, 0, 0);
                lcdPuts(fd, " Serpentine ON");
                lcdPosition(fd,0,1);
                lcdPuts(fd, " Need a hot one");
                delay(3000);
                heater();
                break;
            default :
                printf("Default case");
                // start the cell
                // case for unexpected reboot
                fermentazione(1);
        }
    }
}
