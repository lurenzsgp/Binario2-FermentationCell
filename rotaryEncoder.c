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
    printf("Rotatory initialized!\n");
    pinMode(RoAPin, INPUT);
	pinMode(RoBPin, INPUT);
	pinMode(RoSPin, INPUT);

	pullUpDnControl(RoSPin, PUD_UP);
}

int actionSelector(int fd)
{
	int action = 1;
    int ferm, cold, heat;
    lcdClear(fd);
    time_t start = time(NULL);

    ferm = cold = heat = 0;

    printf("Press to select\n");
    lcdPosition(fd,0,0);
    lcdPuts(fd,"    Press to");

	while(1){
        if (time(NULL) - start > 300) {
            printf("Time out\n");
            return 0;
        }

        switch (action)
        {
            case 1 :
                if (!ferm)
                {
                    printf("Start Fermentation\n");
                    lcdPosition(fd,0,1);
                    lcdPuts(fd,"   Start Cell");
                    ferm = 1;
                    cold = heat = 0;
                }
                break;
            case 2 :
                if (!cold)
                {
                    printf("Cold on\n");
                    lcdPosition(fd,0,1);
                    lcdPuts(fd,"  Start Fridge");
                    cold = 1;
                    ferm = heat = 0;
                }
                break;
            case 3 :
                if (!heat)
                {
                    printf("Heat on\n");;
                    lcdPosition(fd,0,1);
                    lcdPuts(fd,"Start Serpentine");
                    heat = 1;
                    cold = ferm = 0;
                }
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

    printf("Action Selected!\n");
	return action;
}

void setTemperature (int fd, float *t)
{
    printf("Set temperature\nPress to confirm\n");
    lcdPosition(fd,0,0);
    lcdPuts(fd,"Press to confirm");
    while (1) {
        char buff[16];
        sprintf(buff, "Tmp   -->   %2.1f\n", *t);
        lcdPosition(fd,0,1);
        lcdPuts(fd,buff);
        if (rotaryPress())
            return;

        *t += rotaryDeal();

        if (*t > 30) {
            *t = 30;
        } else if (*t < -10) {
            *t = -10;
        }
    }
}
