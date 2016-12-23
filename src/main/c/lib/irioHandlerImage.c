/**************************************************************************//**
 * \file irioHandlerImage.c
 * \authors Mariano Ruiz (Universidad Politécnica de Madrid, UPM)
 * \authors Diego Sanz (Universidad Politécnica de Madrid, UPM)
 * \authors Sergio Esquembri (Universidad Politécnica de Madrid, UPM)
 * \authors Enrique Bernal (Universidad Politécnica de Madrid, UPM)
 * \authors Alvaro Bustos (Universidad Politécnica de Madrid, UPM)
 * \brief CameraLink handler methods for IRIO driver.
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

#include "irioHandlerImage.h"
#include "irioDataTypes.h"
#include "irioResourceFinder.h"
#include "irioError.h"

#include "NiFpga.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/** @name CameraLink Configuration Resource Strings
 * Strings for CameraLink configuration FPGA Resources
 */
///@{
#define STRINGNAME_UARTBYTEMODE 		"_ControlBool_uartByteMode"
#define STRINGNAME_UARTSETBAUDRATE 		"_ControlBool_uartSetBaudRate"
#define STRINGNAME_UARTTRANSMIT 		"_ControlBool_uartTransmit"
#define STRINGNAME_UARTRECEIVE 			"_ControlBool_uartReceive"
#define STRINGNAME_UARTBAUDRATE 		"_ControlU8_uartBaudRate"
#define STRINGNAME_UARTTXBYTE 			"_ControlU8_uartTxByte"
#define STRINGNAME_FVALHIGH 			"_ControlBool_FVALHigh"
#define STRINGNAME_LVALHIGH 			"_ControlBool_LVALHigh"
#define STRINGNAME_DVALHIGH 			"_ControlBool_DVALHigh"
#define STRINGNAME_SPAREHIGH 			"_ControlBool_SpareHigh"
#define STRINGNAME_CONTROLENABLE 		"_ControlBool_ControlEnable"
#define STRINGNAME_LINESCAN 			"_ControlBool_LineScan"
#define STRINGNAME_SIGNALMAPPING 		"_ControlU8_SignalMapping"
#define STRINGNAME_CONFIGURATION 		"_ControlU8_Configuration"
#define STRINGNAME_UARTTXREADY			"_IndicatorBool_uartTxReady"
#define STRINGNAME_UARTRXREADY			"_IndicatorBool_uartRxReady"
#define STRINGNAME_UARTOVERRUNERROR 	"_IndicatorBool_uartOverrunError"
#define STRINGNAME_UARTFRAMINGERROR 	"_IndicatorBool_uartFramingError"
#define STRINGNAME_UARTBREAKINDICATOR 	"_IndicatorBool_uartBreakIndicator"
#define STRINGNAME_UARTRXBYTE 			"_IndicatorU8_uartRxByte"
///@}

/** @name Acknowledge Values
 * Values of acknowledge and not-acknowledge for serial communications
 */
///@{
#define NACK 0x16
#define ACK 0x06
///@}

/** @name CameraLink Parameters Range
 * Definition of range for some CameraLink configuration parameters
 */
///@{
#define CL_CONFIG_LIMIT 2
#define CL_SIGNALMAPPING_LIMIT 2
#define UART_BAUDRATE_LIMIT 7
#define UART_BAUDRATE_USTIMEOUT 1000
///@}

int irio_findCLUart(irioDrv_t* p_DrvPvt,TStatus* status);

int irio_findCLConfig(irioDrv_t* p_DrvPvt,TStatus* status);

int irio_findCL(irioDrv_t* p_DrvPvt, TStatus* status){

	TIRIOStatusCode local_status = IRIO_success;
	local_status |= irio_findCLUart(p_DrvPvt,status);

	local_status |= irio_findCLConfig(p_DrvPvt,status);

	if(local_status<IRIO_error){
		return local_status;
	}else{
		return IRIO_error;
	}
}

