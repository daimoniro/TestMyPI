/*
 * gestioneIO.c
 *
 *  Created on: 14 Aug 2018
 *      Author: pi
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pigpio.h>

#include "debug.h"
#include "pin_raspberry.h"


//--------------------------------------------------
// variabili extern
//--------------------------------------------------
extern char debugSTR[];



//--------------------------------------------------
// Function declaration
//--------------------------------------------------
void *gestioneIO();



//-----------------------------------------------------------------------------
//	StartGestioneIO
//-----------------------------------------------------------------------------
void StartGestioneIO()
{
    // Demone Configurazione Board
    pthread_t ThCapture;

    pthread_create(&ThCapture, NULL, &gestioneIO, NULL);

}



//--------------------------------------------------
// gestioneIO
//--------------------------------------------------
void *gestioneIO()
{

	gpioSetMode(PIN_LED_VERDE, PI_OUTPUT);
	gpioSetMode(PIN_LED_ROSSO, PI_OUTPUT);
	gpioSetMode(BUTTON_PIN, PI_INPUT);

	gpioSetPullUpDown(BUTTON_PIN, PI_PUD_UP);   // Sets a pull-up.

	gpioWrite(PIN_LED_VERDE, 1);   //led on
	gpioWrite(PIN_LED_ROSSO, 0);   //led on

	while(1)
	{
		usleep(100000);


		sprintf(debugSTR,"Stato PIN %d --> %d\n", BUTTON_PIN,gpioRead(BUTTON_PIN));
		TRACE4(1,"IO",BIANCO,NERO_BG,debugSTR,0);
	}
}



