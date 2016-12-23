/******************************************************************************
 * $HeadURL:
 * $Id:
 *
 * Project		 : IRIO Driver
 *
 * Description	 : IRIO Driver Test Program for FlexRIO performance test. Adapter module is not required
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
#include <pthread.h>
#include <unistd.h>
#include "NiFpga.h"
#include <sys/time.h>

#include <irioDriver.h>
#include <irioDataTypes.h>
#include <irioHandlerAnalog.h>
#include <irioHandlerDigital.h>
#include <irioHandlerDMA.h>
#include <irioHandlerImage.h>
#include <irioHandlerSG.h>
#include <irioError.h>
#include <irioResourceFinder.h>
#include <errno.h>


typedef struct{
	irioDrv_t *p_Drv;
	int flagToFinish;
	int threadReady;
	long int blocksRead;
	uint64_t *DBuffer;
}threadData_t;

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

static void *DMAthread(void *p);

void usage(char *name) {
        printf("This example checks the performance of the FlexRIO device\n"
                "If OPEN NI-RIO Linux Version is installed: Use lsrio.py command to identify the RIO devices included in the fast controller\n"
                "If CLOSE NI-RIO Linux Version is installed: Use lsni -v command to identify the RIO devices included in the fast controller\n"        		"\n"
                "\n"
        		"Usage: %s <SERIAL_NUMBER> <RIOMODEL> \n"
                "Example: %s 01666C59 7966 \n", name, name);
}

/**********************************************************************************************/
 // Use this function in case of being necessary for other possible Tests, for obtain diff time
/**********************************************************************************************/
//int timeval_subtract (result, x, y)
//     struct timeval *result, *x, *y;
//{
//  if (x->tv_usec < y->tv_usec) {
//    int nsec = (y->tv_usec - x->tv_usec) / 1000000 + 1;
//    y->tv_usec -= 1000000 * nsec;
//    y->tv_sec += nsec;
//  }
//  if (x->tv_usec - y->tv_usec > 1000000) {
//    int nsec = (x->tv_usec - y->tv_usec) / 1000000;
//    y->tv_usec += 1000000 * nsec;
//    y->tv_sec -= nsec;
//  }
//  result->tv_sec = x->tv_sec - y->tv_sec;
//  result->tv_usec = x->tv_usec - y->tv_usec;
//
//  return x->tv_sec < y->tv_sec;
//}


