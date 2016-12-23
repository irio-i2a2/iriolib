/**************************************************************************//**
 * \file irioHandlerDMA.c
 * \authors Mariano Ruiz (Universidad Politécnica de Madrid, UPM)
 * \authors Diego Sanz (Universidad Politécnica de Madrid, UPM)
 * \authors Sergio Esquembri (Universidad Politécnica de Madrid, UPM)
 * \authors Enrique Bernal (Universidad Politécnica de Madrid, UPM)
 * \authors Alvaro Bustos (Universidad Politécnica de Madrid, UPM)
 * \brief DMA handler methods for IRIO Driver
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

#include "irioHandlerDMA.h"
#include "irioDataTypes.h"
#include "irioResourceFinder.h"
#include "irioError.h"

#include "NiFpga.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/** @name DMA Resource Strings
 * Strings for DMA FPGA Resources
 */
///@{
#define STRINGNAME_DMATTOHOSTNCH "_IndicatorArrayU16_DMATtoHOSTNCh"
#define STRINGNAME_DMATTOHOSTNCHSIZE "_IndicatorArrayU16Size_DMATtoHOSTNCh"
#define STRINGNAME_DMATTOHOSTOVERFLOWS "_IndicatorU16_DMATtoHOSTOverflows"
#define STRINGNAME_DMATTOHOST "_TargetToHostFifoU64_DMATtoHOST"
#define STRINGNAME_DMATTOHOSTENABLE "_ControlBool_DMATtoHOSTEnable"
#define STRINGNAME_DMATTOHOSTSAMPLINGRATE  "_ControlU16_DMATtoHOSTSamplingRate"
#define STRINGNAME_DMATTOHOSTFRAMETYPE "_IndicatorArrayU8_DMATtoHOSTFrameType"
#define STRINGNAME_DMATTOHOSTSAMPLESIZE "_IndicatorArrayU8_DMATtoHOSTSampleSize"
#define STRINGNAME_DMATTOHOSTBLOCKNWORDS "_IndicatorArrayU16_DMATtoHOSTBlockNWords"

///@}