int irio_findCLUart(irioDrv_t* p_DrvPvt, TStatus* status){

	TIRIOStatusCode local_status = IRIO_success;
	//Search for ByteMode
	local_status |= irio_findResourceEnum(p_DrvPvt,STRINGNAME_UARTBYTEMODE,-1,&p_DrvPvt->enumuartByteMode,status,p_DrvPvt->verbosity);

	//Search for SetBaudRate
	local_status |= irio_findResourceEnum(p_DrvPvt,STRINGNAME_UARTSETBAUDRATE,-1,&p_DrvPvt->enumuartSetBaudRate,status,p_DrvPvt->verbosity);

	//Search for Transmit
	local_status |= irio_findResourceEnum(p_DrvPvt,STRINGNAME_UARTTRANSMIT,-1,&p_DrvPvt->enumuartTransmit,status,p_DrvPvt->verbosity);

	//Search for Receive
	local_status |= irio_findResourceEnum(p_DrvPvt,STRINGNAME_UARTRECEIVE,-1,&p_DrvPvt->enumuartReceive,status,p_DrvPvt->verbosity);

	//Search for BaudRate
	local_status |= irio_findResourceEnum(p_DrvPvt,STRINGNAME_UARTBAUDRATE,-1,&p_DrvPvt->enumuartBaudRate,status,p_DrvPvt->verbosity);

	//Search for TxData
	local_status |= irio_findResourceEnum(p_DrvPvt,STRINGNAME_UARTTXBYTE,-1,&p_DrvPvt->enumuartTxByte,status,p_DrvPvt->verbosity);

	//Search for TxReady
	local_status |= irio_findResourceEnum(p_DrvPvt,STRINGNAME_UARTTXREADY,-1,&p_DrvPvt->enumuartTxReady,status,p_DrvPvt->verbosity);

	//Search for RxData
	local_status |= irio_findResourceEnum(p_DrvPvt,STRINGNAME_UARTRXBYTE,-1,&p_DrvPvt->enumuartRxByte,status,p_DrvPvt->verbosity);

	//Search for RxReady
	local_status |= irio_findResourceEnum(p_DrvPvt,STRINGNAME_UARTRXREADY,-1,&p_DrvPvt->enumuartRxReady,status,p_DrvPvt->verbosity);

	//Search for OverrunError
	local_status |= irio_findResourceEnum(p_DrvPvt,STRINGNAME_UARTOVERRUNERROR,-1,&p_DrvPvt->enumuartOverrunError,status,p_DrvPvt->verbosity);

	//Search for FramingError
	local_status |= irio_findResourceEnum(p_DrvPvt,STRINGNAME_UARTFRAMINGERROR,-1,&p_DrvPvt->enumuartFramingError,status,p_DrvPvt->verbosity);

	//Search for BreakIndicator
	local_status |= irio_findResourceEnum(p_DrvPvt,STRINGNAME_UARTBREAKINDICATOR,-1,&p_DrvPvt->enumuartBreakIndicator,status,p_DrvPvt->verbosity);

	if(p_DrvPvt->verbosity && local_status==IRIO_success){
		printf("[%s,%d]-(%s) TRACE CameraLink UART Registers Found\n",__func__,__LINE__,p_DrvPvt->appCallID);
	}

	if(local_status<IRIO_error){
		return local_status;
	}else{
		return IRIO_error;
	}
}

