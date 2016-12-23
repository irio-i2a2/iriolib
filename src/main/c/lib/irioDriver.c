/**************************************************************************//**
 * \file irioDriver.c
 * \authors Mariano Ruiz (Universidad Politécnica de Madrid, UPM)
 * \authors Diego Sanz (Universidad Politécnica de Madrid, UPM)
 * \authors Sergio Esquembri (Universidad Politécnica de Madrid, UPM)
 * \authors Enrique Bernal (Universidad Politécnica de Madrid, UPM)
 * \authors Alvaro Bustos (Universidad Politécnica de Madrid, UPM)
 * \brief Initialization and common resources access methods for IRIO Driver
 * \date Sept., 2010 (Last Review July 2015)
 * \copyright (C) 2010-2015 Universidad Politécnica de Madrid (UPM)
 * \par License: \b
 * 	\n This project is released under the GNU Public License version 2.
 * \cond
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 * \endcond
 *****************************************************************************/

#include "irioDriver.h"
#include "irioDataTypes.h"
#include "irioHandlerAnalog.h"
#include "irioHandlerDigital.h"
#include "irioHandlerDMA.h"
#include "irioHandlerImage.h"
#include "irioHandlerSG.h"
#include "irioResourceFinder.h"
#include "irioError.h"

#include <niflexrio.h> //located in /opt/codac/include
#ifdef IRIO_GPU
#include "irioHandlerDMAGPU.h"
#endif


#include "NiFpga.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>
#include <math.h>


/** @name Common Resource Strings
 * Strings for common FPGA Resources
 */
///@{
#define STRINGNAME_SIGNATURE "_Signature"
#define STRINGNAME_PLATFORM "_IndicatorU8_Platform"
#define STRINGNAME_FPGAVIVERSION "_IndicatorArrayU8_FPGAVIversion"
#define STRINGNAME_INITDONE  "_IndicatorBool_InitDone"
#define STRINGNAME_FREQREF "_IndicatorU32_Fref"
#define STRINGNAME_DEVQUALITYSTATUS "_IndicatorU8_DevQualityStatus"
#define STRINGNAME_DEVTEMP "_IndicatorI16_DevTemp"
#define STRINGNAME_DEVPROFILE "_IndicatorU8_DevProfile"
#define STRINGNAME_DAQSTARTSTOP "_ControlBool_DAQStartStop"
#define STRINGNAME_DEBUGMODE "_ControlBool_DebugMode"
#define STRINGNAME_INSERTEDIOMODULEID "_IndicatorU32_InsertedIOModuleID"
#define STRINGNAME_RIOADAPTERCORRECT "_IndicatorBool_RIOAdapterCorrect"
#define STRINGNAME_INSERTEDIOMODULESID "_IndicatorArrayU16_InsertedIOModulesID" //for cRIO
#define STRINGNAME_CRIOMODULESOK "_IndicatorBool_cRIOModulesOK"
#define STRINGNAME_SAMPLINGRATE "_ControlU16_SamplingRate"
///@}

/**@name cRIO Module Names
 * Strings and IDs to identify cRIO modules
 */
///@{
cRIOmodule cRIOIDs[8] = {
		{0x0000,"NO-MOD"},
		{0x712A,"NI9205"},
		{0x745C,"NI9264"},
		{0x7130,"NI9401"},
		{0x712F,"NI9425"},
		{0x736A,"NI9426"},
		{0x7133,"NI9476"},
		{0x71CB,"NI9477"}
};
///@}

int initializeLibrary(irioDrv_t* p_DrvPvt,TStatus* status);

int finalizeLibrary(irioDrv_t* p_DrvPvt, TStatus* status);

int configureTarget(irioDrv_t* p_DrvPvt, char* bitFilePath, TStatus* status);

int searchPlatform( irioDrv_t* p_DrvPvt,TStatus* status);

int allocFlexRIOEnums(irioDrv_t* p_DrvPvt,TStatus* status);

int allocCRIOEnums(irioDrv_t* p_DrvPvt,TStatus* status);

int freeXRIOEnums(irioDrv_t* p_DrvPvt,TStatus* status);

int searchMandatoryResourcesAllPlatforms(irioDrv_t* p_DrvPvt,TStatus* status);

int searchProfile( irioDrv_t* p_DrvPvt,TStatus* status);

int searchDAQResources(irioDrv_t* p_DrvPvt,TStatus* status);

int searchIMAQResources(irioDrv_t* p_DrvPvt,TStatus* status);

#ifdef IRIO_GPU
int searchDAQGPUResources(irioDrv_t* p_DrvPvt,TStatus* status);

int searchIMAQGPUResources(irioDrv_t* p_DrvPvt,TStatus* status);
#endif

int searchIOResources(irioDrv_t* p_DrvPvt,TStatus* status);

int findIOSamplingRate(irioDrv_t* p_DrvPvt,TStatus* status);

int calcADCValue(irioDrv_t* p_DrvPvt,TStatus* status);



int irio_initDriver(const char *appCallID,const char *DeviceSerialNumber,const char *RIODeviceModel,
					const char *projectName, const char *FPGAversion, int verbosity,
					const char *headerDir,const char *bitfileDir, irioDrv_t* p_DrvPvt, TStatus* status){

	TIRIOStatusCode local_status = IRIO_success;
	memset(p_DrvPvt,0x00,sizeof(irioDrv_t));
	//Driver struct initialization
	p_DrvPvt->headerFile=NULL;
	p_DrvPvt->fpgaStarted=0;
	memset(p_DrvPvt->FPGAVIversion,0,SHORT_CHAR_STRING);
	//Copy data from method parameters to struct
	//EPICS portName
	p_DrvPvt->appCallID=malloc(strlen(appCallID)+1);
	strcpy(p_DrvPvt->appCallID,appCallID);

	//Device Serial Number
	strcpy(p_DrvPvt->DeviceSerialNumber,DeviceSerialNumber);

	//Device Model
	strcpy(p_DrvPvt->RIODeviceModel,RIODeviceModel);

	//Project Name
	p_DrvPvt->projectName=malloc(strlen(projectName)+1);
	strcpy(p_DrvPvt->projectName,projectName);

	//FPGA VI Version
	strcpy(p_DrvPvt->FPGAVIStringversion,FPGAversion);

	//Verbosity
	p_DrvPvt->verbosity = verbosity;

	//Init file for resource search
		//First build the header path
	char* headerPath=NULL;
	asprintf(&headerPath,"%s%s%s%s",headerDir,STRINGNAME_PREFIX,p_DrvPvt->projectName,".h");

	//Call for file init
	local_status |= irio_initFileSearch(p_DrvPvt,headerPath,(void**)&p_DrvPvt->headerFile,status);

	if(status->detailCode==FileNotFound_Error){
		status->detailCode=HeaderNotFound_Error;
	}

	//Free path to header file. Is no longer needed
	free(headerPath);

	//Build bitfilePath
	char* bitFilePath=NULL;
	asprintf(&bitFilePath,"%s%s%s%s",bitfileDir,STRINGNAME_PREFIX,p_DrvPvt->projectName,STRINGNAME_BITFILEEXT);

	//Configure target and dowload bitfile
	if(local_status<IRIO_error){
		local_status |= configureTarget(p_DrvPvt,bitFilePath,status);
	}

	//Search platform
	if(local_status<IRIO_error){
		local_status |= searchPlatform(p_DrvPvt,status);
	}

	//Seach for common resources
	if(local_status<IRIO_error){
		local_status |= searchMandatoryResourcesAllPlatforms(p_DrvPvt,status);
	}

	//Identify and search for profile resources
	if(local_status<IRIO_error){
		local_status |= searchProfile(p_DrvPvt,status);
	}

	//Free headerFile. Not necessary (would be free at irio_closeDriver, but is not longer used. prevents from keep unnecessary memory allocated.
	local_status |= irio_closeFileSearch(p_DrvPvt,(void**)&p_DrvPvt->headerFile,status);

	//If error during initialization, signal it
	if(local_status>=IRIO_error){
		local_status |= irio_mergeStatus(status,status->detailCode,p_DrvPvt->verbosity,"[%s,%d]-(%s) ERROR Initializing IRIO Device. Should finalize IRIO driver.\n",__func__,__LINE__,p_DrvPvt->appCallID);
	}

	if(local_status<IRIO_error){
		return local_status;
	}else{
		return IRIO_error;
	}
}

