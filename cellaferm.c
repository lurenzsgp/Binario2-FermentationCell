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
        return 1;
    }

    while ((numRead = read(fd, buf, 256)) > 0)
    {
        strncpy(tmpData, strstr(buf, "t=") + 2, 5);
        printf("Temp: %s\n", tmpData);
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
        ssize_t numRead;
        int tmpFile = open("temperature", O_RDONLY);
        if (tmpFile == -1)
        {
            perror ("Couldn't open the temperature backup file.");
            return tmp;
        }

        while ((numRead = read(tmpFile, buf, 256)) > 0)
        {
            strncpy(tmpString, strstr(buf, "T=") + 2, 5);
            printf("Temp: %s\n", tmpString);
            tmp = atoi(tmpString)/1000;
        }
        close(tmpFile);
    } else {
        setTemperature (fd, &tmp);
        int tmpFile = open("temperature", O_WRONLY);
        char buff[256];
        memset(buff, 0, 256);
        sprintf(buff, "T=%d", tmp * 1000);
        write(tmpFile, buff, sizeof(buff));
        close(tmpFile);
    }
    return tmp;
}

int fermentazione (int load, int fd)
{
    float t;
    int fOn = 0;	// indica se il frigo e' acceso o spento
    int sOn = 0;	// indica se la serpentina e' accesa o spenta
    float tmp;

    pinMode(frigo, OUTPUT);
    pinMode(serpentina, OUTPUT);

    digitalWrite(frigo, HIGH);	// frigo off
    digitalWrite(serpentina, HIGH);	// serpentina off

    tmp = setCellTemperature(load, fd);

    printf("Cell running");
    // display on screen
    lcdPosition(fd,0,0);
    lcdPuts(fd,"  CELL RUNNING");

    while(1) {
        if(rotaryPress()){ 
            // button pressed 
            // stop the execution
            digitalWrite(frigo, HIGH);      // frigo off
            digitalWrite(serpentina, HIGH); // serpentina off
            return;
        }	

        t = getTemperature()/1000;
        printf("%f\n", t);
        // display on screen
        lcdPosition(fd,0,1);
        char buff[16];
        sprintf(buff, "Tmp -> %2.1f/[%2.1f]", t, tmp);
        lcdPuts(fd,buff);
        // "Tmp [current tmp] ([setted tmp])

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
        delay(1000);
    }
}

void cooler (int fd)
{
    digitalWrite(frigo, LOW);	// frigo ON

    printf("Fridge ON");
    // print on display
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
    lcdPosition(fd,0,0);
    lcdPuts(fd,"  SERPENTINE ON");
    // "Serpentine ON"
    lcdPosition(fd,0,1);
    lcdPuts(fd,"  Press to stop");

    // wait until the button will be pressed
    while (!rotaryPress());

    digitalWrite(serpentina, HIGH);	// serpentina OFF

}