int irio_findCLConfig(irioDrv_t* p_DrvPvt, TStatus* status){

	TIRIOStatusCode local_status = IRIO_success;
	//Search for FVALHigh
	local_status |= irio_findResourceEnum(p_DrvPvt,STRINGNAME_FVALHIGH,-1,&p_DrvPvt->enumFVALHigh,status,p_DrvPvt->verbosity);

	//Search for LVALHigh
	local_status |= irio_findResourceEnum(p_DrvPvt,STRINGNAME_LVALHIGH,-1,&p_DrvPvt->enumLVALHigh,status,p_DrvPvt->verbosity);

	//Search for DVALHigh
	local_status |= irio_findResourceEnum(p_DrvPvt,STRINGNAME_DVALHIGH,-1,&p_DrvPvt->enumDVALHigh,status,p_DrvPvt->verbosity);

	//Search for SpareHigh
	local_status |= irio_findResourceEnum(p_DrvPvt,STRINGNAME_SPAREHIGH,-1,&p_DrvPvt->enumSpareHigh,status,p_DrvPvt->verbosity);

	//Search for ControlEnable
	local_status |= irio_findResourceEnum(p_DrvPvt,STRINGNAME_CONTROLENABLE,-1,&p_DrvPvt->enumControlEnable,status,p_DrvPvt->verbosity);

	//Search for SignalMapping
	local_status |= irio_findResourceEnum(p_DrvPvt,STRINGNAME_SIGNALMAPPING,-1,&p_DrvPvt->enumSignalMapping,status,p_DrvPvt->verbosity);

	//Search for Configuration
	local_status |= irio_findResourceEnum(p_DrvPvt,STRINGNAME_CONFIGURATION,-1,&p_DrvPvt->enumConfiguration,status,p_DrvPvt->verbosity);

	//Search for LineScan
	local_status |= irio_findResourceEnum(p_DrvPvt,STRINGNAME_LINESCAN,-1,&p_DrvPvt->enumLineScan,status,p_DrvPvt->verbosity);

	if(p_DrvPvt->verbosity && local_status==IRIO_success){
		printf("[%s,%d]-(%s) TRACE CameraLink Configuration Registers Found\n",__func__,__LINE__,p_DrvPvt->appCallID);
	}

	if(local_status<IRIO_error){
		return local_status;
	}else{
		return IRIO_error;
	}
}

