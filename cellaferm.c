#include "cellaferm.h"

void initCell ()
{
    pinMode(frigo, OUTPUT);
    pinMode(serpentina, OUTPUT);
}

float getTemperature ()
{
    float t;
    char buf[256];     // Data from device
    char tmpData[6];   // Temp C * 1000 reported by device
    ssize_t numRead;
    int fd = open("/sys/bus/w1/devices/w1_bus_master1/28-0315921cdbff/w1_slave", O_RDONLY);
    memset(tmpData, 0, 6);
    if (fd == -1)
    {
        perror ("Couldn't open the w1 device.");
        return 18000;
    }

    while ((numRead = read(fd, buf, 256)) > 0)
    {
        strncpy(tmpData, strstr(buf, "t=") + 2, 5);
        // printf("Temp: %s\n", tmpData);
        t = atoi(tmpData);
        //printf("Temp: %f C \t", t);
    }
    close(fd);
    return t;
}

float setCellTemperature (int load, int fd)
{
    float tmp = 18.;
    if (load) {
        char buf[256];     // Data from device
        char tmpString[6];   // Temp C * 1000 reported by device
        ssize_t nread=0;
        FILE *tmpFile = fopen("./temperature.txt", "r");
        if (tmpFile == NULL)
        {
            perror ("Couldn't open the temperature backup file.");
            return tmp;
        }

        while ((nread = fread(buf + nread, 1, sizeof(buf) - nread , tmpFile)) > 0);
        strncpy(tmpString, strstr(buf, "T=") + 2, 5);
        // printf("Temp: %s\n", tmpString);
        tmp = atoi(tmpString)/1000;
        fclose(tmpFile);
    } else {
        FILE *tmpFile = fopen("./temperature.txt", "w");
        if (tmpFile == NULL) {
            perror ("Couldn't store the temperature");
            return tmp;
        }
        tmp = setTemperature (fd);
        fprintf(tmpFile, "T=%d\n", (int)tmp * 1000);
        fclose(tmpFile);
        printf("Temperature stored\n");
        delay(500);
    }
    return tmp;
}

int fermentazione (int load, int fd)
{
    float t;
    int fOn = 0;	// indica se il frigo e' acceso o spento
    int sOn = 0;	// indica se la serpentina e' accesa o spenta
    float tmp;

    digitalWrite(frigo, HIGH);	// frigo off
    digitalWrite(serpentina, HIGH);	// serpentina off

    tmp = setCellTemperature(load, fd);

    printf("Cell running\n");

    while(1) {
        if(rotaryPress()){
            // button pressed
            // stop the execution
            digitalWrite(frigo, HIGH);      // frigo off
            digitalWrite(serpentina, HIGH); // serpentina off
            return;
        }

        t = getTemperature()/1000;
        printf("Tmperature: %2.2f\n", t);
        // display on screen
        lcdPosition(fd,0,1);
        char buff[16];
        // "T  -> [current tmp]/ ([setted tmp])
        sprintf(buff, "T  -> %2.1f/[%2.1f]\n", t, tmp);
        

        lcdClear(fd);
        lcdPosition(fd,0,0);
        lcdPuts(fd,"  CELL RUNNING");
        lcdPuts(fd,buff);
        

        if( t >  tmp + 1 && fOn == 0 ) {
            digitalWrite(frigo, LOW);	// frigo on
            printf("frigo on \n");
            fOn = 1;
        }

        if( fOn == 1 && t < tmp ) {
            digitalWrite(frigo, HIGH);	// frigo off
            printf("frigo off\n");
            fOn = 0;
        }

        if( t < tmp - 1 && sOn == 0 ) {
            digitalWrite(serpentina, LOW);	// serpentina on
            printf("serpentina on\n");
            sOn = 1;
        }

        if( sOn == 1 && t > tmp ) {
            digitalWrite(serpentina, HIGH);	// serpentina off
            printf("serpentina off\n");
            sOn = 0;
        }
        delay(2200);
    }
}

void cooler (int fd)
{
    digitalWrite(frigo, LOW);	// frigo ON

    printf("Fridge ON");
    // print on display
    lcdClear(fd);
    lcdPosition(fd,0,0);
    lcdPuts(fd,"   FRIDGE ON");
    // "Fridge ON"
    lcdPosition(fd,0,1);
    lcdPuts(fd,"  Press to stop");

    // wait until the button will be pressed
    while (!rotaryPress());

    digitalWrite(frigo, HIGH);	// frigo OFF
}


void heater (int fd)
{
    digitalWrite(serpentina, LOW);	// serpentina ON

    printf("Serpentine ON");
    // print on display
    lcdClear(fd);
    lcdPosition(fd,0,0);
    lcdPuts(fd,"  SERPENTINE ON");
    // "Serpentine ON"
    lcdPosition(fd,0,1);
    lcdPuts(fd,"  Press to stop");

    // wait until the button will be pressed
    while (!rotaryPress());

    digitalWrite(serpentina, HIGH);	// serpentina OFF

}
