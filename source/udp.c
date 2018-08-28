/*
 * udp.c
 *
 *  Created on: 15 Aug 2018
 *      Author: pi
 */




#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <memory.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

 #include <pthread.h>

//*****************************************************************************
//	function declaration
//*****************************************************************************
void *UDPServerManagement(void *threadid);
int udpOutcomingClientUDP(unsigned char *bufferOut,unsigned char lenght);
int sendMainValueUDP();


//*****************************************************************************
//	Extern Variable
//*****************************************************************************

extern float distanceSonar_0;
extern float distanceSonar_1;
extern float tempDS18D20;
extern float tempDHT22;
extern float humDHT22;
extern int velMotoreDC_0;
extern int velMotoreDC_1;
//-----------------------------------------------------------------------------
//	StartUDPClientManagement
//-----------------------------------------------------------------------------
void StartUDPClientManagement()
{
    // Demone Configurazione Board
    pthread_t ThClientUDP;

    // create tread
    pthread_create(&ThClientUDP, NULL, &UDPServerManagement, NULL);

} // StartUDPClientManagement


//-----------------------------------------------------------------------------
//	UDPServerManagement
//-----------------------------------------------------------------------------
void *UDPServerManagement(void *threadid)
{

	while(1)
	{

		usleep(500000);
		sendMainValueUDP();

		//printf("send: %d\n",returnSend);
	}


}



int sendMainValueUDP()
{
	 unsigned char bufferTx[64];


	memset(bufferTx,0,sizeof(bufferTx));


	memcpy(bufferTx + 1,&tempDS18D20,sizeof(float));
	memcpy(bufferTx + 5,&distanceSonar_0,sizeof(float));
	memcpy(bufferTx + 9,&distanceSonar_1,sizeof(float));
	memcpy(bufferTx + 13,&tempDHT22,sizeof(float));
	memcpy(bufferTx + 17,&humDHT22,sizeof(float));

	bufferTx[21] = (unsigned char)velMotoreDC_0;
	bufferTx[22] = (unsigned char)velMotoreDC_1;




	 bufferTx[0] = 0x23;
	 bufferTx[23] = 0x37;

	 return udpOutcomingClientUDP(bufferTx,24);


}




//-----------------------------------------------------------------------------
//		udpOutcomingClientUDP
//-----------------------------------------------------------------------------
int udpOutcomingClientUDP(unsigned char *bufferOut,unsigned char lenght)
{
   int sockfd;
   struct sockaddr_in servaddr;



   int t = 0;

   int returnValue = -1;

   sockfd=socket(AF_INET,SOCK_DGRAM,0);


   for(t = 0; t < 8;t++)
   {
        servaddr.sin_zero[t]= 0;
   }
   servaddr.sin_family = AF_INET;
   servaddr.sin_addr.s_addr=inet_addr("192.168.2.20");
   servaddr.sin_port=htons(7001);

   //fcntl(sockfd, F_SETFL, O_NONBLOCK);
   //ioctlsocket(sockfd,FIONBIO,O_NDELAY);

   returnValue = sendto(sockfd,(char *) bufferOut, lenght, 0,(struct sockaddr *)&servaddr,sizeof(servaddr));


   close(sockfd);

   return returnValue;
}