int irio_closeDriver(irioDrv_t* p_DrvPvt,uint32_t mode, TStatus* status){

	TIRIOStatusCode local_status = IRIO_success;
	local_status |= irio_closeFileSearch(p_DrvPvt,(void**)&p_DrvPvt->headerFile,status);

	NiFpga_Status fpgaStatus = NiFpga_Status_Success;

	fpgaStatus=NiFpga_Close(p_DrvPvt->session,mode); //instead of a 0
	if(NiFpga_IsError(fpgaStatus)){
		irio_mergeStatus(status,NIRIO_API_Error,p_DrvPvt->verbosity,"[%s,%d]-(%s) ERROR Closing IRIO Device Session. Error Code:%d\n",__func__,__LINE__,p_DrvPvt->appCallID,fpgaStatus);
		local_status |= IRIO_error;
	}

	local_status |= finalizeLibrary(p_DrvPvt,status);

	local_status |= freeXRIOEnums(p_DrvPvt,status);

	free(p_DrvPvt->appCallID);
	free(p_DrvPvt->projectName);

	if(local_status<IRIO_error){
		return local_status;
	}else{
		return IRIO_error;
	}
}

int configureTarget(irioDrv_t* p_DrvPvt, char* bitFilePath, TStatus* status){

	TIRIOStatusCode local_status = IRIO_success;
	//Find device from connected hardware and fill NiFpga driver portname
	local_status |= irio_findRIO(p_DrvPvt,status);

	//Build SignatureName
	char* signatureName=NULL;
	asprintf(&signatureName,"%s%s%s",STRINGNAME_PREFIX,p_DrvPvt->projectName,STRINGNAME_SIGNATURE);

	//Search SignatureValue
	char* signatureValue=NULL;
	local_status |= irio_findHeaderString(p_DrvPvt,p_DrvPvt->headerFile,signatureName,&signatureValue,status);

	if(status->detailCode==ResourceNotFound_Error){
		status->detailCode=SignatureNotFound_Error;
	}else if(status->detailCode==ResourceValueNotValid_Error){
		status->detailCode=SignatureValueNotValid_Error;
	}

	//Load NiFpga library
	if(local_status<IRIO_error){
		local_status |= initializeLibrary(p_DrvPvt,status);
	}

	//Download bitfile to the FPGA and open a session. Parameter '0' is given as OpenAttribute to start running the bitfile immediately
	NiFpga_Status fpgaStatus = NiFpga_Status_Success;
	if(local_status<IRIO_error){
		NiFpga_MergeStatus(&fpgaStatus,NiFpga_Open(bitFilePath,signatureValue,p_DrvPvt->fpgaRIO,NiFpga_OpenAttribute_NoRun,&p_DrvPvt->session));
		if(NiFpga_IsError(fpgaStatus)){
			irio_mergeStatus(status,BitfileDownload_Error,p_DrvPvt->verbosity,"[%s,%d]-(%s) ERROR Downloading bitfile. Error Code:%d\n",__func__,__LINE__,p_DrvPvt->appCallID,fpgaStatus);
			local_status |= IRIO_error;
		}
	}

	if(p_DrvPvt->verbosity && local_status<IRIO_error){
		printf("[%s,%d]-(%s) TRACE Bitfile Downloaded. FPGA Status:%d\n",__func__,__LINE__,p_DrvPvt->appCallID,fpgaStatus);
	}

	free(bitFilePath);
	free(signatureName);
	free(signatureValue);

	if(local_status<IRIO_error){
		return local_status;
	}else{
		return IRIO_error;
	}
}

int initializeLibrary(irioDrv_t* p_DrvPvt,TStatus* status){
	#ifdef CLOSE_VERSION_NIRIO
		NiFpga_Status fpgaStatus = NiFpga_Status_Success;
		fpgaStatus = NiFpga_Initialize();
		if(NiFpga_IsError(fpgaStatus)){
			irio_mergeStatus(status,NIRIO_API_Error,p_DrvPvt->verbosity,"[%s,%d]-(%s) ERROR Initializing NiFpga Library. Error Code:%d\n",__func__,__LINE__,p_DrvPvt->appCallID,fpgaStatus);
			return IRIO_error;
		}
	#endif
	return IRIO_success;
}

int finalizeLibrary(irioDrv_t* p_DrvPvt, TStatus* status){
	#ifdef CLOSE_VERSION_NIRIO
		NiFpga_Status fpgaStatus = NiFpga_Status_Success;
		fpgaStatus = NiFpga_Finalize();
		if(NiFpga_IsError(fpgaStatus)){
			irio_mergeStatus(status,NIRIO_API_Error,p_DrvPvt->verbosity,"[%s,%d]-(%s) ERROR Finalizing NiFpga Library. Error Code:%d\n",__func__,__LINE__,p_DrvPvt->appCallID,fpgaStatus);
			return IRIO_error;
		}
	#endif
		return IRIO_success;
}

