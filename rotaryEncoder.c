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
	int action = 300;
    int ferm, cold, heat;
    time_t start = time(NULL);

    ferm = cold = heat = 0;

    // lcdClear(fd);
    // lcdPosition(fd,0,0);
    // lcdPuts(fd,"    Press to");

	while(1){
        if (time(NULL) - start > 300) {
            printf("Time out\n");
            return -1;
        }

        switch (action)
        {
            case 0 :
                if (!ferm)
                {
                    lcdClear(fd);
                    lcdPosition(fd,0,0);
                    lcdPuts(fd,"    Press to");
                    lcdPosition(fd,0,1);
                    lcdPuts(fd,"   Start Cell");
                    ferm = 1;
                    cold = heat = 0;
                }
                break;
            case 1 :
                if (!cold)
                {
                    lcdClear(fd);
                    lcdPosition(fd,0,0);
                    lcdPuts(fd,"    Press to");
                    lcdPosition(fd,0,1);
                    lcdPuts(fd,"  Start Fridge");
                    cold = 1;
                    ferm = heat = 0;
                }
                break;
            case 2 :
                if (!heat)
                {
                    lcdClear(fd);
                    lcdPosition(fd,0,0);
                    lcdPuts(fd,"    Press to");
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
        action = action % 3;
        if (action < 0)
            action += 3;
    }

	return action;
}

float  setTemperature (int fd)
{
    float t = 18.;
    printf("Set temperature\nPress to confirm\n");

    
    while (1) {
        char buff[16];
        sprintf(buff, "Tmp   -->   %2.1f\n", t);

        lcdClear(fd);
        lcdPosition(fd,0,0);
        lcdPuts(fd,"Press to confirm");
        lcdPosition(fd,0,1);
        lcdPuts(fd,buff);
        
        if (rotaryPress())
            return t; 

        t += rotaryDeal();

        if (t > 30) {
            t = 30;
        } else if (t < -10) {
            t = -10;
        }
    }
}