int irio_configCL(irioDrv_t* p_DrvPvt,int32_t fvalHigh, int32_t lvalHigh, int32_t dvalHigh, int32_t spareHigh, int32_t controlEnable, int32_t linescan,CL_SignalMapping_T signalMapping, CL_Config_T configuration,TStatus* status){
	TIRIOStatusCode local_status = IRIO_success;
	NiFpga_Status fpgaStatus = NiFpga_Status_Success;

	if(!p_DrvPvt->enumFVALHigh.found || !p_DrvPvt->enumLVALHigh.found ||
		!p_DrvPvt->enumDVALHigh.found || !p_DrvPvt->enumSpareHigh.found ||
		!p_DrvPvt->enumControlEnable.found || !p_DrvPvt->enumSignalMapping.found||
		!p_DrvPvt->enumConfiguration.found){

		irio_mergeStatus(status,Write_Resource_Warning,p_DrvPvt->verbosity,"%s[%s,%d]-(%s) WARNING Some of CameraLink mandatory configuration resources were not found. Configuration will be incomplete.\n",__func__,__LINE__,p_DrvPvt->appCallID);
		local_status |= IRIO_warning;
	}

	//Set FVALHigh
	if(p_DrvPvt->enumFVALHigh.found){
		if(fvalHigh==0){
			NiFpga_MergeStatus(&fpgaStatus,NiFpga_WriteBool(p_DrvPvt->session,p_DrvPvt->enumFVALHigh.value,NiFpga_False));
		}else{
			NiFpga_MergeStatus(&fpgaStatus,NiFpga_WriteBool(p_DrvPvt->session,p_DrvPvt->enumFVALHigh.value,NiFpga_True));
		}
		if(NiFpga_IsError(fpgaStatus)){
			irio_mergeStatus(status,Write_NIRIO_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING FPGA Error writing %s. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_FVALHIGH,fpgaStatus);
			local_status |= IRIO_warning;
		}
	}

	//Set LVALHigh
	if(p_DrvPvt->enumLVALHigh.found){
		if(lvalHigh==0){
			NiFpga_MergeStatus(&fpgaStatus,NiFpga_WriteBool(p_DrvPvt->session,p_DrvPvt->enumLVALHigh.value,NiFpga_False));
		}else{
			NiFpga_MergeStatus(&fpgaStatus,NiFpga_WriteBool(p_DrvPvt->session,p_DrvPvt->enumLVALHigh.value,NiFpga_True));
		}
		if(NiFpga_IsError(fpgaStatus)){
			irio_mergeStatus(status,Write_NIRIO_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING FPGA Error writing %s. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_LVALHIGH,fpgaStatus);
			local_status |= IRIO_warning;
		}
	}

	//Set DVALHigh
	if(p_DrvPvt->enumDVALHigh.found){
		if(dvalHigh==0){
			NiFpga_MergeStatus(&fpgaStatus,NiFpga_WriteBool(p_DrvPvt->session,p_DrvPvt->enumDVALHigh.value,NiFpga_False));
		}else{
			NiFpga_MergeStatus(&fpgaStatus,NiFpga_WriteBool(p_DrvPvt->session,p_DrvPvt->enumDVALHigh.value,NiFpga_True));
		}
		if(NiFpga_IsError(fpgaStatus)){
			irio_mergeStatus(status,Write_NIRIO_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING FPGA Error writing %s. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_DVALHIGH,fpgaStatus);
			local_status |= IRIO_warning;
		}
	}

	//Set SpareHigh
	if(p_DrvPvt->enumSpareHigh.found){
		if(spareHigh==0){
			NiFpga_MergeStatus(&fpgaStatus,NiFpga_WriteBool(p_DrvPvt->session,p_DrvPvt->enumSpareHigh.value,NiFpga_False));
		}else{
			NiFpga_MergeStatus(&fpgaStatus,NiFpga_WriteBool(p_DrvPvt->session,p_DrvPvt->enumSpareHigh.value,NiFpga_True));
		}
		if(NiFpga_IsError(fpgaStatus)){
			irio_mergeStatus(status,Write_NIRIO_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING FPGA Error writing %s. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_SPAREHIGH,fpgaStatus);
			local_status |= IRIO_warning;
		}
	}

	//Set ControlEnable
	if(p_DrvPvt->enumControlEnable.found){
		if(controlEnable==0){
			NiFpga_MergeStatus(&fpgaStatus,NiFpga_WriteBool(p_DrvPvt->session,p_DrvPvt->enumControlEnable.value,NiFpga_False));
		}else{
			NiFpga_MergeStatus(&fpgaStatus,NiFpga_WriteBool(p_DrvPvt->session,p_DrvPvt->enumControlEnable.value,NiFpga_True));
		}
		if(NiFpga_IsError(fpgaStatus)){
			irio_mergeStatus(status,Write_NIRIO_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING FPGA Error writing %s. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_CONTROLENABLE,fpgaStatus);
			local_status |= IRIO_warning;
		}
	}


	//Set SignalMapping
	if(p_DrvPvt->enumSignalMapping.found){
		if(signalMapping<=CL_SIGNALMAPPING_LIMIT){
			NiFpga_MergeStatus(&fpgaStatus,NiFpga_WriteU8(p_DrvPvt->session,p_DrvPvt->enumSignalMapping.value,(uint8_t)signalMapping));
			if(NiFpga_IsError(fpgaStatus)){
				irio_mergeStatus(status,Write_NIRIO_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING FPGA Error writing %s. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_SIGNALMAPPING,fpgaStatus);
				local_status |= IRIO_warning;
			}
		}else{
			irio_mergeStatus(status,ValueOOB_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING Value %d was not defined for %s.\n",__func__,__LINE__,p_DrvPvt->appCallID,signalMapping,STRINGNAME_SIGNALMAPPING);
			local_status |= IRIO_warning;
		}
	}

	//Set Configuration
	if(p_DrvPvt->enumConfiguration.found){
		if(configuration<=CL_CONFIG_LIMIT){
			NiFpga_MergeStatus(&fpgaStatus,NiFpga_WriteU8(p_DrvPvt->session,p_DrvPvt->enumConfiguration.value,(uint8_t)configuration));
			if(NiFpga_IsError(fpgaStatus)){
				irio_mergeStatus(status,Write_NIRIO_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING FPGA Error writing %s. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_CONFIGURATION,fpgaStatus);
				local_status |= IRIO_warning;
			}
		}else{
			irio_mergeStatus(status,ValueOOB_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING Value %d was not defined for %s.\n",__func__,__LINE__,p_DrvPvt->appCallID,configuration,STRINGNAME_CONFIGURATION);
			local_status |= IRIO_warning;
		}
	}

	//Set LineScan (Optional)
	if(p_DrvPvt->enumLineScan.found){
		if(linescan==0){
			NiFpga_MergeStatus(&fpgaStatus,NiFpga_WriteBool(p_DrvPvt->session,p_DrvPvt->enumLineScan.value,NiFpga_False));
		}else{
			NiFpga_MergeStatus(&fpgaStatus,NiFpga_WriteBool(p_DrvPvt->session,p_DrvPvt->enumLineScan.value,NiFpga_True));
		}
		if(NiFpga_IsError(fpgaStatus)){
			irio_mergeStatus(status,Write_NIRIO_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING FPGA Error writing %s. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_LINESCAN,fpgaStatus);
			local_status |= IRIO_warning;
		}
	}

	if(local_status<IRIO_error){
		return local_status;
	}else{
		return IRIO_error;
	}
}

int irio_sendCLuart(irioDrv_t* p_DrvPvt, const char *msg, int msg_size,TStatus* status){
	TIRIOStatusCode local_status = IRIO_success;
	int i;
	NiFpga_Bool TxReady;
	uint8_t aux;
	NiFpga_Status fpgaStatus= NiFpga_Status_Success;

	if(!p_DrvPvt->enumuartTxReady.found || !p_DrvPvt->enumuartTxByte.found || !p_DrvPvt->enumuartTransmit.found){
		irio_mergeStatus(status,Write_Resource_Warning,p_DrvPvt->verbosity,"%s[%s,%d]-(%s) WARNING Some of UART mandatory resources for sending messages were not found. Can not send UART messages.\n",__func__,__LINE__,p_DrvPvt->appCallID);
		return IRIO_warning;
	}

	for (i=0;(i<msg_size) && local_status==IRIO_success;i++)
	{
		do{
			fpgaStatus=NiFpga_ReadBool(p_DrvPvt->session,p_DrvPvt->enumuartTxReady.value, &TxReady);
			if(NiFpga_IsError(fpgaStatus)){
				irio_mergeStatus(status,Read_NIRIO_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING FPGA Error reading %s. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_UARTTXREADY,fpgaStatus);
				local_status |= IRIO_warning;
			}

		}while((TxReady==0) && (local_status==IRIO_success));

		if((TxReady== 1) && (local_status==IRIO_success))
		{
			aux=msg[i];

			fpgaStatus=NiFpga_WriteU8(p_DrvPvt->session,p_DrvPvt->enumuartTxByte.value,(uint8_t)aux);
			if(NiFpga_IsError(fpgaStatus)){
				irio_mergeStatus(status,Write_NIRIO_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING FPGA Error writing %s. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_UARTTXBYTE,fpgaStatus);
				local_status |= IRIO_warning;
			}

			if(local_status==IRIO_success){
				//This signal goes to zero by hardware
				fpgaStatus=NiFpga_WriteBool(p_DrvPvt->session,p_DrvPvt->enumuartTransmit.value, (NiFpga_Bool)1);
				if(NiFpga_IsError(fpgaStatus)){
					irio_mergeStatus(status,Write_NIRIO_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING FPGA Error writing %s. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_UARTTRANSMIT,fpgaStatus);
					local_status |= IRIO_warning;
				}
			}
		}
	}

	if(local_status<IRIO_error){
		return local_status;
	}else{
		return IRIO_error;
	}
}

int irio_getCLuart(irioDrv_t* p_DrvPvt, char* data, int* msg_size,TStatus* status){
	TIRIOStatusCode local_status = IRIO_success;
	NiFpga_Status fpgaStatus;
	NiFpga_Bool RxReady;
	NiFpga_Bool receptionCompleted=0;
	uint8_t aux;

	int numbytes=0;

	if(!p_DrvPvt->enumuartRxReady.found || !p_DrvPvt->enumuartRxByte.found || !p_DrvPvt->enumuartReceive.found){
		irio_mergeStatus(status,Read_Resource_Warning,p_DrvPvt->verbosity,"%s[%s,%d]-(%s) WARNING Some of UART mandatory resources for receiving messages were not found. Can not receive UART messages.\n",__func__,__LINE__,p_DrvPvt->appCallID);
		return IRIO_warning;
	}

	// we wait until there are bytes available at serial port

	int timeout=0;
	do{

		fpgaStatus=NiFpga_ReadBool(p_DrvPvt->session,p_DrvPvt->enumuartRxReady.value,&RxReady);
		if(NiFpga_IsError(fpgaStatus)){
			irio_mergeStatus(status,Read_NIRIO_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING FPGA Error reading %s. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_UARTRXREADY,fpgaStatus);
			local_status |= IRIO_warning;
		}

		if(!RxReady){
			usleep(1000);
			timeout++;
		}

		if (timeout==100)
		{
			irio_mergeStatus(status,CLSLinetimeout_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) CameraLink Serial Line Timeout  . Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,CLSLinetimeout_Warning);
			local_status |= IRIO_warning;

		}

	}while(local_status==IRIO_success && !RxReady && timeout<3000);

	timeout=0;
	while (RxReady && local_status==IRIO_success){
		do{
			fpgaStatus=NiFpga_ReadBool(p_DrvPvt->session,p_DrvPvt->enumuartRxReady.value, &RxReady);
			if(NiFpga_IsError(fpgaStatus)){
				irio_mergeStatus(status,Read_NIRIO_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING FPGA Error reading %s. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_UARTRXREADY,fpgaStatus);
				local_status |= IRIO_warning;
			}
			if(!RxReady){
				usleep(1000);
				timeout++;
			}
		}while(!RxReady && timeout<1000);

		if(RxReady && (local_status==IRIO_success))
		{
			fpgaStatus= NiFpga_WriteBool(p_DrvPvt->session, p_DrvPvt->enumuartReceive.value, (NiFpga_Bool)1);
			if(NiFpga_IsError(fpgaStatus)){
				irio_mergeStatus(status,Write_NIRIO_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING FPGA Error writing %s. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_UARTRECEIVE,fpgaStatus);
							local_status |= IRIO_warning;
			}

			do{
				fpgaStatus=NiFpga_ReadBool(p_DrvPvt->session,p_DrvPvt->enumuartReceive.value,&receptionCompleted);
				if(NiFpga_IsError(fpgaStatus)){
					irio_mergeStatus(status,Read_NIRIO_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING FPGA Error reading %s. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_UARTRECEIVE,fpgaStatus);
					local_status |= IRIO_warning;
				}
			}while (receptionCompleted && local_status==IRIO_success);

			fpgaStatus = NiFpga_ReadU8(p_DrvPvt->session,p_DrvPvt->enumuartRxByte.value,&aux);
			if(NiFpga_IsError(fpgaStatus)){
				irio_mergeStatus(status,Read_NIRIO_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING FPGA Error reading %s. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_UARTRXBYTE,fpgaStatus);
				local_status |= IRIO_warning;
			}
			data[numbytes]=aux;
			numbytes++;
		}

	}

	(*msg_size) =numbytes;

	if(local_status<IRIO_error){
		return local_status;
	}else{
		return IRIO_error;
	}
}

int irio_getUARTBaudRate(irioDrv_t* p_DrvPvt,int32_t* value,TStatus* status){
	TIRIOStatusCode local_status = IRIO_success;

	if(p_DrvPvt->enumuartBaudRate.found){
		NiFpga_Status fpgaStatus = NiFpga_Status_Success;
		uint8_t aux;
		fpgaStatus=NiFpga_ReadU8(p_DrvPvt->session,p_DrvPvt->enumuartBaudRate.value,&aux);
		if(NiFpga_IsError(fpgaStatus)){
			irio_mergeStatus(status,Write_NIRIO_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING FPGA Error reading %s. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_UARTBAUDRATE,fpgaStatus);
			local_status |= IRIO_warning;
		}else{
			*value=(int32_t)aux;
		}
	}else{
		irio_mergeStatus(status,Read_Resource_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING %s was not found.\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_UARTBAUDRATE);
		local_status |= IRIO_warning;
	}

	if(local_status<IRIO_error){
		return local_status;
	}else{
		return IRIO_error;
	}
}

int irio_setUARTBaudRate(irioDrv_t* p_DrvPvt,int32_t value,TStatus* status){
	TIRIOStatusCode local_status = IRIO_success;
	if(p_DrvPvt->enumuartBaudRate.found && p_DrvPvt->enumuartSetBaudRate.found){
		if(value<UART_BAUDRATE_LIMIT){
			int timeout=0;
			NiFpga_Status fpgaStatus = NiFpga_Status_Success;

			NiFpga_Bool aux_bool = NiFpga_True;
			uint8_t aux_u8 = value;

			//1st Wait for SetBaudRate=false;
			fpgaStatus = NiFpga_ReadBool(p_DrvPvt->session,p_DrvPvt->enumuartSetBaudRate.value,&aux_bool);
			do{
				if(aux_bool==NiFpga_True && NiFpga_IsNotError(fpgaStatus)){
					usleep(100);
					timeout+=100;
				}
				NiFpga_MergeStatus(&fpgaStatus,NiFpga_ReadBool(p_DrvPvt->session,p_DrvPvt->enumuartSetBaudRate.value,&aux_bool));
			}while(timeout<UART_BAUDRATE_USTIMEOUT && aux_bool==NiFpga_True && NiFpga_IsNotError(fpgaStatus));

			//Check if SetBaudRate=false
			if(NiFpga_IsError(fpgaStatus)){
				irio_mergeStatus(status,Read_NIRIO_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING FPGA Error reading %s. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_UARTSETBAUDRATE,fpgaStatus);
				return IRIO_warning;
			}else if(aux_bool==NiFpga_True){
				irio_mergeStatus(status,ConfigUART_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING Timeout waiting for initial %s fall edge. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_UARTSETBAUDRATE);
				return IRIO_warning;
			}

			//Set BaudRate
			NiFpga_MergeStatus(&fpgaStatus,NiFpga_WriteU8(p_DrvPvt->session,p_DrvPvt->enumuartBaudRate.value,aux_u8));
			NiFpga_MergeStatus(&fpgaStatus,NiFpga_WriteBool(p_DrvPvt->session,p_DrvPvt->enumuartSetBaudRate.value,NiFpga_True));
			if(NiFpga_IsError(fpgaStatus)){
				irio_mergeStatus(status,Write_NIRIO_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING FPGA Error setting %s. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_UARTBAUDRATE,fpgaStatus);
				return IRIO_warning;
			}

			//2nd Wait for SetBaudRate=false;
			timeout=0;
			NiFpga_MergeStatus(&fpgaStatus,NiFpga_ReadBool(p_DrvPvt->session,p_DrvPvt->enumuartSetBaudRate.value,&aux_bool));
			do{
				if(aux_bool==NiFpga_True && NiFpga_IsNotError(fpgaStatus)){
					usleep(10);
					timeout+=10;
				}
				NiFpga_MergeStatus(&fpgaStatus,NiFpga_ReadBool(p_DrvPvt->session,p_DrvPvt->enumuartSetBaudRate.value,&aux_bool));
			}while(timeout<UART_BAUDRATE_USTIMEOUT && aux_bool==NiFpga_True && NiFpga_IsNotError(fpgaStatus));

			//Check if SetBaudRate=false
			if(NiFpga_IsError(fpgaStatus)){
				irio_mergeStatus(status,Read_Resource_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING FPGA Error reading %s. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_UARTSETBAUDRATE,fpgaStatus);
				local_status |= IRIO_warning;
			}else if(aux_bool==NiFpga_True){
				irio_mergeStatus(status,ConfigUART_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING Timeout waiting for final %s fall edge. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_UARTSETBAUDRATE);
				local_status |= IRIO_warning;
			}

		}else{
			irio_mergeStatus(status,ValueOOB_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING Value %d was not defined for %s.\n",__func__,__LINE__,p_DrvPvt->appCallID,value,STRINGNAME_UARTBAUDRATE);
			local_status |= IRIO_warning;
		}
	}else{
		irio_mergeStatus(status,Write_Resource_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING %s was not found.\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_UARTBAUDRATE);
		local_status |= IRIO_warning;
	}

	if(local_status<IRIO_error){
		return local_status;
	}else{
		return IRIO_error;
	}

}

int irio_getUARTBreakIndicator(irioDrv_t* p_DrvPvt,int32_t* value,TStatus* status){
	TIRIOStatusCode local_status = IRIO_success;

	if(p_DrvPvt->enumuartBreakIndicator.found){
		NiFpga_Status fpgaStatus=NiFpga_Status_Success;

		uint16_t aux=0;
		NiFpga_MergeStatus(&fpgaStatus,NiFpga_ReadU16(p_DrvPvt->session,p_DrvPvt->enumuartBreakIndicator.value,&aux));
		if(NiFpga_IsError(fpgaStatus)){
			irio_mergeStatus(status,Read_NIRIO_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING FPGA Error reading %s. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_UARTBREAKINDICATOR,fpgaStatus);
			local_status |= IRIO_warning;
		}else{
			*value=(int32_t)aux;
		}
	}else{
		irio_mergeStatus(status,Read_Resource_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING %s was not found.\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_UARTBREAKINDICATOR);
		local_status |= IRIO_warning;
	}

	if(local_status<IRIO_error){
		return local_status;
	}else{
		return IRIO_error;
	}
}

int irio_getUARTFrammingError(irioDrv_t* p_DrvPvt,int32_t* value,TStatus* status){
	TIRIOStatusCode local_status = IRIO_success;

	if(p_DrvPvt->enumuartFramingError.found){
		NiFpga_Status fpgaStatus=NiFpga_Status_Success;

		uint16_t aux=0;
		NiFpga_MergeStatus(&fpgaStatus,NiFpga_ReadU16(p_DrvPvt->session,p_DrvPvt->enumuartFramingError.value,&aux));
		if(NiFpga_IsError(fpgaStatus)){
			irio_mergeStatus(status,Read_NIRIO_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING FPGA Error reading %s. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_UARTFRAMINGERROR,fpgaStatus);
			local_status |= IRIO_warning;
		}else{
			*value=(int32_t)aux;
		}
	}else{
		irio_mergeStatus(status,Read_Resource_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING %s was not found.\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_UARTFRAMINGERROR);
		local_status |= IRIO_warning;
	}

	if(local_status<IRIO_error){
		return local_status;
	}else{
		return IRIO_error;
	}
}

int irio_getUARTOverrunError(irioDrv_t* p_DrvPvt,int32_t* value,TStatus* status){
	TIRIOStatusCode local_status = IRIO_success;

	if(p_DrvPvt->enumuartOverrunError.found){
		NiFpga_Status fpgaStatus=NiFpga_Status_Success;

		uint16_t aux=0;
		NiFpga_MergeStatus(&fpgaStatus,NiFpga_ReadU16(p_DrvPvt->session,p_DrvPvt->enumuartOverrunError.value,&aux));
		if(NiFpga_IsError(fpgaStatus)){
			irio_mergeStatus(status,Read_NIRIO_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING FPGA Error reading %s. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_UARTOVERRUNERROR,fpgaStatus);
			local_status |= IRIO_warning;
		}else{
			*value=(int32_t)aux;
		}
	}else{
		irio_mergeStatus(status,Read_Resource_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING %s was not found.\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_UARTOVERRUNERROR);
		local_status |= IRIO_warning;
	}

	if(local_status<IRIO_error){
		return local_status;
	}else{
		return IRIO_error;
	}
}
