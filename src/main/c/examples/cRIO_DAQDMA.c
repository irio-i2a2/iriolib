/******************************************************************************
 * $HeadURL:
 * $Id:
 *
 * Project		 : IRIO Driver
 *
 * Description	 : IRIO driver Test Program for cRIO devices using Data Acquisition profile
 *
 * Author		 : Universidad Politécnica de Madrid (UPM) (Technical University of Madrid)
 *
 * Copyright (c) : 2010-2015 Universidad Politécnica de Madrid (UPM)(Technical University of Madrid)
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

#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>

#include <fcntl.h>
#include <math.h>
#include <errno.h>

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
					printf("####################################################\n"); \
					printf("IRIO test" #n " " #function " function\\s\n"); \
					printf("####################################################\n")

void msgerr(TIRIOStatusCode code, int nTest, const char* testName, TStatus* status,int verbosity, int exitOnFailure){
	if(code==IRIO_success){
		printf("IRIO test %d %s function\\s successful execution [OK] \n\n",nTest,testName);
		fflush(stdout);
	}else{
		char* detailStr=NULL;
		irio_getErrorString(status->detailCode,&detailStr);
		printf("[ERROR] IRIO test %d %s function\\s unsuccessful execution. Error Code: %d (%s) \n",nTest,testName,status->detailCode,detailStr);
		if(verbosity==0){
			printf("\n\tIRIO ERRORS:\n %s",status->msg);
		}else{
			printf("\nCheck previous messages for more detailed information of the error\n");
		}
		free(detailStr);
		fflush(stdout);
		irio_resetStatus(status);

		if(exitOnFailure){
			exit(-1);
		}
	}
}

/**
 * This function is used to print out the instructions for using this application
 */
void usage(char *name) {
        printf("This example checks the use of cRIO DAQ profile. This example requires a specific chassis configuration.\n"
                "If OPEN NI-RIO Linux Version is installed: Use lsrio.py command to identify the RIO devices included in the fast controller\n"
                "If CLOSE NI-RIO Linux Version is installed: Use lsni -v command to identify the RIO devices included in the fast controller\n"        		"\n"
                "\n"
        		"Below there is information about the input parameters required: \n"
        		"\t<Serial_Number>: Device serial number, uses lsrio.py/lsni -v command to get it.\n"
        		"\t<cRIO_model>: Device model, uses lsrio.py/lsni -v command to get it.\n"
        		"\t<debug_mode>: configures real or simulated acquisition; debug_mode=1-> analog input 0 and 1 will have +5.0 and -5.0.\n"
        		"\t<voltage_NI9264_CH0>: value in volts for configuring NI9264 CH0 output.\n"
        		"\t<voltage_NI9264_CH1>: value in volts for configuring NI9264 CH1 output.\n"
                "\t<Total samples_to_read>: Total number of samples to be read when data acquisition_mode is finite. Min: blocksize implemented in the FPGA\n"
                "\t<Acquisition_mode>: Acquisition mode can be finite (0) or continuous (1). Continuous is is stopped with Ctrl+C\n"
        		"\t<block length>: Number of samples to be read from data acquisition device. Minimun: blocksize\n"
                "\n"
                "Usage: %s <s/n> <Serial_Number> <cRIO_model> <debug_mode> <voltage_NI9264_CH0> <voltage_NI9264_CH0> <Total_samples_to_read> <Acquisition_mode> <block_length>\n"
                "Example: %s 0234CBAC 9159 +5.0 -5.0 128 0 128\n", name, name);
}




int flagExit=0; //Global variable used for checking if continuous acquisition must be stopped.
/**
 *  SIGNAL handler. In case of user type Ctrl+C while acquisition is continuous, then acquisition stops, and program continues
 */
static void sighandler (int signal)
{
	switch (signal)
	{
	case SIGINT:
		flagExit = 1;
		break;
	default:
		break;
	}
}
/**
 * Main Test program.
 * Input parameters expected:
 * Param 1<s/n>: Device serial number, uses lsrio.py command to get it.
 * Param 2<cRIO model>: Device model, uses lsrio.py command to get it.
 * Param 3<debug_mode>: configures real or simulated acquisition; debug_mode=1-> analog input 0 and 1 will have +5.0 and -5.0
 * Param 4<voltage_NI9264_CH0>: value in volts for configuring NI9264 CH0 output
 * Param 5<voltage_NI9264_CH1>: value in volts for configuring NI9264 CH1 output
 * Param 6<samples_to_read>: Number of samples to be read when acquisition_mode parameter is 0 (finite acquisition)
 * Param 7<acquisition_mode>: Acquisition mode can be finite: this is with value 0, or it can be continuous: this is with value 1. to stop it use Ctrl+C
 * Param 8<N_blocks>: When acquisition mode is continuous, N_blocks indicates the number of blocs that are read from DMA FIFO (Host side) at once.
 */
