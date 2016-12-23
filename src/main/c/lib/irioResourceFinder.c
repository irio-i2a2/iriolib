/*******************************************************//**
 * \file irioResourceFinder.c
 * \authors Mariano Ruiz (Universidad Politécnica de Madrid, UPM)
 * \authors Diego Sanz (Universidad Politécnica de Madrid, UPM)
 * \authors Sergio Esquembri (Universidad Politécnica de Madrid, UPM)
 * \authors Enrique Bernal (Universidad Politécnica de Madrid, UPM)
 * \authors Alvaro Bustos (Universidad Politécnica de Madrid, UPM)
 * \brief Resource finder and error management methods for IRIO driver
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

#include "irioResourceFinder.h"
#include "irioDataTypes.h"
#include "irioError.h"

#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

/** @name Driver System Call Strings
 * Strings for call driver and print it output in a file
 */
///@{
#ifdef CLOSE_VERSION_NIRIO
	#define DRV_CALL "lsni -v "
#else
	#define DRV_CALL "lsrio.py "
#endif
#define TMP_FILE "/tmp/RIOinfo.txt"
#define RM_CALL "rm -f "
///@}

/** @name Driver Resource Strings
 * Strings for field search in driver output
 */
///@{
#define STRINGNAME_PORT "RIO"
#define STRINGNAME_MODEL "Model Name"
#define STRINGNAME_SERIALNO "Serial Number"
#define STRINGNAME_DEVICE "Device"
#define STRINGNAME_SUBSYSDEVICE "SubSystemDevice"
#define STRINGNAME_PORTEND "--" //!<String to search for moving after STRINGNAME_PORT and search for another device
#define STRINGNAME_RESOURCEINIT "System Configuration API resources found:" //!<String to move right before resource list in the privative driver
///@}


int parseDriverInfo(irioDrv_t *p_DrvPvt,TStatus* status);

int findDeviceInfo(irioDrv_t *p_DrvPvt, const char* fileContent, const char* toSearch, char* info,TStatus* status);

