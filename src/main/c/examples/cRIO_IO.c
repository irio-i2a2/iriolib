/******************************************************************************
 * $HeadURL:
 * $Id:
 *
 * Project		 : IRIO Driver
 *
 * Description	 : IRIO Driver Test Program for analog and digital I/O for cRIO devices
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

void usage(char *name) {
        printf("This example checks the CRIO I/O FPGA design\n"
                "This example requires a cRIO device\n"
                "If OPEN NI-RIO Linux Version is installed: Use lsrio.py command to identify the RIO devices included in the fast controller\n"
                "If CLOSE NI-RIO Linux Version is installed: Use lsni -v command to identify the RIO devices included in the fast controller\n"
        		"\n"
        		"Below there is information about the input parameters required: \n"
        		"\t<Serial_Number>: Device serial number, uses lsrio.py/lsni -v command to get it.\n"
        		"\t<cRIO_model>: Device model, uses lsrio.py/lsni -v command to get it.\n"
        		"\n"
                "Usage: %s <Serial_Number> <cRIO_model> \n"
                "Example: %s 01666C59 9159\n", name, name);
}

int main (int argc, char **argv)
{
	irioDrv_t p_DrvPvt;
	TStatus status;
	irio_initStatus(&status);
	TIRIOStatusCode st=IRIO_success;
	int32_t aivalue;
	size_t valueLength;
	char VIversion[4];

	char *bitfileName=NULL;
	char *NIcriomodel;
	char *bitFilePath=NULL;
	int verbosity=1;



	int i=0; //iterator variable

	if (argc!=3)
	{
		usage(argv[0]);
		return -1;
	}

	asprintf(&bitfileName,"cRIOIO_%s",argv[2]);
	asprintf(&NIcriomodel,"NI %s",argv[2]);
	asprintf(&bitFilePath,"%s/resourceTest/%s/",get_current_dir_name(),argv[2]);

	printf ("Pwd: %s \n", bitFilePath);

	msgtest(0,irio_initDriver);

	st=irio_initDriver("test",argv[1],NIcriomodel,bitfileName,"V1.1",verbosity,bitFilePath,bitFilePath,&p_DrvPvt,&status );
	free(bitfileName);
	free(NIcriomodel);
	free(bitFilePath);

	msgerr(st,0,"irio_initDriver",&status,verbosity,1);

	msgtest(1,irio_getVersion);
	char *version=calloc(10,1);
	st=irio_getVersion( version, &status);
	printf("[irio_getVersion function] Version return: %s\n",version);
	free(version);
	msgerr(st,1,"irio_getVersion",&status,verbosity,0);

	msgtest(2,irio_setFPGAStart & irio_getFPGAStart);
	printf("[irio_setFPGAStart function] Trying to start FPGA\n");
	st=irio_setFPGAStart(&p_DrvPvt,1, &status);
	st|=irio_getFPGAStart(&p_DrvPvt, &aivalue,&status);
	printf("[irio_getFPGAStart function] FPGA State is: %d . (1-running), (0-stopped)\n", aivalue);
	msgerr(st,2,"irio_setFPGAStart & irio_getFPGAStart",&status,verbosity,1);

	msgtest(3,irio_setDAQStartStop & irio_getDAQStartStop);
	printf("[irio_setDAQStartStop function] DAQStartStop is written with value %d (ON) \n", 1);
	st=irio_setDAQStartStop(&p_DrvPvt,1,&status);
	st|=irio_getDAQStartStop(&p_DrvPvt,&aivalue,&status);
	printf("[irio_getDAQStartStop function] value read from DAQStartStop is %d  \n",aivalue);
	msgerr(st,3,"irio_setDAQStartStop & irio_getDAQStartStop",&status,verbosity,1);

	msgtest(4,irio_getFPGAVIVersion);
	st=irio_getFPGAVIVersion(&p_DrvPvt,VIversion, 4, &valueLength,&status);
	printf("[irio_getFPGAVIVersion function] IRIO test 3: VI version: %c.%c \n",VIversion[1],VIversion[3]);
	msgerr(st,4,"irio_getFPGAVIVersion",&status,verbosity,0);

	msgtest(5,irio_getDevQualityStatus);
	st=irio_getDevQualityStatus(&p_DrvPvt,&aivalue,&status);
	printf("[irio_getDevQualityStatus function] Device Quality Status expected is 0. Value read %d \n", aivalue);
	msgerr(st,5,"irio_getDevQualityStatus",&status,verbosity,0);

	msgtest(6,irio_getDevTemp);
	st=irio_getDevTemp(&p_DrvPvt,&aivalue,&status);
	printf("[irio_getDevTemp function] FPGA Temperature %.2fºC. This value depends on the temperature conditions.\n", aivalue*0.25);
	msgerr(st,6,"irio_getDevTemp",&status,verbosity,0);

	msgtest(7,irio_getDevProfile);
	st=irio_getDevProfile(&p_DrvPvt,&aivalue,&status);
	printf("[irio_getDevProfile function] Device Profile %d . This Device Profile should be 1. Point by Point \n", aivalue);
	msgerr(st,7,"irio_getDevProfile",&status,verbosity,0);

	msgtest(8,irio_setSamplingRate irio_getSamplingRate);
	st=irio_setSamplingRate(&p_DrvPvt,0,aivalue,&status);
	printf("[irio_setSamplingRate function] Sampling Rate wrote %d .  \n", aivalue*p_DrvPvt.Fref);
	st=irio_getSamplingRate(&p_DrvPvt,0,&aivalue,&status);
	printf("[irio_getSamplingRate function] Sampling Rate read %d .  \n", aivalue*p_DrvPvt.Fref);
	msgerr(st,8,"irio_setSamplingRate irio_getSamplingRate",&status,verbosity,0);

	msgtest(9,irio_setDebugMode irio_getDebugMode irio_setAO irio_getAO irio_getAI);
	printf("[irio_setDebugMode function] DebugMode Value is Set to: 0 (OFF) \n");
	st=irio_setDebugMode(&p_DrvPvt,0,&status);
	st|=irio_getDebugMode(&p_DrvPvt,&aivalue,&status);
	printf("[irio_getDebugMode function] value read from DebugMode: %d \n",aivalue);

	usleep(100000);

	/**
	 * AO0, AO1 & AO2 ARE GOING TO BE SET TO 0V
	 */

	printf("[irio_setAO function] AO0, AO1 & AO2 are configured with 0.00V.\n");
	st|=irio_setAO(&p_DrvPvt,0,(int)(0.0*p_DrvPvt.CVDAC),&status); // AO0 is set to 0V.
	st|=irio_setAO(&p_DrvPvt,1,(int)(0.0*p_DrvPvt.CVDAC),&status); // AO1 is set to 0V.
	st|=irio_setAO(&p_DrvPvt,2,(int)(0.0*p_DrvPvt.CVDAC),&status); // AO2 is set to 0V.
	st|=irio_getAO(&p_DrvPvt,0,&aivalue,&status); //AO0 is set to 0V
	printf("[irio_getAO function] value read from AO0 is: %.2fV \n", aivalue/p_DrvPvt.CVDAC);
	st|=irio_getAO(&p_DrvPvt,1,&aivalue,&status); //AO0 is set to 0V
	printf("[irio_getAO function] value read from AO1 is: %.2fV \n", aivalue/p_DrvPvt.CVDAC);
	st|=irio_getAO(&p_DrvPvt,2,&aivalue,&status); //AO0 is set to 0V
	printf("[irio_getAO function] value read from AO2 is: %.2fV \n", aivalue/p_DrvPvt.CVDAC);

	printf("Values read from AI0, AI1 and AO2 must be close to 0V. Expected absolute value is less than 0.002V \n");
	st|=irio_getAI(&p_DrvPvt,0,&aivalue,&status);
	printf("[irio_getAI function] Slot 2 9205. value read from AI0:  %fV \n", aivalue*p_DrvPvt.CVADC);
	st|=irio_getAI(&p_DrvPvt,1,&aivalue,&status);
	printf("[irio_getAI function] Slot 2 9205. value read from AI1: %fV \n", aivalue*p_DrvPvt.CVADC);
	st|=irio_getAI(&p_DrvPvt,2,&aivalue,&status);
	printf("[irio_getAI function] Slot 2 9205. value read from AI2: %fV \n", aivalue*p_DrvPvt.CVADC);

	printf("[irio_setDebugMode function] DebugMode Value is Set to: 1 (ON) \n");
	st|=irio_setDebugMode(&p_DrvPvt,1,&status);
	st|=irio_getDebugMode(&p_DrvPvt,&aivalue,&status);
	printf("[irio_getDebugMode function] value read from DebugMode: %d \n",aivalue);
	printf("Setting DebugMode to ON, values expected from AIs are: AI0=1.25, AI1=3.125 & AI2=-4.75\n");

	usleep(100000);
	st|=irio_getAI(&p_DrvPvt,0,&aivalue,&status);
	printf("[irio_getAI function] Value read from AI0 Value %fV \n", aivalue*p_DrvPvt.CVADC);
	st|=irio_getAI(&p_DrvPvt,1,&aivalue,&status);
	printf("[irio_getAI function] Value read from AI1 Value %fV \n", aivalue*p_DrvPvt.CVADC);
	st|=irio_getAI(&p_DrvPvt,2,&aivalue,&status);
	printf("[irio_getAI function] Value read from AI2 Value %fV \n", aivalue*p_DrvPvt.CVADC);
	msgerr(st,9,"irio_setDebugMode irio_getDebugMode irio_setAO irio_getAO irio_getAI",&status,verbosity,0);

	/**
	 * TEST 10 CONFIGURE ANALOG OUTPUTS WITH DIFFERENT VALUES. THESE ONES MUST BE READ FROM ANALOG INPUTS
	 */

	msgtest(10,irio_setDebugMode irio_getDebugMode irio_setAOEnable irio_getAOEnable irio_setAO irio_getAO irio_getAI);
	printf("IRIO Test 5 configure analog outputs with determined volt values, and read them from analog Inputs.\n");
	printf("Analog Inputs and analog outputs should be interconnected externally.\n\n");

	printf("[irio_setDebugMode function] DebugMode Value is Set to: 0 (OFF). Then Analog Inputs read from physical analog inputs. \n");
	st=irio_setDebugMode(&p_DrvPvt,0,&status);
	st|=irio_getDebugMode(&p_DrvPvt,&aivalue,&status);
	printf("[irio_getDebugMode function] value read from DebugMode: %d \n\n",aivalue);

	usleep(100000);

	printf("[irio_setAOEnable function] AOEnable0 is set to:%d (Enable) \n",NiFpga_True);
	st|=irio_setAOEnable(&p_DrvPvt,0,NiFpga_True, &status);
	st|=irio_getAOEnable(&p_DrvPvt,0,&aivalue, &status);
	printf("[irio_getAOEnable function] the value read from AOEnable0 is: %d \n\n",aivalue);

	printf("[irio_setAO function] AO0 is configured with:4000(digital value), this means %fV \n",4000/p_DrvPvt.CVDAC);
	st|=irio_setAO(&p_DrvPvt,0, 4000, &status);
	st|=irio_getAO(&p_DrvPvt,0, &aivalue, &status);
	printf("[irio_getAO function] value read from AO0 is:%d (digital value), this means %fV \n",aivalue,aivalue/p_DrvPvt.CVDAC);

	usleep(100000);

	st|=irio_getAI(&p_DrvPvt,0, &aivalue, &status);
	printf("[irio_getAI function] AI0 reads %fV from analog input 0\n\n",aivalue*p_DrvPvt.CVADC);


	printf("[irio_setAOEnable function] AOEnable1 is set to:%d (Enable) \n",NiFpga_True);
	st|=irio_setAOEnable(&p_DrvPvt,1,NiFpga_True, &status);
	st|=irio_getAOEnable(&p_DrvPvt,1,&aivalue, &status);
	printf("[irio_getAOEnable function] the value read from AOEnable1 is: %d \n\n",aivalue);

	printf("[irio_setAO function] AO1 is configured with:8000(digital value), this means %fV \n",8000/p_DrvPvt.CVDAC);
	st|=irio_setAO(&p_DrvPvt,1, 8000, &status);
	st|=irio_getAO(&p_DrvPvt,1, &aivalue, &status);
	printf("[irio_getAO function] value read from AO1 is:%d (digital value), this means %fV \n",aivalue,aivalue/p_DrvPvt.CVDAC);

	usleep(100000);

	st|=irio_getAI(&p_DrvPvt,1, &aivalue, &status);
	printf("[irio_getAI function] AI1 reads %fV from analog input 1\n\n",aivalue*p_DrvPvt.CVADC);

	printf("[irio_setAOEnable function] AOEnable2 is set to:%d (Enable) \n",NiFpga_True);
	st|=irio_setAOEnable(&p_DrvPvt,2,NiFpga_True, &status);
	st|=irio_getAOEnable(&p_DrvPvt,2,&aivalue, &status);
	printf("[irio_getAOEnable function] the value read from AOEnable2 is: %d \n\n",aivalue);

	printf("[irio_setAO function] AO2 is configured with:-8000(digital value), this means %fV \n",-8000/p_DrvPvt.CVDAC);
	st|=irio_setAO(&p_DrvPvt,2, -8000, &status);
	st|=irio_getAO(&p_DrvPvt,2, &aivalue, &status);
	printf("[irio_getAO function] value read from AO2 is:%d (digital value), this means %fV \n",aivalue,aivalue/p_DrvPvt.CVDAC);

	usleep(100000);

	st|=irio_getAI(&p_DrvPvt,2, &aivalue, &status);
	printf("[irio_getAI function] AI2 reads %fV from analog input 2\n\n",aivalue*p_DrvPvt.CVADC);

	for (i=0;i<3;i++)
	{
		printf("[irio_setAOEnable function] AOEnable%d is set to:%d (Disable) \n",i,NiFpga_False);
		st|=irio_setAOEnable(&p_DrvPvt,i,NiFpga_False, &status);
		st|=irio_getAOEnable(&p_DrvPvt,i,&aivalue, &status);
		printf("[irio_getAOEnable function] the value read from AOEnable%d is: %d \n\n",i,aivalue);
	}
	msgerr(st,10,"irio_setDebugMode irio_getDebugMode irio_setAOEnable irio_getAOEnable irio_setAO irio_getAO irio_getAI",&status,verbosity,0);

	/**
	 *TEST 12 CHECKS DIGITAL I/O WRITTING AND READING
	 */
	msgtest(11,irio_setDO irio_getDO irio_getDI);

	st=IRIO_success;
	//digital I/O performance with cRIO-NI9401 module
	for(i=0;i<2;i++)
	{
		printf("[irio_setDO function] NI9401 DO0 Value is set: %d \n",i);
		st|=irio_setDO(&p_DrvPvt,0,i, &status);
		st|=irio_getDO(&p_DrvPvt,0,&aivalue, &status);
		printf("[irio_getDO function] value read from NI9401 DO0 is: %d\n",aivalue);
		usleep(100000);
		st|=irio_getDI(&p_DrvPvt,0,&aivalue, &status);
		printf("[irio_getDI function] value read from NI9401 DI0 %d \n\n", aivalue);
	}

	//digital I/O performance with cRIO-NI9477 as Digital Output and cRIO-NI9426 as Digital Input
	for(i=0;i<2;i++)
	{
		printf("[irio_setDO function] NI9477 DO1 Value is set: %d \n",i);
		st|=irio_setDO(&p_DrvPvt,1,i, &status);
		st|=irio_getDO(&p_DrvPvt,1,&aivalue, &status);
		printf("[irio_getDO function] value read from NI9477 DO1 is: %d\n",aivalue);
		usleep(100000);
		st|=irio_getDI(&p_DrvPvt,1,&aivalue, &status);
		printf("[irio_getDI function] value read from NI 9426 DI1 %d \n\n", aivalue);
	}

	//digital I/O performance with cRIO-NI9476 as Digital Output and cRIO-NI9425 as Digital Input
	for(i=0;i<2;i++)
	{
		printf("[irio_setDO function] NI9476 DO2 Value is set: %d \n",i);
		st|=irio_setDO(&p_DrvPvt,2,i, &status);
		st|=irio_getDO(&p_DrvPvt,2,&aivalue, &status);
		printf("[irio_getDO function] value read from NI9476 DO2 is: %d\n",aivalue);
		usleep(100000);
		st|=irio_getDI(&p_DrvPvt,2,&aivalue, &status);
		printf("[irio_getDI function] value read from NI 9425 DI2 %d \n\n", aivalue);
	}
	msgerr(st,11,"irio_setDO irio_getDO irio_getDI",&status,verbosity,0);

	/**
	 * AUXILIARY ANALOG AND DIGITAL I/O READING AND WRITTING
	 */
	msgtest(12,irio_setAuxAO irio_getAuxAO irio_getAuxAI irio_setAuxDO irio_getAuxDO irio_getAuxDI);
	printf("[irio_setAuxAO function] AuxAO0 is set to value: 5000\n");
	st=irio_setAuxAO(&p_DrvPvt,0,5000, &status);
	st|=irio_getAuxAO(&p_DrvPvt,0,&aivalue, &status);
	printf("[irio_getAuxAO function] value read from AuxAO0 is: %d \n",aivalue);
	usleep(100000);
	st|=irio_getAuxAI(&p_DrvPvt,0,&aivalue,&status);
	printf("[irio_getAuxAI function] value read from AuxAI0 is %d \n", aivalue);

	printf("[irio_setAuxAO function] AuxAO1 is set to value: 250\n");
	st|=irio_setAuxAO(&p_DrvPvt,1,250, &status);
	st|=irio_getAuxAO(&p_DrvPvt,1,&aivalue, &status);
	printf("[irio_getAuxAO function] value read from AuxAO1 is: %d \n",aivalue);
	usleep(100000);
	st|=irio_getAuxAI(&p_DrvPvt,1,&aivalue,&status);
	printf("[irio_getAuxAI function] value read from AuxAI1 is %d \n", aivalue);

	//Write 0 and 1 into DO0. It reads value in DO0 and DI0
	printf("[irio_setAuxDO function] AuxDO0 is set to value: 1\n");
	st|=irio_setAuxDO(&p_DrvPvt,0,1, &status);
	st|=irio_getAuxDO(&p_DrvPvt,0,&aivalue, &status);
	printf("[irio_getAuxDO function] value read from AuxDO0 is: %d \n",aivalue);
	usleep(100000);
	st|=irio_getAuxDI(&p_DrvPvt,0,&aivalue,&status);
	printf("[irio_getAuxDI function] value read from AuxDI0 is %d \n", aivalue);
	printf("[irio_setAuxDO function] AuxDO0 is set to value: 0\n");
	st|=irio_setAuxDO(&p_DrvPvt,0,0, &status);
	st|=irio_getAuxDO(&p_DrvPvt,0,&aivalue, &status);
	printf("[irio_getAuxDO function] value read from AuxDO0 is: %d \n",aivalue);
	usleep(100000);
	st|=irio_getAuxDI(&p_DrvPvt,0,&aivalue,&status);
	printf("[irio_getAuxDI function] value read from AuxDI0 is %d \n", aivalue);

	//Write 0 and 1 into DO1. It reads value in DO1 and DI1

	printf("[irio_setAuxDO function] AuxDO1 is set to value: 1\n");
	st|=irio_setAuxDO(&p_DrvPvt,1,1, &status);
	st|=irio_getAuxDO(&p_DrvPvt,1,&aivalue, &status);
	printf("[irio_getAuxDO function] value read from AuxDO1 is: %d \n",aivalue);
	usleep(100000);
	st|=irio_getAuxDI(&p_DrvPvt,1,&aivalue,&status);
	printf("[irio_getAuxDI function] value read from AuxDI1 is %d \n", aivalue);
	printf("[irio_setAuxDO function] AuxDO1 is set to value: 0\n");
	st|=irio_setAuxDO(&p_DrvPvt,1,0, &status);
	st|=irio_getAuxDO(&p_DrvPvt,1,&aivalue, &status);
	printf("[irio_getAuxDO function] value read from AuxDO1 is: %d \n",aivalue);
	usleep(100000);
	st|=irio_getAuxDI(&p_DrvPvt,1,&aivalue,&status);
	printf("[irio_getAuxDI function] value read from AuxDI1 is %d \n", aivalue);

	msgerr(st,12,"irio_setAuxAO irio_getAuxAO irio_getAuxAI irio_setAuxDO irio_getAuxDO irio_getAuxDI",&status,verbosity,0);

	/**
	 * STOP ACQUISITION
	 */
	msgtest(13,irio_setDAQStartStop);
	printf("[irio_setDAQStartStop function] DAQStartStop Value to Set: 0\n");
	st|=irio_setDAQStartStop(&p_DrvPvt,0,&status);
	st|=irio_getDAQStartStop(&p_DrvPvt,&aivalue,&status);
	printf("[irio_getDAQStartStop function] value read from DAQStartStop is %d value \n",aivalue);
	msgerr(st,13,"irio_setDAQStartStop",&status,verbosity,0);

	/**
	 * CLOSE DRIVER
	 */
	msgtest(14,irio_closeDriver);
	st|=irio_closeDriver(&p_DrvPvt,0, &status);
	msgerr(st,14,"irio_closeDriver",&status,verbosity,0);

	return 0;
}

