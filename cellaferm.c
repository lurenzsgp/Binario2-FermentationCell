#include "cellaferm.h"

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

int fermentazione ()
{
	float t;
	int fOn = 0;	// indica se il frigo e' acceso o spento
	int sOn = 0;	// indica se la serpentina e' accesa o spenta

	pinMode(frigo, OUTPUT);
	pinMode(serpentina, OUTPUT);
	pinMode(button, INPUT);

        pullUpDnControl(button, PUD_UP);  //pull up to 3.3V,make GPIO1 a stable level

	digitalWrite(frigo, HIGH);	// frigo off
	digitalWrite(serpentina, HIGH);	// serpentina off
	
	while(1) {
		if(digitalRead(button) == 0){ //indicate that button has pressed down
			digitalWrite(frigo, HIGH);      // frigo off
			digitalWrite(serpentina, HIGH); // serpentina off
			delay(1500);
			//exit(0);
			return;
		}	

		t = getTemperature();
		printf("%f\n", t);
	
		if( t > TMAX && fOn == 0 ) {
			digitalWrite(frigo, LOW);	// frigo on
			printf("frigo on \n");
			fOn = 1;
		}
		
		if( fOn == 1 && t < TMP ) {
			digitalWrite(frigo, HIGH);	// frigo off
			printf("frigo off\n");
			fOn = 0;
		}

		if( t < TMIN && sOn == 0 ) {
			digitalWrite(serpentina, LOW);	// serpentina on
			printf("serpentina on\n");
			sOn = 1;
		}

		if( sOn == 1 && t > TMP ) {
			digitalWrite(serpentina, HIGH);	// serpentina off
			printf("serpentina off\n");
			sOn = 0;
		}
		delay(1000);
	}
}
