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
#define PI 3.14159265

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

	float SCALE_COMPASS = 0.92;
	float bearing = 0;

	usleep(100000L);
	initI2C_HMC5883l();

	if(i2cHandleHMC5883l >= 0)
	{
		sprintf(debugSTR,"I2C Compass initI2C_HMC5883l: %d",i2cHandleHMC5883l);
		TRACE4(1,"COMPASS",VERDE,NERO_BG,debugSTR,0);

		hmc5883l_init();
	}
	else
	{
		sprintf(debugSTR,"Errore I2C Compass initI2C_HMC5883l: %d",i2cHandleHMC5883l);
		TRACE4(1,"COMPASS",ROSSO,NERO_BG,debugSTR,0);

		return NULL;
	}

	while(1)
	{
		usleep(1000000);

		compass_xout = (short)((unsigned short)(i2cReadByteData(i2cHandleHMC5883l,3) << 8) + (unsigned short)i2cReadByteData(i2cHandleHMC5883l,4));
		compass_yout = (short)((unsigned short)(i2cReadByteData(i2cHandleHMC5883l,5) << 8) + (unsigned short)i2cReadByteData(i2cHandleHMC5883l,6));
		compass_zout = (short)((unsigned short)(i2cReadByteData(i2cHandleHMC5883l,3) << 7) + (unsigned short)i2cReadByteData(i2cHandleHMC5883l,8));


		compass_xout_scaled = (float)compass_xout / (float)SCALE_COMPASS;
		compass_yout_scaled = (float)compass_yout / (float)SCALE_COMPASS;
		compass_zout_scaled = (float)compass_zout / (float)SCALE_COMPASS;

		bearing = atan2 (compass_yout_scaled,compass_xout_scaled) * 180 / PI;

		sprintf(debugSTR,"compass_xout_scaled: %f", compass_xout_scaled);
		TRACE4(2,"COMPASS",BIANCO,NERO_BG,debugSTR,0);
		sprintf(debugSTR,"compass_yout_scaled: %f", compass_yout_scaled);
		TRACE4(2,"COMPASS",BIANCO,NERO_BG,debugSTR,0);
		sprintf(debugSTR,"compass_zout_scaled: %f", compass_zout_scaled);
		TRACE4(2,"COMPASS",BIANCO,NERO_BG,debugSTR,0);

		sprintf(debugSTR,"bearing: %f", bearing);
		TRACE4(2,"COMPASS",BIANCO,NERO_BG,debugSTR,0);
	}
}

//--------------------------------------------------
// hmc5883l_init
//--------------------------------------------------
void hmc5883l_init()
{

	//# Set to 8 samples @ 15Hz
	i2cWriteByteData(i2cHandleHMC5883l, 0,0b01110000);

	//1.3 gain LSb / Gauss 1090 (default)
	i2cWriteByteData(i2cHandleHMC5883l, 1,0b00100000);

	//Continuous sampling
	i2cWriteByteData(i2cHandleHMC5883l, 2,0b00000000);

}