int main (int argc, char **argv)
{
	irioDrv_t p_DrvPvt;
	TStatus status;
	irio_initStatus(&status);
	int myStatus=0;
	pthread_t thread;
	threadData_t data;
	int accIncr=0;

	int DMAsOverflow=0;
	int verbosity=1;

//	struct sched_param sched_param_data; // Uncomment this in case of use thread parameter configuration

//	pthread_attr_t threadAttr;

	char *filePath=NULL;
	char *bitfileName=NULL;
	char *NIriomodel=NULL;

	if (argc != 3)
	{
		usage(argv[0]);
		return 1;
	}

	bitfileName=(char*)malloc(128);

	asprintf(&filePath,"%s/resourceTest/%s/",get_current_dir_name(),argv[2]);
	asprintf(&NIriomodel,"PXIe-%sR",argv[2]);
	asprintf(&bitfileName,"FlexRIO_perf_%s",argv[2]);


	msgtest(0, irio_initDriver);
	myStatus=irio_initDriver("RIOT",argv[1],NIriomodel,bitfileName,"V1.1",1,filePath,filePath,&p_DrvPvt,&status);
	msgerr(myStatus,0,"irio_initDriver",&status,verbosity,1);

	//FPGA START
	msgtest(1, irio_setFPGAStart);
	myStatus=irio_setFPGAStart(&p_DrvPvt,1,&status);
	usleep(100);
	msgerr(myStatus,1,"irio_setFPGAStart",&status,verbosity,1);

	msgtest(2, irio_setDebugMode);
	myStatus=irio_setDebugMode(&p_DrvPvt,1,&status); // Debug mode set to ON
	msgerr(myStatus,2,"irio_setFPGAStart",&status,verbosity,0);
	usleep(100);

	// Signal configured to be generated y(t)=4096sin(2*pi*1000*t)
	msgtest(3, irio_setSGSignalType);
	myStatus=irio_setSGSignalType(&p_DrvPvt,0,1,&status); // Sine signal configured, value 1
	msgerr(myStatus,3,"irio_setSGSignalType",&status,verbosity,0);
	usleep(100);

	msgtest(4, irio_setSGUpdateRate);
	myStatus=irio_setSGUpdateRate(&p_DrvPvt,0,10,&status); // (Fref/SGUpdateRate)=S/s e.g., the value 10 means 10MS/s
	msgerr(myStatus,4,"irio_setSGUpdateRate",&status,verbosity,0);
	usleep(100);

	msgtest(5, irio_setSGFreq);
	accIncr=100*(4294967296.0/10000000.0);
	myStatus=irio_setSGFreq(&p_DrvPvt,0,accIncr,&status); //SGFreq=(freq_desired*(2to32))/(Fref/SGUpdateRate))
	msgerr(myStatus,5,"irio_setSGFreq",&status,verbosity,0);
	usleep(100);

	msgtest(6, irio_setSGAmp);
	myStatus=irio_setSGAmp(&p_DrvPvt,0,4096,&status);
	msgerr(myStatus,6,"irio_setSGAmp",&status,verbosity,0);
	usleep(100);
	/*****************************************************************************/

	msgtest(7, irio_setAOEnable);
	myStatus=irio_setAOEnable(&p_DrvPvt,0,1,&status); // AO enable
	msgerr(myStatus,7,"irio_setAOEnable",&status,verbosity,0);
	usleep(100);

	msgtest(8, irio_cleanDMAsTtoHost);
	irio_cleanDMAsTtoHost(&p_DrvPvt,&status); // DMA FIFOs are cleaned
	msgerr(myStatus,8,"irio_cleanDMAsTtoHost",&status,verbosity,0);
	usleep(100);

	msgtest(9, irio_setUpDMAsTtoHost);
	myStatus=irio_setUpDMAsTtoHost(&p_DrvPvt,&status); // DMAs are configured
	msgerr(myStatus,9,"irio_setUpDMAsTtoHost",&status,verbosity,1);
	usleep(100);

	msgtest(10, irio_setDMATtoHostSamplingRate);
	myStatus=irio_setDMATtoHostSamplingRate(&p_DrvPvt,0,1000,&status); // DMA SamplingRate is configured to Fref/1000-->100kS/s | Fref=100MHz
	msgerr(myStatus,10,"irio_setDMATtoHostSamplingRate",&status,verbosity,0);
	usleep(100);

	msgtest(11, irio_setDMATtoHostEnable);
	myStatus=irio_setDMATtoHostEnable(&p_DrvPvt,0,1,&status); //DMA data transfer to Host is activated
	msgerr(myStatus,11,"irio_setDMATtoHostEnable",&status,verbosity,1);
	usleep(100);


	data.p_Drv=&p_DrvPvt;
	data.flagToFinish=0;
	data.blocksRead=0;
	data.threadReady=0;
	data.DBuffer=(uint64_t*)malloc(4096*8*10);

	//In case of required to tune the thread uncomment this sentences and apply values

//	pthread_attr_init(&threadAttr);
//	sched_param_data.sched_priority=90;
//	pthread_attr_setschedpolicy(&threadAttr,SCHED_FIFO);
//	pthread_attr_setschedparam(&threadAttr,&sched_param_data);
//	pthread_create(&thread,&threadAttr,&DMAthread,&data);
	pthread_create(&thread,NULL,&DMAthread,&data);
	do{
		usleep(1000);
	}while(!data.threadReady);
	printf("\n\n\n***  PERFORMANCE TEST ***\n");
	printf("This test pretends to show that the IRIO library is able to overcome the throughput of 750MB/s \n\n");

	msgtest(12, Performance Test 1);
	printf("\n\nIRIO Performance Test 1. Bandwidth tested: 800kB/s during 5 seconds\n");

	myStatus=irio_setDAQStartStop(&p_DrvPvt,1,&status); // Data acquisition is started
	if(myStatus!=IRIO_success)
	{
		msgerr(myStatus,12,"Performance Test 1",&status,verbosity,1);
	}
	sleep(5); // 5 Seconds to wait while the thread acquires data.
/********************************************* Another Way to do the Test HEAD ***********************/
//	myStatus=irio_setDMATtoHostSamplingRate(&p_DrvPvt,0,1,&status); // DMA SamplingRate is configured to Fref/2-->62.5MS/s | Fref=125MHz
//	if(myStatus!=0)
//	{
//		irio_resetStatus(&status);
//		printf("iRIOCore test 3. irio_setDMATtoHostSamplingRate function unsuccessful execution [ERROR] %d\n",myStatus);fflush(stdout);
//		return 0;
//	}
//	int freqconf=0;
//	myStatus=irio_getDMATtoHostSamplingRate(&p_DrvPvt,0,&freqconf,&status); // DMA SamplingRate is configured to Fref/2-->62.5MS/s | Fref=125MHz
//	if(myStatus!=0)
//	{
//		irio_resetStatus(&status);
//		printf("iRIOCore test 3. irio_setDMATtoHostSamplingRate function unsuccessful execution [ERROR] %d\n",myStatus);fflush(stdout);
//		return 0;
//	}
//	uint64_t *dataBuffer;
//
//	dataBuffer = malloc((sizeof(uint64_t))*4096*10);
//	short int *bufferI16;
//	struct timeval t_end, t_start, result;
//	long long unsigned int t_diff = 0;
//	printf(" Test 4. Bandwidth tested: 500MB/s during 10 seconds. \n");
//	irio_setDAQStartStop(&p_DrvPvt,1,&status); // Data acquisition is started
//	gettimeofday(&t_start, NULL);
//
//	do{
//		myStatus=irio_getDMATtoHostData(&p_DrvPvt,10,0,dataBuffer, &elementsRead,&status);
//		if(myStatus==IRIO_success)
//			{
//				if (elementsRead==10)
//				{
//					bufferI16=(short int *)dataBuffer;
//					data.blocksRead=data.blocksRead+elementsRead;
//					printf("\n Blocks read:%d\n",data.blocksRead);
//				}
//				else
//				{
//					usleep(10);
//				}
//			}
//			else
//			{
//				irio_resetStatus(&status);
//				printf("DEBUG:Error on irio_getDMATtoHostData:%d\n",myStatus);
//			}
//		}while(data.blocksRead<50000);
//	gettimeofday(&t_end, NULL);
//	irio_setDAQStartStop(&p_DrvPvt,0,&status);
//
//	timeval_subtract(&result, &t_end, &t_start);
//
//	t_diff = (long long unsigned int)result.tv_sec * 1E6 + (long long unsigned int)result.tv_usec;
//	printf("Test 4. Througput: %.2f . diff time:%d \n",((float)(data.blocksRead*8.0*4096.0))/((float)(t_diff)),t_diff);fflush(stdout);
//	free(dataBuffer);
//
//
//
//	irio_getDMATtoHostOverflow(&p_DrvPvt, &DMAsOverflow , &status);
//	if (DMAsOverflow==0)
//	{
//			printf(" Test 4. Bandwidth tested:500MB/s [OK]. data transfered:%.2fMB\n",(((data.blocksRead)*4096.0)*8)/1000000);
//	}
//	else
//	{
//		printf(" Test 4. Bandwidth tested: 500MB/s [ERROR]. DMA Overflow with data loss\n");
//	}
	/********************************************* Another Way to do the Test FOOT ***********************/


	myStatus|=irio_setDAQStartStop(&p_DrvPvt,0,&status); // Data acquisition is started
	myStatus|=irio_getDMATtoHostOverflow(&p_DrvPvt, &DMAsOverflow , &status);


	if (DMAsOverflow==0)
	{
		printf("IRIO performance Test 1. 800kB/s during 5 seconds, 4MB of data reception are expected\n");
		printf("IRIO performance Test 1. Bandwidth tested: 800kB/s [OK]. data transfered:%.2fMB\n\n",(((data.blocksRead)*4096.0)*8)/1000000);
	}
	else
	{
		irio_mergeStatus(&status,Generic_Error,verbosity,"IRIO performance Test 1. Bandwidth tested: 800kB/s [ERROR]. DMA Overflow with data loss\n");
	}
	msgerr(myStatus,12,"Performance Test 1",&status,verbosity,1);

	//TODO: Clean DMAs Overflow register

	msgtest(13, Performance Test 2);
	printf("IRIO performance Test 2. Bandwidth tested: 8MB/s during 10 seconds\n");
	myStatus=irio_cleanDMAsTtoHost(&p_DrvPvt,&status); // DMA FIFOs are cleaned
	usleep(100);
	myStatus|=irio_setDMATtoHostSamplingRate(&p_DrvPvt,0,100,&status); // DMA SamplingRate is configured to 100MHz/100
	usleep(100);
	data.blocksRead=0;
	myStatus|=irio_setDAQStartStop(&p_DrvPvt,1,&status); // Data acquisition is started
	if(myStatus!=IRIO_success)
	{
		msgerr(myStatus,13,"Performance Test 2",&status,verbosity,1);
	}

	sleep(10);
	irio_setDAQStartStop(&p_DrvPvt,0,&status); // Data acquisition is started
	irio_getDMATtoHostOverflow(&p_DrvPvt, &DMAsOverflow , &status);
	if (DMAsOverflow==0)
	{
		printf("IRIO performance Test 2. 8MB/s during 10 seconds, 80MB are expected\n");
		printf("IRIO performance Test 2. Bandwidth tested: 8MB/s [OK]. data transfered:%.2fMB \n\n",((data.blocksRead)*4096.0*8)/1000000);
	}
	else
	{
		irio_mergeStatus(&status,Generic_Error,verbosity,"IRIO performance Test 2. Bandwidth tested: 8MB/s [ERROR]. DMA Overflow with data loss \n");
	}
	msgerr(myStatus,13,"Performance Test 2",&status,verbosity,1);

	//TODO: Clean DMAs Overflow register
	msgtest(14, Performance Test 3);
	printf("IRIO performance Test 3. Bandwidth tested: 80MB/s  during 30 seconds\n");
	myStatus=irio_cleanDMAsTtoHost(&p_DrvPvt,&status); // DMA FIFOs are cleaned
	usleep(100);
	myStatus|=irio_setDMATtoHostSamplingRate(&p_DrvPvt,0,10,&status); // DMA SamplingRate is configured to 100/10-->10MS/s
	usleep(100);
	data.blocksRead=0;
	myStatus|=irio_setDAQStartStop(&p_DrvPvt,1,&status); // Data acquisition is started
	if(myStatus!=IRIO_success)
	{
		msgerr(myStatus,14,"Performance Test 3",&status,verbosity,1);
	}

	sleep(30);
	irio_setDAQStartStop(&p_DrvPvt,0,&status); // Data acquisition is started
	irio_getDMATtoHostOverflow(&p_DrvPvt, &DMAsOverflow , &status);
	if (DMAsOverflow==0)
	{
		printf("IRIO performance Test 3. 80MB/s during 30 seconds, 2400MB are expected\n");
		printf("IRIO performance Test 3. Bandwidth tested: 80MB/s [OK]. data transfered:%.2fMB \n\n",((8*(data.blocksRead)*4096.0))/1000000);
	}
	else
	{
		irio_mergeStatus(&status,Generic_Error,verbosity,"IRIO performance Test 3. Bandwidth tested: 100MB/s [ERROR]. DMA Overflow with data loss \n\n");
	}
	msgerr(myStatus,14,"Performance Test 3",&status,verbosity,1);

/********************************************************************************************************************************/
	//TODO: Clean DMAs Overflow register
	msgtest(15, Performance Test 4);
	printf("IRIO performance Test 4. Bandwidth tested: 800MB/s during 60 seconds. Throughput expected> 750MB/s \n");
	myStatus=irio_cleanDMAsTtoHost(&p_DrvPvt,&status); // DMA FIFOs are cleaned
	usleep(100);
	myStatus|=irio_setDMATtoHostSamplingRate(&p_DrvPvt,0,1,&status);
	usleep(100);
	data.blocksRead=0;
	myStatus|=irio_setDAQStartStop(&p_DrvPvt,2,&status); // Data acquisition is started
	if(myStatus!=IRIO_success)
	{
		msgerr(myStatus,15,"Performance Test 4",&status,verbosity,1);
	}
	sleep(60);
	irio_setDAQStartStop(&p_DrvPvt,0,&status); // Data acquisition is started
	irio_getDMATtoHostOverflow(&p_DrvPvt, &DMAsOverflow , &status);
	if (DMAsOverflow==0)
	{
		printf("IRIO performance Test 4. Bandwidth tested: 800MB/s [OK]. Througput:%.2fMB/s \n",((8*(data.blocksRead)*4096.0)/60000000.0));
	}
	else
	{
		irio_mergeStatus(&status,Generic_Error,verbosity,"IRIO performance Test 4. Bandwidth tested: 800MB/s [ERROR]. DMA Overflow with data loss. Througput: %.2f MB/s \n\n",((8*(data.blocksRead)*4096.0)/60000000.0));
	}
	msgerr(myStatus,15,"Performance Test 4",&status,verbosity,1);

	data.flagToFinish=1;
	pthread_join(thread,NULL);

	irio_closeDriver(&p_DrvPvt,0, &status);

	free(bitfileName);
	free(data.DBuffer);
	return 0;
}



