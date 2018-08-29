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
#include <pigpio.h>

#include "debug.h"
#include "gestioneIO.h"
#include "udp.h"
#include "serverTCP.h"
#include "version.h"
#include "distance.h"
#include "tempHumSensor.h"
#include "gestioneMotoriDC.h"
#include "gestioneMotoriStepper.h"
#include "udpServer.h"
#include "pin_raspberry.h"
#include "gyroAccelerometer.h"
#include "compass.h"

char debugSTR[256];


extern int i2cHandleMPU6050;

//--------------------------------------------------
// Function declaration
//--------------------------------------------------
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





	printf("---------------------------------\n");
	printf("---------  TestMyPI  ver: %s ------\n",VERSION);
	printf("---------------------------------\n\n");

	if (gpioInitialise() < 0)
	{
	   TRACE4(1,"MAIN",ROSSO,NERO_BG,"Setup pigpio failed ",0);
	   return -1;
	}
	else
	{
		 TRACE4(1,"MAIN",BIANCO,NERO_BG,"Setup pigpio OK ",0);
	}

	StartGestioneMotoriDC();
	StartGestioneMotoreStepper();

	//StartTCPServerManagement();
	StartUDPServerManagement();
	StartUDPClientManagement();


	StartTemperatureHumManagement();
	StartDistanceSonarManagement();
	StartGestioneGyroAccelerometer();
	StartGestioneCompass();

	while(1)
	{

		sleep(2);
	}

	return 0;
}

//--------------------------------------------------
// sig_handler
//--------------------------------------------------
void sig_handler(int signo)
{
	// printf("received %d\n",signo);
	 gpioPWM(PIN_MOTOR_0_PWM,0);
	 gpioPWM(PIN_MOTOR_1_PWM,0);
	 gpioTerminate();

	 if(i2cHandleMPU6050 > 0)
		 i2cClose(i2cHandleMPU6050);

	 exit(0);

}


