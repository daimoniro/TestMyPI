/*
 * compass.c
 *
 *  Created on: 29 Aug 2018
 *      Author: pi
 */


#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pigpio.h>
#include <math.h>

#include "compass.h"
#include "pin_raspberry.h"
#include "debug.h"


#define	HMC5883l_ADDR 			0x0d


//--------------------------------------------------
// variabili globali
//--------------------------------------------------
int i2cHandleHMC5883l = -1;

float compass_xout_scaled = 0;
float compass_yout_scaled = 0;
float compass_zout_scaled = 0;


//--------------------------------------------------
// variabili extern
//--------------------------------------------------
extern char debugSTR[];

//--------------------------------------------------
// Function declaration
//--------------------------------------------------
void *gestioneCompass();
void hmc5883l_init();

//-----------------------------------------------------------------------------
//	StartGestioneGyroAccelerometer
//-----------------------------------------------------------------------------
void StartGestioneCompass()
{
    // Demone Configurazione Board
    pthread_t ThCapture;

    pthread_create(&ThCapture, NULL, &gestioneCompass, NULL);

}

//--------------------------------------------------
// initI2C_HMC5883l
//--------------------------------------------------
void initI2C_HMC5883l()
{
	i2cHandleHMC5883l = i2cOpen(1,HMC5883l_ADDR,0);
}


//--------------------------------------------------
// gestioneCompass
//--------------------------------------------------
void *gestioneCompass()
{
	int compass_xout = 0;
	int compass_yout = 0;
	int compass_zout = 0;

	usleep(100000L);
	initI2C_HMC5883l();

	if(i2cHandleHMC5883l >= 0)
	{
		sprintf(debugSTR,"I2C Gyro initI2C_HMC5883l: %d",i2cHandleHMC5883l);
		TRACE4(1,"COMPASS",VERDE,NERO_BG,debugSTR,0);

		hmc5883l_init();
	}
	else
	{
		sprintf(debugSTR,"Errore I2C Gyro initI2C_HMC5883l: %d",i2cHandleHMC5883l);
		TRACE4(1,"COMPASS",ROSSO,NERO_BG,debugSTR,0);

		return NULL;
	}

	while(1)
	{
		usleep(1000000);
	}
}

//--------------------------------------------------
// hmc5883l_init
//--------------------------------------------------
void hmc5883l_init()
{

}