int irio_findRIO(irioDrv_t *p_DrvPvt,TStatus* status){
	TIRIOStatusCode local_status = IRIO_success;
	char command[50];
	strcpy(command,DRV_CALL);
	strcat(command," >> ");
	strcat(command,TMP_FILE);
	if(system(command)==0){
		local_status |= parseDriverInfo(p_DrvPvt,status);
	}else{
		irio_mergeStatus(status,ListRIODevicesCommand_Error,p_DrvPvt->verbosity,"[%s,%d]-(%s) ERROR calling driver for hardware info: %s.\n",__func__,__LINE__,p_DrvPvt->appCallID,command);
		local_status |= IRIO_error;
	}
	strcpy(command,RM_CALL);
	strcat(command,TMP_FILE);
	if(system(command)!=0){
		irio_mergeStatus(status,TemporaryFileDelete_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING removing device info file : %s.\n",__func__,__LINE__,p_DrvPvt->appCallID,TMP_FILE);
		local_status |= IRIO_warning;
	}

	if(local_status<IRIO_error){
		return local_status;
	}else{
		return IRIO_error;
	}
}

int parseDriverInfo(irioDrv_t *p_DrvPvt, TStatus* status){
	char *fileContent=NULL;
	TIRIOStatusCode local_status = IRIO_success;
	local_status=irio_initFileSearch(p_DrvPvt,TMP_FILE,(void**)&fileContent,status);
	if (local_status==IRIO_success){
		int found=0;
		int end=0;
		char* deviceInfo=NULL;
		char* port=NULL;
		char serialNo[20];
		asprintf(&port,"%s","RIOxxxx");
		//In privative version need to move the initial pointer
		#ifdef CLOSE_VERSION_NIRIO
		if((deviceInfo=strstr(fileContent,STRINGNAME_RESOURCEINIT))==NULL){
			irio_mergeStatus(status,HardwareNotFound_Error,p_DrvPvt->verbosity,"[%s,%d]-(%s) ERROR No hardware found.\n",__func__,__LINE__,p_DrvPvt->appCallID);
			local_status |= IRIO_error;
			end=1;
		}
		#else
			deviceInfo=fileContent;
		#endif
		while(!found && !end){//Search while the device has not been found and still no errors
			if((deviceInfo=strstr(deviceInfo,STRINGNAME_PORT))!=NULL){//Still devices to search
				sscanf(deviceInfo,"%s",port);
			}else{
				end=1;
				break;
			}
			//Search fields values
			local_status=findDeviceInfo(p_DrvPvt,deviceInfo,STRINGNAME_SERIALNO,serialNo,status);
			//Device validation
			if(strcmp(serialNo,p_DrvPvt->DeviceSerialNumber)==0){
				found=1;
			}else{
				deviceInfo=strstr(deviceInfo,STRINGNAME_PORTEND);//Move forward and search in another port
			}
		}

		//Target found or error
		if(found==1){
			//Load found values into driver struct
			strcpy(p_DrvPvt->fpgaRIO,port);
		}else{
			irio_mergeStatus(status,HardwareNotFound_Error,p_DrvPvt->verbosity,"[%s,%d]-(%s) ERROR No Matching RIO found: %s.\n",__func__,__LINE__,p_DrvPvt->appCallID,TMP_FILE);
			local_status |= IRIO_error;
		}

		local_status |= irio_closeFileSearch(p_DrvPvt,(void**)&fileContent, status);
		free(port);
	}

	if(local_status<IRIO_error){
		return local_status;
	}else{
		return IRIO_error;
	}
}

int irio_initFileSearch(irioDrv_t *p_DrvPvt, char* filePath, void** fileContent,TStatus* status){
	int fd;					// File descriptor
	struct stat infofile;	// Data structure to take infofile
	int auxStatus;			// Operation status

	fd=open(filePath,O_RDONLY);

	if (fd==-1){
		irio_mergeStatus(status,FileNotFound_Error,p_DrvPvt->verbosity,"[%s,%d]-(%s) ERROR Opening file :%s. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,filePath,fd);
		return IRIO_error;
	}

	auxStatus = lstat(filePath, &infofile);
	if(auxStatus<0){
		irio_mergeStatus(status,FileAccess_Error,p_DrvPvt->verbosity,"[%s,%d]-(%s) ERROR Getting file stats: %s. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,filePath,auxStatus);
		return IRIO_error;
	}

	*fileContent=(void*)calloc(infofile.st_size+2,1);
	if(*fileContent==NULL){
		irio_mergeStatus(status,MemoryAllocation_Error,p_DrvPvt->verbosity,"[%s,%d]-(%s) ERROR Allocating memory for the file content: %s.\n",__func__,__LINE__,p_DrvPvt->appCallID,filePath);
		return IRIO_error;
	}

	if(read(fd,(char*)*fileContent, infofile.st_size)==-1){
		irio_mergeStatus(status,FileAccess_Error,p_DrvPvt->verbosity,"[%s,%d]-(%s) ERROR Reading file: %s.\n",__func__,__LINE__,p_DrvPvt->appCallID,filePath);
		free(*fileContent);
		return IRIO_error;
	}
	((char *)(*fileContent))[infofile.st_size+1]='\0';
	if(close(fd)==-1){
		irio_mergeStatus(status,FileAccess_Error,p_DrvPvt->verbosity,"[%s,%d]-(%s) ERROR Closing file: %s . System Error: %s.\n",__func__,__LINE__,p_DrvPvt->appCallID,filePath,strerror(errno));
		free(*fileContent);
		return IRIO_error;
	}
	return IRIO_success;
}


int irio_closeFileSearch(irioDrv_t *p_DrvPvt, void** fileContent,TStatus* status){
	if(*fileContent!=NULL){
		free(*fileContent);
		*fileContent=NULL;
	}
	return IRIO_success;
}

int findDeviceInfo(irioDrv_t *p_DrvPvt, const char* fileContent, const char* toSearch, char* info, TStatus* status){
	char* aux;
	TIRIOStatusCode local_status = IRIO_success;
	//Device info field pattern --Field Name:		Value
	if((aux=strstr(fileContent,toSearch))==NULL){
		irio_mergeStatus(status,ListRIODevicesParsing_Error,p_DrvPvt->verbosity,"[%s,%d]-(%s) ERROR Finding %s.\n",__func__,__LINE__,p_DrvPvt->appCallID,toSearch);
		local_status |= IRIO_error;
	}else{
		//If found pattern move pointer before colon and scan the value
		aux=strstr(aux,":");
		if(sscanf(aux,"%*s %s",info)==0){
			irio_mergeStatus(status,ListRIODevicesParsing_Error,p_DrvPvt->verbosity,"[%s,%d]-(%s) ERROR Value not found for:%s.\n",__func__,__LINE__,p_DrvPvt->appCallID,toSearch);
			local_status|= IRIO_error;
		}
	}

	if(local_status<IRIO_error){
		return local_status;
	}else{
		return IRIO_error;
	}
}

int irio_findResourceEnum(irioDrv_t *p_DrvPvt, const char* resourceName, int32_t index, TResourcePort* port,TStatus* status,int printErrMsg){
	TIRIOStatusCode local_status = IRIO_success;
	char* baseName = NULL;
	int sys_status=0;
	port->found=0;
	if(index>=0){
		sys_status = asprintf(&baseName,"%s%d",resourceName,index);
	}else{
		sys_status = asprintf(&baseName,"%s",resourceName);
	}

	if(sys_status < 0){
		printf("[%s,%d]-(%s)ERROR Syscall failed.\n",__func__,__LINE__,p_DrvPvt->appCallID);
		status->detailCode=MemoryAllocation_Error;
		status->code=IRIO_error;
		return IRIO_error;
	}

	char* aux = NULL; //first initialized to NULL to avoid valgrind warning
	char *aux2=NULL;
	aux2=p_DrvPvt->headerFile;
	char* completeName = NULL;
	sys_status = asprintf(&completeName,"%s%s%s",STRINGNAME_PREFIX,p_DrvPvt->projectName,baseName);
	if(sys_status < 0){
		printf("[%s,%d]-(%s)ERROR Syscall failed.\n",__func__,__LINE__,p_DrvPvt->appCallID);
		status->detailCode=MemoryAllocation_Error;
		status->code=IRIO_error;
		return IRIO_error;
	}
	aux=strstr(aux2,completeName);
	//Move the file pointer right before the port name.
	if(aux==NULL){
		irio_mergeStatus(status,ResourceNotFound_Error,printErrMsg,"[%s,%d]-(%s)ERROR Finding %s.\n",__func__,__LINE__,p_DrvPvt->appCallID,completeName);
		local_status = IRIO_error;
	}else
	//Read the port value
	if(sscanf(aux,"%*s %*s %X",&port->value)==0){//First, try to read an hexadecimal value
		if (sscanf(aux,"%*s %*s %d",&port->value)==0){//If failed, try to read a decimal value
			irio_mergeStatus(status,ResourceValueNotValid_Error,printErrMsg,"[%s,%d]-(%s) ERROR Value not found for:%s.\n",__func__,__LINE__,p_DrvPvt->appCallID,completeName);
			local_status = IRIO_error;
		}else{
			port->found=1;
		}
	}else{
		port->found=1;
	}

	free(completeName);
	free(baseName);

	if(local_status<IRIO_error){
		return local_status;
	}else{
		return IRIO_error;
	}
}

int irio_findHeaderString(irioDrv_t *p_DrvPvt, char* fileContent, const char* toSearch, char** str,TStatus* status){
	char* aux;
	char *indexcoma1;
	char *indexcoma2;
	int length;

	aux=fileContent;
	if((aux=strstr(aux,toSearch))==NULL){
		irio_mergeStatus(status,ResourceNotFound_Error,p_DrvPvt->verbosity,"[%s,%d]-(%s)ERROR Finding %s.\n",__func__,__LINE__,p_DrvPvt->appCallID,toSearch);
		return IRIO_error;
	}
	indexcoma1=strstr(aux,"\"");
	indexcoma2=strstr ((indexcoma1+1),"\"");
	if(indexcoma1==NULL || indexcoma2==NULL){
		irio_mergeStatus(status,ResourceValueNotValid_Error,p_DrvPvt->verbosity,"[%s,%d]-(%s) ERROR String value not found for:%s.\n",__func__,__LINE__,p_DrvPvt->appCallID,toSearch);
		return IRIO_error;
	}
	length=indexcoma2-indexcoma1-1;

	*str=malloc(length+1);
	strncpy(*str, (indexcoma1+1), length);
	(*str)[length]='\0';

	return IRIO_success;
}
