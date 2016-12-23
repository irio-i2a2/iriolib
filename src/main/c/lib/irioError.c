/**************************************************************************//**
 * \file irioError.c
 * \authors Mariano Ruiz (Universidad Politécnica de Madrid, UPM)
 * \authors Diego Sanz (Universidad Politécnica de Madrid, UPM)
 * \authors Sergio Esquembri (Universidad Politécnica de Madrid, UPM)
 * \authors Enrique Bernal (Universidad Politécnica de Madrid, UPM)
 * \brief Functions for error management
 * \date Nov., 2015
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

#include "irioError.h"

#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int irio_initStatus(TStatus* status){
	status->code = IRIO_success;
	status->detailCode = Success;
	status->msg = NULL;
	return IRIO_success;
}

int irio_resetStatus(TStatus* status){
	status->code = IRIO_success;
	status->detailCode = Success;
	free(status->msg);
	status->msg = NULL;
	return IRIO_success;
}

int irio_mergeStatus(TStatus* status,TErrorDetailCode code, int printMsg, const char *format, ...){
	va_list argptr;
	va_start(argptr,format);
	char* newMsg = NULL;
	if(vasprintf(&newMsg,format,argptr)<=0){
		printf("\n\nERROR in irio_mergeStatus\n\n");
		return -1;
	}

	if(printMsg){
		printf("\n\n%s\n",newMsg);
	}

	if(status->msg == NULL){
		status->msg = newMsg;
	}else{
		char* msg_aux=NULL;
		msg_aux = malloc(strlen(newMsg)+strlen(status->msg)+1);
		if(msg_aux!=NULL){
			strcpy(msg_aux,status->msg);
			strcat(msg_aux,newMsg);
			free(newMsg);
			free(status->msg);
			status->msg=msg_aux;
		}else{
			printf("\n\nERROR in irio_mergeStatus\n\n");
			return -1;
		}
	}

	if(status->detailCode==Success){
		status->detailCode=code;
	}else if(code<0 && code<status->detailCode){
		status->detailCode=code;
	}

	if(status->detailCode<0){
		status->code=IRIO_error;
	}else if(status->detailCode>0){
		status->code=IRIO_warning;
	}else{
		status->code=IRIO_success;
	}

	return 0;
}

int irio_getErrorString(TErrorDetailCode error, char** str){
	*str=NULL;
	switch(error){
	case Generic_Error:
		asprintf(str,"Error generated outside irio. Check error messages for more detailed information");
		break;
	case HardwareNotFound_Error:
		asprintf(str,"Specified RIO device not found. Review specified serial number.");
		break;
	case BitfileDownload_Error:
		asprintf(str,"Error occurr downloading the bitfile. Check if bitfile was compiled for the specified target.");
		break;
	case InitDone_Error://!< Could not configure adapter module
		asprintf(str,"Init done wait ended in timeout. Check if the connected adapter module is the intended and is properly connected.\n Check the initialization logic in of the project.");
		break;
	case IOModule_Error:			//!< IO module/s check error
		asprintf(str,"Connected IO module is not the expected IO Module\n Review bitfile downloaded.");
		break;
	case NIRIO_API_Error:
		asprintf(str,"NI-RIO C API returned error. Check returned error in error log.");
		break;
	case ListRIODevicesCommand_Error:
		asprintf(str,"System call for listing connected RIO devices failed. Check if NI-RIO driver is installed and the executable is accessible.");
		break;
	case ListRIODevicesParsing_Error:
		asprintf(str,"Failed to parse system call for listing connected RIO. Check version of NI-RIO Driver installed.");
		break;
	case SignatureNotFound_Error:
		asprintf(str,"Can't find bitfile signature string in the given header file. Check NI FPGA C API Generator version used.");
		break;
	case ResourceNotFound_Error:
		asprintf(str,"A mandatory resource wasn't found. Check error messages to find missing resources.");
		break;
	case SignatureValueNotValid_Error:
		asprintf(str,"Can't read value of bitfile signature in the given header file. Check NI FPGA C API Generator version used.");
		break;
	case ResourceValueNotValid_Error:
		asprintf(str,"Can't read address of a resource in the given header file. Check NI FPGA C API Generator version used.");
		break;
	case MemoryAllocation_Error:
		asprintf(str,"IRIO tried to allocate memory unsuccessfully. Check possible memory leaks in user application.");
		break;
	case BitfileNotFound_Error:
		asprintf(str,"Bitfile file for specified project was not found. Check project name and search path.");
		break;
	case HeaderNotFound_Error:
		asprintf(str,"Header file for specified project was not found. Check project name and search path.");
		break;
	case FileAccess_Error:
		asprintf(str,"A problem occur while reading a file. Check file permissions.");
		break;
	case FileNotFound_Error:
		asprintf(str,"Tried to open a file and failed. Check file name and search path.");
		break;
	case FeatureNotImplemented_Error:
		asprintf(str,"The target device or device profile are not supported. Check values in the project with the design rules");
		break;
	case Success:
		asprintf(str,"Device status is OK");
		break;
	case TemporaryFileDelete_Warning:
		asprintf(str,"Could not delete temporary file. Check /tmp/ folder permissions or previous warnings/errors.");
		break;
	case ResourceNotFound_Warning: //!< Part of optional resource not found (e.g. AO0 Present but AOEnable0 not present)
		asprintf(str,"An optional resource (or part of it for complex resources) wasn't found. Check error messages to find missing resources.");
		break;
	case FPGAAlreadyRunning_Warning:	//!< Tried to change FPGAStart after FPGA was started/FPGA was started (bitfile downloaded was already in the FPGA)
		asprintf(str,"Tried change FPGAStart value after FPGA was started or FPGA was started before downloading the bitfile (previous session). If this is not the intended behaviour, reset the FPGA.");
		break;
	case Read_NIRIO_Warning:
		asprintf(str,"Tried to access an existing FPGA resource unsuccessfully. Check error messages to find NI-RIO driver error code.");
		break;
	case Read_Resource_Warning:		//!< Tried to access a not-present resource
		asprintf(str,"Tried to access a non-existing FPGA resource. Check implemented resources in the project or enabling verbosity mode in irio_initDriver call.");
		break;
	case Write_NIRIO_Warning:
		asprintf(str,"Tried to write in an existing FPGA resource unsuccessfully. Check error messages to find NI-RIO driver error code.");
		break;
	case Write_Resource_Warning:
		asprintf(str,"Tried to write in a non-existing FPGA resource. Check implemented resources in the project or enabling verbosity mode in irio_initDriver call.");
		break;
	case ConfigDMA_Warning:			//!< Attempt to Configure, Start or Stop DMA failed
		asprintf(str,"Tried to configure/start/stop DMA FIFO unsuccessfully. Check error messages to find NI-RIO driver error code.");
		break;
	case ConfigUART_Warning:			//!< Timeout for setting baudrate
		asprintf(str,"Tried to configure UART baud rate unsuccessfully. UART seems busy.");
		break;
	case ValueOOB_Warning:
		asprintf(str,"Tried to write a value Out of Bounds. Check design rules for operational range.");
		break;
	case Generic_Warning:
		asprintf(str,"Warning generated outside irio. Check warning messages for more detailed information");
		break;
	case DAQtimeout_Warning:
		asprintf(str,"Timeout reached when waiting for data from data acquisition");
		break;
	case CLSLinetimeout_Warning:
		asprintf(str,"Timeout reached when waiting for cameralink serial data");
		break;
	case ResourceRelease_Warning:
		asprintf(str,"NIRIO error while releasing Fifo elements");
		break;
	default:
		asprintf(str,"Error code not defined");
		break;
	}
	return IRIO_success;
}
