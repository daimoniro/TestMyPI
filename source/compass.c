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

#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>


#include "compass.h"
#include "pin_raspberry.h"
#include "debug.h"


#define	HMC5883l_ADDR 			0x0d
#define PI 3.14159265

#define		I2C_BUS			"/dev/i2c-1"

//--------------------------------------------------
// variabili globali
//--------------------------------------------------
int i2cHandleHMC5883l = -1;

// I2C device
static int		i2c_dev = -1;

float compass_xout_scaled = 0;
float compass_yout_scaled = 0;
float compass_zout_scaled = 0;

int errorCompass = 0;

//--------------------------------------------------
// variabili extern
//--------------------------------------------------
extern char debugSTR[];

//--------------------------------------------------
// Function declaration
//--------------------------------------------------
void *gestioneCompass();
void hmc5883l_init();
int init_i2c();
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
	//printf("prima\n");
	i2cHandleHMC5883l = i2cOpen(1,HMC5883l_ADDR,0);
	//printf("dopo --> %d\n",i2cHandleHMC5883l);
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

	int probeByte = 0;

	//init_i2c();

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


		probeByte = i2cReadByteData(i2cHandleHMC5883l,0);

		if(probeByte < 0)
		{
			//printf("probeByte: %d\n",probeByte);

			errorCompass++;
			continue;
		}
		errorCompass = 0;

		//printf("test: %d\n",test);

		compass_xout = (short)((unsigned short)(probeByte << 8) + (unsigned short)i2cReadByteData(i2cHandleHMC5883l,4));
		compass_yout = (short)((unsigned short)(i2cReadByteData(i2cHandleHMC5883l,5) << 8) + (unsigned short)i2cReadByteData(i2cHandleHMC5883l,6));
		compass_zout = (short)((unsigned short)(i2cReadByteData(i2cHandleHMC5883l,7) << 8) + (unsigned short)i2cReadByteData(i2cHandleHMC5883l,8));

		sprintf(debugSTR,"compass_xout: %d", compass_xout);
		TRACE4(2,"COMPASS",BIANCO,NERO_BG,debugSTR,0);
		sprintf(debugSTR,"compass_yout: %d", compass_yout);
		TRACE4(2,"COMPASS",BIANCO,NERO_BG,debugSTR,0);
		sprintf(debugSTR,"compass_zout: %d", compass_zout);
		TRACE4(2,"COMPASS",BIANCO,NERO_BG,debugSTR,0);

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


int init_i2c(void)
{
	printf("open I2C device  ...");

	// Open the device node for the I2C adapter
	i2c_dev = open(I2C_BUS, O_RDWR);
	if (i2c_dev < 0)
	{
		printf("FAIL!\n");
		return -1;
	}
	printf("OK!\n");


	printf("set device addr ...");
	if (ioctl(i2c_dev, I2C_SLAVE, HMC5883l_ADDR) < 0) {
		printf("FAIL!\n");
		return -1;
	}
	else
	{
		printf("OK!\n");
	}

	return 0;
}

