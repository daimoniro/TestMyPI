/*
 * main.c
 *
 *  Created on: 14 Aug 2018
 *      Author: pi
 */


#include <stdio.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
//#include <wiringPi.h>
#include <pigpio.h>

#include "gestioneIO.h"
#include "udp.h"
#include "serverTCP.h"
#include "version.h"
#include "distance.h"
#include "tempHumSensor.h"
#include "gestioneMotoriDC.h"
#include "udpServer.h"
#include "pin_raspberry.h"
char debugSTR[256];

//--------------------------------------------------
// Function declaration
//--------------------------------------------------
void showMenu();
void switchMenu(char scelta);
void sig_handler(int signo);


//--------------------------------------------------
// main
//--------------------------------------------------
int main(int argc, char **argv)
{

	printf("\033[H\033[2J");

	signal(SIGINT, sig_handler);
	signal(SIGTERM, sig_handler);
	signal(SIGKILL, sig_handler);
	signal(2, sig_handler);


	/*if(wiringPiSetup() == -1)
	{
		//when initialize wiring failed,print messageto screen
			printf("setup wiringPi failed !");
			return 1;
	}*/



	printf("---------------------------------\n");
	printf("---------  TestMyPI  ver: %s ------\n",VERSION);
	printf("---------------------------------\n\n");

	if (gpioInitialise() < 0)
	{
	   printf("setup pigpio failed !\n");
	   return -1;
	}
	else
	{
	   printf("setup pigpio ok !\n");
	}

	StartGestioneMotoriDC();

	//StartTCPServerManagement();
	StartUDPServerManagement();
	StartUDPClientManagement();


	StartTemperatureHumManagement();
	StartDistanceSonarManagement();


	while(1)
	{
		/*showMenu();
		c = getchar();
		while(getchar() != '\n');
		switchMenu(c);*/

		sleep(2);


	}

	return 0;
}

//--------------------------------------------------
// sig_handler
//--------------------------------------------------
void sig_handler(int signo)
{
	 printf("received %d\n",signo);
	 gpioPWM(PIN_MOTOR_0_PWM,0);
	 gpioPWM(PIN_MOTOR_1_PWM,0);
	 gpioTerminate();

	 exit(0);

}

//--------------------------------------------------
// showMenu
//--------------------------------------------------
void showMenu()
{
		printf("a)....... Gestione I/O\n");
		printf("b)....... Distance\n");
		printf("c)....... Motori DC\n");
		printf("d)....... Motori Servo\n");

		printf("\nScelta menu': ");
}


//--------------------------------------------------
// switchMenu
//--------------------------------------------------
void switchMenu(char scelta)
{
	switch(scelta)
	{
		case 'a':
			gestioneIO();
			break;
		case 'b':
		//	gestioneDistance();
			break;
		case 'c':
			//gestioneMotoriDC();
			break;
		case 'd':
			//gestioneMotoriServo();
			break;
	}
}


