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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <math.h>
#include <errno.h>
#include <signal.h>

#include <irioDriver.h>
#include <irioDataTypes.h>
#include <irioHandlerAnalog.h>
#include <irioHandlerDigital.h>
#include <irioHandlerDMA.h>
#include <irioHandlerImage.h>
#include <irioHandlerSG.h>
#include <irioResourceFinder.h>
#include <limits.h>

#define msgtest(n,function)  \
					printf("####################################################\n"); \
					printf("IRIO test" #n " " #function " function\\s\n"); \
					printf("####################################################\n")

#define msgerr(a,n,f,stat) if (a==IRIO_success){\
						printf("IRIO test" #n " " #f " function successful execution [OK] \n\n");fflush(stdout);\
					  }\
					else{\
						printf("IRIO test" #n " " #f " function unsuccessful execution [ERROR] %s\n\n",stat.msg);fflush(stdout);\
						free(stat.msg);stat.msg=NULL;stat.code=IRIO_success;exit(1);\
					 }
int continuousStop=0;

void signal_handler(int signum){
	printf("\n+++++++++++++Stopping Continuous acquisition++++++++++++++++++++\n");
	continuousStop=1;
}

void usage(char *name) {
        printf("This example checks the use of cRIO DAQ profile\n"
                "This example requires a specific chassis configuration\n"
                "debug_mode=1-> analog input 0 and 1 will have +5.0 and -5.0\n"
        		"debug_mode=0-> MOD 0 AI0= NI9264 CH0/ MOD 1 AI0 = NI9264 CH1\n"
                "\n"
                "Usage: %s <s/n> <cRIO model> <debug_mode> <voltage NI9264 CH0 voltage value> <voltage NI9264 CH1 voltage value> <samples_to_read> \n"
                "Example: %s 0\n", name, name);
}
int main(int argc, char **argv)
{
	irioDrv_t p_DrvPvt;

	TStatus status;
	status.code=IRIO_success;
	status.msg=NULL;
	TIRIOStatusCode st=IRIO_success;

	char *currPath=NULL;
	char *bitFilePath;
	uint64_t *data;
	uint64_t *data2;

	char *value;

	size_t valueLength;
	int32_t aivalues;
	int elementsRead;


	char criomodel[20];
	char NIcriomodel[20];
	char criomodel_serial[20];

	char *intermediatePath;
	char *bitfileName;
	int blocksize=0;
	char viprojectversion[5];
	sprintf(viprojectversion,"V1.1");
	signal(SIGINT,signal_handler);

	if (argc != 7) {
	    usage(argv[0]);
	    return 1;
	}
	if( (strlen(argv[1]) > 10) || (strlen(argv[2])>10) )
	{
		usage(argv[0]);
		return 1;
	}
	value=calloc(20,sizeof(char));
	data=malloc(100000*sizeof(uint64_t));


	strcpy(criomodel_serial,argv[1]);
	strcpy(criomodel,argv[2]);

	bitFilePath=(char*)malloc(1024);
	intermediatePath=(char*)malloc(512);
	bitfileName=(char*)malloc(128);

	sprintf(intermediatePath,"/resourceTest/%s/",criomodel);
	sprintf(bitfileName,"cRIODAQDMA_%s",criomodel);
	sprintf(NIcriomodel,"NI %s",criomodel);

	currPath = get_current_dir_name();

	asprintf(&bitFilePath,"%s/resourceTest/%s/",currPath,criomodel);
	free(currPath);
	printf ("Pwd: %s \n", bitFilePath);


	//CHASSIS1 S/N: 19ED079 3x9205, 1x9264, 1x9401 1x9426 2x9477
	msgtest(0,irio_initDriver);
	st|=irio_initDriver("test",criomodel_serial,NIcriomodel,bitfileName,viprojectversion,1,bitFilePath,bitFilePath,&p_DrvPvt,&status );
	msgerr(st,0,irio_initDriver,status);

	msgtest(1,irio_setUpDMAsTtoHost);
	st|=irio_setUpDMAsTtoHost(&p_DrvPvt,&status);
	msgerr(st,1,irio_initDriver,status);

	st|=irio_cleanDMATtoHost(&p_DrvPvt,0,data, sizeof(data),&status);
	msgerr(st,1,irio_cleanDMATtoHost,status);
	free(data);


	/**
	 * FPGA START
	 */

	msgtest(2,irio_setFPGAStart & irio_getFPGAStart);
	printf("[irio_setFPGAStart function] FPGA hardware logic is started (\"Running\") Value %d \n", 1);
	st|=irio_setFPGAStart(&p_DrvPvt,1, &status);
	st|=irio_getFPGAStart(&p_DrvPvt, &aivalues,&status);
	printf("[irio_getFPGAStart function] FPGA State is: %d . 1-->\"running\" 0-->\"stopped\"\n", aivalues);
	msgerr(st,2,irio_setFPGAStart & irio_getFPGAStart,status)





	msgtest(3,irio_setDMATtoHostSamplingRate & irio_getDMATtoHostSamplingRate);
	printf("[irio_setDMATtoHostSamplingRate] value configured is 1. This means Fref/1: %dS/s\n",p_DrvPvt.Fref);
	st|=irio_setDMATtoHostSamplingRate(&p_DrvPvt,0,1, &status);
	st|=irio_getDMATtoHostSamplingRate(&p_DrvPvt,0,&aivalues,&status);
	printf("[irio_getDMATtoHostSamplingRate] value read from DMATtoHostSamplingRate0 is: %d. This means %dS/s\n", aivalues, aivalues*p_DrvPvt.Fref);
	msgerr(st,3,irio_getDMATtoHostSamplingRate & irio_getDMATtoHostSamplingRate,status);

	



	/**
	 * DEVICE INFORMATION READ FROM FPGA
	 */
	msgtest(4,irio_getFPGAVIVersion irio_getDevQualityStatus irio_getDevTemp irio_getDevProfile);
	st|=irio_getFPGAVIVersion(&p_DrvPvt,value,10, &valueLength,&status);
	printf("[irio_getFPGAVIVersion function] IRIO test 4: VI version read from FPGA: %s\n",value);
	free(value);

	st|=irio_getDevQualityStatus(&p_DrvPvt,&aivalues,&status);
	printf("[irio_getDevQualityStatus function] Device Quality Status read from FPGA %d \n", aivalues);
	st|=irio_getDevTemp(&p_DrvPvt,&aivalues,&status);
	printf("[irio_getDevTemp function] Device Temperature read from FPGA %.2fºC \n", aivalues*0.25);
	st|=irio_getDevProfile(&p_DrvPvt,&aivalues,&status);
	printf("[irio_getDevProfile function] Device Profile configured in the FPGA is %d.  This Device should be 1. FlexRIO-->0, cRIO-->1   \n", aivalues);
	msgerr(st,4,irio_getDevProfile,status);

	/**
	 * WAVEFORM GENERATOR CONFIGURATION
	 */
	msgtest(5,signal generator configuration.);
	printf("Test 5 checks the following functions: irio_setDebugMode irio_getDebugMode irio_setAO irio_getAO\n");
	printf("irio_setDAQStartStop irio_getDAQStartStop irio_setSGUpdateRate irio_setSGFreq irio_getSGFreq irio_setSGSignalType\n");
	printf("irio_getSGSignalType irio_setSGAmp irio_getSGAmp irio_setSGPhase irio_getSGPhase irio_setAOEnable irio_getAOEnable\n\n");
	printf("[irio_setDebugMode function] Debug Mode is configured with value: %d. 0-->OFF, 1-->ON \n", aivalues);
	st|=irio_setDebugMode(&p_DrvPvt,atoi(argv[3]),&status);
	st|=irio_getDebugMode(&p_DrvPvt,&aivalues,&status);
	printf("[irio_getDebugMode function] value read from DebugMode is: %d \n", aivalues);

	//// Using analog input and outputs
	printf("[irio_setAO function] value written in AO0 is: %d. This is %.1fV\n",(int)(atof(argv[4])*p_DrvPvt.CVDAC), atof(argv[4]));
	st|=irio_setAO(&p_DrvPvt,0,atof(argv[4])*p_DrvPvt.CVDAC,&status);
	st|=irio_getAO(&p_DrvPvt,0,&aivalues,&status);
	printf("[irio_getAO function] value read from AO0 is: %d. This is %.1fV\n",aivalues, aivalues/p_DrvPvt.CVDAC);
	usleep(1000000);

	printf("[irio_setAO function] value written in AO1 is: %d. This is %.1fV\n",(int)(atof(argv[5])*p_DrvPvt.CVDAC), atof(argv[5]));
	st|=irio_setAO(&p_DrvPvt,1,atof(argv[5])*p_DrvPvt.CVDAC,&status);
	st|=irio_getAO(&p_DrvPvt,1,&aivalues,&status);
	printf("[irio_getAO function] value read from AO1 is: %d .This is %.1fV\n",aivalues, aivalues/p_DrvPvt.CVDAC);

	usleep(1000000);

	printf("[irio_setDAQStartStop function] DAQStartStop is written with %d value (ON) \n", 1);
	st=irio_setDAQStartStop(&p_DrvPvt,1,&status);
	st|=irio_getDAQStartStop(&p_DrvPvt,&aivalues,&status);
	printf("[irio_getDAQStartStop function] value read from DAQStartStop is: %d \n",aivalues);

	//Programming the signal generator 0. 100Hz, 1000S/s, 5V
	printf("[irio_setSGUpdateRate function] Waveform generator (SGUpdateRate0) is configured with value 10. This is %dS/s\n",p_DrvPvt.SGfref[0]/10);
	st|=irio_setSGUpdateRate(&p_DrvPvt,0,10,&status);
	st|=irio_getSGUpdateRate(&p_DrvPvt,0,&aivalues,&status);
	printf("[irio_getSGUpdateRate function] value read from SGUpdateRate0 is: %d. This is %dS/s\n", aivalues,p_DrvPvt.SGfref[0]/aivalues);

	int updaterate=p_DrvPvt.SGfref[0]/10;

	// we want program signal generator with 100Hz, periodic signal
	// equation to apply to obtain freq_desired is: SGFreq=freq_desired*( (2to32) / ( SGFref/(S/s)) )
	int accIncr=100*(UINT_MAX/updaterate); //100 Hz sampled with 1kHz means 10 samples per cycle
	printf("[irio_setSGFreq function] SGfreq 0 is set to %d, that means %dHz \n",accIncr,100);
	st|=irio_setSGFreq(&p_DrvPvt,0,accIncr,&status);
	st|=irio_getSGFreq(&p_DrvPvt,0,&aivalues,&status);
	printf("[irio_getSGFreq function] SGFreq 0 read from FPGA is %d that means %dHz \n",aivalues, aivalues/(UINT_MAX/(1000)));

	printf("[irio_setSGSignalType function] SGSignalType 0 is set to: 2 (SQUARE) \n");
	st|=irio_setSGSignalType(&p_DrvPvt,0,2,&status); //0=DC, 1=SINE 2=SQUARE 3=TRIANGULAR
	st|=irio_getSGSignalType(&p_DrvPvt,0,&aivalues,&status);
	printf("[irio_setSGSignalType function] value read from SGSignalType 0 is: %d \n", aivalues);

	printf("[irio_setSGAmp function] value written in SGAmp0 is: %d. This is an amplitude of %.2fV  \n",(int)((0.4)*(p_DrvPvt.CVDAC)),0.4);
	st|=irio_setSGAmp(&p_DrvPvt,0,(int)((0.4)*(p_DrvPvt.CVDAC)),&status);
	st|=irio_getSGAmp(&p_DrvPvt,0,&aivalues,&status);
	printf("[irio_getSGAmp function] value read from SGAmp0 is: %d. This is %.2fV \n", aivalues, aivalues/p_DrvPvt.CVDAC);

	printf("[irio_setSGPhase function] SGPhase0 is configured to: 0 \n");
	st|=irio_setSGPhase(&p_DrvPvt,0,0,&status);
	st|=irio_getSGPhase(&p_DrvPvt,0,&aivalues,&status);
	printf("[irio_getSGPhase function] value read from SGPhase0 is: %d \n", aivalues);

	printf("[irio_setAOEnable function] AOEnable0 is set to: 1 (Enable)\n");
	st|=irio_setAOEnable(&p_DrvPvt,0,1,&status);
	st|=irio_getAOEnable(&p_DrvPvt,0,&aivalues,&status);
	printf("[irio_getAOEnable function] value read from AOEnable0 is: %d. 0-->Disable, 1-->Enable\n",aivalues);

	printf("[irio_setAOEnable function] AOEnable1 is set to: 1 (Enable)\n");
	st|=irio_setAOEnable(&p_DrvPvt,1,1,&status);
	st|=irio_getAOEnable(&p_DrvPvt,1,&aivalues,&status);
	printf("[irio_getAOEnable function] value read from AOEnable1 is: %d. 0-->Disable, 1-->Enable\n",aivalues);

	usleep(100000);

	st|=irio_getAI(&p_DrvPvt,1, &aivalues, &status);
	printf("AO1 has been configured previously to -5.0V. value read from AI1 is %.6fV\n", aivalues*p_DrvPvt.CVADC);

	msgerr(st,5,signal generator configuration,status);

	msgtest(6,irio_setDMATtoHostEnable irio_getDMATtoHostEnable irio_getDMATtoHostData);


	st|=irio_setDMATtoHostEnable(&p_DrvPvt,0,1,&status);
	printf("[irio_setDMATtoHostEnable function] DMATtoHostEnable0 Value is set to: 1\n");


	st|=irio_getDMATtoHostEnable(&p_DrvPvt,0,&aivalues,&status);
	printf("[irio_getDMATtoHostEnable function] value read from DMATtoHostEnable0 is %d \n", aivalues);


	blocksize=p_DrvPvt.DMATtoHOSTBlockNWords[0];
	int number_of_blocks=atoi(argv[6])/blocksize;
    int rest=atoi(argv[6])%blocksize;

	if (rest>0)
	{
		number_of_blocks++;
	}

	data2=(uint64_t *)malloc(sizeof(uint64_t)*blocksize*number_of_blocks);

	usleep(1000000); //1 second
	printf("Block length used by DMA: %d\n", blocksize);
	printf("Fref=%dHz\n", p_DrvPvt.Fref);
	printf("Number_of_blocks:%d\n",number_of_blocks);
	printf("Press CRTL+C to finish continuos data acquisition\n");
	fflush(stdout);
	int k=0;
	do{
		//attempt to read 1 block
		st|=irio_getDMATtoHostData(&p_DrvPvt, number_of_blocks, 0, data2, &elementsRead, &status);

		if (st!=IRIO_success)
		{
			printf("Elements Read =%d\n", elementsRead*blocksize);
			printf("Error or Warning %s\n",status.msg);
			free(status.msg);status.msg=NULL;status.code=IRIO_success;
			continuousStop=1;
		}
		//elementsRead should be a block if available otherwise 0
		
		if (elementsRead==number_of_blocks){
					
			k++;
			if (k %100 ==0){
					printf("k:=%d\r",k); 
					fflush(stdout);
			}
		}
		else usleep(100000); //100ms
	} while (continuousStop==0 );//k<number_of_blocks
	printf ("Number of blocks acquired: %d\n",k);
	msgerr(st,6,irio_getDMATtoHostData,status);
	free(data2);
	free(intermediatePath);
	free(bitFilePath);


	msgtest(7,Using aux terminal );
	st|=irio_setAuxAO(&p_DrvPvt,0,5000, &status);
	printf("AuxAO0 Value to Set: 5000\n");
	msgerr(st,7,irio_setAuxAO,status);

	st|=irio_getAuxAO(&p_DrvPvt,0,&aivalues,&status);
	printf("AuxAO0 Value Set %d \n", aivalues);
	msgerr(st,7,irio_getAuxAO,status);

	usleep(100000);

	st|=irio_getAuxAI(&p_DrvPvt,0,&aivalues,&status);
	printf("AuxAI0 Value %d \n", aivalues);
	msgerr(st,7,irio_getAuxAI,status);

	msgtest(8,Closing DMA and Driver );

	st|=irio_cleanDMAsTtoHost(&p_DrvPvt,&status);
	msgerr(st,8,irio_cleanDMAsTtoHost,status);

	st|=irio_closeDMAsTtoHost(&p_DrvPvt,&status);
	msgerr(st,8,irio_closeDMAsTtoHost,status);

	st|=irio_closeDriver(&p_DrvPvt,0,&status);
	msgerr(st,8,irio_closeDriver,status);

	return 0;
}

