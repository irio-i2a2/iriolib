/******************************************************************************
 * $HeadURL:
 * $Id:
 *
 * Project		 : IRIO Driver
 *
 * Description	 : IRIO Driver Test Program for resource search in FlexRIO devices
 * 				using the different profiles.
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

//#define RIOMODEL "PXIe-7965R"
//#define RIOMODEL_FOLDER "7965"
//#define RIOMODEL_SERIAL  "01666C59"


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
        printf("This example checks the terminals in the FPGA design\n"
                "This example requires a FlexRIO device\n"
                "If OPEN NI-RIO Linux Version is installed: Use lsrio.py command to identify the RIO devices included in the fast controller\n"
                "If CLOSE NI-RIO Linux Version is installed: Use lsni -v command to identify the RIO devices included in the fast controller\n"        		"\n"
                "\n"
        		"Below there is information about the input parameters required: \n"
        		"\n"
        		"\t<Serial_Number>: Device serial number, uses lsrio.py/lsni -v command to get it.\n"
        		"\t<FlexRIO_model>: Device model, uses lsrio.py/lsni -v command to get it.\n"
        		"\n"
        		"Usage: %s <Serial_Number> <FlexRIO_Model> \n"
                "Example: %s 01666C59 79xx \n", name, name);
}
//TODO: This app is pending of rebuiding bitfiles with sufix 7965
int main (int argc, char **argv)
{
	irioDrv_t p_DrvPvt;
	TStatus status;
	irio_initStatus(&status);
	int myStatus;

	char *NIriomodel=NULL;
	char *filePath=NULL;
	char *bitfileName=NULL;

	int verbosity=1;

	if (argc != 3) {
		    usage(argv[0]);
		    return 1;
	}

	asprintf(&NIriomodel,"PXIe-%sR",argv[2]);
	asprintf(&filePath,"%s/%s/%s/",(char*)get_current_dir_name(),"resourceTest",argv[2]);

	//-----------------CPUDAQ Test
	msgtest(0,CPUDAQ Test);
	asprintf(&bitfileName,"FlexRIO_CPUDAQ_%s",argv[2]);
	printf("Test 0: CPU DAQ.Should Found:\n");
	printf("2 DMAs\n");
	printf("2 AI\n");
	printf("2 AO\n");
	printf("2 auxAI\n");
	printf("2 auxAO\n");
	printf("2 DI\n");
	printf("2 DO\n");
	printf("2 auxDI\n");
	printf("2 auxDO\n");
	printf("2 SG\n");

	myStatus=irio_initDriver("testCPUDAQ",argv[1],NIriomodel,bitfileName,"V1.0",verbosity,filePath,filePath,&p_DrvPvt,&status);
	myStatus|=irio_closeDriver(&p_DrvPvt,0, &status);
	msgerr(myStatus,0,"CPUDAQ Test",&status,verbosity,0);


	//-----------------CPUIMAQ Test
	msgtest(1,CPUIMAQ Test);
	asprintf(&bitfileName,"FlexRIO_CPUIMAQ_%s",argv[2]);
	printf("Test 1: CPUIMAQ.Should Found:\n");
	printf("1 IMAQ DMAs\n");
	printf("1 CLConfig\n");
	printf("1 CLUART\n");
	printf("2 auxAI\n");
	printf("2 auxAO\n");
	printf("2 DI\n");
	printf("2 DO\n");
	printf("2 auxDI\n");
	printf("2 auxDO\n");

	myStatus=irio_initDriver("testCPUIMAQ",argv[1],NIriomodel,bitfileName,"V1.0",verbosity,filePath,filePath,&p_DrvPvt,&status);
	myStatus|=irio_closeDriver(&p_DrvPvt,0, &status);
	msgerr(myStatus,1,"CPUIMAQ Test",&status,verbosity,0);

	//-----------------all possible FlexRIO I/O resources Test
	msgtest(2,MAXIO Test);
	asprintf(&bitfileName,"FlexRIOonlyResources_%s",argv[2]);
	printf("Test 2: MAXIO. It should be Found:\n");
	printf("1 DMA\n");
	printf("2 AO\n");
	printf("16 auxAI\n");
	printf("16 auxAO\n");
	printf("16 auxDI\n");
	printf("16 auxDO\n");
	printf("54 DI \n");
	printf("54 DO \n");
	printf("2 SG\n");

//	asprintf(&VIName,"FlexRIOonlyResources_%s",riomodel_folder);
	myStatus=irio_initDriver("testMAXIO",argv[1],NIriomodel,bitfileName,"V1.1",verbosity,filePath,filePath,&p_DrvPvt,&status);
	myStatus|=irio_closeDriver(&p_DrvPvt,0, &status);
	msgerr(myStatus,2,"MAXIO Test",&status,verbosity,0);

	msgtest(3,Missing resources test);
	printf("[Bug7516] This tests checks the correct order in error messages when verbosity=1 \n");
	printf("Test 3: Missing resources. It should be Found:\n");
	printf("1 AO + Error finding AO1Enable\n");
	printf("16 auxAI\n");
	printf("16 auxAO\n");
	printf("16 auxDI\n");
	printf("16 auxDO\n");
	printf("54 DI \n");
	printf("54 DO \n");
	printf("0 SG + Error finding SGFref0 and SGSignalType1\n");
	printf("0 DMAs + Error finding DMATtoHOSTSamplingRate0\n");
	char* errorFilePath =NULL;
	asprintf(&filePath,"%s/%s/%s/",(char*)get_current_dir_name(),"resourceFail",argv[2]);

	myStatus=irio_initDriver("testMAXIO",argv[1],NIriomodel,bitfileName,"V1.1",verbosity,filePath,filePath,&p_DrvPvt,&status);
	myStatus|=irio_closeDriver(&p_DrvPvt,0, &status);
	free(errorFilePath);
	//Invert status -> previous call should fail
	if(myStatus!=IRIO_success){
		myStatus=IRIO_success;
	}else{
		myStatus=IRIO_error;
	}
	msgerr(myStatus,3,"Missing resources test",&status,verbosity,0);

	//-----------------GPUDAQ Test
//	msgtest(4,GPUDAQ Test);
//	printf("Test 4: GPUDAQ.Should Found:\n");
//	asprintf(&bitfileName,"FlexRIO_GPUDAQ_%s",argv[2]);
//	printf("2 GPUDMAs\n");
//	printf("2 AI\n");
//	printf("2 AO\n");
//	printf("2 auxAI\n");
//	printf("2 auxAO\n");
//	printf("2 DI\n");
//	printf("2 DO\n");
//	printf("2 auxDI\n");
//	printf("2 auxDO\n");
//	printf("2 SG\n");
//	printf("This test must be skipped. GPUDAQ is not supported yet.\n");
//	myStatus=irio_initDriver("testGPUDAQ",argv[1],NIriomodel,bitfileName,"V1.0",verbosity,filePath,filePath,&p_DrvPvt,&status);
//	myStatus|=irio_closeDriver(&p_DrvPvt,0, &status);
//	msgerr(myStatus,4,"GPUDAQ Test",&status,verbosity,0);
//

//	//-----------------GPUIMAQ Test
//	msgtest(5,GPUIMAQ Test);
//	asprintf(&bitfileName,"FlexRIO_GPUIMAQ_%s",riomodel_folder);
//	printf("Test 5: GPUIMAQ.Should Found:\n");
//	printf("1 GPUDMAs\n");
//	printf("1 CLConfig");
//	printf("1 CLUART");
//	printf("2 auxAI\n");
//	printf("2 auxAO\n");
//	printf("2 DI\n");
//	printf("2 DO\n");
//	printf("2 auxDI\n");
//	printf("2 auxDO\n");
//	printf("This test must be skipped. GPUIMAQ is not supported yet.\n");
//	myStatus=irio_initDriver("testGPUIMAQ",argv[1],NIriomodel,bitfileName,"V1.0",verbosity,filePath,filePath,&p_DrvPvt,&status);
//	myStatus|=irio_closeDriver(&p_DrvPvt,0, &status);
//	msgerr(myStatus,5,"GPUIMAQ Test",&status,verbosity,0);

	free(NIriomodel);
	free(filePath);
	free(bitfileName);

	return 0;
}