int main(int argc, char **argv)
{
	irioDrv_t p_DrvPvt;

	TStatus status;
	irio_initStatus(&status);
	TIRIOStatusCode st=IRIO_success;

	char *bitFilePath;
	uint64_t *data;
	uint64_t *data2;
	size_t valueLength;
	int32_t aivalues;
	int elementsRead;
	int errorFounds=0;

	char *bitfileName;
	int continueAcq = 0;
	int TotalsamplesToAcq=0;

	int blocksize=0;
	int nBlocksPerDMAAcc=0;

	if (argc != 9) {
	    usage(argv[0]);
	    return (-1);
	}

	int verbosity=1;

	char *NIcriomodel;

	asprintf(&bitfileName,"cRIODAQDMA_%s",argv[2]);
	asprintf(&NIcriomodel,"NI %s",argv[2]);
	asprintf(&bitFilePath,"%s/resourceTest/%s/",get_current_dir_name(),argv[2]);

	//Signal handler registration. It permits stops continuous acquisition with Ctrl+C
	signal(SIGINT,sighandler);

	//CHASSIS1 N/S: 19ED079 3x9205, 1x9264, 1x9401 1x9426 2x9477
	msgtest(0,irio_initDriver);
	st|=irio_initDriver("test",argv[1],NIcriomodel,bitfileName,"V1.1",verbosity,bitFilePath,bitFilePath,&p_DrvPvt,&status);
	free(bitfileName);bitfileName=NULL;
	free(NIcriomodel);NIcriomodel=NULL;
	free(bitFilePath);bitFilePath=NULL;
	msgerr(st,0,"irio_initDriver",&status,verbosity,1);


	msgtest(1,irio_setUpDMAsTtoHost);
	st|=irio_setUpDMAsTtoHost(&p_DrvPvt,&status);
	msgerr(st,1,"irio_setUpDMAsTtoHost",&status,verbosity,1);

	data=malloc(100000*sizeof(uint64_t));
	st|=irio_cleanDMATtoHost(&p_DrvPvt,0,data, sizeof(data),&status);
	free(data);
	msgerr(st,1,"irio_cleanDMATtoHost",&status,verbosity,1);

	/**
	 * FPGA START
	 */

	msgtest(2,irio_setFPGAStart & irio_getFPGAStart);
	printf("[irio_setFPGAStart function] FPGA hardware logic is started (\"Running\") Value %d \n", 1);
	st|=irio_setFPGAStart(&p_DrvPvt,1, &status);
	st|=irio_getFPGAStart(&p_DrvPvt, &aivalues,&status);
	printf("[irio_getFPGAStart function] FPGA State is: %d . 1-->\"running\" 0-->\"stopped\"\n", aivalues);
	msgerr(st,2,"irio_setFPGAStart & irio_getFPGAStart",&status,verbosity,1);



	msgtest(3,irio_setDMATtoHostSamplingRate & irio_getDMATtoHostSamplingRate);
	printf("[irio_setDMATtoHostSamplingRate] SamplingRate for DMA 0 set to 1. This means Fref/1: %dS/s\n",p_DrvPvt.Fref);
	st|=irio_setDMATtoHostSamplingRate(&p_DrvPvt,0,1, &status);
	float ts=1/(float)(p_DrvPvt.Fref);
	st|=irio_getDMATtoHostSamplingRate(&p_DrvPvt,0,&aivalues,&status);
	printf("[irio_getDMATtoHostSamplingRate] SamplingRate for DMA 0 read %d. This means %dS/s\n", aivalues, aivalues*p_DrvPvt.Fref);
	msgerr(st,3,"irio_getDMATtoHostSamplingRate & irio_getDMATtoHostSamplingRate",&status,verbosity,0);


	/**
	 * DEVICE INFORMATION READ FROM FPGA
	 */
	msgtest(4,irio_getFPGAVIVersion irio_getDevQualityStatus irio_getDevTemp irio_getDevProfile);
	char *value;
	value=calloc(20,1);
	st|=irio_getFPGAVIVersion(&p_DrvPvt,value,10, &valueLength,&status);
	printf("[irio_getFPGAVIVersion function] VI version read from FPGA: %s\n",value);
	free(value);
	st|=irio_getDevQualityStatus(&p_DrvPvt,&aivalues,&status);
	printf("[irio_getDevQualityStatus function] Device Quality Status read from FPGA %d \n", aivalues);
	st|=irio_getDevTemp(&p_DrvPvt,&aivalues,&status);
	printf("[irio_getDevTemp function] Device Temperature read from FPGA %.2fºC \n", aivalues*0.25);
	st|=irio_getDevProfile(&p_DrvPvt,&aivalues,&status);
	printf("[irio_getDevProfile function] Device Profile configured in the FPGA is %d. This Device should be 0. 0-> DAQ DMA, ->1 Point by Point\n", aivalues);
	msgerr(st,4,"irio_getDevProfile",&status,verbosity,0);

	/**
	 * WAVEFORM GENERATOR CONFIGURATION
	 */
	msgtest(5,signal generator configuration (Pre DAQ Start));
	printf("Test 5 checks the following functions: irio_setDebugMode irio_getDebugMode irio_setAO irio_getAO ");
	printf("irio_setDAQStartStop irio_getDAQStartStop irio_setSGUpdateRate irio_setSGFreq irio_getSGFreq irio_setSGSignalType ");
	printf("irio_getSGSignalType irio_setSGAmp irio_getSGAmp irio_setSGPhase irio_getSGPhase irio_setAOEnable irio_getAOEnable\n\n");
	printf("[irio_setDebugMode function] Debug Mode set to %d. 0-->OFF, 1-->ON \n", atoi(argv[3]));
	st|=irio_setDebugMode(&p_DrvPvt,atoi(argv[3]),&status);
	st|=irio_getDebugMode(&p_DrvPvt,&aivalues,&status);
	printf("[irio_getDebugMode function] DebugMode read %d \n", aivalues);

	//// Using analog input and outputs
	printf("[irio_setAO function] AO0 set to %d, meaning %.1fV\n",(int)(atof(argv[4])*p_DrvPvt.CVDAC), atof(argv[4]));
	st|=irio_setAO(&p_DrvPvt,0,atof(argv[4])*p_DrvPvt.CVDAC,&status);
	st|=irio_getAO(&p_DrvPvt,0,&aivalues,&status);
	printf("[irio_getAO function] AO0 read %d, meaning %.1fV\n",aivalues, aivalues/p_DrvPvt.CVDAC);

	usleep(10000);


	printf("[irio_setAO function] AO1 set to %d, meaning %.1fV\n",(int)(atof(argv[5])*p_DrvPvt.CVDAC), atof(argv[5]));
	st|=irio_setAO(&p_DrvPvt,1,atof(argv[5])*p_DrvPvt.CVDAC,&status);
	st|=irio_getAO(&p_DrvPvt,1,&aivalues,&status);
	printf("[irio_getAO function] AO1 read %d, meaning %.1fV\n",aivalues, aivalues/p_DrvPvt.CVDAC);
	msgerr(st,5,"signal generator configuration (Pre DAQ Start)",&status,verbosity,0);

	usleep(10000);

	msgtest(6,DAQ Start);
	printf("[irio_setDAQStartStop function] DAQStartStop set to 1 (ON) \n");
	st|=irio_setDAQStartStop(&p_DrvPvt,1,&status);
	st|=irio_getDAQStartStop(&p_DrvPvt,&aivalues,&status);
	printf("[irio_getDAQStartStop function] DAQStartStop read %d \n",aivalues);
	msgerr(st,6,"DAQ Start",&status,verbosity,1);

	//Programming the signal generator 0. 100Hz, 1000S/s, 5V
	msgtest(7,signal generator configuration (Post DAQ Start));
	printf("[irio_setSGUpdateRate function] SGUpdateRate0 set to 10, meaning %dS/s\n",p_DrvPvt.SGfref[0]/10);
	st|=irio_setSGUpdateRate(&p_DrvPvt,0,10,&status);
	st|=irio_getSGUpdateRate(&p_DrvPvt,0,&aivalues,&status);
	printf("[irio_getSGUpdateRate function] SGUpdateRate0 read %d, meaning %dS/s\n", aivalues,p_DrvPvt.SGfref[0]/aivalues);

	int updaterate=p_DrvPvt.SGfref[0]/10;

	// we want program signal generator with 100Hz, periodic signal
	// equation to apply to obtain freq_desired is: SGFreq=freq_desired*( (2to32) / ( SGFref/(S/s)) )
	int accIncr=100*(UINT_MAX/updaterate); //100 Hz sampled with 1kHz means 10 samples per cycle
	printf("[irio_setSGFreq function] SGfreq 0 set to %d, meaning %dHz \n",accIncr,100);
	st|=irio_setSGFreq(&p_DrvPvt,0,accIncr,&status);
	st|=irio_getSGFreq(&p_DrvPvt,0,&aivalues,&status);
	printf("[irio_getSGFreq function] SGFreq 0 read %d, meaning %dHz \n",aivalues, aivalues/(UINT_MAX/(1000)));

	printf("[irio_setSGSignalType function] SGSignalType 0 set to 2 (SQUARE) \n");
	st|=irio_setSGSignalType(&p_DrvPvt,0,2,&status); //0=DC, 1=SINE 2=SQUARE 3=TRIANGULAR
	st|=irio_getSGSignalType(&p_DrvPvt,0,&aivalues,&status);
	printf("[irio_setSGSignalType function] SGSignalType 0 read %d \n", aivalues);

	printf("[irio_setSGAmp function] SGAmp0 set to %d, meaning %.2fV  \n",(int)((0.4)*(p_DrvPvt.CVDAC)),0.4);
	st|=irio_setSGAmp(&p_DrvPvt,0,(int)((0.4)*(p_DrvPvt.CVDAC)),&status);
	st|=irio_getSGAmp(&p_DrvPvt,0,&aivalues,&status);
	printf("[irio_getSGAmp function] SGAmp0 read %d, meaning %.2fV \n", aivalues, aivalues/p_DrvPvt.CVDAC);

	printf("[irio_setSGPhase function] SGPhase0 set to 0 \n");
	st|=irio_setSGPhase(&p_DrvPvt,0,0,&status);
	st|=irio_getSGPhase(&p_DrvPvt,0,&aivalues,&status);
	printf("[irio_getSGPhase function] SGPhase0 read %d \n", aivalues);

	printf("[irio_setAOEnable function] AOEnable0 set to 1 (Enable)\n");
	st|=irio_setAOEnable(&p_DrvPvt,0,1,&status);
	st|=irio_getAOEnable(&p_DrvPvt,0,&aivalues,&status);
	printf("[irio_getAOEnable function] AOEnable0 read %d. 0-->Disable, 1-->Enable\n",aivalues);

	printf("[irio_setAOEnable function] AOEnable1 is set to 1 (Enable)\n");
	st|=irio_setAOEnable(&p_DrvPvt,1,1,&status);
	st|=irio_getAOEnable(&p_DrvPvt,1,&aivalues,&status);
	printf("[irio_getAOEnable function] AOEnable1 read %d. 0-->Disable, 1-->Enable\n",aivalues);

	usleep(100000);

	st|=irio_getAI(&p_DrvPvt,1, &aivalues, &status);
	printf("AO1 has been configured previously to -5.0V. value read from AI1 is %.6fV\n", aivalues*p_DrvPvt.CVADC);

	msgerr(st,7,"signal generator configuration (Post DAQ Start)",&status,verbosity,0);

	msgtest(8,irio_setDMATtoHostEnable irio_getDMATtoHostEnable irio_getDMATtoHostData);

	st|=irio_setDMATtoHostEnable(&p_DrvPvt,0,1,&status);
	printf("[irio_setDMATtoHostEnable function] DMATtoHostEnable0 Value is set to: 1\n");

	st|=irio_getDMATtoHostEnable(&p_DrvPvt,0,&aivalues,&status);
	printf("[irio_getDMATtoHostEnable function] DMATtoHostEnable0 read %d \n", aivalues);

	blocksize=p_DrvPvt.DMATtoHOSTBlockNWords[0]; //blocksize implemented in the FPGA
	printf("DEBUG: blocksize:%d (MAXIMUN: %d ) \n",blocksize, SIZE_HOST_DMAS);

	//Total samples to acquire
	if (atoi(argv[6])<=blocksize) TotalsamplesToAcq=blocksize; //This is the minimun lenght that can be acquired
	else	TotalsamplesToAcq = atoi(argv[6]);

	//Block length
	if ((atoi(argv[8])>0) && atoi(argv[8])<=(SIZE_HOST_DMAS))
	{
		nBlocksPerDMAAcc = atoi(argv[8])/blocksize; //number if blocks read at once from FIFO (Host) DMA.
		if (atoi(argv[8])%blocksize > 0) nBlocksPerDMAAcc++;

	}

	//continues acquisition parameter is set
	if ( (atoi(argv[7]))==1) {
		continueAcq = 1;
		printf("DEBUG: Continuous data acquisition reading buffers of length %d, %d blocks of %d samples:, \n",blocksize*nBlocksPerDMAAcc,nBlocksPerDMAAcc, blocksize);
	}
	else{
		continueAcq = 0;
		printf("DEBUG: Finite data  acquisition reading buffers of length %d, %d blocks of %d samples:, \n",blocksize*nBlocksPerDMAAcc,nBlocksPerDMAAcc, blocksize);
	}

	int sample_counter=0;

	printf("NI9264 AO0 is connected to NI9205 AI0 physically. AO0 is configured with a 100Hz Square signal Vp=0.4V\n");
	printf("Sampling rate of AI0 is 1000S/s. In AI0 channel square signal must be read. every ~10 samples 1 square cycle must be checked\n");
	printf("AI1 is connected to AO1 physically.In AI1, the value read should be close to the 5th parameter introduced when this "
			"program has been executed.\n");
	printf("Fref=%dHz\n", p_DrvPvt.Fref);
	
	data2=(uint64_t *)malloc(sizeof(uint64_t)*blocksize*nBlocksPerDMAAcc);

    int timeout=0;
	do{
		int32_t *dmaaivalues=NULL;
		//attempt to read nBlocksPerDMAAcc block
		st|=irio_getDMATtoHostData(&p_DrvPvt,nBlocksPerDMAAcc , 0, data2, &elementsRead, &status);

		//elementsRead should be a block if available otherwise 0
		dmaaivalues=(int32_t*)data2;
		if (st!=IRIO_success)
			{
				errorFounds++;
				status.code=IRIO_success;
				st=IRIO_success;
			}
		if (elementsRead == nBlocksPerDMAAcc)
		{
			timeout=0;
			sample_counter += (nBlocksPerDMAAcc*blocksize);
			//We count blocks to acquire only in finite acquisition
			if (continueAcq == 0) {

				if (sample_counter>= TotalsamplesToAcq) flagExit=1;
				// In finite mode printf the samples acquired
				int x;
				for (x=0;x<blocksize*nBlocksPerDMAAcc; x++)
				{
					printf("Nº sample[%03d] AI0=%+.6fV, ",x,dmaaivalues[(x*2)]*p_DrvPvt.CVADC);
					printf("AI1=%+.6fV \n",(double)dmaaivalues[(x*2)+1]*p_DrvPvt.CVADC);

				}
			}
			if ((sample_counter%(nBlocksPerDMAAcc*blocksize))==0)
				printf("----Total Samples per channel acquired: %d,  Size of a block: %d, Number of blocks per read: %d, number of channels %d, Error Founds:%d \n",
							sample_counter,
							p_DrvPvt.DMATtoHOSTBlockNWords[0],
							nBlocksPerDMAAcc,
							p_DrvPvt.DMATtoHOSTNCh[0],
							errorFounds);
		}
		else{
			usleep(500000*ts*nBlocksPerDMAAcc*blocksize); //half time to get an entire block, then 60ms  multiplied by the number of blocks required
			timeout ++;
			if (timeout==10)
				{
					flagExit=1;
					irio_mergeStatus(&status,DAQtimeout_Warning,p_DrvPvt.verbosity,"[%s,%d]-(%s) WARNING timeout error reading from DAQ buffer %s%d. Error Code: %d\n",__func__,__LINE__,p_DrvPvt.appCallID);
				}
		}
	} while (flagExit == 0); //Acquisition running until user press Ctrl+C .

	printf("\n\t  Total Samples acquired: %d\n",sample_counter);

	msgerr(st,8,"irio_getDMATtoHostData irio_setDMATtoHostEnable irio_getDMATtoHostEnable",&status,verbosity,0);
	free(data2);

	msgtest(9,Closing DMA and Driver );

	st|=irio_cleanDMAsTtoHost(&p_DrvPvt,&status);
	msgerr(st,9,"irio_cleanDMAsTtoHost",&status,verbosity,0);

	st|=irio_closeDMAsTtoHost(&p_DrvPvt,&status);
	msgerr(st,9,"irio_closeDMAsTtoHost",&status,verbosity,0);

	st|=irio_closeDriver(&p_DrvPvt,0,&status);
	msgerr(st,9,"irio_closeDriver",&status,verbosity,0);
	return 0;
}
