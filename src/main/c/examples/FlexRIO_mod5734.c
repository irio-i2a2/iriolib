/******************************************************************************
 * $HeadURL:
 * $Id:
 *
 * Project		 : IRIO Driver
 *
 * Description	 : IRIO Driver Test Program for FlexRIO PXIe-7966R+NI5734 analog adapter module
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
#include <limits.h>

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
        printf("This example checks the data acquisition profile in the FlexRIO device. "
                "This example requires a 7966 FlexRIO device with a NI5734 adapter module\n"
        		"Connect the analog signal to CH0\n"
        		"Sampling rate is 120MS/s"
                "If OPEN NI-RIO Linux Version is installed: Use lsrio.py command to identify the RIO devices included in the fast controller\n"
                "If CLOSE NI-RIO Linux Version is installed: Use lsni -v command to identify the RIO devices included in the fast controller\n"        		"\n"
                "\n"
        		"Usage: %s <SERIAL_NUMBER> <RIOMODEL> <NI5734 Coupling AC=0,DC=1>\n"
                "Example: %s 01666C59 7966 0\n", name, name);
}

int main (int argc, char **argv)
{
	irioDrv_t p_DrvPvt;
	TStatus status;
	irio_initStatus(&status);
	int myStatus=0;
	int i=0;
	int valueReadI32=0;

	int auxAI9Value=0;
	int sampleCounter=0;
	int positiveTest=0;
	int negativeTest=0;

	
	int verbosity=1;

	char *filePath=NULL;
	char *bitfileName=NULL;
	char *NIriomodel=NULL;


	int DMATtoHOSTBlockNWords=0;
	int DMATtoHOSTNCh=0;
	int samplingrate=0;

	int numOfSamplesToShow=0;

	int coupling =0;
	if (argc != 4) {
			usage(argv[0]);
			return 1;
	}

	/**
	 * PATH BUILDING FOR THE BITFILE AND HEADER FILE
	 */
	asprintf(&filePath,"%s/resourceTest/%s/",get_current_dir_name(),argv[2]);
	asprintf(&bitfileName,"FlexRIOMod5734_%s",argv[2]);
	asprintf(&NIriomodel,"PXIe-%sR",argv[2]);

	uint64_t *dataBuffer=NULL;
	short int *auxDataBuffer=NULL;
	int elementsRead=0;
	dataBuffer=(uint64_t *)malloc(4096*8);///4096 data block size

	// initialize the RIO device calling the irio_initDriver
	msgtest(0,irio_initDriver);
	myStatus=irio_initDriver("RIO0",argv[1],NIriomodel,bitfileName,"V1.1",verbosity,filePath,filePath,&p_DrvPvt,&status);
	free(filePath);
	free(bitfileName);
	free(NIriomodel);
	msgerr(myStatus,0,"irio_initDriver",&status,verbosity,1);

	// start the execution of the FPGA
	msgtest(1,irio_setFPGAStart);
	myStatus=irio_setFPGAStart(&p_DrvPvt,1,&status);
	msgerr(myStatus,1,"irio_setFPGAStart",&status,verbosity,1);

	usleep(100);

	/**
	 * DEBUG MODE CONFIGURATION: OFF MODE
	 */
	msgtest(2,irio_setDebugMode & irio_getDebugMode);
	printf("[irio_setDebugMode function] DebugMode set to 0 (OFF)\n");
	myStatus=irio_setDebugMode(&p_DrvPvt,0,&status); // Debug mode set to OFF
	myStatus|=irio_getDebugMode(&p_DrvPvt,&valueReadI32,&status); // DebugMode FPGA register is read
	printf("[irio_getDebugMode function] DebugMode read %d\n",valueReadI32);
	msgerr(myStatus,2,"irio_setDebugMode & irio_getDebugMode",&status,verbosity,0);

	/**
	 * SIGNALTYPE FOR WG 0 CONFIGURATION: DC TYPE
	 */
	usleep(100);
	printf("\n##############################################################\n");
	printf("The purpose of tests: 3, 4, 5 and 6 consist of configuring internal waveform-generator 0 \n");
	printf("with a DC (digital value), and read the output of the waveform-generator from and auxiliary register. \n"); \
	printf("################################################################\n");
	msgtest(3,irio_setSGSignalType & irio_getSGSignalType);
	printf("[irio_setSGSignalType function] SGSignalType0 set to 0 (DC)\n");
	myStatus=irio_setSGSignalType(&p_DrvPvt,0,0,&status); // DC signal configured, value 0
	myStatus|=irio_getSGSignalType(&p_DrvPvt,0,&valueReadI32,&status);
	printf("[irio_getSGSignalType function] SGSignalType0 read %d\n",valueReadI32);
	msgerr(myStatus,3,"irio_setSGSignalType & irio_getSGSignalType",&status,verbosity,0);

   /**
	* AO0 (FROM WG0) IS CONFIGURED WITH 2048 DIGITAL DC VALUE
	*/
	usleep(100);
	msgtest(4,irio_setAO & irio_getAO);
	printf("[irio_setAO function] AO0 set to 2048 (digital value)\n");
	myStatus=irio_setAO(&p_DrvPvt,0,2048,&status); // Set AO terminal to 2048 digital value
	myStatus|=irio_getAO(&p_DrvPvt,0,&valueReadI32,&status);
	printf("[irio_getAO function] AO0 read %d\n",valueReadI32);
	msgerr(myStatus,4,"irio_setAO & irio_getAO",&status,verbosity,0);

	/**
	 * AO0_ENABLE IS ACTIVATED
	 */
	usleep(100);
	msgtest(5,irio_setAOEnable & irio_getAOEnable);
	printf("[irio_setAOEnable function] AOEnable0 set to 1 (ON)\n");
	myStatus=irio_setAOEnable(&p_DrvPvt,0,1,&status); // AO is enabled
	myStatus|=irio_getAOEnable(&p_DrvPvt,0,&valueReadI32,&status);
	printf("[irio_getAOEnable function] AOEnable0 read %d\n",valueReadI32);
	msgerr(myStatus,5,"irio_setAOEnable & irio_getAOEnable",&status,verbosity,0);

	usleep(100);

	//In this design the auxAI9 terminal has the last value written by the waveform generator implemented in the FPGA.
	//Above, we have configured the DC output and we have written a 2048, Therefore if we read auxAI9 the expected value is 2048

   /**
	* auxAI9 WILL BE READ, AND 2048 DIGITAL VALUE IS EXPECTED
	*/
	msgtest(6,irio_getAuxAI);
	myStatus=irio_getAuxAI(&p_DrvPvt,9,&auxAI9Value,&status);
	printf("The auxAI9 register must have the same value as the output of the signal generator: in this case 2048\n");
	printf("[irio_getAuxAI function] auxAI9 read %d\n",auxAI9Value);
	msgerr(myStatus,6,"irio_getAuxAI",&status,verbosity,0);
	/**
	 * DMA FUNCTION TESTS
	 */
	sampleCounter=0;
	positiveTest=0;
	negativeTest=0;
	elementsRead=0;

	/**
	 * DMA CLEANING
	 */
	msgtest(7,irio_cleanDMAsTtoHost);
	myStatus=irio_cleanDMAsTtoHost(&p_DrvPvt,&status); // DMA FIFOs are cleaned
	msgerr(myStatus,7,"irio_cleanDMAsTtoHost",&status,verbosity,0);
	usleep(100);

	msgtest(8,irio_setUpDMAsTtoHost);
	// All DMAs are configured. In this case there is only one DMA implemented in the FPGA with four channels (every channel has a digital value of 16 bits)
	myStatus=irio_setUpDMAsTtoHost(&p_DrvPvt,&status);
	msgerr(myStatus,8,"irio_setUpDMAsTtoHost",&status,verbosity,1);

	/**
	 * DMA SAMPLING RATE CONFIGURATION
	 */
	msgtest(9,irio_setDMATtoHostSamplingRate & irio_getDMATtoHostSamplingRate);
	printf("FPGA Clock reference (Fref value) is: %d Hz\n", p_DrvPvt.Fref);
	printf("[irio_setDMATtoHostSamplingRate function] Sampling rate for DMA0 set to %dS/s\n", 120000000); //sampling rate desired 1.2MS/s
	myStatus=irio_setDMATtoHostSamplingRate(&p_DrvPvt, 0, (p_DrvPvt.Fref/120000000), &status);
	// equation applied to set DMATtoHostSamplingRate: Fref/samplingRate=DecimationFactor
	// Where - Fref is p_DrvPvt.Fref, this value is read from FPGA by irioDriver initialization
	//		 - SamplingRate is the sampling rate desired to be configured
	//		 - decimationFactor, is the value configured in the FPGA to obtain the sampling rate desired
	// E.g., If you want 10000S/s then configure (p_DrvPvt.Fref/10000) in third parameter of irio_setDMATtoHostSamplingRate
 	myStatus|=irio_getDMATtoHostSamplingRate(&p_DrvPvt, 0, &valueReadI32, &status);
	printf("[irio_getDMATtoHostSamplingRate function] Sampling rate for DMA0 read %dS/s\n", p_DrvPvt.Fref/valueReadI32);
	msgerr(myStatus,9,"irio_setDMATtoHostSamplingRate & irio_getDMATtoHostSamplingRate",&status,verbosity,0);

	//Coupling configuration for NI5734 is not supported by IRIO
