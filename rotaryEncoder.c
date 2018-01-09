#include "rotaryEncoder.h"

unsigned char flag;
unsigned char Last_RoB_Status;
unsigned char Current_RoB_Status;

int rotaryDeal(void)
{
	Last_RoB_Status = digitalRead(RoBPin);

	while(!digitalRead(RoAPin)){
		Current_RoB_Status = digitalRead(RoBPin);
		flag = 1;
	}

	if(flag == 1){
		flag = 0;
		if((Last_RoB_Status == 0)&&(Current_RoB_Status == 1)){
			return 1;
		}
		if((Last_RoB_Status == 1)&&(Current_RoB_Status == 0)){
			return -1;
        }
	}
}

int rotaryPress(void)
{
    return !digitalRead(RoSPin);	
}

void initRotatory ()
{
    pinMode(RoAPin, INPUT);
	pinMode(RoBPin, INPUT);
	pinMode(RoSPin, INPUT);

	pullUpDnControl(RoSPin, PUD_UP);
}

int actionSelector(int fd)
{
	int action = 1;
    lcdClear(fd);
    time_t start = time(NULL);
	
	while(1){
        if (time(NULL) - start < 300) {
            return 0;
        }

        printf("Press to select\n");
        lcdPosition(fd,0,0);
        lcdPuts(fd,"    Press to");
        switch (action)
        {
            case 1 :
                printf("Start Fermentation");
                lcdPosition(fd,0,1);
                lcdPuts(fd,"   Start Cell");
                break;
            case 2 :
                printf("Cold on");
                lcdPosition(fd,0,1);
                lcdPuts(fd,"  Start Fridge");
                break;
            case 3 :
                printf("Heat on");;
                lcdPosition(fd,0,1);
                lcdPuts(fd,"Start Serpentine");
                break;

        }
        if (rotaryPress())
            break;

		action += rotaryDeal();
        if (action < 1) {
            action = 1;
        } else if (action > 3) {
            action = 3;
        }
	}

	return action;
}

void setTemperature (int fd, float *t)
{
    printf("Press to confirm\n");
    lcdPosition(fd,0,0);
    lcdPuts(fd,"Press to confirm");
    while (1) {
        char buff[16];
        sprintf(buff, "Tmp   -->   %2.1f", *t);
        printf(buff);
        lcdPosition(fd,0,1);
        lcdPuts(fd,buff);
        if (rotaryPress())
            break;

        *t += rotaryDeal();

        if (*t > 30) {
            *t = 30;
        } else if (*t < -10) {
            *t = -10;
        }
    }
}