int searchPlatform( irioDrv_t* p_DrvPvt,TStatus* status){

	TIRIOStatusCode local_status = IRIO_success;
	NiFpga_Status fpgaStatus = NiFpga_Status_Success;
	//Search for Platform
	local_status |= irio_findResourceEnum(p_DrvPvt,STRINGNAME_PLATFORM,-1,&p_DrvPvt->enumPlatform,status,p_DrvPvt->verbosity);

	if(local_status>=IRIO_error){
		return IRIO_error;
	}

	p_DrvPvt->platform=-1;
	NiFpga_MergeStatus(&fpgaStatus,NiFpga_ReadU8(p_DrvPvt->session,p_DrvPvt->enumPlatform.value,&p_DrvPvt->platform));
	if(NiFpga_IsError(fpgaStatus)){
		irio_mergeStatus(status,NIRIO_API_Error,p_DrvPvt->verbosity,"[%s,%d]-(%s) ERROR Reading %s. Error Code:%d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_PLATFORM,fpgaStatus);
		return IRIO_error;
	}

	switch(p_DrvPvt->platform){
	case IRIO_FlexRIO:
		if(p_DrvPvt->verbosity && status->code!=IRIO_error){
			printf("[%s,%d]-(%s) TRACE Detected platform FlexRIO.\n",__func__,__LINE__,p_DrvPvt->appCallID);
		}
		local_status |= allocFlexRIOEnums(p_DrvPvt,status);
		break;
	case IRIO_cRIO:
		if(p_DrvPvt->verbosity && status->code!=IRIO_error){
			printf("[%s,%d]-(%s) TRACE Detected platform cRIO.\n",__func__,__LINE__,p_DrvPvt->appCallID);
		}
		local_status |= allocCRIOEnums(p_DrvPvt,status);
		break;
	case IRIO_RSeries:
		irio_mergeStatus(status,FeatureNotImplemented_Error,p_DrvPvt->verbosity,"[%s,%d]-(%s) ERROR Platform RSeries Not Supported\n",__func__,__LINE__,p_DrvPvt->appCallID);
		local_status |= IRIO_error;
		break;
	}

	if(local_status<IRIO_error){
		return local_status;
	}else{
		return IRIO_error;
	}
}

int allocFlexRIOEnums(irioDrv_t* p_DrvPvt,TStatus* status){
	//Analog Limits
	p_DrvPvt->max_analoginputs = FLEXRIO_MAX_ANALOGS_IN;
	p_DrvPvt->enumAnalogInput = calloc(p_DrvPvt->max_analoginputs,sizeof(TResourcePort));
	p_DrvPvt->max_auxanaloginputs = FLEXRIO_MAX_AUXA_IN;
	p_DrvPvt->enumauxAI = calloc(p_DrvPvt->max_auxanaloginputs,sizeof(TResourcePort));
	p_DrvPvt->max_analogoutputs = FLEXRIO_MAX_ANALOGS_OUT;
	p_DrvPvt->enumAnalogOutput= calloc(p_DrvPvt->max_analogoutputs,sizeof(TResourcePort));
	p_DrvPvt->enumAOEnable = calloc(p_DrvPvt->max_analogoutputs,sizeof(TResourcePort));
	p_DrvPvt->max_auxanalogoutputs = FLEXRIO_MAX_AUXA_OUT;
	p_DrvPvt->enumauxAO = calloc(p_DrvPvt->max_auxanalogoutputs,sizeof(TResourcePort));

	//Digital Limits
	p_DrvPvt->max_digitalsinputs = FLEXRIO_MAX_DIGITALS;
	p_DrvPvt->enumDigitalInput = calloc(p_DrvPvt->max_digitalsinputs,sizeof(TResourcePort));
	p_DrvPvt->max_auxdigitalsinputs = FLEXRIO_MAX_AUXDIGITALS;
	p_DrvPvt->enumauxDI = calloc(p_DrvPvt->max_auxdigitalsinputs,sizeof(TResourcePort));
	p_DrvPvt->max_digitalsoutputs = FLEXRIO_MAX_DIGITALS;
	p_DrvPvt->enumDigitalOutput= calloc(p_DrvPvt->max_digitalsoutputs,sizeof(TResourcePort));
	p_DrvPvt->max_auxdigitalsoutputs = FLEXRIO_MAX_AUXDIGITALS;
	p_DrvPvt->enumauxDO= calloc(p_DrvPvt->max_auxdigitalsoutputs,sizeof(TResourcePort));

	//DMA Limits
	p_DrvPvt->max_dmas = FLEXRIO_MAX_DMAS;
	p_DrvPvt->DMATtoHOSTNCh = calloc(p_DrvPvt->max_dmas,sizeof(uint16_t));
	p_DrvPvt->DMATtoHOSTChIndex = calloc(p_DrvPvt->max_dmas,sizeof(uint16_t));
	p_DrvPvt->enumDMATtoHOST = calloc(p_DrvPvt->max_dmas,sizeof(TResourcePort));
	p_DrvPvt->enumDMATtoHOSTEnable = calloc(p_DrvPvt->max_dmas,sizeof(TResourcePort));
	p_DrvPvt->enumDMATtoHOSTSamplingRate = calloc(p_DrvPvt->max_dmas,sizeof(TResourcePort));
	p_DrvPvt->DMATtoHOSTFrameType = calloc(p_DrvPvt->max_dmas,sizeof(uint8_t));
	p_DrvPvt->DMATtoHOSTSampleSize = calloc(p_DrvPvt->max_dmas,sizeof(uint8_t));
	p_DrvPvt->DMATtoHOSTBlockNWords = calloc(p_DrvPvt->max_dmas,sizeof(uint16_t));

	//Signal Generator Limits
	p_DrvPvt->max_numberofSG = FLEXRIO_MAX_SIGNALGENERATOR;
	p_DrvPvt->enumSGFreq = calloc(p_DrvPvt->max_numberofSG,sizeof(TResourcePort));
	p_DrvPvt->enumSGAmp = calloc(p_DrvPvt->max_numberofSG,sizeof(TResourcePort));
	p_DrvPvt->enumSGPhase= calloc(p_DrvPvt->max_numberofSG,sizeof(TResourcePort));
	p_DrvPvt->enumSGSignalType = calloc(p_DrvPvt->max_numberofSG,sizeof(TResourcePort));
	p_DrvPvt->enumSGUpdateRate = calloc(p_DrvPvt->max_numberofSG,sizeof(TResourcePort));
	p_DrvPvt->enumSGFref = calloc(p_DrvPvt->max_numberofSG,sizeof(TResourcePort));
	p_DrvPvt->SGfref = calloc(p_DrvPvt->max_numberofSG,sizeof(uint32_t));

	//DMA for GPU  limits
	p_DrvPvt->DMATtoGPUNCh = calloc(p_DrvPvt->max_dmas,sizeof(uint16_t));
	p_DrvPvt->DMATtoGPUChIndex = calloc(p_DrvPvt->max_dmas,sizeof(uint16_t));
	p_DrvPvt->enumDMATtoGPU = calloc(p_DrvPvt->max_dmas,sizeof(TResourcePort));
	p_DrvPvt->enumDMATtoGPUEnable = calloc(p_DrvPvt->max_dmas,sizeof(TResourcePort));
	p_DrvPvt->enumDMATtoGPUSamplingRate = calloc(p_DrvPvt->max_dmas,sizeof(TResourcePort));
	p_DrvPvt->DMATtoGPUFrameType = calloc(p_DrvPvt->max_dmas,sizeof(uint8_t));
	p_DrvPvt->DMATtoGPUSampleSize = calloc(p_DrvPvt->max_dmas,sizeof(uint8_t));
	p_DrvPvt->DMATtoGPUBlockNWords = calloc(p_DrvPvt->max_dmas,sizeof(uint16_t));

	return IRIO_success;
}

int allocCRIOEnums(irioDrv_t* p_DrvPvt, TStatus* status){
	//IOModules
	p_DrvPvt->InsertedModulesID = calloc(CRIO_MAX_MODULES,sizeof(uint16_t));
	p_DrvPvt->enumSamplingRate = calloc(CRIO_MAX_MODULES,sizeof(TResourcePort));

	//Analog Limits
	p_DrvPvt->max_analoginputs = CRIO_MAX_ANALOGS_IN;
	p_DrvPvt->enumAnalogInput = calloc(p_DrvPvt->max_analoginputs,sizeof(TResourcePort));
	p_DrvPvt->max_auxanaloginputs = CRIO_MAX_AUXA_IN;
	p_DrvPvt->enumauxAI = calloc(p_DrvPvt->max_auxanaloginputs,sizeof(TResourcePort));
	p_DrvPvt->max_analogoutputs = CRIO_MAX_ANALOGS_OUT;
	p_DrvPvt->enumAnalogOutput= calloc(p_DrvPvt->max_analogoutputs,sizeof(TResourcePort));
	p_DrvPvt->enumAOEnable = calloc(p_DrvPvt->max_analogoutputs,sizeof(TResourcePort));
	p_DrvPvt->max_auxanalogoutputs = CRIO_MAX_AUXA_OUT;
	p_DrvPvt->enumauxAO = calloc(p_DrvPvt->max_auxanalogoutputs,sizeof(TResourcePort));

	//Digital Limits
	p_DrvPvt->max_digitalsinputs = CRIO_MAX_DIGITALS;
	p_DrvPvt->enumDigitalInput = calloc(p_DrvPvt->max_digitalsinputs,sizeof(TResourcePort));
	p_DrvPvt->max_auxdigitalsinputs = CRIO_MAX_AUXDIGITALS;
	p_DrvPvt->enumauxDI = calloc(p_DrvPvt->max_auxdigitalsinputs,sizeof(TResourcePort));
	p_DrvPvt->max_digitalsoutputs = CRIO_MAX_DIGITALS;
	p_DrvPvt->enumDigitalOutput= calloc(p_DrvPvt->max_digitalsoutputs,sizeof(TResourcePort));
	p_DrvPvt->max_auxdigitalsoutputs = CRIO_MAX_AUXDIGITALS;
	p_DrvPvt->enumauxDO= calloc(p_DrvPvt->max_auxdigitalsoutputs,sizeof(TResourcePort));

	//DMA Limits
	p_DrvPvt->max_dmas = CRIO_MAX_DMAS;
	p_DrvPvt->DMATtoHOSTNCh = calloc(p_DrvPvt->max_dmas,sizeof(uint16_t));
	p_DrvPvt->DMATtoHOSTChIndex = calloc(p_DrvPvt->max_dmas,sizeof(uint16_t));
	p_DrvPvt->enumDMATtoHOST = calloc(p_DrvPvt->max_dmas,sizeof(TResourcePort));
	p_DrvPvt->enumDMATtoHOSTEnable = calloc(p_DrvPvt->max_dmas,sizeof(TResourcePort));
	p_DrvPvt->enumDMATtoHOSTSamplingRate = calloc(p_DrvPvt->max_dmas,sizeof(TResourcePort));
	p_DrvPvt->DMATtoHOSTFrameType = calloc(p_DrvPvt->max_dmas,sizeof(uint8_t));
	p_DrvPvt->DMATtoHOSTSampleSize = calloc(p_DrvPvt->max_dmas,sizeof(uint8_t));
	p_DrvPvt->DMATtoHOSTBlockNWords = calloc(p_DrvPvt->max_dmas,sizeof(uint16_t));

	//Signal Generator Limits
	p_DrvPvt->max_numberofSG = CRIO_MAX_SIGNALGENERATOR;
	p_DrvPvt->enumSGFreq= calloc(p_DrvPvt->max_numberofSG,sizeof(TResourcePort));
	p_DrvPvt->enumSGAmp = calloc(p_DrvPvt->max_numberofSG,sizeof(TResourcePort));
	p_DrvPvt->enumSGPhase= calloc(p_DrvPvt->max_numberofSG,sizeof(TResourcePort));
	p_DrvPvt->enumSGSignalType = calloc(p_DrvPvt->max_numberofSG,sizeof(TResourcePort));
	p_DrvPvt->enumSGUpdateRate = calloc(p_DrvPvt->max_numberofSG,sizeof(TResourcePort));
	p_DrvPvt->enumSGFref = calloc(p_DrvPvt->max_numberofSG,sizeof(TResourcePort));
	p_DrvPvt->SGfref = calloc(p_DrvPvt->max_numberofSG,sizeof(uint32_t));

	return IRIO_success;
}

int freeXRIOEnums(irioDrv_t* p_DrvPvt, TStatus* status){
	//cRIO only
	free(p_DrvPvt->enumSamplingRate);
	free(p_DrvPvt->InsertedModulesID);

	//Analog Limits
	free(p_DrvPvt->enumAnalogInput);
	free(p_DrvPvt->enumauxAI);
	free(p_DrvPvt->enumAnalogOutput);
	free(p_DrvPvt->enumAOEnable);
	free(p_DrvPvt->enumauxAO);

	//Digital Limits
	free(p_DrvPvt->enumDigitalInput);
	free(p_DrvPvt->enumauxDI);
	free(p_DrvPvt->enumDigitalOutput);
	free(p_DrvPvt->enumauxDO);

	//DMA Limits
	free(p_DrvPvt->DMATtoHOSTNCh);
	free(p_DrvPvt->DMATtoHOSTChIndex);
	free(p_DrvPvt->enumDMATtoHOST);
	free(p_DrvPvt->enumDMATtoHOSTEnable);
	free(p_DrvPvt->enumDMATtoHOSTSamplingRate);
	free(p_DrvPvt->DMATtoHOSTFrameType);
	free(p_DrvPvt->DMATtoHOSTSampleSize);
	free(p_DrvPvt->DMATtoHOSTBlockNWords);

	//Signal Generator Limits
	free(p_DrvPvt->enumSGFreq);
	free(p_DrvPvt->enumSGAmp);
	free(p_DrvPvt->enumSGPhase);
	free(p_DrvPvt->enumSGSignalType);
	free(p_DrvPvt->enumSGUpdateRate);
	free(p_DrvPvt->enumSGFref);

	// DMA for GPU

	free(p_DrvPvt->DMATtoGPUNCh);
	free(p_DrvPvt->DMATtoGPUChIndex);
	free(p_DrvPvt->enumDMATtoGPU);
	free(p_DrvPvt->enumDMATtoGPUEnable);
	free(p_DrvPvt->enumDMATtoGPUSamplingRate);
	free(p_DrvPvt->DMATtoGPUFrameType);
	free(p_DrvPvt->DMATtoGPUSampleSize);
	free(p_DrvPvt->DMATtoGPUBlockNWords);
	return IRIO_success;
}

int searchMandatoryResourcesAllPlatforms( irioDrv_t* p_DrvPvt, TStatus* status){

	TIRIOStatusCode local_status = IRIO_success;
	NiFpga_Status fpgaStatus = NiFpga_Status_Success;

	//Mandatory Resources. If error occur, let it be.

	//Search for FPGAVIversion
	local_status |= irio_findResourceEnum(p_DrvPvt,STRINGNAME_FPGAVIVERSION,-1,&p_DrvPvt->enumFPGAVIversion,status,p_DrvPvt->verbosity);

	//Check FPGAVIversion
	//char auxVersion[SHORT_CHAR_STRING]="         ";
	char *auxVersion=NULL;
	NiFpga_MergeStatus(&fpgaStatus,NiFpga_ReadArrayU8(p_DrvPvt->session,p_DrvPvt->enumFPGAVIversion.value,(uint8_t*)p_DrvPvt->FPGAVIversion,2));
	if(NiFpga_IsNotError(fpgaStatus)){
		asprintf(&auxVersion,"V%d.%d",p_DrvPvt->FPGAVIversion[0],p_DrvPvt->FPGAVIversion[1]);
	}
	if(NiFpga_IsError(fpgaStatus) || strcmp(auxVersion,p_DrvPvt->FPGAVIStringversion)!=0){
		irio_mergeStatus(status,NIRIO_API_Error,p_DrvPvt->verbosity,"[%s,%d]-(%s) ERROR VI Version Check Failed. Expected:%s Read:%s .Error Code:%d\n",__func__,__LINE__,p_DrvPvt->appCallID,p_DrvPvt->FPGAVIStringversion,auxVersion,fpgaStatus);
		local_status |= IRIO_error;
		fpgaStatus=NiFpga_Status_Success;
	}
	free(auxVersion);
	//Search for InitDone
	local_status |= irio_findResourceEnum(p_DrvPvt,STRINGNAME_INITDONE,-1,&p_DrvPvt->enumInitDone,status,p_DrvPvt->verbosity);

	switch (p_DrvPvt->platform)
	{
		case IRIO_FlexRIO:
			//Search for InsertedIOModulesID
			local_status |= irio_findResourceEnum(p_DrvPvt,STRINGNAME_INSERTEDIOMODULEID,-1,&p_DrvPvt->enumInsertedIoModuleID,status,p_DrvPvt->verbosity);

			//Search for RIOAdapterCorrect
			local_status |= irio_findResourceEnum(p_DrvPvt,STRINGNAME_RIOADAPTERCORRECT,-1,&p_DrvPvt->enumRIOAdapterCorrect,status,p_DrvPvt->verbosity);
			break;

		case IRIO_cRIO:
			// Search for InsertedIOModulesID: Be careful, this is different from FlexRIO and now it is an array


			local_status |= irio_findResourceEnum(p_DrvPvt,STRINGNAME_INSERTEDIOMODULESID,-1,&p_DrvPvt->enumInsertedModulesID,status,p_DrvPvt->verbosity);

			//Search for cRIOModulesOK
			local_status |= irio_findResourceEnum(p_DrvPvt,STRINGNAME_CRIOMODULESOK,-1,&p_DrvPvt->enumcRIOModulesOk,status,p_DrvPvt->verbosity);
			break;

		case IRIO_RSeries:
			//TO BE DETERMINED, NOT CURRENTLY SUPPORTED
		break;
	}

	//Search for Fref
	local_status |= irio_findResourceEnum(p_DrvPvt,STRINGNAME_FREQREF,-1,&p_DrvPvt->enumFref,status,p_DrvPvt->verbosity);

	//Read Fref
	NiFpga_MergeStatus(&fpgaStatus,NiFpga_ReadU32(p_DrvPvt->session,p_DrvPvt->enumFref.value,&p_DrvPvt->Fref));
	if(NiFpga_IsError(fpgaStatus) || p_DrvPvt->Fref==0){
		irio_mergeStatus(status,NIRIO_API_Error,p_DrvPvt->verbosity,"[%s,%d]-(%s) ERROR Reading %s. Error Code:%d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_FREQREF,fpgaStatus);
		local_status |= IRIO_error;
		fpgaStatus=NiFpga_Status_Success;
	}

	//Search for DevQualityStatus.
	local_status |= irio_findResourceEnum(p_DrvPvt,STRINGNAME_DEVQUALITYSTATUS,-1,&p_DrvPvt->enumDevQualityStatus,status,p_DrvPvt->verbosity);

	//Search for DeviceTemp.
	local_status |= irio_findResourceEnum(p_DrvPvt,STRINGNAME_DEVTEMP,-1,&p_DrvPvt->enumDevTemp,status,p_DrvPvt->verbosity);

	//Search for DevProfile
	local_status |= irio_findResourceEnum(p_DrvPvt,STRINGNAME_DEVPROFILE,-1,&p_DrvPvt->enumDevProfile,status,p_DrvPvt->verbosity);

	//Search for DAQStartStop
	local_status |= irio_findResourceEnum(p_DrvPvt,STRINGNAME_DAQSTARTSTOP,-1,&p_DrvPvt->enumDAQStartStop,status,p_DrvPvt->verbosity);

	//Search for DebugMode
	local_status |= irio_findResourceEnum(p_DrvPvt,STRINGNAME_DEBUGMODE,-1,&p_DrvPvt->enumDebugMode,status,p_DrvPvt->verbosity);

	if(local_status<IRIO_error){
		return local_status;
	}else{
		return IRIO_error;
	}
}

int searchProfile(irioDrv_t* p_DrvPvt,TStatus* status){

	TIRIOStatusCode local_status = IRIO_success;
	NiFpga_Status fpgaStatus = NiFpga_Status_Success;
	p_DrvPvt->devProfile=-1;
	NiFpga_MergeStatus(&fpgaStatus,NiFpga_ReadU8(p_DrvPvt->session,p_DrvPvt->enumDevProfile.value,&p_DrvPvt->devProfile));
	if(NiFpga_IsError(fpgaStatus)){
		irio_mergeStatus(status,NIRIO_API_Error,p_DrvPvt->verbosity,"[%s,%d]-(%s) ERROR Reading . Error Code:%d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_DEVPROFILE,fpgaStatus);
		local_status|=IRIO_error;
	}else{
		switch (p_DrvPvt->platform){
			case IRIO_FlexRIO:
				switch (p_DrvPvt->devProfile) {
					case 0:
						if(p_DrvPvt->verbosity){
							printf("[%s,%d]-(%s) TRACE FlexRIO DAQ profile detected.\n",__func__,__LINE__,p_DrvPvt->appCallID);
						}
						local_status |= searchDAQResources(p_DrvPvt,status);
						break;
					case 1:
						if(p_DrvPvt->verbosity){
							printf("[%s,%d]-(%s) TRACE FlexRIO IMAQ profile detected.\n",__func__,__LINE__,p_DrvPvt->appCallID);
						}
						local_status |= searchIMAQResources(p_DrvPvt,status);
						break;
					#ifdef IRIO_GPU
					case 2:
						if(p_DrvPvt->verbosity){
							printf("[%s,%d]-(%s) TRACE FlexRIO DAQ GPU profile detected.\n",__func__,__LINE__,p_DrvPvt->appCallID);
						}
						local_status |= searchDAQGPUResources(p_DrvPvt,status);
						break;
					case 3:
						if(p_DrvPvt->verbosity){
							printf("[%s,%d]-(%s) TRACE FlexRIO IMAQ GPU profile detected.\n",__func__,__LINE__,p_DrvPvt->appCallID);
						}
						local_status |= searchIMAQGPUResources(p_DrvPvt,status);
						break;
					#endif
					default:
						irio_mergeStatus(status,FeatureNotImplemented_Error,p_DrvPvt->verbosity,"[%s,%d]-(%s) TRACE ERROR FlexRIO profile (%d) not defined.\n",__func__,__LINE__,p_DrvPvt->appCallID,p_DrvPvt->devProfile);
						local_status |= IRIO_error;
						break;
				}
			break;
			case IRIO_cRIO:
				switch (p_DrvPvt->devProfile) {

					case 0:
						if(p_DrvPvt->verbosity){
							printf("[%s,%d]-(%s) cRIO DAQ profile detected.\n",__func__,__LINE__,p_DrvPvt->appCallID);
						}
						local_status |= searchDAQResources(p_DrvPvt,status);

						break;
					case 1:
						if(p_DrvPvt->verbosity){
							printf("[%s,%d]-(%s) cRIO IO profile detected.\n",__func__,__LINE__,p_DrvPvt->appCallID);
						}
						local_status = searchIOResources(p_DrvPvt,status);
						break;

					default:
						irio_mergeStatus(status,FeatureNotImplemented_Error,p_DrvPvt->verbosity,"[%s,%d]-(%s) ERROR cRIO profile (%d) not defined.\n",__func__,__LINE__,p_DrvPvt->appCallID,p_DrvPvt->devProfile);
						local_status |= IRIO_error;
						break;
				}
			break;
			case IRIO_RSeries:
				switch (p_DrvPvt->devProfile) {

					default:
						irio_mergeStatus(status,FeatureNotImplemented_Error,p_DrvPvt->verbosity,"[%s,%d]-(%s) ERROR RSeries profile (%d) not defined.\n",__func__,__LINE__,p_DrvPvt->appCallID,p_DrvPvt->devProfile);
						local_status |= IRIO_error;
						break;
				}
			break;

		}
	}

	if(local_status<IRIO_error){
		return local_status;
	}else{
		return IRIO_error;
	}
}

int searchDAQResources(irioDrv_t* p_DrvPvt,TStatus* status){

	TIRIOStatusCode local_status = IRIO_success;

	local_status |= calcADCValue(p_DrvPvt,status);

	local_status |= irio_findAnalogs(p_DrvPvt,status);

	local_status |= irio_findAuxAnalogs(p_DrvPvt,status);

	local_status |= irio_findDigitals(p_DrvPvt,status);

	local_status |= irio_findAuxDigitals(p_DrvPvt,status);

	local_status |= irio_findSGs(p_DrvPvt,status);

	local_status |= irio_findDAQDMAs(p_DrvPvt,status);

	if(local_status<IRIO_error){
		return local_status;
	}else{
		return IRIO_error;
	}
}

#ifdef IRIO_GPU
int searchDAQGPUResources(irioDrv_t* p_DrvPvt, TStatus* status){

	TIRIOStatusCode local_status = IRIO_success;

	local_status |= calcADCValue(p_DrvPvt,status);

	local_status |= irio_findAnalogs(p_DrvPvt,status);

	local_status |= irio_findAuxAnalogs(p_DrvPvt,status);
	

	local_status |= irio_findDigitals(p_DrvPvt,status);


	local_status |= irio_findAuxDigitals(p_DrvPvt,status);

	local_status |= irio_findSGs(p_DrvPvt,status);

	local_status |= irio_findDAQDMAsGPU(p_DrvPvt,status);

	if(local_status<IRIO_error){
		return local_status;
	}else{
		return IRIO_error;
	}
}
#endif

int searchIOResources(irioDrv_t* p_DrvPvt, TStatus* status){

	TIRIOStatusCode local_status = IRIO_success;
	local_status |= calcADCValue(p_DrvPvt,status);

	local_status |= irio_findAnalogs(p_DrvPvt,status);

	local_status |= irio_findDigitals(p_DrvPvt,status);

	local_status |= irio_findAuxAnalogs(p_DrvPvt,status);

	local_status |= irio_findAuxDigitals(p_DrvPvt,status);

	local_status |= irio_findSGs(p_DrvPvt,status);

	local_status |= findIOSamplingRate(p_DrvPvt,status);

	if(local_status<IRIO_error){
		return local_status;
	}else{
		return IRIO_error;
	}
}

int findIOSamplingRate(irioDrv_t* p_DrvPvt,TStatus* status){
	//Search for IO sampling rates
	TStatus local_status;
	irio_initStatus(&local_status);
	int i;
	int count=0;

	if(p_DrvPvt->verbosity){
		printf("[%s,%d]-(%s) TRACE IO Sampling Rates found:",__func__,__LINE__,p_DrvPvt->appCallID);
	}

	//Sampling Rates are optional.
	//There is no fail for not finding and SamplingRate
	for(i=0;i<CRIO_MAX_MODULES;i++){
		if(irio_findResourceEnum(p_DrvPvt,STRINGNAME_SAMPLINGRATE,i,&p_DrvPvt->enumSamplingRate[i],&local_status,0)==IRIO_success){
			if(p_DrvPvt->verbosity){
				printf("SamplingRate%d, ",i);
			}
			count++;
		}
		irio_resetStatus(&local_status);
	}

	if(p_DrvPvt->verbosity){
		printf("\n");
		printf("[%s,%d]-(%s) TRACE TOTAL Sampling Rates found: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,count);
	}

	return IRIO_success;
}

int searchIMAQResources(irioDrv_t* p_DrvPvt,TStatus* status){

	TIRIOStatusCode local_status = IRIO_success;
	local_status |= irio_findIMAQDMAs(p_DrvPvt,status);

	local_status |= irio_findCL(p_DrvPvt,status);

	local_status |= irio_findDigitals(p_DrvPvt,status);

	local_status |= irio_findAuxAnalogs(p_DrvPvt,status);

	local_status |= irio_findAuxDigitals(p_DrvPvt,status);

	if(local_status<IRIO_error){
		return local_status;
	}else{
		return IRIO_error;
	}
}

#ifdef IRIO_GPU
int searchIMAQGPUResources(irioDrv_t* p_DrvPvt,TStatus* status){

	TIRIOStatusCode local_status = IRIO_success;

	local_status |= irio_findIMAQDMAsGPU(p_DrvPvt,status);

	local_status |= irio_findCL(p_DrvPvt,status);

	local_status |= irio_findAuxAnalogs(p_DrvPvt,status);

	local_status |= irio_findAuxDigitals(p_DrvPvt,status);

	if(local_status<IRIO_error){
		return local_status;
	}else{
		return IRIO_error;
	}
}
#endif

int calcADCValue(irioDrv_t* p_DrvPvt,TStatus* status){
	TIRIOStatusCode local_status = IRIO_success;
	double auxADC=1;
	double auxDAC=1;
	int32_t moduleID;
	int32_t statuscode;
	switch (p_DrvPvt->platform)
	{
	case IRIO_FlexRIO:
		//we are not verifying the module in use..
		//Two options, read using IRIO library the InsetedIOModule (this only can be called if FPGA is running!!!) or using flexRIO functions
		//Using FlexRIO library here because FPGA is not running

		statuscode=NiFlexRio_GetAttribute(p_DrvPvt->session, NIFLEXRIO_Attr_InsertedFamID, NIFLEXRIO_ValueType_U32,&moduleID);
		printf("Module ID found=%x\n", moduleID);
		//Modules ID for Analog or Digital
		// NI5761: 0x109374C6, supported by ITER in AC version not DC, 4 analog inputs
		// NI5781: 0x1093748A, not supported by ITER, 2 AI and 2 AO

		// Ni6581: 0x10937418, supported by ITER, digital I/O module
		switch (moduleID){

		case FlexRIO_Module_IO_NI5761: //module ID for NI5761. Problem: not known if AC or DC :(!!!

			printf("Module NI5761 found connected to FlexRIO device\n");
			p_DrvPvt->couplingMode=IRIO_coupling_AC; //configured with AC coupling (Initial value)
			p_DrvPvt->CVADC=(1.035/8191.0);  //this conversion is for NI5761 module (AC) from www.ni.com/pdf/375509.pdf
			//DrvPvt->CVDAC=32767 //with NI5781 module
			p_DrvPvt->CVDAC=8191.0/1.035;  //with NI5761 there is no physical DAC. Same Digital valuer generated by DAC is read by ADC
			//If using signal generator in this design we assume that this is internally connected in the FPGA
			p_DrvPvt->maxAnalogOut=1;
			p_DrvPvt->minAnalogOut=-1;

			break;
		case FlexRIO_Module_IO_NI6581: //module ID for NI6581.

			printf("Module NI6581 found connected to FlexRIO device\n");
			p_DrvPvt->CVADC=1;

			p_DrvPvt->CVDAC=1;
			p_DrvPvt->couplingMode=IRIO_coupling_NULL; // coupling mode has no sense in digital input
			//If using signal generator in this design we assume that this is internally connected in the FPGA

			p_DrvPvt->maxAnalogOut=1;
			p_DrvPvt->minAnalogOut=-1;

			break;
		case FlexRIO_Module_IO_NI5734:
			printf("Module NI5734 found connected to FlexRIO device. This adapter module is not in ITER HW catalog\n");
			//This module has oncly DC coulping input
			//This module has differential or single ended connection
			p_DrvPvt->couplingMode=IRIO_coupling_NULL; //This module supports configuration of coupling using SPI
			p_DrvPvt->CVADC=(2.0/65536.0);
			p_DrvPvt->CVDAC=65536.0/2.0;
			p_DrvPvt->maxAnalogOut=1;
			p_DrvPvt->minAnalogOut=-1;
			break;
		case FlexRIO_Module_IO_NI5781: //module ID for NI5781.

			printf("Module NI5781 found connected to FlexRIO device. This adpater modeule is not in ITER HW catalog\n");
			//This module has oncly DC coulping input
			//This module has differential or single ended connection
			p_DrvPvt->couplingMode=IRIO_coupling_DC; //configured with AC coupling (Initial value)
			p_DrvPvt->CVADC=(2/16384.0);  //this conversion is for NI5781 module (DC) from www.ni.com/pdf/372968a.pdf
			p_DrvPvt->CVDAC=65536;  //with NI5781 there is physical ADC. Output range is +-0.5V with 50ohm load.
			//TODO: considering the connection of the outputs signals, differential, single ended, 50ohms or infinite load?
			p_DrvPvt->maxAnalogOut=0.5;
			p_DrvPvt->minAnalogOut=-0.5;

//		case IRIO_coupling_DC: // DC coupling
//			p_DrvPvt->couplingMode=IRIO_coupling_DC;
//			p_DrvPvt->CVADC=(0.635/8191.0);  //this conversion is for NI5761 module (DC)
//			p_DrvPvt->CVDAC=(8191.0/0.635);
//			p_DrvPvt->maxAnalogOut=0.635;
//			p_DrvPvt->minAnalogOut=-0.635;

			break;
		}


		break;
	case IRIO_cRIO:
		// we are considering that we have only one model in cRIO C Module
		// cRIO C Module NI9205 for ADC
		// cRIO C Module NI9264 for DAC
		auxADC=auxADC*pow(2,-21);
		auxDAC=1*pow(2,15);
		p_DrvPvt->CVADC=auxADC;  //NI9205 is read in raw format with 32 bits (FXP). Conversion is done multiplying by 2(^-21)
		p_DrvPvt->CVDAC=auxDAC;  //NI9264 DAC module
		p_DrvPvt->maxAnalogOut=10;
		p_DrvPvt->minAnalogOut=-10;
		p_DrvPvt->couplingMode=IRIO_coupling_DC; //NI9205 is DC only
		break;
	}
	p_DrvPvt->minSamplingRate=((float)p_DrvPvt->Fref/U16BIT); //!< Minimum sampling rate permitted by the FPGA
	p_DrvPvt->maxSamplingRate=(p_DrvPvt->Fref);

	if(p_DrvPvt->verbosity){
		printf("Sampling rate Min: %f, Max %f\n", p_DrvPvt->minSamplingRate, p_DrvPvt->maxSamplingRate);
	}

	if(local_status<IRIO_error){
		return local_status;
	}else{
		return IRIO_error;
	}
}

int irio_setAICoupling(irioDrv_t* p_DrvPvt,TIRIOCouplingMode value, TStatus* status)
{
	int32_t moduleID;
	int32_t statuscode;
	if (p_DrvPvt->platform == IRIO_FlexRIO){
		statuscode=NiFlexRio_GetAttribute(p_DrvPvt->session, NIFLEXRIO_Attr_InsertedFamID, NIFLEXRIO_ValueType_U32,&moduleID);
		printf("Module ID found=%x\n", moduleID);
		switch (moduleID)			{
			case FlexRIO_Module_IO_NI5761: //only for NI5761
				switch(value)
						{
						case IRIO_coupling_AC: // AC Coupling
							p_DrvPvt->couplingMode=IRIO_coupling_AC; //configured with AC coupling (Initial value)
							p_DrvPvt->CVADC=(1.035/8191.0);  //this conversion is for NI5761 module (AC) from www.ni.com/pdf/375509.pdf
							//DrvPvt->CVDAC=32767 //with NI5781 module
							p_DrvPvt->CVDAC=8191.0/1.035;  //with NI5761 there is no physical ADC. Same Digital valuer generated by DAC is read by ADC
							//If using signal generator in this design we assume that this is internally connected in the FPGA
							p_DrvPvt->maxAnalogOut=1;
							p_DrvPvt->minAnalogOut=-1;

						break;

						case IRIO_coupling_DC: // DC coupling
							p_DrvPvt->couplingMode=IRIO_coupling_DC;
							p_DrvPvt->CVADC=(0.635/8191.0);  //this conversion is for NI5761 module (DC)
							p_DrvPvt->CVDAC=(8191.0/0.635);
							p_DrvPvt->maxAnalogOut=0.635;
							p_DrvPvt->minAnalogOut=-0.635;

						break;

						default:
							irio_mergeStatus(status,ValueOOB_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) Incorrect value for configuring AC/DC coupling mode  \n",__func__,__LINE__,p_DrvPvt->appCallID);
							return IRIO_warning;
						break;
						}
			break;

			default:
				// AC/DC coupling not supported for this flexRIO adapter module
				irio_mergeStatus(status,ValueOOB_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) AC/DC coupling mode not supported in this FlexRIO Adapter Module \n",__func__,__LINE__,p_DrvPvt->appCallID);
				return IRIO_warning;
			break;
		}
	}
	//else REVIEW: What happens in cRIO?. This applies only to NI9205 analog input module. This module is only DC

	return IRIO_success;
}

int irio_getAICoupling(irioDrv_t* p_DrvPvt,TIRIOCouplingMode* value, TStatus* status)
{
	*value=p_DrvPvt->couplingMode;
	return IRIO_success;
}

int irio_getVersion(char *version,TStatus* status)
{
	sprintf(version,"%s",IRIOVERSION);
	return IRIO_success;
}

int irio_setFPGAStart(irioDrv_t* p_DrvPvt,int32_t value,TStatus* status){
	int timeouterror=0;
	TIRIOStatusCode local_status = IRIO_success;

	if(p_DrvPvt->fpgaStarted==1){
		irio_mergeStatus(status,FPGAAlreadyRunning_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING FPGA status can not be changed after started \n",__func__,__LINE__,p_DrvPvt->appCallID);
		return IRIO_warning;
	}else if(value==1){

		NiFpga_Status fpgaStatus=NiFpga_Status_Success;
		p_DrvPvt->initDone=0;

		//Run FPGA. RunAttribute is set to 0 (No wait until done)
		NiFpga_MergeStatus(&fpgaStatus,NiFpga_Run(p_DrvPvt->session,0));
		if (fpgaStatus==-NiFpga_Status_FpgaAlreadyRunning){
			irio_mergeStatus(status,FPGAAlreadyRunning_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING Bitfile downloaded was already running \n",__func__,__LINE__,p_DrvPvt->appCallID);
		}else if(NiFpga_IsError(fpgaStatus)){
			irio_mergeStatus(status,NIRIO_API_Error,p_DrvPvt->verbosity,"[%s,%d]-(%s) ERROR Run target failed .Error Code:%d\n",__func__,__LINE__,p_DrvPvt->appCallID,fpgaStatus);
			return IRIO_error;
		}

		//Wait 5 seconds to InitDone
		if(p_DrvPvt->verbosity){
			printf("[%s,%d]-(%s) TRACE Waiting for %s up to 5 seconds.\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_INITDONE);
		}
		do{

			usleep(500000);
			NiFpga_MergeStatus(&fpgaStatus,NiFpga_ReadBool(p_DrvPvt->session,p_DrvPvt->enumInitDone.value,&p_DrvPvt->initDone));
			timeouterror++;
			if (timeouterror>10){ // TimeOut 5 seconds. Then FPGA is not ready to work
				irio_mergeStatus(status,InitDone_Error,p_DrvPvt->verbosity,"[%s,%d]-(%s) ERROR Timeout waiting for %s. Adapter Module appears not to be connected or is incorrect\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_INITDONE);
				local_status |= IRIO_error;
			}
			if(NiFpga_IsError(fpgaStatus)){
				irio_mergeStatus(status,NIRIO_API_Error,p_DrvPvt->verbosity,"[%s,%d]-(%s) ERROR Reading %s. Error Code:%d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_INITDONE,fpgaStatus);
				local_status |= IRIO_error;
			}
		}while(p_DrvPvt->initDone==0 && local_status==IRIO_success);

		if(local_status>=IRIO_error){
			return IRIO_error;
		}else if(p_DrvPvt->verbosity){
			printf("[%s,%d]-(%s) TRACE %s wait ended.\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_INITDONE);
		}
		///
		switch (p_DrvPvt->platform)
		{
		case IRIO_FlexRIO:

			//Read IOModuleID and RIOAdapterCorrect
			NiFpga_MergeStatus(&fpgaStatus,NiFpga_ReadU32(p_DrvPvt->session,p_DrvPvt->enumInsertedIoModuleID.value,&p_DrvPvt->moduleValue));
			if(NiFpga_IsNotError(fpgaStatus)){
				NiFpga_MergeStatus(&fpgaStatus,NiFpga_ReadBool(p_DrvPvt->session,p_DrvPvt->enumRIOAdapterCorrect.value,&p_DrvPvt->RIOAdapterCorrect));
			}
			//Check for errors
			if(NiFpga_IsError(fpgaStatus)){
				irio_mergeStatus(status,NIRIO_API_Error,p_DrvPvt->verbosity,"[%s,%d]-(%s) ERROR Reading %s or %s. Error Code:%d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_INSERTEDIOMODULEID,STRINGNAME_RIOADAPTERCORRECT,fpgaStatus);
				local_status |= IRIO_error;
				fpgaStatus=NiFpga_Status_Success;
			}else if(p_DrvPvt->moduleValue!=0 && p_DrvPvt->RIOAdapterCorrect==0){
				irio_mergeStatus(status,IOModule_Error,p_DrvPvt->verbosity,"[%s,%d]-(%s) ERROR IOModule Check Failed.\n",__func__,__LINE__,p_DrvPvt->appCallID);
				local_status |= IRIO_error;
			}
			break;
		case IRIO_cRIO:
			//Read InsertedModulesID and cRIOModulesOk
			NiFpga_MergeStatus(&fpgaStatus,NiFpga_ReadArrayU16(p_DrvPvt->session,p_DrvPvt->enumInsertedModulesID.value,p_DrvPvt->InsertedModulesID,16));
			if(NiFpga_IsNotError(fpgaStatus) ){
				NiFpga_MergeStatus(&fpgaStatus,NiFpga_ReadBool(p_DrvPvt->session,p_DrvPvt->enumcRIOModulesOk.value,&p_DrvPvt->cRIOModulesOK));
			}

			//There is no check of the inserted modules. Bitfile developer must determine whether or not activate cRIOModulesOk
			if(NiFpga_IsError(fpgaStatus)){
				irio_mergeStatus(status,NIRIO_API_Error,p_DrvPvt->verbosity,"[%s,%d]-(%s) ERROR Reading %s or %s. Error Code:%d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_INSERTEDIOMODULESID,STRINGNAME_CRIOMODULESOK,fpgaStatus);
				local_status |= IRIO_error;
				fpgaStatus=NiFpga_Status_Success;
			}else if(p_DrvPvt->cRIOModulesOK==0){
				irio_mergeStatus(status,IOModule_Error,p_DrvPvt->verbosity,"[%s,%d]-(%s) ERROR cRIO IO Modules Check Failed.\n",__func__,__LINE__,p_DrvPvt->appCallID);
				local_status |= IRIO_error;
			}
			break;

		}

		//Stop DAQ
		if(local_status<IRIO_error){
			NiFpga_MergeStatus(&fpgaStatus,NiFpga_WriteBool(p_DrvPvt->session,p_DrvPvt->enumDAQStartStop.value,(NiFpga_Bool)0));
			if(NiFpga_IsError(fpgaStatus)){
				irio_mergeStatus(status,NIRIO_API_Error,p_DrvPvt->verbosity,"[%s,%d]-(%s) ERROR Stopping DAQ .Error Code:%d\n",__func__,__LINE__,p_DrvPvt->appCallID,fpgaStatus);
				local_status |= IRIO_error;
			}
		}

		if(local_status<IRIO_error){
			p_DrvPvt->fpgaStarted=1;
		}
	}

	if(local_status<IRIO_error){
		return local_status;
	}else{
		return IRIO_error;
	}
}

int irio_getFPGAStart(irioDrv_t* p_DrvPvt, int32_t* value,TStatus* status){
	*value=p_DrvPvt->fpgaStarted;
	return IRIO_success;
}

int irio_getFPGAVIVersion(irioDrv_t* p_DrvPvt,char* value, size_t maxLength, size_t* valueLength, TStatus* status){
	TIRIOStatusCode local_status = IRIO_success;
	if(strlen(p_DrvPvt->FPGAVIStringversion)>maxLength){
		*valueLength=maxLength;
		irio_mergeStatus(status,Read_Resource_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING FPGAVIVersion did not fit in the given pointer. Will be truncated.\n",__func__,__LINE__,p_DrvPvt->appCallID);
		local_status |= IRIO_warning;
	}else{
		*valueLength=strlen(p_DrvPvt->FPGAVIStringversion);
	}
	strncpy(value,p_DrvPvt->FPGAVIStringversion,*valueLength);

	if(local_status<IRIO_error){
		return local_status;
	}else{
		return IRIO_error;
	}
}

int irio_getDevQualityStatus(irioDrv_t* p_DrvPvt,int32_t* value,TStatus* status){
	TIRIOStatusCode local_status = IRIO_success;
	if(p_DrvPvt->enumDevQualityStatus.found){
		NiFpga_Status fpgaStatus = NiFpga_Status_Success;
		uint8_t aux=0;
		fpgaStatus=NiFpga_ReadU8(p_DrvPvt->session,p_DrvPvt->enumDevQualityStatus.value,&aux);
		if(NiFpga_IsError(fpgaStatus)){
			irio_mergeStatus(status,Read_NIRIO_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING FPGA Error reading %s. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_DEVQUALITYSTATUS,fpgaStatus);
			local_status |= IRIO_warning;
		}else{
			*value=(int32_t) aux;
		}
	}else{
		irio_mergeStatus(status,Read_Resource_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING %s was not found.\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_DEVQUALITYSTATUS);
		local_status |= IRIO_warning;
	}

	if(local_status<IRIO_error){
		return local_status;
	}else{
		return IRIO_error;
	}
}

int irio_getDevTemp(irioDrv_t* p_DrvPvt,int32_t* value, TStatus* status){
	TIRIOStatusCode local_status = IRIO_success;
	if(p_DrvPvt->enumDevTemp.found){
		NiFpga_Status fpgaStatus = NiFpga_Status_Success;
		int16_t aux=0;
		fpgaStatus=NiFpga_ReadI16(p_DrvPvt->session,p_DrvPvt->enumDevTemp.value,&aux);
		if(NiFpga_IsError(fpgaStatus)){
			irio_mergeStatus(status,Read_NIRIO_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING FPGA Error reading %s. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_DEVTEMP, fpgaStatus);
			local_status |= IRIO_warning;
		}else{
			*value=(int32_t) aux;
		}
	}else{
		irio_mergeStatus(status,Read_Resource_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING %s was not found.\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_DEVTEMP);
		local_status |= IRIO_warning;
	}

	if(local_status<IRIO_error){
		return local_status;
	}else{
		return IRIO_error;
	}
}

int irio_getDevProfile(irioDrv_t* p_DrvPvt,int32_t* value, TStatus* status){
	TIRIOStatusCode local_status = IRIO_success;
	if(p_DrvPvt->enumDevProfile.found){
		NiFpga_Status fpgaStatus = NiFpga_Status_Success;
		uint8_t aux=0;
		fpgaStatus=NiFpga_ReadU8(p_DrvPvt->session,p_DrvPvt->enumDevProfile.value,&aux);
		if(NiFpga_IsError(fpgaStatus)){
			irio_mergeStatus(status,Read_NIRIO_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING FPGA Error reading %s. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_DEVPROFILE,fpgaStatus);
			local_status |= IRIO_warning;
		}else{
			*value=(int32_t)aux;
		}
	}else{
		irio_mergeStatus(status,Read_Resource_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING %s was not found.\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_DEVPROFILE);
		local_status |= IRIO_warning;
	}

	if(local_status<IRIO_error){
		return local_status;
	}else{
		return IRIO_error;
	}
}

int irio_setDebugMode(irioDrv_t* p_DrvPvt,int32_t value, TStatus* status){
	TIRIOStatusCode local_status = IRIO_success;
	if(p_DrvPvt->enumDebugMode.found){//Should not be necessary, is a mandatory register
		NiFpga_Status fpgaStatus = NiFpga_Status_Success;
		if(value==0){
			fpgaStatus=NiFpga_WriteBool(p_DrvPvt->session,p_DrvPvt->enumDebugMode.value,NiFpga_False);
		}else{
			fpgaStatus=NiFpga_WriteBool(p_DrvPvt->session,p_DrvPvt->enumDebugMode.value,NiFpga_True);
		}
		if(NiFpga_IsError(fpgaStatus)){
			irio_mergeStatus(status,Write_NIRIO_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING FPGA Error writing %s. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_DEBUGMODE,fpgaStatus);
			local_status |= IRIO_warning;
		}
	}else{
		irio_mergeStatus(status,Write_Resource_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING %s was not found.\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_DEBUGMODE);
		local_status |= IRIO_warning;
	}

	if(local_status<IRIO_error){
		return local_status;
	}else{
		return IRIO_error;
	}
}

int irio_getDebugMode(irioDrv_t* p_DrvPvt,int32_t* value, TStatus* status){
	TIRIOStatusCode local_status = IRIO_success;
	if(p_DrvPvt->enumDebugMode.found){//Should not be necessary, is a mandatory register
		NiFpga_Status fpgaStatus = NiFpga_Status_Success;
		NiFpga_Bool aux=NiFpga_True;
		fpgaStatus=NiFpga_ReadBool(p_DrvPvt->session,p_DrvPvt->enumDebugMode.value,&aux);
		if(NiFpga_IsError(fpgaStatus)){
			irio_mergeStatus(status,Read_NIRIO_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING FPGA Error reading %s. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_DEBUGMODE,fpgaStatus);
			local_status |= IRIO_warning;
		}else{
			*value=(int32_t)aux;
		}
	}else{
		irio_mergeStatus(status,Read_Resource_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING %s was not found.\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_DEBUGMODE);
		local_status |= IRIO_warning;
	}

	if(local_status<IRIO_error){
		return local_status;
	}else{
		return IRIO_error;
	}
}

int irio_setDAQStartStop(irioDrv_t* p_DrvPvt,int32_t value, TStatus* status){
	TIRIOStatusCode local_status = IRIO_success;
	if(p_DrvPvt->enumDAQStartStop.found){//Should not be necessary, is a mandatory register
		NiFpga_Status fpgaStatus = NiFpga_Status_Success;
		if(value==0){
			fpgaStatus=NiFpga_WriteBool(p_DrvPvt->session,p_DrvPvt->enumDAQStartStop.value,NiFpga_False);
		}else{
			fpgaStatus=NiFpga_WriteBool(p_DrvPvt->session,p_DrvPvt->enumDAQStartStop.value,NiFpga_True);
		}
		if(NiFpga_IsError(fpgaStatus)){
			irio_mergeStatus(status,Write_NIRIO_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING FPGA Error writing %s. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_DAQSTARTSTOP,fpgaStatus);
			local_status |= IRIO_warning;
		}
	}else{
		irio_mergeStatus(status,Write_Resource_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING %s was not found.\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_DAQSTARTSTOP);
		local_status |= IRIO_warning;
	}

	if(local_status<IRIO_error){
		return local_status;
	}else{
		return IRIO_error;
	}
}

int irio_getDAQStartStop(irioDrv_t* p_DrvPvt,int32_t* value, TStatus* status){
	TIRIOStatusCode local_status = IRIO_success;
	if(p_DrvPvt->enumDAQStartStop.found){//Should not be necessary, is a mandatory register
		NiFpga_Status fpgaStatus = NiFpga_Status_Success;
		NiFpga_Bool aux=NiFpga_True;
		fpgaStatus=NiFpga_ReadBool(p_DrvPvt->session,p_DrvPvt->enumDAQStartStop.value,&aux);
		if(NiFpga_IsError(fpgaStatus)){
			irio_mergeStatus(status,Read_NIRIO_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING FPGA Error reading %s. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_DAQSTARTSTOP,fpgaStatus);
			local_status |= IRIO_warning;
		}else{
			*value=(int32_t)aux;
		}
	}else{
		irio_mergeStatus(status,Read_Resource_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING %s was not found.\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_DAQSTARTSTOP);
		local_status |= IRIO_warning;
	}

	if(local_status<IRIO_error){
		return local_status;
	}else{
		return IRIO_error;
	}
}

int irio_setSamplingRate(irioDrv_t* p_DrvPvt,int n,int32_t value, TStatus* status){
	TIRIOStatusCode local_status = IRIO_success;
	if(p_DrvPvt->platform==IRIO_cRIO && n>=0 && n<CRIO_MAX_MODULES && p_DrvPvt->enumSamplingRate[n].found){
		NiFpga_Status fpgaStatus = NiFpga_Status_Success;
		uint16_t aux=(uint16_t)value;
		NiFpga_MergeStatus(&fpgaStatus,NiFpga_WriteU16(p_DrvPvt->session,p_DrvPvt->enumSamplingRate[n].value,aux));
		if(NiFpga_IsError(fpgaStatus)){
			irio_mergeStatus(status,Write_NIRIO_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING FPGA Error writing %s%d. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_SAMPLINGRATE,n,fpgaStatus);
			local_status |= IRIO_warning;
		}
	}else{
		irio_mergeStatus(status,Write_Resource_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING %s%d was not found.\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_SAMPLINGRATE,n);
		local_status |= IRIO_warning;
	}

	if(local_status<IRIO_error){
		return local_status;
	}else{
		return IRIO_error;
	}
}

int irio_getSamplingRate(irioDrv_t* p_DrvPvt,int n,int32_t* value, TStatus* status){
	TIRIOStatusCode local_status = IRIO_success;
	if(p_DrvPvt->platform==IRIO_cRIO && n>=0 && n<CRIO_MAX_MODULES && p_DrvPvt->enumSamplingRate[n].found){
		NiFpga_Status fpgaStatus = NiFpga_Status_Success;
		uint16_t aux;
		NiFpga_MergeStatus(&fpgaStatus,NiFpga_ReadU16(p_DrvPvt->session,p_DrvPvt->enumSamplingRate[n].value,&aux));
		if(NiFpga_IsError(fpgaStatus)){
			irio_mergeStatus(status,Read_NIRIO_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING FPGA Error reading %s%d. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_SAMPLINGRATE,n,fpgaStatus);
			local_status |= IRIO_warning;
		}else{
			*value=(int32_t)aux;
		}
	}else{
		irio_mergeStatus(status,Read_Resource_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING %s%d was not found.\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_SAMPLINGRATE,n);
		local_status |= IRIO_warning;
	}

	if(local_status<IRIO_error){
		return local_status;
	}else{
		return IRIO_error;
	}
}
