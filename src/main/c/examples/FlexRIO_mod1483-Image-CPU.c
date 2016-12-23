/******************************************************************************
 * $HeadURL:
 * $Id:
 *
 * Project		 : IRIO Driver
 *
 * Description	 : IRIO Driver Test Program for image acquisition using
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
        printf("This example checks the image acquisition from a cameralink compatible device\n"
                "This example requires a FlexRIO device 7966 with a NI1483\n"
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

	if (argc != 3)
	{
		usage(argv[0]);
		return 1;
	}

	asprintf(&filePath,"%s/resourceTest/%s/",get_current_dir_name(),argv[2]);
	asprintf(&NIriomodel,"PXIe-%sR",argv[2]);
	asprintf(&bitfileName,"FlexRIOMod1483_%s",argv[2]);


	printf("*****************IRIO IMAQ Profile Example*****************\n");
	printf("*****************Image Acquisition Example*****************\n");

	//******** Download bitfile
	msgtest(0,irio_initDriver);
	myStatus=irio_initDriver("testCPUDAQ",argv[1],NIriomodel,bitfileName,"V1.1",1,filePath,filePath,&p_DrvPvt,&status);
	free(filePath);
	free(NIriomodel);
	free(bitfileName);
	msgerr(myStatus,0,"irio_initDriver",&status,verbosity,1);


	msgtest(1,irio_configCL);
	//Config CameraLink Adapter Module
	//myStatus|= irio_configCL(&p_DrvPvt,1,1,1,1,1,0,CL_FULL,CL_STANDARD,&status);
	myStatus|= irio_configCL(&p_DrvPvt,1,1,1,1,1,0,CL_STANDARD,CL_FULL,&status);
	//Config DMA
	msgerr(myStatus,1,"irio_configCL",&status,verbosity,1);


	//********* FPGA Start
	msgtest(2,irio_setFPGAStart);
	usleep(1000);
	myStatus = irio_setFPGAStart(&p_DrvPvt,1,&status);
	msgerr(myStatus,2,"irio_setFPGAStart",&status,verbosity,1);

	//*********** Acquire Images
	//Acquire 1000 Images and check FrameCounter
	msgtest(3,acquiring images);
	int i=0;
	uint16_t fc=0;
	uint16_t *fc2;
	int firstImage=1;
	int count=0;
	//Allocate Memory for image
	uint64_t* dataBuffer=malloc((256*256/8)*sizeof(uint64_t));
	//Start Acquisition
	myStatus = irio_setDMATtoHostEnable(&p_DrvPvt,0,1,&status);
	myStatus |= irio_setDAQStartStop(&p_DrvPvt,1,&status);
	//The example is going to acquire 1000 images. This loop needs a variable time depending on the framerate programmed in the camera.
	printf(" 1000 frames are going to be acquired. The counter embedded in the frame will be recovered and printed. The counter value can start at any number but all the values must be consecutive.\n");
	while(myStatus==IRIO_success && i<1000){
		myStatus |= irio_getDMATtoHostImage(&p_DrvPvt, 256*256, 0, dataBuffer, &count, &status);

		if(myStatus==IRIO_success){
			if(count==256*256){
				fc2=(uint16_t*)dataBuffer; //In this example the cameralink simulator is injecting a counter in the first two pixels (bytes)
				if(firstImage){
					firstImage=0;
				}else if((fc+1)%65536!=fc2[0]){
					irio_mergeStatus(&status,Generic_Error,verbosity,"\nFrameCounter Error at Image fc[i]=%d, fc[i-1]=%d, img: %d\n",fc2[0],fc, i);
					myStatus=IRIO_error;
					break;
				}
				//printf ("fc2=%d\n",fc2[0]);
				fc=fc2[0];
				printf("Frame acquired [%i], counter value obtained from image:%d\n",i,fc);
				i++;
			}else{
				usleep(1000);
			}
		}
	}
	msgerr(myStatus,3,"acquiring images",&status,verbosity,0);

	msgtest(4,irio_closeDriver);
	myStatus = irio_closeDriver(&p_DrvPvt,0, &status);
	msgerr(myStatus,4,"irio_closeDriver",&status,verbosity,0);

	return 0;
}
