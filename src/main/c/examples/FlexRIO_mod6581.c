/******************************************************************************
 * $HeadURL:
 * $Id:
 *
 * Project		 : IRIO Driver
 *
 * Description	 : IRIO Driver Test Program for FlexRIO without adapter module
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
        printf("This example checks the use of digital lines in the NI6581\n"
                "This example requires a FlexRIO device with a NI6581 adapter module\n"
                "If OPEN NI-RIO Linux Version is installed: Use lsrio.py command to identify the RIO devices included in the fast controller\n"
                "If CLOSE NI-RIO Linux Version is installed: Use lsni -v command to identify the RIO devices included in the fast controller\n"        		"\n"
                "\n"
        		"Usage: %s <SERIAL_NUMBER> <RIOMODEL> \n"
                "Example: %s 01666C59 7961 \n", name, name);
}

int main (int argc, char** argv)
{
	irioDrv_t p_DrvPvt;
	TStatus status;
	irio_initStatus(&status);
//	int myStatus=0;
	TIRIOStatusCode myStatus=IRIO_success;
	int i=0;
	int FPGATemp;
	int valueI32;
	int verbosity=1;

	int valueReadI32=0;
	char *filePath=NULL;
	char *bitfileName=NULL;
	char *NIriomodel=NULL;

	if (argc != 3)
	{
		usage(argv[0]);
		return 1;
	}


	/**
	 * PATH BUILDING FOR THE BITFILE AND HEADER FILE
	 */
	asprintf(&filePath,"%s/resourceTest/%s/",get_current_dir_name(),argv[2]);
	asprintf(&bitfileName,"FlexRIOMod6581_%s",argv[2]);
	asprintf(&NIriomodel,"PXIe-%sR",argv[2]);

	///Initializing the RIO device
	msgtest(0,irio_initDriver);
	myStatus|=irio_initDriver("RIOX",argv[1],NIriomodel,bitfileName,"V1.1",verbosity,filePath,filePath,&p_DrvPvt,&status);
	free(filePath);filePath=NULL;
	free(bitfileName);bitfileName=NULL;
	free(NIriomodel);NIriomodel=NULL;
	msgerr(myStatus,0,"irio_initDriver",&status,verbosity,1);

	///Starting the FPGA in the FlexRIO
	msgtest(1,irio_setFPGAStart);
	myStatus|=irio_setFPGAStart(&p_DrvPvt,1,&status);
	msgerr(myStatus,1,"irio_setFPGAStart",&status,verbosity,1);

	usleep(100);

	// In this example PORT2 has been configured as output and PORT0 and 1 as inputs.

	msgtest(2,irio_setAuxDO irio_getAuxDO);
	printf("[irio_setAuxDO function] write '1' value into auxDO6\n");
	myStatus=irio_setAuxDO(&p_DrvPvt,6,1,&status);
	myStatus|=irio_getAuxDO(&p_DrvPvt, 6, &valueReadI32, &status);
	printf("[irio_getAuxDO function] read value from auxDO6 is: %d \n",valueReadI32);
	printf("auxDO6 configured to 1, means that DO[7-0] are connected to port 2 in Connector A\n");
	msgerr(myStatus,2,"irio_setAuxDO irio_getAuxDO",&status,verbosity,0);
	usleep(10);
	// When auxDO6 is true, PORT 2 is updated with the value written in terminals DO0, DO1-- DO7. (see labview implementation)
	// This digital values are expected in the port 1.

	/**
	 * WRITING AND READING DIGITAL I/O, BOTH VALUES: 0 AND 1.
	 */
	msgtest(3,irio_setDO irio_getDO irio_getDI);
	printf("\nIRIO test 3: This test write 0 and 1, into every digital output, and it is read from DO register.\n");fflush(stdout);
	printf("Hardware digital I/O [7-0] are interconnected physically, then the value written in DO must be read in the DI too.\n");fflush(stdout);
	printf("Although all values written and read are showed, in case of reading an unexpected value, Error message will be shown\n\n");fflush(stdout);
	for(i=0;i<8;i++)
	{
		printf("[irio_setDO function]IRIO test 3: Write 0 value in DO%d\n",i);fflush(stdout);
		myStatus|=irio_setDO(&p_DrvPvt, i, 0, &status);
		myStatus|=irio_getDO(&p_DrvPvt, i, &valueReadI32, &status);
		printf("[irio_getDO function]IRIO test 3: value read from DO%d: %d\n",i,valueReadI32);fflush(stdout);
		if(valueReadI32!=0)
		{
			irio_mergeStatus(&status,Generic_Error,verbosity,"Expected value in D0%d is 0, but value read is:%d [ERROR]\n",i,valueReadI32);
		}
		usleep(20);

		myStatus|=irio_getDI(&p_DrvPvt,i,&valueReadI32,&status);
		printf("[irio_getDI function]IRIO test 3: value read from DI%d: %d\n\n",i,valueReadI32);fflush(stdout);

		if(valueReadI32!=0)
		{
			irio_mergeStatus(&status,Generic_Error,verbosity,"Expected value in DI%d is 0, but value read is:%d [ERROR]\n",i,valueReadI32);
		}

		printf("[irio_setDO function]IRIO test 3: Write 1 value in DO%d\n",i);fflush(stdout);
		myStatus|=irio_setDO(&p_DrvPvt,i,1,&status);
		myStatus|=irio_getDO(&p_DrvPvt, i, &valueReadI32, &status);
		printf("[irio_getDO function]IRIO test 3: value read from DO%d: %d\n",i,valueReadI32);fflush(stdout);

		if(valueReadI32!=1)
		{
			irio_mergeStatus(&status,Generic_Error,verbosity,"Expected value in D0%d is 1, but value read is:%d [ERROR]\n",i,valueReadI32);
		}
		usleep(10);

		myStatus|=irio_getDI(&p_DrvPvt,i,&valueI32,&status);
		printf("[irio_getDI function]IRIO test 3: value read from DI%d: %d\n\n",i,valueReadI32);fflush(stdout);

		if(valueReadI32!=1)
		{
			irio_mergeStatus(&status,Generic_Error,verbosity,"Expected value in DI%d is 1, but value read is:%d [ERROR]\n",i,valueReadI32);
		}
	}
	msgerr(myStatus,3,"irio_setDO irio_getDO irio_getDI",&status,verbosity,0);

	/**
	* READING FPGA TEMPERATURE
	*/
	FPGATemp=-1;
	msgtest(4,irio_getDevTemp);
	myStatus=irio_getDevTemp(&p_DrvPvt,&FPGATemp,&status);
	printf("[irio_getDevTemp function] Temperature value read from FPGA is %.2fºC\n",(float)((float)FPGATemp*0.25));
	msgerr(myStatus,4,"irio_getDevTemp",&status,verbosity,0);

	/**
	 * IRIO CLOSE DRIVER
	 */
	msgtest(5,irio_closeDriver);
	myStatus=irio_closeDriver(&p_DrvPvt,0, &status);
	msgerr(myStatus,5,"irio_closeDriver",&status,verbosity,0);
	return 0;
}
