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
#include "gestioneServo.h"

//--------------------------------------------------
// variabili globali
//--------------------------------------------------
char debugSTR[256];

//--------------------------------------------------
// variabili extern
//--------------------------------------------------
extern int i2cHandleMPU6050;
extern int i2cHandleHMC5883l;
extern int i2cHandle_pca6585;

//--------------------------------------------------
// Function declaration
//--------------------------------------------------
void sig_handler(int signo);
void closePigpioLybrary();

//--------------------------------------------------
// main
//--------------------------------------------------
int main(int argc, char **argv)
{

	printf("\033[H\033[2J");

	//signal(SIGINT, sig_handler);
	//gpioSetSignalFunc(SIGINT, sig_handler);
	//signal(SIGTERM, sig_handler);
//	signal(SIGKILL, sig_handler);
	//signal(SIGSEGV, sig_handler);





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

	//printf("Pigpio version: %d\n",gpioVersion());


	//signal(SIGINT, sig_handler);
	gpioSetSignalFunc(SIGINT, sig_handler);

	StartGestioneIO();
	StartGestioneMotoriDC();
	StartGestioneMotoreStepper();

	StartUDPServerManagement();
	StartUDPClientManagement();


	StartTemperatureHumManagement();
	StartDistanceSonarManagement();
	StartGestioneGyroAccelerometer();
	StartGestioneCompass();
	StartGestioneServo();


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
	 closePigpioLybrary();
	 exit(0);

}

//--------------------------------------------------
// closePigpioLybrary
//--------------------------------------------------
void closePigpioLybrary()
{
	gpioWrite(PIN_LED_VERDE, 0);   //led off
	gpioWrite(PIN_LED_ROSSO, 0);   //led off
	gpioPWM(PIN_MOTOR_0_PWM,0);
	gpioPWM(PIN_MOTOR_1_PWM,0);

	if(i2cHandleMPU6050 > 0)
	 i2cClose(i2cHandleMPU6050);

	if(i2cHandleHMC5883l > 0)
	 i2cClose(i2cHandleHMC5883l);

	if(i2cHandle_pca6585 > 0)
	 i2cClose(i2cHandle_pca6585);


	sleep(1);

	gpioTerminate();
}



