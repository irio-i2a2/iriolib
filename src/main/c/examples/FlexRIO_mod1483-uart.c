/******************************************************************************
 * $HeadURL:
 * $Id:
 *
 * Project		 : IRIO Driver
 *
 * Description	 : IRIO Driver Test Program for UART communication using
 * 					a 7966 FlexRIO Device with a 1483 Adapter module
 *
 * Author		 : Universidad Politécnica de Madrid (UPM) (Technical University of Madrid)
 *
 * Copyright (c) : 2010-2015 Universidad Politécnica de Madrid (UPM) (Technical University of Madrid)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <irioDriver.h>
#include <irioDataTypes.h>
#include <irioHandlerAnalog.h>
#include <irioHandlerDigital.h>
#include <irioHandlerDMA.h>
#include <irioHandlerImage.h>
#include <irioHandlerSG.h>
#include <irioError.h>
#include <irioResourceFinder.h>

//#define RIOMODEL "PXIe-7966R"
//#define RIOMODEL_SERIAL  "0177A2AD"

#define msgtest(n,function)  \
					printf("\n####################################################\n"); \
					printf("IRIO test " #n " " #function " function\\s\n"); \
					printf("####################################################\n")

void msgerr(TIRIOStatusCode code, int nTest, const char* testName, TStatus* status,int verbosity, int exitOnFailure){
	if(code==IRIO_success){
		printf("IRIO test %d %s function\\s successful execution [OK] \n\n",nTest,testName);
		fflush(stdout);
	}else{
		char* detailStr=NULL;
		irio_getErrorString(status->detailCode,&detailStr);
		printf("IRIO test %d %s function\\s unsuccessful execution. Error Code: %d (%s) [ERROR]\n",nTest,testName,status->detailCode,detailStr);
		if(verbosity==0){
			printf("\n\tIRIO ERRORS:\n %s",status->msg);
		}else{
			printf("\n\tCheck previous messages for more detailed information of the error\n");
		}
		free(detailStr);
		fflush(stdout);
		irio_resetStatus(status);

		if(exitOnFailure){
			printf("\n CRITICAL FALIURE Aborting execution\n");
			fflush(stdout);
			exit(-1);
		}
	}
}


void usage(char *name) {
        printf("This example checks the uart available in the cameralink interface\n"
                "This example requires a FlexRIO device 7966 and a NI1483\n"
                "If OPEN NI-RIO Linux Version is installed: Use lsrio.py command to identify the RIO devices included in the fast controller\n"
                "If CLOSE NI-RIO Linux Version is installed: Use lsni -v command to identify the RIO devices included in the fast controller\n"        		"\n"
                "Usage: %s <SERIAL_NUMBER> <RIOMODEL> \n"
                "Example: %s 01666C59 7965 \n", name, name);
}
int main (int argc, char **argv)
{
	irioDrv_t p_DrvPvt;
	TStatus status;
	irio_initStatus(&status);
	int myStatus;
	int verbosity=1;

	char *filePath=NULL;
	char *bitfileName=NULL;
	char *NIriomodel=NULL;

	char msgr[80];

	if (argc != 3)
	{
		usage(argv[0]);
		return 1;
	}

	asprintf(&filePath,"%s/resourceTest/%s/",get_current_dir_name(),argv[2]);
	asprintf(&NIriomodel,"PXIe-%sR",argv[2]);
	asprintf(&bitfileName,"FlexRIOMod1483_%s",argv[2]);

	printf("*****************IRIO IMAQ Profile Example*****************\n");
	printf("*****************UART Handler Example*****************\n");

	//******** Download bitfile
	msgtest(0,irio_initDriver);
	myStatus=irio_initDriver("testCPUDAQ",argv[1],NIriomodel,bitfileName,"V1.1",verbosity,filePath,filePath,&p_DrvPvt,&status);
	free(filePath);filePath=NULL;
	free(NIriomodel);NIriomodel=NULL;
	free(bitfileName);bitfileName=NULL;
	msgerr(myStatus,0,"irio_initDriver",&status,verbosity,1);

	//******* Pre FPGAStart Config
	msgtest(1,irio_configCL irio_getUARTBaudRate irio_getUARTBreakIndicator irio_getUARTFrammingError irio_getUARTOverrunError);
	//Config CameraLink Adapter Module
	int value;
	myStatus|= irio_configCL(&p_DrvPvt,1,1,1,1,1,0,CL_STANDARD,CL_FULL, &status);
	//Config DMA
	myStatus|= irio_getUARTBaudRate(&p_DrvPvt,&value,&status);
	printf("Default baud rate: %d\n",value);
	myStatus|= irio_getUARTBreakIndicator(&p_DrvPvt,&value,&status);
	printf("Default Break Indicator: %d\n",value);
	myStatus|= irio_getUARTFrammingError(&p_DrvPvt,&value,&status);
	printf("Framing Error: %d\n",value);
	myStatus|= irio_getUARTOverrunError(&p_DrvPvt,&value,&status);
	printf("Overrun Error: %d\n",value);
	msgerr(myStatus,1,"irio_configCL irio_getUARTBaudRate irio_getUARTBreakIndicator irio_getUARTFrammingError irio_getUARTOverrunError",&status,verbosity,1);

	msgtest(2,irio_setUpDMAsTtoHost);
	myStatus |= irio_setUpDMAsTtoHost(&p_DrvPvt,&status);
	msgerr(myStatus,2,"irio_setUpDMAsTtoHost",&status,verbosity,1);

	//********* FPGA Start
	usleep(1000);
	msgtest(3,irio_setFPGAStart);
	myStatus = irio_setFPGAStart(&p_DrvPvt,1,&status);
	msgerr(myStatus,3,"irio_setFPGAStart",&status,verbosity,1);


	//******** Set BaudRate
	msgtest(4,irio_setUARTBaudRate);
	printf("Setting FPGA UART BaudRate to 9600\n");
	myStatus = irio_setUARTBaudRate(&p_DrvPvt,0,&status);
	msgerr(myStatus,4,"irio_setUARTBaudRate",&status,verbosity,1);


	puts("Introduce 4 characters and pulse intro, to send data to the camera\n");
	fgets(msgr,80,stdin);


	//********** Send UART Message
	msgtest(5,irio_sendCLuart);
	printf("Sending UART Message. Sending %s\n",msgr);
	myStatus = irio_sendCLuart(&p_DrvPvt,msgr,strlen(msgr),&status);
	msgerr(myStatus,5,"irio_sendCLuart",&status,verbosity,0);


	puts("Please check the message displayed in the EDTpdv terminal application. Press intro to continue\n");
	fgets(msgr,80,stdin);


	puts("Introduce 4 characters into the EDTpdv terminal application. After writing the 4 characters, press intro in this terminal.\n");
	fgets(msgr,80,stdin);

	//*********** Receive UART Message
	msgtest(6,irio_getCLuart);
	char* msg = malloc(40*sizeof(char));
	int len=0;
	printf("Receiving UART Message.\n");
	myStatus= irio_getCLuart(&p_DrvPvt,msg,&len,&status);

	if (len>0){
		printf("MSG received: %s\n",msg);
	}else{
		printf("MSG not received\n");
	}
	free(msg);
	msgerr(myStatus,6,"irio_getCLuart",&status,verbosity,0);


	msgtest(7,irio_closeDriver);
	myStatus = irio_closeDriver(&p_DrvPvt,0, &status);
	msgerr(myStatus,7,"irio_closeDriver",&status,verbosity,0);

	return 0;
}
