/*
 * distance.c
 *
 *  Created on: 16 Aug 2018
 *      Author: pi
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pigpio.h>

#include "pin_raspberry.h"

float distanceSonar_0 = 0;
float distanceSonar_1 = 0;

void *gestioneDistance();
void getDistance(unsigned char idSonarSendor);


//-----------------------------------------------------------------------------
//	StartTemperatureHumManagement
//-----------------------------------------------------------------------------
void StartDistanceSonarManagement()
{
    // Demone Configurazione Board
    pthread_t ThCapture;

    pthread_create(&ThCapture, NULL, &gestioneDistance, NULL);

}

void *gestioneDistance()
{

	printf("sonki 1");
	gpioSetMode(TRIG_0, PI_OUTPUT);
	gpioSetMode(ECHO_0, PI_INPUT);

	gpioSetMode(TRIG_1, PI_OUTPUT);
	gpioSetMode(ECHO_1, PI_INPUT);
	printf("sonki 2");

	// turn it off
	gpioWrite(TRIG_0, 0);
	gpioWrite(TRIG_1, 0);
    printf("Waiting For Sensor To Settle\n");
    sleep(2);



    while (1)
    {
    	getDistance(0);
    	  usleep(100000);
    	getDistance(1);

        usleep(100000);
    }

    return 0;
}


void getDistance(unsigned char idSonarSendor)
{

   struct timespec start, stop;
   double pulse_duration;
   double distance;


   unsigned char localTrig = 0 ;
   unsigned char localEcho = 0 ;


   switch(idSonarSendor)
   {
   	   case 0:
   		   localTrig = TRIG_0 ;
   		   localEcho = ECHO_0 ;
   		   break;

   	   case 1:
   		   localTrig = TRIG_1 ;
   		   	localEcho = ECHO_1 ;
   	   		break;

   	   default:
   		   printf("Errore idSensor sbagliato!!!\n");
   		   return;
   		   break;
   }



  gpioWrite(localTrig, 1);
  gpioDelay(10);
  gpioWrite(localTrig, 0);


  while(gpioRead(localEcho) == 0)
	  clock_gettime(CLOCK_REALTIME, &start);

  while(gpioRead(localEcho) == 1)
	  clock_gettime( CLOCK_REALTIME, &stop);

  stop.tv_nsec += (stop.tv_sec - start.tv_sec)*1000000000;

  pulse_duration = ( stop.tv_nsec - start.tv_nsec );

  //il pulse_duration è in ns quindi divido per 1000000 per arrivare ai ms
  distance = pulse_duration/1000000 *(345/2)/10;
 // printf("Sensor #%d --> pulse_duration: %f ms --> distance: %3.1f cm\n",idSonarSendor,pulse_duration/1000000,distance);

	switch(idSonarSendor)
	{
		case 0:
			distanceSonar_0 = distance;
			break;

		case 1:
			distanceSonar_1 = distance;
   	   		break;
   }

}