int irio_findDAQDMAs(irioDrv_t* p_DrvPvt, TStatus* status){

	NiFpga_Status fpgaStatus = NiFpga_Status_Success;
	TIRIOStatusCode local_status = IRIO_success;

	//Search for NCHperDMATtoHOST and size
	//Search for FrameTypeDMATtoHOST and size
	//Search for Bytes
	local_status |= irio_findResourceEnum(p_DrvPvt,STRINGNAME_DMATTOHOSTNCH,-1,&p_DrvPvt->enumDMATtoHOSTNCh,status,p_DrvPvt->verbosity);
	local_status |= irio_findResourceEnum(p_DrvPvt,STRINGNAME_DMATTOHOSTNCHSIZE,-1,&p_DrvPvt->DMATtoHOSTNo,status,p_DrvPvt->verbosity);
	local_status |= irio_findResourceEnum(p_DrvPvt,STRINGNAME_DMATTOHOSTOVERFLOWS,-1,&p_DrvPvt->enumDMATtoHOSTOverflows,status,p_DrvPvt->verbosity);
	local_status |= irio_findResourceEnum(p_DrvPvt,STRINGNAME_DMATTOHOSTFRAMETYPE,-1,&p_DrvPvt->enumDMATtoHOSTFrameType,status,p_DrvPvt->verbosity);
	local_status |= irio_findResourceEnum(p_DrvPvt,STRINGNAME_DMATTOHOSTSAMPLESIZE,-1,&p_DrvPvt->enumDMATtoHOSTSampleSize,status,p_DrvPvt->verbosity);
	local_status |= irio_findResourceEnum(p_DrvPvt,STRINGNAME_DMATTOHOSTBLOCKNWORDS,-1,&p_DrvPvt->enumDMATtoHOSTBlockNWords,status,p_DrvPvt->verbosity);

	//reading the array NCHperDMATtoHOST
	if(p_DrvPvt->enumDMATtoHOSTNCh.found && p_DrvPvt->DMATtoHOSTNo.found){
		NiFpga_MergeStatus(&fpgaStatus,NiFpga_ReadArrayU16(p_DrvPvt->session,p_DrvPvt->enumDMATtoHOSTNCh.value,p_DrvPvt->DMATtoHOSTNCh,p_DrvPvt->DMATtoHOSTNo.value));
		if(NiFpga_IsError(fpgaStatus)){
			irio_mergeStatus(status,NIRIO_API_Error,p_DrvPvt->verbosity,"[%s,%d]-(%s) ERROR FPGA Error reading %s. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_DMATTOHOSTNCH,fpgaStatus);
			local_status |= IRIO_error;
			fpgaStatus = NiFpga_Status_Success;
		}
	}

	//reading the array DMATtoHOSTFrameType
	if(p_DrvPvt->enumDMATtoHOSTFrameType.found && p_DrvPvt->DMATtoHOSTNo.found){
		//we are assuming that the size of this array is the same that the size for the array with number of channels.
		NiFpga_MergeStatus(&fpgaStatus,NiFpga_ReadArrayU8(p_DrvPvt->session,p_DrvPvt->enumDMATtoHOSTFrameType.value,p_DrvPvt->DMATtoHOSTFrameType,p_DrvPvt->DMATtoHOSTNo.value));
		if(NiFpga_IsError(fpgaStatus)){
			irio_mergeStatus(status,NIRIO_API_Error,p_DrvPvt->verbosity,"[%s,%d]-(%s) ERROR FPGA Error reading %s. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_DMATTOHOSTFRAMETYPE,fpgaStatus);
			local_status |= IRIO_error;
			fpgaStatus = NiFpga_Status_Success;
		}
	}

	//reading the array DMATtoHOSTSampleSize
	if(p_DrvPvt->enumDMATtoHOSTSampleSize.found && p_DrvPvt->DMATtoHOSTNo.found){
		//we are assuming that the size of this array is the same that the size for the array with number of channels.
		NiFpga_MergeStatus(&fpgaStatus,NiFpga_ReadArrayU8(p_DrvPvt->session,p_DrvPvt->enumDMATtoHOSTSampleSize.value,p_DrvPvt->DMATtoHOSTSampleSize,p_DrvPvt->DMATtoHOSTNo.value));
		if(NiFpga_IsError(fpgaStatus)){
			irio_mergeStatus(status,NIRIO_API_Error,p_DrvPvt->verbosity,"[%s,%d]-(%s) ERROR FPGA Error reading %s. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_DMATTOHOSTSAMPLESIZE,fpgaStatus);
			local_status |= IRIO_error;
			fpgaStatus = NiFpga_Status_Success;
		}
	}

	//reading the array DMATtoHOSTBlockNWords
	if(p_DrvPvt->enumDMATtoHOSTBlockNWords.found && p_DrvPvt->DMATtoHOSTNo.found){
		//we are assuming that the size of this array is the same that the size for the array with number of channels.
		NiFpga_MergeStatus(&fpgaStatus,NiFpga_ReadArrayU16(p_DrvPvt->session,p_DrvPvt->enumDMATtoHOSTBlockNWords.value,p_DrvPvt->DMATtoHOSTBlockNWords,p_DrvPvt->DMATtoHOSTNo.value));
		if(NiFpga_IsError(fpgaStatus)){
			irio_mergeStatus(status,NIRIO_API_Error,p_DrvPvt->verbosity,"[%s,%d]-(%s) ERROR FPGA Error reading %s. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_DMATTOHOSTBLOCKNWORDS,fpgaStatus);
			local_status |= IRIO_error;
			fpgaStatus = NiFpga_Status_Success;
		}
	}

	//Search DMAs
	if(p_DrvPvt->enumDMATtoHOSTNCh.found && p_DrvPvt->DMATtoHOSTNo.found){

		if(p_DrvPvt->verbosity){
			printf("[%s,%d]-(%s) TRACE DAQ DMAs found (Searching %d):\n",__func__,__LINE__,p_DrvPvt->appCallID,p_DrvPvt->DMATtoHOSTNo.value);
		}


		//Search NumberOfDmas DMAs
		TStatus dma_status,accum_status;
		irio_initStatus(&dma_status);
		irio_initStatus(&accum_status);
		int i=0,accum=0;
		int count=0;
		while(i<p_DrvPvt->DMATtoHOSTNo.value){
			//Find mandatory DMA ports
			irio_findResourceEnum(p_DrvPvt,STRINGNAME_DMATTOHOST,i,&p_DrvPvt->enumDMATtoHOST[i],&dma_status,0);
			irio_findResourceEnum(p_DrvPvt,STRINGNAME_DMATTOHOSTSAMPLINGRATE,i,&p_DrvPvt->enumDMATtoHOSTSamplingRate[i],&dma_status,0);
			irio_findResourceEnum(p_DrvPvt,STRINGNAME_DMATTOHOSTENABLE,i,&p_DrvPvt->enumDMATtoHOSTEnable[i],&dma_status,0);

			if(dma_status.code==IRIO_success){
				//Index the correlation of channels found
				p_DrvPvt->DMATtoHOSTChIndex[i]=accum;
				accum+=p_DrvPvt->DMATtoHOSTNCh[i];
				if(p_DrvPvt->verbosity){
					printf("[%s,%d]-(%s) DMA%d:DMATtoHOSTBlockNWords=%d ;  DMATtoHOSTSampleSize=%d ; DMATtoHOSTFrameType=%d \n",__func__,__LINE__,p_DrvPvt->appCallID,i,p_DrvPvt->DMATtoHOSTBlockNWords[i],p_DrvPvt->DMATtoHOSTSampleSize[i],p_DrvPvt->DMATtoHOSTFrameType[i]);
				}
				count++;
			}else{
				irio_mergeStatus(&accum_status,ResourceNotFound_Error,0,"[%s,%d]-(%s) ERROR Searching DMA%d resources:\n%s",__func__,__LINE__,p_DrvPvt->appCallID,i,dma_status.msg);
				local_status |= IRIO_error;
				irio_resetStatus(&dma_status);
				p_DrvPvt->enumDMATtoHOST[i].found=0;
				p_DrvPvt->enumDMATtoHOSTSamplingRate[i].found=0;
				p_DrvPvt->enumDMATtoHOSTEnable[i].found=0;
			}
			i++;
		}

		if(accum_status.code!=IRIO_success){
			irio_mergeStatus(status,accum_status.detailCode,p_DrvPvt->verbosity,"%s",accum_status.msg);
			irio_resetStatus(&accum_status);
		}

		if(p_DrvPvt->verbosity){
			printf("[%s,%d]-(%s) TRACE TOTAL DAQ DMAs found: %d/%d\n",__func__,__LINE__,p_DrvPvt->appCallID,count,p_DrvPvt->DMATtoHOSTNo.value);
		}
	}

	if(local_status<IRIO_error){
		return local_status;
	}else{
		return IRIO_error;
	}
}

int irio_findIMAQDMAs(irioDrv_t* p_DrvPvt,TStatus* status){

	TIRIOStatusCode local_status = IRIO_success;
	NiFpga_Status fpgaStatus = NiFpga_Status_Success;
	//Search for NCHperDMATtoHOST size

	local_status |= irio_findResourceEnum(p_DrvPvt,STRINGNAME_DMATTOHOSTNCH,-1,&p_DrvPvt->enumDMATtoHOSTNCh,status,p_DrvPvt->verbosity);
	local_status |= irio_findResourceEnum(p_DrvPvt,STRINGNAME_DMATTOHOSTNCHSIZE,-1,&p_DrvPvt->DMATtoHOSTNo,status,p_DrvPvt->verbosity);
	local_status |= irio_findResourceEnum(p_DrvPvt,STRINGNAME_DMATTOHOSTOVERFLOWS,-1,&p_DrvPvt->enumDMATtoHOSTOverflows,status,p_DrvPvt->verbosity);
	local_status |= irio_findResourceEnum(p_DrvPvt,STRINGNAME_DMATTOHOSTFRAMETYPE,-1,&p_DrvPvt->enumDMATtoHOSTFrameType,status,p_DrvPvt->verbosity);
	local_status |= irio_findResourceEnum(p_DrvPvt,STRINGNAME_DMATTOHOSTSAMPLESIZE,-1,&p_DrvPvt->enumDMATtoHOSTSampleSize,status,p_DrvPvt->verbosity);



	//reading the array NCHperDMATtoHOST
		if(p_DrvPvt->enumDMATtoHOSTNCh.found && p_DrvPvt->DMATtoHOSTNo.found){
			NiFpga_MergeStatus(&fpgaStatus,NiFpga_ReadArrayU16(p_DrvPvt->session,p_DrvPvt->enumDMATtoHOSTNCh.value,p_DrvPvt->DMATtoHOSTNCh,p_DrvPvt->DMATtoHOSTNo.value));
			if(NiFpga_IsError(fpgaStatus)){
				irio_mergeStatus(status,NIRIO_API_Error,p_DrvPvt->verbosity,"[%s,%d]-(%s) ERROR FPGA Error reading %s. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_DMATTOHOSTNCH,fpgaStatus);
				local_status |= IRIO_error;
				fpgaStatus = NiFpga_Status_Success;
			}
		}

	//reading the array DMATtoHOSTFrameType
	if(p_DrvPvt->enumDMATtoHOSTFrameType.found && p_DrvPvt->DMATtoHOSTNo.found){
		//we are assuming that the size of this array is the same that the size for the array with number of channels.
		NiFpga_MergeStatus(&fpgaStatus,NiFpga_ReadArrayU8(p_DrvPvt->session,p_DrvPvt->enumDMATtoHOSTFrameType.value,p_DrvPvt->DMATtoHOSTFrameType,p_DrvPvt->DMATtoHOSTNo.value));
		if(NiFpga_IsError(fpgaStatus)){
			irio_mergeStatus(status,NIRIO_API_Error,p_DrvPvt->verbosity,"[%s,%d]-(%s) ERROR FPGA Error reading %s. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_DMATTOHOSTFRAMETYPE,fpgaStatus);
			local_status |= IRIO_error;
			fpgaStatus = NiFpga_Status_Success;
		}
	}

	//reading the array DMATtoHOSTSampleSize
	if(p_DrvPvt->enumDMATtoHOSTSampleSize.found && p_DrvPvt->DMATtoHOSTNo.found){
		//we are assuming that the size of this array is the same that the size for the array with number of channels.
		NiFpga_MergeStatus(&fpgaStatus,NiFpga_ReadArrayU8(p_DrvPvt->session,p_DrvPvt->enumDMATtoHOSTSampleSize.value,p_DrvPvt->DMATtoHOSTSampleSize,p_DrvPvt->DMATtoHOSTNo.value));
		if(NiFpga_IsError(fpgaStatus)){
			irio_mergeStatus(status,NIRIO_API_Error,p_DrvPvt->verbosity,"[%s,%d]-(%s) ERROR FPGA Error reading %s. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_DMATTOHOSTSAMPLESIZE,fpgaStatus);
			local_status |= IRIO_error;
			fpgaStatus = NiFpga_Status_Success;
		}
	}


	//Search DMAs
	if(p_DrvPvt->DMATtoHOSTNo.found){

		if(p_DrvPvt->verbosity){
			printf("[%s,%d]-(%s) TRACE IMAQ DMAs found (Searching %d):",__func__,__LINE__,p_DrvPvt->appCallID,p_DrvPvt->DMATtoHOSTNo.value);
		}

		//Search NumberOfDmas DMAs
		int i=0;
		int count=0;
		TStatus dma_status,accum_status;
		irio_initStatus(&dma_status);
		irio_initStatus(&accum_status);
		while(i<p_DrvPvt->DMATtoHOSTNo.value){
			//Find mandatory DMA ports
			irio_findResourceEnum(p_DrvPvt,STRINGNAME_DMATTOHOST,i,&p_DrvPvt->enumDMATtoHOST[i],&dma_status,0);
			irio_findResourceEnum(p_DrvPvt,STRINGNAME_DMATTOHOSTENABLE,i,&p_DrvPvt->enumDMATtoHOSTEnable[i],&dma_status,0);

			if(dma_status.code==IRIO_success){
				//Index the correlation of channels found
				p_DrvPvt->DMATtoHOSTChIndex[i]=i;
				if(p_DrvPvt->verbosity){
					printf("DMA%d, ",i);
				}
				count++;
			}else{
				irio_mergeStatus(&accum_status,ResourceNotFound_Error,0,"[%s,%d]-(%s) ERROR Searching DMA%d resources:\n%s",__func__,__LINE__,p_DrvPvt->appCallID,i,dma_status.msg);
				irio_resetStatus(&dma_status);
				local_status |= IRIO_error;
				p_DrvPvt->enumDMATtoHOST[i].found=0;
				p_DrvPvt->enumDMATtoHOSTEnable[i].found=0;
			}
			i++;
		}

		if(accum_status.code!=IRIO_success){
			irio_mergeStatus(status,accum_status.detailCode,p_DrvPvt->verbosity,"%s",accum_status.msg);
			irio_resetStatus(&accum_status);
		}

		if(p_DrvPvt->verbosity){
			printf("\n");
			printf("[%s,%d]-(%s) TRACE TOTAL IMAQ DMAs found: %d/%d\n",__func__,__LINE__,p_DrvPvt->appCallID,count,p_DrvPvt->DMATtoHOSTNo.value);
		}
	}

	if(local_status<IRIO_error){
		return local_status;
	}else{
		return IRIO_error;
	}
}

int irio_setUpDMAsTtoHost(irioDrv_t* p_DrvPvt,TStatus* status){

	if(!p_DrvPvt->DMATtoHOSTNo.found){
		irio_mergeStatus(status,Read_Resource_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING Can not set up DMAs. DMAs were not searched or found.\n",__func__,__LINE__,p_DrvPvt->appCallID);
		return IRIO_warning;
	}

	int i;
	TIRIOStatusCode local_status = IRIO_success;
	NiFpga_Status fpgaStatus=NiFpga_Status_Success;
	//Configure and start FIFOs
	for (i=0; i<p_DrvPvt->DMATtoHOSTNo.value; i++){
		if(!p_DrvPvt->enumDMATtoHOST[i].found){
			irio_mergeStatus(status,Read_Resource_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING Can not set up DMA%d. DMA was not found. DMAs were not searched or found.\n",__func__,__LINE__,p_DrvPvt->appCallID,i);
			local_status |= IRIO_warning;
		}else{
			NiFpga_MergeStatus(&fpgaStatus,
					NiFpga_ConfigureFifo(p_DrvPvt->session,p_DrvPvt->enumDMATtoHOST[i].value,SIZE_HOST_DMAS));
			NiFpga_MergeStatus(&fpgaStatus,
				NiFpga_StartFifo(p_DrvPvt->session,p_DrvPvt->enumDMATtoHOST[i].value));
			if(NiFpga_IsError(fpgaStatus)){
				irio_mergeStatus(status,ConfigDMA_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING FPGA Error setting up DMA%d. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,i,fpgaStatus,p_DrvPvt->DMATtoHOSTNo.value);
				local_status |= IRIO_warning;
				fpgaStatus=NiFpga_Status_Success;
			}
		}
	}

	//Not necessary to stop DAQ, is already stopped from initialTargetCheck()

	//Clean DMAs from trash data
	local_status |= irio_cleanDMAsTtoHost(p_DrvPvt,status);

	if(local_status<IRIO_error){
		return local_status;
	}else{
		return IRIO_error;
	}
}

int irio_closeDMAsTtoHost(irioDrv_t* p_DrvPvt, TStatus* status){

	if(!p_DrvPvt->DMATtoHOSTNo.found){
		irio_mergeStatus(status,Read_Resource_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING Can not close DMAs. DMAs were not searched or found.\n",__func__,__LINE__,p_DrvPvt->appCallID);
		return IRIO_warning;
	}

	NiFpga_Status fpgaStatus = NiFpga_Status_Success;
	TIRIOStatusCode local_status = IRIO_success;
	int i=0;
	for(i=0;i<p_DrvPvt->DMATtoHOSTNo.value;i++){
		if(!p_DrvPvt->enumDMATtoHOST[i].found){
			irio_mergeStatus(status,Read_Resource_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING Can not close DMA%d. DMA was not found.\n",__func__,__LINE__,p_DrvPvt->appCallID,i);
			local_status |= IRIO_warning;
		}else{
			NiFpga_MergeStatus(&fpgaStatus,NiFpga_StopFifo(p_DrvPvt->session,p_DrvPvt->enumDMATtoHOST[i].value));
			if(NiFpga_IsError(fpgaStatus)){
				irio_mergeStatus(status,ConfigDMA_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING FPGA Error closing DMA%d. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,i,fpgaStatus);
				local_status |= IRIO_warning;
				fpgaStatus=NiFpga_Status_Success;
			}
		}
	}

	if(local_status<IRIO_error){
		return local_status;
	}else{
		return IRIO_error;
	}
}

int irio_cleanDMAsTtoHost(irioDrv_t* p_DrvPvt, TStatus* status){

	if(!p_DrvPvt->DMATtoHOSTNo.found){
		irio_mergeStatus(status,Read_Resource_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING Can not clean DMAs. DMAs were not searched or found.\n",__func__,__LINE__,p_DrvPvt->appCallID);
		return IRIO_warning;
	}

	int i;
	TIRIOStatusCode local_status = IRIO_success;
	uint64_t* cleanbuffer=calloc(20000,sizeof(uint64_t));
	for (i=0; i<p_DrvPvt->DMATtoHOSTNo.value; i++){
		local_status |= irio_cleanDMATtoHost(p_DrvPvt,i,cleanbuffer,20000,status);
	}
	free(cleanbuffer);

	if(local_status<IRIO_error){
		return local_status;
	}else{
		return IRIO_error;
	}
}

int irio_cleanDMATtoHost(irioDrv_t* p_DrvPvt, int n,uint64_t* cleanbuffer, size_t buffersize, TStatus* status){
	
	int toRead=0;
	if(!p_DrvPvt->enumDMATtoHOST[n].found){
		irio_mergeStatus(status,Read_Resource_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) ERROR Can not close DMA%d. DMA was not found. .\n",__func__,__LINE__,p_DrvPvt->appCallID,n);
		return IRIO_warning;
	}
	
	NiFpga_Status fpgaStatus = NiFpga_Status_Success;
	size_t last;
	NiFpga_MergeStatus(&fpgaStatus,
			NiFpga_ReadFifoU64(p_DrvPvt->session,p_DrvPvt->enumDMATtoHOST[n].value,
			  cleanbuffer,0,0,&last));
	while(last>0 && NiFpga_IsNotError(fpgaStatus)){
		if(last>buffersize){
			toRead=buffersize;
		}else{
			toRead=last;
		}
		NiFpga_MergeStatus(&fpgaStatus,
			NiFpga_ReadFifoU64(p_DrvPvt->session, p_DrvPvt->enumDMATtoHOST[n].value,
				  cleanbuffer,toRead,10,&last));
	}

	if(NiFpga_IsError(fpgaStatus)){
		irio_mergeStatus(status,Read_NIRIO_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING FPGA Error cleaning DMA%d. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,n,fpgaStatus);
		return IRIO_warning;
	}else{
		return IRIO_success;
	}
}

int irio_getDMATtoHostOverflow(irioDrv_t* p_DrvPvt,int32_t* value, TStatus* status){
	TIRIOStatusCode local_status = IRIO_success;

	if(p_DrvPvt->enumDMATtoHOSTOverflows.found){
		NiFpga_Status fpgaStatus=NiFpga_Status_Success;

		uint16_t aux=0;
		NiFpga_MergeStatus(&fpgaStatus,NiFpga_ReadU16(p_DrvPvt->session,p_DrvPvt->enumDMATtoHOSTOverflows.value,&aux));
		if(NiFpga_IsError(fpgaStatus)){
			irio_mergeStatus(status,Read_NIRIO_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING FPGA Error reading %s. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_DMATTOHOSTOVERFLOWS,fpgaStatus);
			local_status |= IRIO_warning;
		}else{
			*value=(int32_t)aux;
		}
	}else{
		irio_mergeStatus(status,Read_Resource_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING %s was not found.\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_DMATTOHOSTOVERFLOWS);
		local_status |= IRIO_warning;
	}

	if(local_status<IRIO_error){
		return local_status;
	}else{
		return IRIO_error;
	}
}

int irio_getDMATtoHostSamplingRate(irioDrv_t* p_DrvPvt, int n, int32_t *value, TStatus* status){
	
	if(!p_DrvPvt->DMATtoHOSTNo.found){
		irio_mergeStatus(status,Read_Resource_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING Can not use DMAs. DMAs were not searched or found.\n",__func__,__LINE__,p_DrvPvt->appCallID);
		return IRIO_warning;
	}
	
	TIRIOStatusCode local_status = IRIO_success;
	if(n>=0 && n<p_DrvPvt->DMATtoHOSTNo.value && p_DrvPvt->enumDMATtoHOSTSamplingRate[n].found){
		uint16_t aux=0;
		NiFpga_Status fpgaStatus =NiFpga_Status_Success;
		NiFpga_MergeStatus(&fpgaStatus,NiFpga_ReadU16(p_DrvPvt->session,p_DrvPvt->enumDMATtoHOSTSamplingRate[n].value,&aux));
		if(NiFpga_IsError(fpgaStatus)){
			irio_mergeStatus(status,Read_NIRIO_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING FPGA Error reading %s%d. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_DMATTOHOSTSAMPLINGRATE,n,fpgaStatus);
			local_status |= IRIO_warning;
		}else{
			*value=(int32_t)aux;
		}
	}else{
		irio_mergeStatus(status,Read_Resource_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING %s%d was not found.\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_DMATTOHOSTSAMPLINGRATE,n);
		local_status |= IRIO_warning;
	}

	if(local_status<IRIO_error){
		return local_status;
	}else{
		return IRIO_error;
	}
}

int irio_setDMATtoHostSamplingRate(irioDrv_t* p_DrvPvt,int n, int32_t value, TStatus* status){

	if(!p_DrvPvt->DMATtoHOSTNo.found){
		irio_mergeStatus(status,Write_Resource_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING Can not use DMAs. DMAs were not searched or found.\n",__func__,__LINE__,p_DrvPvt->appCallID);
		return IRIO_warning;
	}

	TIRIOStatusCode local_status = IRIO_success;
	if(n>=0 && n<p_DrvPvt->DMATtoHOSTNo.value && p_DrvPvt->enumDMATtoHOSTSamplingRate[n].found){
		NiFpga_Status fpgaStatus =NiFpga_Status_Success;
		int16_t aux = (int16_t)value;
		NiFpga_MergeStatus(&fpgaStatus,NiFpga_WriteU16(p_DrvPvt->session,p_DrvPvt->enumDMATtoHOSTSamplingRate[n].value,aux));
		if(NiFpga_IsError(fpgaStatus)){
			irio_mergeStatus(status,Write_NIRIO_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING FPGA Error writing %s%d. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_DMATTOHOSTSAMPLINGRATE,n,fpgaStatus);
			local_status |= IRIO_warning;
		}
	}else{
		irio_mergeStatus(status,Write_Resource_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING %s%d was not found.\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_DMATTOHOSTSAMPLINGRATE,n);
		local_status |= IRIO_warning;
	}

	if(local_status<IRIO_error){
		return local_status;
	}else{
		return IRIO_error;
	}
}

int irio_getDMATtoHostEnable(irioDrv_t* p_DrvPvt,int n,int32_t  *value,TStatus* status){

	if(!p_DrvPvt->DMATtoHOSTNo.found){
		irio_mergeStatus(status,Read_Resource_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING Can not use DMAs. DMAs were not searched or found.\n",__func__,__LINE__,p_DrvPvt->appCallID);
		return IRIO_warning;
	}

	TIRIOStatusCode local_status = IRIO_success;
	if(n>=0 && n<p_DrvPvt->DMATtoHOSTNo.value && p_DrvPvt->enumDMATtoHOSTEnable[n].found){
		NiFpga_Status fpgaStatus =NiFpga_Status_Success;
		NiFpga_Bool aux=NiFpga_True;
		NiFpga_MergeStatus(&fpgaStatus,NiFpga_ReadBool(p_DrvPvt->session,p_DrvPvt->enumDMATtoHOSTEnable[n].value,&aux));
		if(NiFpga_IsError(fpgaStatus)){
			irio_mergeStatus(status,Read_NIRIO_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING FPGA Error reading %s%d. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_DMATTOHOSTENABLE,n,fpgaStatus);
			local_status |= IRIO_warning;
		}else{
			*value=(int32_t)aux;
		}
	}else{
		irio_mergeStatus(status,Read_Resource_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING %s%d was not found.\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_DMATTOHOSTENABLE,n);
		local_status |= IRIO_warning;
	}

	if(local_status<IRIO_error){
		return local_status;
	}else{
		return IRIO_error;
	}
}

int irio_setDMATtoHostEnable(irioDrv_t* p_DrvPvt,int n,int32_t value, TStatus* status){

	if(!p_DrvPvt->DMATtoHOSTNo.found){
		irio_mergeStatus(status,Write_Resource_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING Can not use DMAs. DMAs were not searched or found.\n",__func__,__LINE__,p_DrvPvt->appCallID);
		return IRIO_warning;
	}

	TIRIOStatusCode local_status = IRIO_success;
	if(n>=0 && n<p_DrvPvt->DMATtoHOSTNo.value && p_DrvPvt->enumDMATtoHOSTEnable[n].found){
		NiFpga_Status fpgaStatus = NiFpga_Status_Success;
		if(value==0){
			NiFpga_MergeStatus(&fpgaStatus,NiFpga_WriteBool(p_DrvPvt->session,p_DrvPvt->enumDMATtoHOSTEnable[n].value,NiFpga_False));
		}else{
			NiFpga_MergeStatus(&fpgaStatus,NiFpga_WriteBool(p_DrvPvt->session,p_DrvPvt->enumDMATtoHOSTEnable[n].value,NiFpga_True));
		}
		if(NiFpga_IsError(fpgaStatus)){
			irio_mergeStatus(status,Write_NIRIO_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING FPGA Error writing %s%d. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_DMATTOHOSTENABLE,n,fpgaStatus);
			local_status |= IRIO_warning;
		}
	}else{
		irio_mergeStatus(status,Write_Resource_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING %s%d was not found.\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_DMATTOHOSTENABLE,n);
		local_status |= IRIO_warning;
	}

	if(local_status<IRIO_error){
		return local_status;
	}else{
		return IRIO_error;
	}
}

int irio_getDMATtoHostData(irioDrv_t* p_DrvPvt, int NBlocks, int n, uint64_t *data, int* elementsRead, TStatus* status){

	if(!p_DrvPvt->DMATtoHOSTNo.found){
		irio_mergeStatus(status,Read_Resource_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING Can not use DMAs. DMAs were not searched or found.\n",__func__,__LINE__,p_DrvPvt->appCallID);
		return IRIO_warning;
	}

	TIRIOStatusCode local_status = IRIO_success;
	if(n>=0 && n<p_DrvPvt->DMATtoHOSTNo.value && p_DrvPvt->enumDMATtoHOST[n].found){
		NiFpga_Status fpgaStatus = NiFpga_Status_Success;
		size_t elementsToRead=0;

		//If irioasyn driver is used iriolib:
		//From DMATtoHOSTFrameType=0 to DMATtoHOSTFrameType=127 data conversion factor used is: I/O Module conversion factor.
		//From DMATtoHOSTFrameType=128 to DMATtoHOSTFrameType=255 data conversion factor used is: user defined conversion factor.
		switch(p_DrvPvt->DMATtoHOSTFrameType[n]){
		case 0:
			elementsToRead=NBlocks*p_DrvPvt->DMATtoHOSTBlockNWords[n];
			break;
		case 1:
			elementsToRead=NBlocks*(p_DrvPvt->DMATtoHOSTBlockNWords[n]+2); //each DMA data block includes two extra U64 words to include timestamp
			break;
		case 128:
			elementsToRead=NBlocks*(p_DrvPvt->DMATtoHOSTBlockNWords[n]);
			break;
		case 129:
			elementsToRead=NBlocks*(p_DrvPvt->DMATtoHOSTBlockNWords[n]+2); //each DMA data block includes two extra U64 words to include timestamp
			break;
		default:
			elementsToRead=NBlocks*(p_DrvPvt->DMATtoHOSTBlockNWords[n]);
			break;
		}

		size_t elementsRemaining=0;
		*elementsRead=0;
		NiFpga_MergeStatus(&fpgaStatus,NiFpga_ReadFifoU64(p_DrvPvt->session,p_DrvPvt->enumDMATtoHOST[n].value,data,0,0,&elementsRemaining));


		if (elementsRemaining>=elementsToRead){
			int attempts=0;
			do{ //[BUG7807] elementRemaining indicates that samples greater than elementsToRead are in the buffer but next call can fail.
				NiFpga_MergeStatus(&fpgaStatus,NiFpga_ReadFifoU64(p_DrvPvt->session,
								  p_DrvPvt->enumDMATtoHOST[n].value,
								  data,
								  elementsToRead,
								  //FPGA_READ_BUFFER_TIMEOUT_1ms,
								  5000, //This timeout should not be necessary
								  &elementsRemaining));
				if(NiFpga_IsError(fpgaStatus)){
					irio_mergeStatus(status,Read_NIRIO_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING FPGA Error reading %s%d. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_DMATTOHOST,n,fpgaStatus);
					printf("\n[BUG7807] DMATtoHOSTBlockNWords:%d elementsToRead=%d elementsRemaining=%d\n",p_DrvPvt->DMATtoHOSTBlockNWords[n], (int) elementsToRead, (int)elementsRemaining);
					attempts++;
					printf("[BUG7807] Attempt #N=%d\n", attempts);
					if (attempts==3) {
						local_status |= IRIO_warning;
						printf("[BUG7807] Exiting with error\n");
					}else
					{
						fpgaStatus = NiFpga_Status_Success;
						printf("[BUG7807]Attempting...\n");
					}
				}else{
					*elementsRead=NBlocks;
					attempts=3;
					}
			}while (attempts<3);
		  }
	}else{
		irio_mergeStatus(status,Read_Resource_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING %s%d was not found.\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_DMATTOHOST,n);
		local_status |= IRIO_warning;
	}

	if(local_status<IRIO_error){
		return local_status;
	}else{
		return IRIO_error;
	}
}

int irio_getDMATtoHostImage(irioDrv_t* p_DrvPvt, int imageSize, int n, uint64_t *data, int* elementsRead, TStatus* status){

	if(!p_DrvPvt->DMATtoHOSTNo.found){
		irio_mergeStatus(status,Read_Resource_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING Can not use DMAs. DMAs were not searched or found.\n",__func__,__LINE__,p_DrvPvt->appCallID);
		return IRIO_warning;
	}

	TIRIOStatusCode local_status = IRIO_success;
	if(n>=0 && n<p_DrvPvt->DMATtoHOSTNo.value && p_DrvPvt->enumDMATtoHOST[n].found){
		NiFpga_Status fpgaStatus = NiFpga_Status_Success;
		size_t elementsToRead=0;
		elementsToRead=imageSize*p_DrvPvt->DMATtoHOSTSampleSize[n]/8;
		size_t elementsRemaining=0;
		*elementsRead=0;
		NiFpga_MergeStatus(&fpgaStatus,NiFpga_ReadFifoU64(p_DrvPvt->session,p_DrvPvt->enumDMATtoHOST[n].value,data,0,0,&elementsRemaining));
		if (elementsRemaining>=elementsToRead){
			NiFpga_MergeStatus(&fpgaStatus,NiFpga_ReadFifoU64(p_DrvPvt->session,
								  p_DrvPvt->enumDMATtoHOST[n].value,
								  data,
								  elementsToRead,
								  0,
								  &elementsRemaining));
			if(NiFpga_IsError(fpgaStatus)){
				irio_mergeStatus(status,Read_NIRIO_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING FPGA Error reading %s%d. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_DMATTOHOST,n,fpgaStatus);
				local_status |= IRIO_warning;
			}else{
				*elementsRead=imageSize;


			}
		}
	}else{
		irio_mergeStatus(status,Read_Resource_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING %s%d was not found.\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_DMATTOHOST,n);
		local_status |= IRIO_warning;
	}

	if(local_status<IRIO_error){
		return local_status;
	}else{
		return IRIO_error;
	}

}

