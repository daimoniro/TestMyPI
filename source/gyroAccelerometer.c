/*
 * gyroAccelerometer.c
 *
 *  Created on: 28 Aug 2018
 *      Author: pi
 */



#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pigpio.h>
#include <math.h>

#include "gyroAccelerometer.h"
#include "pin_raspberry.h"
#include "debug.h"

#define	MPU_6050_ADDR 			0x68

//--------------------------------------------------
// variabili globali
//--------------------------------------------------
int i2cHandleMPU6050 = -1;
float gyro_xout_scaled = 0;
float gyro_yout_scaled = 0;
float gyro_zout_scaled = 0;

float accel_xout_scaled = 0;
float accel_yout_scaled = 0;
float accel_zout_scaled = 0;

float x_rotation = 0;
float y_rotation = 0;

//--------------------------------------------------
// variabili extern
//--------------------------------------------------
extern char debugSTR[];

//--------------------------------------------------
// Function declaration
//--------------------------------------------------
void *gestioneGyroAccelerometer();
float dist(float a, float b);
float get_x_rotation(float x, float y, float z );
float get_y_rotation(float x, float y, float z );


//-----------------------------------------------------------------------------
//	StartGestioneGyroAccelerometer
//-----------------------------------------------------------------------------
void StartGestioneGyroAccelerometer()
{
    // Demone Configurazione Board
    pthread_t ThCapture;

    pthread_create(&ThCapture, NULL, &gestioneGyroAccelerometer, NULL);

}

//--------------------------------------------------
// initI2C_MPU6050
//--------------------------------------------------
void initI2C_MPU6050()
{
	i2cHandleMPU6050 = i2cOpen(1,MPU_6050_ADDR,0);
}


//--------------------------------------------------
// gestioneGyroAccelerometer
//--------------------------------------------------
void *gestioneGyroAccelerometer()
{
	int gyro_xout = 0;
	int gyro_yout = 0;
	int gyro_zout = 0;

	int accel_xout = 0;
	int accel_yout = 0;
	int accel_zout = 0;


	initI2C_MPU6050();

	if(i2cHandleMPU6050 > 0)
	{
		sprintf(debugSTR,"I2C Gyro i2cHandleMPU6050: %d",i2cHandleMPU6050);
		TRACE4(1,"GYRO",VERDE,NERO_BG,debugSTR,0);
		i2cWriteByteData(i2cHandleMPU6050,0x6b,1);
	}
	else
	{
		sprintf(debugSTR,"Errore I2C Gyro i2cHandleMPU6050: %d",i2cHandleMPU6050);
		TRACE4(1,"GYRO",ROSSO,NERO_BG,debugSTR,0);
	}

	while(1)
	{
		usleep(100000);

		gyro_xout = i2cReadWordData(i2cHandleMPU6050,0x43);
		gyro_yout = i2cReadWordData(i2cHandleMPU6050,0x45);
		gyro_zout = i2cReadWordData(i2cHandleMPU6050,0x47);

		accel_xout = i2cReadWordData(i2cHandleMPU6050,0x3b);
		accel_yout = i2cReadWordData(i2cHandleMPU6050,0x3d);
		accel_zout = i2cReadWordData(i2cHandleMPU6050,0x3f);



		gyro_xout_scaled = (float)gyro_xout / (float)131;
		gyro_yout_scaled = (float)gyro_yout / (float)131;
		gyro_zout_scaled = (float)gyro_zout / (float)131;

		accel_xout_scaled = (float)accel_xout / (float)16384;
		accel_yout_scaled = (float)accel_yout / (float)16384;
		accel_zout_scaled = (float)accel_zout / (float)16384;

		x_rotation = get_x_rotation(accel_xout_scaled,accel_yout_scaled,accel_zout_scaled);
		y_rotation = get_y_rotation(accel_xout_scaled,accel_yout_scaled,accel_zout_scaled);


		sprintf(debugSTR,"gyro_xout: %d scaled: %f",gyro_xout, gyro_xout_scaled);
		TRACE4(1,"GYRO",BIANCO,NERO_BG,debugSTR,0);
		sprintf(debugSTR,"gyro_yout: %d scaled: %f",gyro_yout, gyro_yout_scaled);
		TRACE4(1,"GYRO",BIANCO,NERO_BG,debugSTR,0);
		sprintf(debugSTR,"gyro_zout: %d scaled: %f",gyro_zout, gyro_zout_scaled);
		TRACE4(1,"GYRO",BIANCO,NERO_BG,debugSTR,0);

		sprintf(debugSTR,"accel_xout: %d scaled: %f",accel_xout, accel_xout_scaled);
		TRACE4(1,"GYRO",BIANCO,NERO_BG,debugSTR,0);
		sprintf(debugSTR,"accel_yout: %d scaled: %f",accel_yout, accel_yout_scaled);
		TRACE4(1,"GYRO",BIANCO,NERO_BG,debugSTR,0);
		sprintf(debugSTR,"accel_zout: %d scaled: %f",accel_zout, accel_zout_scaled);
		TRACE4(1,"GYRO",BIANCO,NERO_BG,debugSTR,0);

		sprintf(debugSTR,"x_rotation: %f",x_rotation);
		TRACE4(1,"GYRO",BIANCO,NERO_BG,debugSTR,0);
		sprintf(debugSTR,"y_rotation: %f",y_rotation);
		TRACE4(1,"GYRO",BIANCO,NERO_BG,debugSTR,0);

	}
}

//--------------------------------------------------
// dist
//--------------------------------------------------
float dist(float a, float b)
{
	return sqrt(a*a + b*b);
}

//--------------------------------------------------
// get_x_rotation
//--------------------------------------------------
float get_x_rotation(float x, float y, float z )
{
	float radians = atan2(y,dist(x,z));

	return radians * 180.0f / 3.14159265359f;

}

//--------------------------------------------------
// get_y_rotation
//--------------------------------------------------
float get_y_rotation(float x, float y, float z )
{
	float radians = atan2(x,dist(y,z));

	return radians * 180.0f / 3.14159265359f;

}
