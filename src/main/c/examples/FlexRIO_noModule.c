/******************************************************************************
 * $HeadURL:
 * $Id:
 *
 * Project		 : IRIO Driver
 *
 * Description	 : IRIO Driver Test Program for resource access in  FlexRIO devices
 *
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
        		"Usage: %s <Serial_Number> <FlexRIO_model> \n"
                "Example: %s 01666C59 79xx \n", name, name);
}

int main (int argc, char **argv)
{
	irioDrv_t p_DrvPvt;
	TStatus status;
	irio_initStatus(&status);
	int myStatus=0;
	int i=0;
//	size_t valueLength;
//	char VIversion[3];
//	int devProfile;
	int FPGATemp=0;
	int aivalue=0;
	int verbosity=1;

	char *filePath=NULL;
	char *bitfileName=NULL;
	char *NIriomodel=NULL;


	if (argc != 3) {
			usage(argv[0]);
			return 1;
	}


	asprintf(&filePath,"%s/resourceTest/%s/",get_current_dir_name(),argv[2]);
	asprintf(&bitfileName,"FlexRIOnoModule_%s",argv[2]);
	asprintf(&NIriomodel,"PXIe-%sR",argv[2]);

	///iRIOCore testing initNIRIODriver
	msgtest(0,irio_initDriver);
	myStatus=irio_initDriver("RIOX",argv[1],NIriomodel,bitfileName,"V1.1",verbosity,filePath,filePath,&p_DrvPvt,&status);
	free(filePath);
	free(bitfileName);
	free(NIriomodel);
	msgerr(myStatus,0,"irioinitDriver",&status,verbosity,1);

	//iRIOCore testing setFPGAStart
	msgtest(1,irio_setFPGAStart );
	printf("[irio_setFPGAStart function] FPGA hardware logic is started (\"Running\") Value %d \n", 1);
	myStatus=irio_setFPGAStart(&p_DrvPvt,1,&status);
	myStatus|=irio_getFPGAStart(&p_DrvPvt, &aivalue,&status);
	printf("[irio_getFPGAStart function] FPGA State is: %d . 1-->\"running\" 0-->\"stopped\"\n", aivalue);
	msgerr(myStatus,1,"irio_setFPGAStart",&status,verbosity,1);


	usleep(100);

	///iRIOCore testing: irio_setAuxAO irio_getAuxAI functions
	msgtest(2,irio_setAuxAO irio_getAuxAO irio_getAuxAI);
	for(i=0;i<6;i++)
	{
		printf("[irio_setAuxAO function] value 0 is set in auxAO%d \n",i);
		myStatus|=irio_setAuxAO(&p_DrvPvt,i,0,&status);
		myStatus|=irio_getAuxAO(&p_DrvPvt,i,&aivalue,&status);
		printf("[irio_getAuxAO function] value read from auxAO%d is:%d\n",i, aivalue);

		myStatus=irio_getAuxAI(&p_DrvPvt,i,&aivalue,&status);
		printf("[irio_getAuxAI function] value read from auxAI%d is:%d\n\n",i, aivalue);

		if(aivalue!=0)
		{
			irio_mergeStatus(&status,Generic_Error,verbosity,"AuxAI%d expected value 0, read value:%d [ERROR]\n",i,aivalue);
		}
		printf("[irio_setAuxAO function] value 100 is set in auxAO%d \n",i);
		myStatus|=irio_setAuxAO(&p_DrvPvt,i,100,&status);
		myStatus|=irio_getAuxAO(&p_DrvPvt,i,&aivalue,&status);
		printf("[irio_getAuxAO function] value read from auxAO%d is:%d\n",i, aivalue);

		myStatus|=irio_getAuxAI(&p_DrvPvt,i,&aivalue,&status);
		printf("[irio_getAuxAI function] value read from auxAI%d is:%d\n\n",i, aivalue);

		if(aivalue!=100)
		{
			irio_mergeStatus(&status,Generic_Error,verbosity,"AuxAI%d expected value 1,read value: %d [ERROR]\n",i,aivalue);
		}
	}
	msgerr(myStatus,2,"irio_setAuxAO irio_getAuxAO irio_getAuxAI",&status,verbosity,0);

	///iRIOCore testing: irio_setAuxDO irio_getAuxDI functions
	msgtest(3,irio_setAuxDO irio_getAuxDO irio_getAuxDI);
	for(i=0;i<6;i++)
	{

		printf("[irio_setAuxDO function] value 0 is set in auxDO%d \n",i);
		myStatus|=irio_setAuxDO(&p_DrvPvt,i,0,&status);
		myStatus|=irio_getAuxDO(&p_DrvPvt,i,&aivalue,&status);
		printf("[irio_getAuxDO function] value read from auxDO%d is:%d\n",i, aivalue);
		myStatus|=irio_getAuxDI(&p_DrvPvt,i,&aivalue,&status);
		printf("[irio_getAuxDI function] value read from auxDI%d is:%d\n\n",i, aivalue);

		if(aivalue!=0)
		{
			irio_mergeStatus(&status,Generic_Error,verbosity,"AuxDI%d expected value 0, value read:%d [ERROR]\n",i,aivalue);
		}

		printf("[irio_setAuxDO function] value 1 is set in auxDO%d \n",i);
		myStatus|=irio_setAuxDO(&p_DrvPvt,i,1,&status);
		myStatus|=irio_getAuxDO(&p_DrvPvt,i,&aivalue,&status);
		printf("[irio_getAuxDO function] value read from auxDO%d is:%d\n",i, aivalue);
		myStatus|=irio_getAuxDI(&p_DrvPvt,i,&aivalue,&status);
		printf("[irio_getAuxDI function] value read from auxDI%d is:%d\n\n",i, aivalue);

		if(aivalue!=1)
		{
			irio_mergeStatus(&status,Generic_Error,verbosity,"AuxDI%d expected value 1,read value: %d [ERROR]\n",i,aivalue);
		}
	}
	msgerr(myStatus,3,"irio_setAuxDO irio_getAuxDO irio_getAuxDI",&status,verbosity,0);


	/// iRIOCore testing irio_getDevTemp function
	FPGATemp=-1;
	msgtest(4,irio_getDevTemp);
	myStatus=irio_getDevTemp(&p_DrvPvt,&FPGATemp,&status);
	printf("[irio_getDevTemp function] temperature read from FPGA: %.2fºC\n",FPGATemp*0.25);
	msgerr(myStatus,4,"irio_getDevTemp",&status,verbosity,0);

	/// iRIOCore testing irio_closeDriver function
	msgtest(5,irio_closeDriver);
	myStatus=irio_closeDriver(&p_DrvPvt,0, &status);
	msgerr(myStatus,5,"irio_closeDriver",&status,verbosity,0);

	return 0;
}