//	msgtest(10,irio_setAICoupling & irio_getAICoupling);
//	coupling=atoi(argv[3]);
//	printf("[irio_setAICoupling function] AICoupling set to %d (AC=0, DC=1)\n", coupling);
//	myStatus|=irio_setAICoupling(&p_DrvPvt,coupling, &status);
//	myStatus|=irio_getAICoupling(&p_DrvPvt, (TIRIOCouplingMode *)&coupling, &status);
//	printf("[irio_getAICoupling function] AICoupling read %d\n",coupling);
//	msgerr(myStatus,10,"irio_setAICoupling & irio_getAICoupling",&status,verbosity,0);


	// setting DC mode directly.
		coupling=atoi(argv[3]);
    // This part is setting the DC coupling for channel 0 only
		myStatus=irio_setAuxAO(&p_DrvPvt,6,3,&status); //3 is the command to change the DC coupling
		myStatus=irio_setAuxAO(&p_DrvPvt,7,0,&status); //1 is the channel to setup; 0 in this example
		myStatus=irio_setAuxAO(&p_DrvPvt,8,coupling,&status); //1 is DC mode
		myStatus=irio_setAuxDO(&p_DrvPvt,6,0,&status); //Rising edge generation
		myStatus=irio_setAuxDO(&p_DrvPvt,6,1,&status); //Rising edge generation

	/**
	 * DMA ENABLE
	 */
	samplingrate=p_DrvPvt.Fref/valueReadI32;
	msgtest(11,irio_setDMATtoHostEnable & irio_getDMATtoHostEnable);
	printf("[irio_setDMATtoHostEnable function] DMATtoHostEnable0 set to 1 (ON)\n");
	myStatus=irio_setDMATtoHostEnable(&p_DrvPvt,0,1,&status); //DMA data transfer to Host is activated
	myStatus|=irio_getDMATtoHostEnable(&p_DrvPvt,0,&valueReadI32,&status);
	printf("[irio_getDMATtoHostEnable function] DMATtoHostEnable0 read %d\n",valueReadI32);
	msgerr(myStatus,11,"irio_setDMATtoHostEnable & irio_getDMATtoHostEnable",&status,verbosity,1);

	/**
	 * DAQ START
	 */
	usleep(100000);
	usleep(100000);
	usleep(100000);

	msgtest(12,irio_setDAQStartStop irio_getDAQStartStop);
	printf("[irio_setDAQStartStop function] DAQStartStop set to 1 (ON)\n");
	myStatus=irio_setDAQStartStop(&p_DrvPvt,1,&status); // Data acquisition is started
	myStatus|=irio_getDAQStartStop(&p_DrvPvt,&valueReadI32,&status);
	printf("[irio_getDAQStartStop function] DAQStartStop read %d\n",valueReadI32);
	msgerr(myStatus,12,"irio_setDAQStartStop irio_getDAQStartStop",&status,verbosity,1);
    DMATtoHOSTBlockNWords=p_DrvPvt.DMATtoHOSTBlockNWords[0];
	DMATtoHOSTNCh=p_DrvPvt.DMATtoHOSTNCh[0];

	sampleCounter=0;
	elementsRead=0;
	
	

	/**
    * LOOP FOR ACQUIRING 1 BLOCK AND ONLY SHOW 51 SAMPLES
	*/

	do{
		myStatus=irio_getDMATtoHostData(&p_DrvPvt,1,0,dataBuffer, &elementsRead,&status);
		if (myStatus!=IRIO_success)
		{
				msgerr(myStatus,18,"irio_getDMATtoHostData & irio_setDAQStartStop",&status,verbosity,1);
		}
		if(elementsRead==1)
		{
			numOfSamplesToShow=200; // 50 Samples must represent 1 sine cycle. 10e more are required to check that entire cycle is acquired
			auxDataBuffer=(short int *)dataBuffer;
			sampleCounter++;
			for (i=0;i<numOfSamplesToShow;i++) //only one part of the block is displayed to simplify the output
			printf("Sample[%02d]=%+f\n", i, (double)(auxDataBuffer[(i*DMATtoHOSTNCh)+0])*p_DrvPvt.CVADC); //one period has 100 samples
			//We are displying only samples from CH0!!!!
		}
		else
		{
			usleep(((1/(double)samplingrate)*DMATtoHOSTBlockNWords)*500000);// we wait at least half the duration of the block in microseconds
		}
	}while (sampleCounter<1);
	msgerr(myStatus,18,"irio_getDMATtoHostData & irio_setDAQStartStop",&status,verbosity,0);
	myStatus=irio_setDAQStartStop(&p_DrvPvt,0,&status); // Data acquisition is stopped

	/**
	 * IRIO CLOSE DRIVER
	 */
	msgtest(19,irio_closeDriver);
	myStatus=irio_closeDriver(&p_DrvPvt,0, &status);
	msgerr(myStatus,19,"irio_closeDriver",&status,verbosity,0);

	return 0;
}