/**********TEST PERF BUG **************/

static void *DMAthread(void *p)
{
	threadData_t *data=(threadData_t *)p;
	TStatus status;
	status.msg=NULL;
	status.detailCode=Success;
	status.code=IRIO_success;
//	uint64_t *dataBuffer;
	int elementsRead=0;


//	dataBuffer=malloc((sizeof(uint64_t))*4096*10);


	data->threadReady=1;
	do{
		int myStatus=0;
		myStatus=irio_getDMATtoHostData(data->p_Drv,10,0,data->DBuffer, &elementsRead,&status);
		if(myStatus==IRIO_success)
		{
			if (elementsRead==10)
			{
				short int *bufferI16;
				bufferI16=(short int *)data->DBuffer;

				data->blocksRead=data->blocksRead+elementsRead;

// This is another way to check integrity. Take into account data expected from FPGA.

//				if(bufferI16[(1024*4)+3]==1024 && bufferI16[(1024*4)+2]==1029 && bufferI16[(1024*4)+1]==1034 && bufferI16[(1024*4)]==1039)
//				{
//					data->blocksRead=data->blocksRead+elementsRead;
//					//			irio_getDMATtoHostOverflow(data->p_Drv, &DMAsOverflow , &status);
//					//			if (DMAsOverflow!=0)
//					//			{
//					//				printf(" DMA Overflow. data read:%.2fMB, Blocks read:%d\n",((SECONDS_TEST_400MBS*(data->blocksRead)*4.0)/1000),(data->blocksRead));
//					//				fflush(stdout);
//					//				pthread_exit(NULL);
//					//			}
//				}
//				else
//				{
//					printf(" Data loss has been detected. Blocks read:%ld \n",data->blocksRead);
//					fflush(stdout);
//					pthread_exit(NULL);
//				}

			}
			else
			{
				usleep(10);
			}
		}
		else
		{
			printf("DEBUG:Error on irio_getDMATtoHostData:%d\n",myStatus);
		}


	}while(data->flagToFinish==0);

	irio_resetStatus(&status);

	return NULL;
}
