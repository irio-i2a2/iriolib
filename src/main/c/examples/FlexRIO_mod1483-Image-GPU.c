/******************************************************************************
 * $HeadURL:
 * $Id:
 *
 * Project		 : CODAC Core System
 *
 * Description	 : irioCorelib Test Program for FlexRIO without adapter module
 *
 * Author		 : Diego Sanz (UPM)
 *
 * Co-Author	 :
 *
 * Copyright (c) : 2010-2016 ITER Organization,
 *				   CS 90 046
 *				   13067 St. Paul-lez-Durance Cedex
 *				   France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iriocuda.h>

#include <irioDriver.h>
#include <irioDataTypes.h>
#include <irioHandlerAnalog.h>
#include <irioHandlerDigital.h>
#include <irioHandlerDMAGPU.h>
#include <irioHandlerImage.h>
#include <irioHandlerSG.h>
#include <irioError.h>
#include <irioResourceFinder.h>

#define RIOMODEL "PXIe-7966R"
#define RIOMODEL_SERIAL  "0177A2AD"
#define FIFO_REQUESTED_DEPTH 8192 //8192 elements of 64bits for storing a 256*256 image of 8btis.
#define IMGSIZE 256*256

uint64_t* cpu_buffer;
uint64_t* gpu_bufffer0;

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
        printf("This example checks the image acquisition from a FlexRIONI1483/NVIDA bundle acquiring from a\n"
        		"cameralink compatible device. \n"
                "If OPEN NI-RIO Linux Version is installed: Use lsrio.py command to identify the RIO devices included in the fast controller\n"
                "If CLOSE NI-RIO Linux Version is installed: Use lsni -v command to identify the RIO devices included in the fast controller\n"
        		"\n"
                "Usage: %s <SERIAL_NUMBER> <RIOMODEL> \n"
                "Example: %s 01666C59 7965 \n", name, name);
}


int main (int argc, char **argv)
{
	irioDrv_t p_DrvPvt;
	TStatus status;
	irio_initStatus(&status);
	int myStatus;
	char *filePath=NULL;
	char *bitfileName=NULL;
	char *NIriomodel=NULL;
	int gpu_status;
	int verbosity=1;
	uint64_t* gpu_ptr;
	int gpuDeviceId = 0;
	uint16_t fcimg; //frame counter embedded in the image

	if (argc != 3)
	{
		usage(argv[0]);
		return 1;
	}

	msgtest(0,GPU_Open & GPU_SetUserBuffer_U64);
	cpu_buffer=(uint64_t*)malloc(IMGSIZE/8* sizeof(uint64_t));
	gpu_status = GPU_does_system_contain_an_allowed_nvidia_device(&gpuDeviceId);
	if (gpu_status != CUDA_SUCCESS_CODE)
	{
		irio_mergeStatus(&status,Generic_Error,verbosity,"No NVIDIA GPU card installed supporting RDMA. Return code = %d",gpu_status);
		msgerr(IRIO_error,0,"GPU_Open & GPU_SetUserBuffer_U64",&status,verbosity,1);
	}
	// Opening the GPU: Selecting the GPU to use.

	gpu_status=GPU_Open (gpuDeviceId); //there is no close

	if (gpu_status != CUDA_SUCCESS_CODE){
		irio_mergeStatus(&status,Generic_Error,verbosity,"GPU_Open return error. Code:%d",gpu_status);
		msgerr(IRIO_error,0,"GPU_Open & GPU_SetUserBuffer_U64",&status,verbosity,1);
	}

	// Allocating memory in the GPU
	gpu_status= GPU_SetUserBuffer_U64(&gpu_bufffer0, FIFO_REQUESTED_DEPTH);

	if (gpu_status != CUDA_SUCCESS_CODE){
		irio_mergeStatus(&status,Generic_Error,verbosity,"GPU_SetUserBuffer_U64 return error. Code:%d",gpu_status);
		msgerr(IRIO_error,0,"GPU_Open & GPU_SetUserBuffer_U64",&status,verbosity,1);
	}
	msgerr(IRIO_success,0,"GPU_Open & GPU_SetUserBuffer_U64",&status,verbosity,1);


	printf("*****************IRIO IMAQ GPU Profile Example*************\n");
	printf("*****************Image Acquisition Example*****************\n");

	msgtest(1,irio_initDriver);
	printf("Initializing IRIO.\n Configure FPGA and search resources\n");
	asprintf(&filePath,"%s/resourceTest/%s/",get_current_dir_name(),argv[2]);
	asprintf(&NIriomodel,"PXIe-%sR",argv[2]);
	asprintf(&bitfileName,"FPGA1483_8tap8GPUV1_0");
	myStatus=irio_initDriver("testGPUDAQ",argv[1],NIriomodel,bitfileName,"V1.1",verbosity,filePath,filePath,&p_DrvPvt,&status);
	free(filePath);
	free(NIriomodel);
	free(bitfileName);
	msgerr(myStatus,1,"irio_initDriver",&status,verbosity,1);

	p_DrvPvt.gpu_buffer=gpu_bufffer0;

	//******* Pre FPGAStart Config
	msgtest(2,irio_configCL & irio_setUpDMAsTtoGPU);
	printf("Configurations previous to FPGAStart.\n Configure 1483 Adapter Module parameters. Setting up DMAs\n");
	//Config CameraLink Adapter Module
	myStatus|= irio_configCL(&p_DrvPvt,1,1,1,1,1,0,CL_FULL,CL_STANDARD,&status);
	//Config DMA
	myStatus |= irio_setUpDMAsTtoGPU(&p_DrvPvt,FIFO_REQUESTED_DEPTH,&status);
	msgerr(myStatus,2,"irio_configCL & irio_setUpDMAsTtoGPU",&status,verbosity,1);

	//********* FPGA Start
	usleep(1000);

	msgtest(3,irio_setFPGAStart);
	printf("Starting FPGA\n");
	myStatus = irio_setFPGAStart(&p_DrvPvt,1,&status);
	msgerr(myStatus,3,"irio_setFPGAStart",&status,verbosity,1);

	//*********** Acquire Images
	//Acquire 1000 Images and check FrameCounter
	msgtest(4,irio_setDMATtoGPUEnable irio_setDAQStartStop irio_getDMATtoGPUImage GPU_MemcpyDeviceToHost_U64 irio_releaseDMATtoGPUImage);
	printf("Receiving Images. Get 1000 images and check frame counter embedded by the camera link simulator\n");
	uint16_t fc=0;
	int firstImage=1;
	int count=0;
	unsigned int fcimgnew;

	//myStatus = irio_setDMATtoHostEnable(&p_DrvPvt,0,1,&status);
	myStatus = irio_setDMATtoGPUEnable(&p_DrvPvt,0,1,&status);
	myStatus |= irio_setDAQStartStop(&p_DrvPvt,1,&status);

	while(myStatus==IRIO_success && fc<1000){

			myStatus |= irio_getDMATtoGPUImage(&p_DrvPvt, 256*256, 0, &gpu_ptr, &count, &status);

			if(myStatus==IRIO_success && count==256*256){
				//printf("count=%d\n",count);

				GPU_MemcpyDeviceToHost_U64(cpu_buffer, &gpu_ptr, IMGSIZE/8);
				fcimgnew=((uint16_t *)cpu_buffer)[0];
				irio_releaseDMATtoGPUImage(&p_DrvPvt, 0,256*256/8, &status);
				if (firstImage)
				{
					firstImage=0;
				}
				else if ((fcimg+1)%65536!=fcimgnew){
						irio_mergeStatus(&status,Generic_Error,verbosity,"\n FrameCounter Error at Image fc[%u]=%u, fc[%u-a]=%u", fc, fcimgnew, fc,fcimg);
						myStatus=IRIO_error;
				}
				fc++;
				fcimg=fcimgnew;
			}
			else{
				usleep(1000);
			}
	}
	free(cpu_buffer);
	msgerr(myStatus,4,"irio_setDMATtoGPUEnable irio_setDAQStartStop irio_getDMATtoGPUImage GPU_MemcpyDeviceToHost_U64 irio_releaseDMATtoGPUImage",&status,verbosity,0);

	msgtest(5,irio_closeDMAsTtoGPU);
	myStatus=irio_closeDMAsTtoGPU(&p_DrvPvt, &status);
	msgerr(myStatus,5,"irio_closeDMAsTtoGPU",&status,verbosity,0);

	GPU_UnsetUserBuffer_U64(&gpu_bufffer0); //releasing GPU memory

	msgtest(6,irio_closeDriver);
	printf("Finalizing Driver. Close session and free memory\n");
	myStatus = irio_closeDriver(&p_DrvPvt,0, &status);
	msgerr(myStatus,6,"irio_closeDriver",&status,verbosity,0);
	return 0;
}
