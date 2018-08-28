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

#include "pin_raspberry.h"

//--------------------------------------------------
// Function declaration
//--------------------------------------------------
void showMenuIO();
void switchMenuGestioneIO(char scelta);
int scriviPIN();
int leggiStatoPIN();
int followPIN();
int blinkPIN();
int breathingLed();

//--------------------------------------------------
// gestioneIO
//--------------------------------------------------
void gestioneIO()
{
	showMenuIO();
}


//--------------------------------------------------
// showMenuIO
//--------------------------------------------------
void showMenuIO()
{
		char c;
		system("clear");

		//pullUpDnControl (0,PUD_DOWN ) ;

		printf("---------------------------------\n");
		printf("-----------  Gestione IO --------\n");
		printf("---------------------------------\n\n");

		printf("a)....... Write PIN\n");
		printf("b)....... Read PIN\n");
		printf("c)....... Follow PIN\n");
		printf("d)....... Blink PIN\n");
		//printf("e)....... Breathing LED\n");


		printf("\nScelta menu' Gestione IO: ");
		c = getchar();
		while(getchar() != '\n');

		switchMenuGestioneIO(c);


}

//--------------------------------------------------
// switchMenuGestioneIO
//--------------------------------------------------
void switchMenuGestioneIO(char scelta)
{
	switch(scelta)
	{
		case 'a':
			scriviPIN();
			break;
		case 'b':
			leggiStatoPIN();
			break;
		case 'c':
			followPIN();
			break;
		case 'd':
			blinkPIN();
			break;
		/*case 'e':
			breathingLed();
			break;*/
	}


}

//--------------------------------------------------
// scriviPIN
//--------------------------------------------------
int scriviPIN()
{

	int num = 0;
	char c = 0;
	char buf[8];

	int STATE_DEFAULT = 0;
	int state = 0;

	memset(buf,0,sizeof(buf));

	gpioSetMode(PIN_OUTPUT, PI_OUTPUT);  // Set GPIO17 as input.

	while(1)
	{
		printf("Inserisci stato(0,1) [default: %d]: ", STATE_DEFAULT);


		memset(buf,0,sizeof(buf));
		num = 0;
		do
		{
			c = getchar();
			buf[num] = c;
			num++;
		}while( c!= '\n');


		if(num != 1)
		{
			state = atoi(buf);
		}
		else
		{
			state = STATE_DEFAULT;
		}

		if((state >= 0) && (state <= 1))
		{
			printf("\e[32mSetto a %d\e[37m\n",state);

			//digitalWrite(LED_PIN, state);
			gpioWrite(PIN_OUTPUT, state);
		}
		else
			printf("\e[31mErrore: %d\e[37m\n",state);


	}
	return 1;
}

//--------------------------------------------------
// leggiStatoPIN
//--------------------------------------------------
int leggiStatoPIN()
{
	int oldValue_0 = -1;


	gpioSetMode(BUTTON_PIN, PI_INPUT);  // Set GPIO17 as input.
	gpioSetPullUpDown(BUTTON_PIN, PI_PUD_UP);   // Sets a pull-up.


	while(1)
	{
			usleep(500);
			if(gpioRead(BUTTON_PIN) != oldValue_0)
			{
				printf("Stato PIN %d --> %d\n", BUTTON_PIN,gpioRead(BUTTON_PIN));
				oldValue_0 = gpioRead(BUTTON_PIN);
			}

	}

	return 1;
}

//--------------------------------------------------
// followPIN
//--------------------------------------------------
int followPIN()
{
	printf("followPIN\n");

	gpioSetMode(PIN_OUTPUT, PI_OUTPUT);
	gpioSetMode(BUTTON_PIN, PI_INPUT);

	gpioSetPullUpDown(BUTTON_PIN, PI_PUD_UP);   // Sets a pull-up.


	while(1)
	{


			if(gpioRead(BUTTON_PIN) == 0)
			{
				gpioWrite(PIN_OUTPUT, 0);   //led on
			}
			else
			{
				gpioWrite(PIN_OUTPUT, 1);

			}
	}

	return 1;
}

//--------------------------------------------------
// blinkPIN
//--------------------------------------------------
int blinkPIN()
{
	printf("blinkPIN 1\n");
//	pinMode(LED_PIN, OUTPUT);

	gpioSetMode(PIN_OUTPUT, PI_OUTPUT);  // Set GPIO17 as input.
	while(1)
	{
		gpioWrite(PIN_OUTPUT, 0);  //led on
		printf(".\n");

		usleep(500000);
		gpioWrite(PIN_OUTPUT, 1);  //led off
		printf("|\n");
		usleep(500000);
	}



	return 1;
}


//--------------------------------------------------
// breathingLed
//--------------------------------------------------
int breathingLed()
{
	//int i = 0;
	printf("breathingLed\n");

/*	gpioSetMode(BREATHING_LED_PIN, PI_ALT5);

	while(1)
	{
		for (i =0 ; i < 256; i++)
		{
			gpioPWM(BREATHING_LED_PIN, i);  //led on
			usleep(2000);
		}
	}

*/
	return 1;
}



