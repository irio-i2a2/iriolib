/**************************************************************************//**
 * \file irioHandlerDMAGPU.c
 * \authors Mariano Ruiz (Universidad Politécnica de Madrid, UPM)
 * \authors Diego Sanz (Universidad Politécnica de Madrid, UPM)
 * \authors Sergio Esquembri (Universidad Politécnica de Madrid, UPM)
 * \authors Enrique Bernal (Universidad Politécnica de Madrid, UPM)
 * \authors Alvaro Bustos (Universidad Politécnica de Madrid, UPM)
 * \brief GPU DMA handler methods for IRIO driver
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

#include "irioHandlerDMAGPU.h"
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
#define STRINGNAME_DMATTOGPUNCH "_IndicatorArrayU16_DMATtoGPUNCh"
#define STRINGNAME_DMATTOGPUNCHSIZE "_IndicatorArrayU16Size_DMATtoGPUNCh"
#define STRINGNAME_DMATTOGPUOVERFLOWS "_IndicatorU16_DMATtoGPUOverflows"
#define STRINGNAME_DMATTOGPU "_TargetToHostFifoU64_DMATtoGPU"
#define STRINGNAME_DMATTOGPUENABLE "_ControlBool_DMATtoGPUEnable"
#define STRINGNAME_DMATTOGPUSAMPLINGRATE  "_ControlU16_DMATtoGPUSamplingRate"
#define STRINGNAME_DMATTOGPUFRAMETYPE "_IndicatorArrayU8_DMATtoGPUFrameType"
#define STRINGNAME_DMATTOGPUSAMPLESIZE "_IndicatorArrayU8_DMATtoGPUSampleSize"
#define STRINGNAME_DMATTOGPUBLOCKNWORDS "_IndicatorArrayU16_DMATtoGPUBlockNWords"
///@}

int irio_findDAQDMAsGPU(irioDrv_t* p_DrvPvt,TStatus* status){

	TIRIOStatusCode local_status = IRIO_success;
	NiFpga_Status fpgaStatus = NiFpga_Status_Success;
	//Search for NCHperDMATtoGPU and size
	//Search for FrameTypeDMATtoGPU and size
	//Search for Bytes
	local_status |= irio_findResourceEnum(p_DrvPvt,STRINGNAME_DMATTOGPUNCH,-1,&p_DrvPvt->enumDMATtoGPUNCh,status,p_DrvPvt->verbosity);
	local_status |= irio_findResourceEnum(p_DrvPvt,STRINGNAME_DMATTOGPUNCHSIZE,-1,&p_DrvPvt->DMATtoGPUNo,status,p_DrvPvt->verbosity);
	local_status |= irio_findResourceEnum(p_DrvPvt,STRINGNAME_DMATTOGPUOVERFLOWS,-1,&p_DrvPvt->enumDMATtoGPUOverflows,status,p_DrvPvt->verbosity);
	local_status |= irio_findResourceEnum(p_DrvPvt,STRINGNAME_DMATTOGPUFRAMETYPE,-1,&p_DrvPvt->enumDMATtoGPUFrameType,status,p_DrvPvt->verbosity);
	local_status |= irio_findResourceEnum(p_DrvPvt,STRINGNAME_DMATTOGPUSAMPLESIZE,-1,&p_DrvPvt->enumDMATtoGPUSampleSize,status,p_DrvPvt->verbosity);
	local_status |= irio_findResourceEnum(p_DrvPvt,STRINGNAME_DMATTOGPUBLOCKNWORDS,-1,&p_DrvPvt->enumDMATtoGPUBlockNWords,status,p_DrvPvt->verbosity);

	//reading the array NCHperDMATtoGPU
	if(p_DrvPvt->enumDMATtoGPUNCh.found && p_DrvPvt->DMATtoGPUNo.found){ 
		NiFpga_MergeStatus(&fpgaStatus,NiFpga_ReadArrayU16(p_DrvPvt->session,p_DrvPvt->enumDMATtoGPUNCh.value,p_DrvPvt->DMATtoGPUNCh,p_DrvPvt->DMATtoGPUNo.value));
		if(NiFpga_IsError(fpgaStatus)){
			irio_mergeStatus(status,NIRIO_API_Error,p_DrvPvt->verbosity,"[%s,%d]-(%s) ERROR FPGA Error reading %s. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_DMATTOGPUNCH,fpgaStatus);
			local_status |= IRIO_error;
			fpgaStatus = NiFpga_Status_Success;
		}
	}

	//reading the array FrameTypeDMATtoGPU
	if(p_DrvPvt->enumDMATtoGPUFrameType.found && p_DrvPvt->DMATtoGPUNo.found){
		//we are assuming that the size of this array is the same that the size for the array with number of channels.
		NiFpga_MergeStatus(&fpgaStatus,NiFpga_ReadArrayU8(p_DrvPvt->session,p_DrvPvt->enumDMATtoGPUFrameType.value,p_DrvPvt->DMATtoGPUFrameType,p_DrvPvt->DMATtoGPUNo.value));
		if(NiFpga_IsError(fpgaStatus)){
			irio_mergeStatus(status,NIRIO_API_Error,p_DrvPvt->verbosity,"[%s,%d]-(%s) ERROR FPGA Error reading %s. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_DMATTOGPUFRAMETYPE,fpgaStatus);
			local_status |= IRIO_error;
			fpgaStatus = NiFpga_Status_Success;
		}
	}

	//reading the array BytesperSampleChannelDMATtoGPU
	if(p_DrvPvt->enumDMATtoGPUSampleSize.found && p_DrvPvt->DMATtoGPUNo.found){
		//we are assuming that the size of this array is the same that the size for the array with number of channels.
		NiFpga_MergeStatus(&fpgaStatus,NiFpga_ReadArrayU8(p_DrvPvt->session,p_DrvPvt->enumDMATtoGPUSampleSize.value,p_DrvPvt->DMATtoGPUSampleSize,p_DrvPvt->DMATtoGPUNo.value));
		if(NiFpga_IsError(fpgaStatus)){
			irio_mergeStatus(status,NIRIO_API_Error,p_DrvPvt->verbosity,"[%s,%d]-(%s) ERROR FPGA Error reading %s. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_DMATTOGPUSAMPLESIZE,fpgaStatus);
			local_status |= IRIO_error;
			fpgaStatus = NiFpga_Status_Success;
		}
	}

	//reading the array BlockNWordsDMATtoGPU
	if(p_DrvPvt->enumDMATtoGPUBlockNWords.found && p_DrvPvt->DMATtoGPUNo.found){
		//we are assuming that the size of this array is the same that the size for the array with number of channels.
		NiFpga_MergeStatus(&fpgaStatus,NiFpga_ReadArrayU16(p_DrvPvt->session,p_DrvPvt->enumDMATtoGPUBlockNWords.value,p_DrvPvt->DMATtoGPUBlockNWords,p_DrvPvt->DMATtoGPUNo.value));
		if(NiFpga_IsError(fpgaStatus)){
			irio_mergeStatus(status,NIRIO_API_Error,p_DrvPvt->verbosity,"[%s,%d]-(%s) ERROR FPGA Error reading %s. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_DMATTOGPUBLOCKNWORDS,fpgaStatus);
			local_status |= IRIO_error;
			fpgaStatus = NiFpga_Status_Success;
		}
	}



	//Search DMAs
	if(p_DrvPvt->enumDMATtoGPUNCh.found && p_DrvPvt->DMATtoGPUNo.found){

		if(p_DrvPvt->verbosity){
			printf("[%s,%d]-(%s) TRACE GPU DAQ DMAs found (Searching %d):",__func__,__LINE__,p_DrvPvt->appCallID,p_DrvPvt->DMATtoGPUNo.value);
		}

		//Search NumberOfDmas DMAs
		TStatus dma_status,accum_status;
		irio_initStatus(&dma_status);
		irio_initStatus(&accum_status);
		int i=0,accum=0;
		int count=0;
		while(i<p_DrvPvt->DMATtoGPUNo.value){
			//Find mandatory DMA ports
			irio_findResourceEnum(p_DrvPvt,STRINGNAME_DMATTOGPU,i,&p_DrvPvt->enumDMATtoGPU[i],&dma_status,0);
			irio_findResourceEnum(p_DrvPvt,STRINGNAME_DMATTOGPUSAMPLINGRATE,i,&p_DrvPvt->enumDMATtoGPUSamplingRate[i],&dma_status,0);
			irio_findResourceEnum(p_DrvPvt,STRINGNAME_DMATTOGPUENABLE,i,&p_DrvPvt->enumDMATtoGPUEnable[i],&dma_status,0);

			if(dma_status.code==IRIO_success){
				//Index the correlation of channels found
				p_DrvPvt->DMATtoGPUChIndex[i]=accum;
				accum+=p_DrvPvt->DMATtoGPUNCh[i];
				if(p_DrvPvt->verbosity){
					printf("DMA%d, ",i);
				}
				count++;
			}else{
				irio_mergeStatus(&accum_status,ResourceNotFound_Error,0,"[%s,%d]-(%s) ERROR Searching GPU DMA%d resources:\n%s",__func__,__LINE__,p_DrvPvt->appCallID,i,dma_status.msg);
				local_status |= IRIO_error;
				irio_resetStatus(&dma_status);
				p_DrvPvt->enumDMATtoGPU[i].found=0;
				p_DrvPvt->enumDMATtoGPUSamplingRate[i].found=0;
				p_DrvPvt->enumDMATtoGPUEnable[i].found=0;
			}
			i++;
		}

		if(accum_status.code!=IRIO_success){
			irio_mergeStatus(status,accum_status.detailCode,p_DrvPvt->verbosity,"%s",accum_status.msg);
			irio_resetStatus(&accum_status);
		}

		if(p_DrvPvt->verbosity){
			printf("\n");
			printf("[%s,%d]-(%s) TRACE TOTAL GPU DAQ DMAs found: %d/%d\n",__func__,__LINE__,p_DrvPvt->appCallID,count,p_DrvPvt->DMATtoGPUNo.value);
		}
	}

	if(local_status<IRIO_error){
		return local_status;
	}else{
		return IRIO_error;
	}
}

int irio_findIMAQDMAsGPU(irioDrv_t* p_DrvPvt, TStatus* status){

	NiFpga_Status fpgaStatus = NiFpga_Status_Success;
	TIRIOStatusCode local_status = IRIO_success;

	//Search for NCHperDMATtoGPU size
	local_status |= irio_findResourceEnum(p_DrvPvt,STRINGNAME_DMATTOGPUNCHSIZE,-1,&p_DrvPvt->DMATtoGPUNo,status,p_DrvPvt->verbosity);
	local_status |= irio_findResourceEnum(p_DrvPvt,STRINGNAME_DMATTOGPUOVERFLOWS,-1,&p_DrvPvt->enumDMATtoGPUOverflows,status,p_DrvPvt->verbosity);
	local_status |= irio_findResourceEnum(p_DrvPvt,STRINGNAME_DMATTOGPUFRAMETYPE,-1,&p_DrvPvt->enumDMATtoGPUFrameType,status,p_DrvPvt->verbosity);
	local_status |= irio_findResourceEnum(p_DrvPvt,STRINGNAME_DMATTOGPUSAMPLESIZE,-1,&p_DrvPvt->enumDMATtoGPUSampleSize,status,p_DrvPvt->verbosity);

	//reading the array FrameTypeDMATtoGPU
	if(p_DrvPvt->enumDMATtoGPUFrameType.found && p_DrvPvt->DMATtoGPUNo.found){
		//we are assuming that the size of this array is the same that the size for the array with number of channels.
		NiFpga_MergeStatus(&fpgaStatus,NiFpga_ReadArrayU8(p_DrvPvt->session,p_DrvPvt->enumDMATtoGPUFrameType.value,p_DrvPvt->DMATtoGPUFrameType,p_DrvPvt->DMATtoGPUNo.value));
		if(NiFpga_IsError(fpgaStatus)){
			irio_mergeStatus(status,NIRIO_API_Error,p_DrvPvt->verbosity,"[%s,%d]-(%s) ERROR FPGA Error reading %s. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_DMATTOGPUFRAMETYPE,fpgaStatus);
			local_status |= IRIO_error;
			fpgaStatus = NiFpga_Status_Success;
		}
	}

	//reading the array BytesperSampleChannelDMATtoGPU
	if(p_DrvPvt->enumDMATtoGPUSampleSize.found && p_DrvPvt->DMATtoGPUNo.found){
		//we are assuming that the size of this array is the same that the size for the array with number of channels.
		NiFpga_MergeStatus(&fpgaStatus,NiFpga_ReadArrayU8(p_DrvPvt->session,p_DrvPvt->enumDMATtoGPUSampleSize.value,p_DrvPvt->DMATtoGPUSampleSize,p_DrvPvt->DMATtoGPUNo.value));
		if(NiFpga_IsError(fpgaStatus)){
			irio_mergeStatus(status,NIRIO_API_Error,p_DrvPvt->verbosity,"[%s,%d]-(%s) ERROR FPGA Error reading %s. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_DMATTOGPUSAMPLESIZE,fpgaStatus);
			local_status |= IRIO_error;
			fpgaStatus = NiFpga_Status_Success;
		}
	}

	//Search DMAs
	if(p_DrvPvt->DMATtoGPUNo.found){

		if(p_DrvPvt->verbosity){
			printf("[%s,%d]-(%s) TRACE GPU IMAQ DMAs found (Searching %d):",__func__,__LINE__,p_DrvPvt->appCallID,p_DrvPvt->DMATtoGPUNo.value);
		}

		//Search NumberOfDmas DMAs
		int i=0;
		int count=0;
		TStatus dma_status,accum_status;
		irio_initStatus(&dma_status);
		irio_initStatus(&accum_status);
		while(i<p_DrvPvt->DMATtoGPUNo.value){
			//Find mandatory DMA ports
			irio_findResourceEnum(p_DrvPvt,STRINGNAME_DMATTOGPU,i,&p_DrvPvt->enumDMATtoGPU[i],&dma_status,0);
			irio_findResourceEnum(p_DrvPvt,STRINGNAME_DMATTOGPUENABLE,i,&p_DrvPvt->enumDMATtoGPUEnable[i],&dma_status,0);
			if(dma_status.code==IRIO_success){
				//Index the correlation of channels found
				p_DrvPvt->DMATtoGPUChIndex[i]=i;
				if(p_DrvPvt->verbosity){
					printf("DMA%d, ",i);
				}
				count++;
			}else{
				irio_mergeStatus(&accum_status,ResourceNotFound_Error,0,"[%s,%d]-(%s) ERROR Searching GPU DMA%d resources:\n%s",__func__,__LINE__,p_DrvPvt->appCallID,i,dma_status.msg);
				irio_resetStatus(&dma_status);

				local_status |= IRIO_error;

				p_DrvPvt->enumDMATtoGPU[i].found=0;
				p_DrvPvt->enumDMATtoGPUEnable[i].found=0;
			}
			i++;
		}

		if(accum_status.code!=IRIO_success){
			irio_mergeStatus(status,accum_status.detailCode,p_DrvPvt->verbosity,"%s",accum_status.msg);
			irio_resetStatus(&accum_status);
		}

		if(p_DrvPvt->verbosity){
			printf("\n");
			printf("[%s,%d]-(%s) TRACE TOTAL GPU IMAQ DMAs found: %d/%d\n",__func__,__LINE__,p_DrvPvt->appCallID,count,p_DrvPvt->DMATtoGPUNo.value);
		}
	}

	if(local_status<IRIO_error){
		return local_status;
	}else{
		return IRIO_error;
	}
}

int irio_setUpDMAsTtoGPU(irioDrv_t* p_DrvPvt,int depth, TStatus* status){

	if(!p_DrvPvt->DMATtoGPUNo.found){
		irio_mergeStatus(status,Read_Resource_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING Can not set up GPU DMAs. DMAs were not searched or found.\n",__func__,__LINE__,p_DrvPvt->appCallID);
		return IRIO_warning;
	}

	int i;
	TIRIOStatusCode local_status = IRIO_success;
	NiFpga_Status fpgaStatus=NiFpga_Status_Success;
	//Configure and start FIFOs
	for (i=0; i<p_DrvPvt->DMATtoGPUNo.value; i++){
		if(!p_DrvPvt->enumDMATtoGPU[i].found){
			irio_mergeStatus(status,Read_Resource_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING Can not set up GPU DMA%d. DMA was not found. DMAs were not searched or found.\n",__func__,__LINE__,p_DrvPvt->appCallID,i);
			local_status |= IRIO_warning;
		}else{
			//printf("Pointer");
			NiFpga_MergeStatus(&fpgaStatus,
			NiFpgaEx_ConfigureFifoGpu(p_DrvPvt->session,p_DrvPvt->enumDMATtoGPU[i].value,depth,(p_DrvPvt->gpu_buffer)));
			NiFpga_MergeStatus(&fpgaStatus,
				NiFpga_StartFifo(p_DrvPvt->session,p_DrvPvt->enumDMATtoGPU[i].value));
			if(NiFpga_IsError(fpgaStatus)){
				irio_mergeStatus(status,ConfigDMA_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING FPGA Error setting up GPU DMA%d. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,i,fpgaStatus);
				local_status |= IRIO_warning;
				fpgaStatus=NiFpga_Status_Success;
			}
		}
	}

	//Not necessary to stop DAQ, is already stopped from initialTargetCheck()

	//Clean DMAs from trash data
	local_status |= irio_cleanDMAsTtoGPU(p_DrvPvt,status); 

	if(local_status<IRIO_error){
		return local_status;
	}else{
		return IRIO_error;
	}
}

int irio_closeDMAsTtoGPU(irioDrv_t* p_DrvPvt, TStatus* status){

	if(!p_DrvPvt->DMATtoGPUNo.found){
		irio_mergeStatus(status,Read_Resource_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING Can not close GPU DMAs. DMAs were not searched or found.\n",__func__,__LINE__,p_DrvPvt->appCallID);
		return IRIO_warning;
	}

	NiFpga_Status fpgaStatus = NiFpga_Status_Success;
	TIRIOStatusCode local_status = IRIO_success;
	int i=0;
	for(i=0;i<p_DrvPvt->DMATtoGPUNo.value;i++){
		if(!p_DrvPvt->enumDMATtoGPU[i].found){
			irio_mergeStatus(status,Read_Resource_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING Can not close GPU DMA%d. DMA was not found.\n",__func__,__LINE__,p_DrvPvt->appCallID,i);
			local_status |= IRIO_warning;
		}else{
			NiFpga_MergeStatus(&fpgaStatus,NiFpga_StopFifo(p_DrvPvt->session,p_DrvPvt->enumDMATtoGPU[i].value));
			if(NiFpga_IsError(fpgaStatus)){
				irio_mergeStatus(status,ConfigDMA_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING FPGA Error closing GPU DMA%d. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,i,fpgaStatus);
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

int irio_cleanDMAsTtoGPU(irioDrv_t* p_DrvPvt, TStatus* status){

	if(!p_DrvPvt->DMATtoGPUNo.found){
		irio_mergeStatus(status,Read_Resource_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING Can not clean GPU DMAs. DMAs were not searched or found.\n",__func__,__LINE__,p_DrvPvt->appCallID);
		return IRIO_warning;
	}

	int i;
	TIRIOStatusCode local_status = IRIO_success;
	uint64_t** cleanbuffer=calloc(20000,sizeof(uint64_t));
	for (i=0; i<p_DrvPvt->DMATtoGPUNo.value; i++){
		local_status |= irio_cleanDMATtoGPU(p_DrvPvt,i,cleanbuffer,20000,status);
	}
	free(cleanbuffer);

	if(local_status<IRIO_error){
		return local_status;
	}else{
		return IRIO_error;
	}
}

int irio_cleanDMATtoGPU(irioDrv_t* p_DrvPvt, int n,uint64_t** cleanbuffer, size_t buffersize,TStatus* status){

	if(!p_DrvPvt->enumDMATtoGPU[n].found){
		irio_mergeStatus(status,Read_Resource_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) ERROR Can not close GPU DMA%d. DMA was not found. .\n",__func__,__LINE__,p_DrvPvt->appCallID,n);
		return IRIO_warning;
	}

	NiFpga_Status fpgaStatus = NiFpga_Status_Success;
	size_t toRead,acq, last;
	NiFpga_MergeStatus(&fpgaStatus,
			NiFpga_AcquireFifoReadElementsU64(p_DrvPvt->session,p_DrvPvt->enumDMATtoGPU[n].value,
			  cleanbuffer,0,0,&acq,&last));
	NiFpga_MergeStatus(&fpgaStatus,NiFpga_ReleaseFifoElements(p_DrvPvt->session,p_DrvPvt->enumDMATtoGPU[n].value,acq));

	while(last>0 && NiFpga_IsNotError(fpgaStatus)){
		if(last>buffersize){
			toRead=buffersize;
		}else{
			toRead=last;
		}
		NiFpga_MergeStatus(&fpgaStatus,
				NiFpga_AcquireFifoReadElementsU64(p_DrvPvt->session, p_DrvPvt->enumDMATtoGPU[n].value,
				  cleanbuffer,toRead,10,&acq,&last));
		NiFpga_MergeStatus(&fpgaStatus,NiFpga_ReleaseFifoElements(p_DrvPvt->session,p_DrvPvt->enumDMATtoGPU[n].value,acq));
	}

	if(NiFpga_IsError(fpgaStatus)){
		irio_mergeStatus(status,Read_NIRIO_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING FPGA Error cleaning GPU DMA%d. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,n,fpgaStatus);
		return IRIO_warning;
	}else{
		return IRIO_success;
	}
}

int irio_getDMATtoGPUOverflow(irioDrv_t* p_DrvPvt,int32_t* value, TStatus* status){
	TIRIOStatusCode local_status = IRIO_success;

	if(p_DrvPvt->enumDMATtoGPUOverflows.found){
		NiFpga_Status fpgaStatus=NiFpga_Status_Success;
		uint16_t aux=0;
		NiFpga_MergeStatus(&fpgaStatus,NiFpga_ReadU16(p_DrvPvt->session,p_DrvPvt->enumDMATtoGPUOverflows.value,&aux));
		if(NiFpga_IsError(fpgaStatus)){
			irio_mergeStatus(status,Read_NIRIO_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING FPGA Error reading %s. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_DMATTOGPUOVERFLOWS,fpgaStatus);
			local_status |= IRIO_warning;
		}else{
			*value=(int32_t)aux;
		}
	}else{
		irio_mergeStatus(status,Read_Resource_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING %s was not found.\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_DMATTOGPUOVERFLOWS);
		local_status |= IRIO_warning;
	}

	if(local_status<IRIO_error){
		return local_status;
	}else{
		return IRIO_error;
	}
}

int irio_getDMATtoGPUSamplingRate(irioDrv_t* p_DrvPvt, int n, int32_t *value, TStatus* status){

	if(!p_DrvPvt->DMATtoGPUNo.found){
		irio_mergeStatus(status,Read_Resource_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING Can not use GPU DMAs. DMAs were not searched or found.\n",__func__,__LINE__,p_DrvPvt->appCallID);
		return IRIO_warning;
	}

	TIRIOStatusCode local_status = IRIO_success;
	if(n>=0 && n<p_DrvPvt->DMATtoGPUNo.value && p_DrvPvt->enumDMATtoGPUSamplingRate[n].found){
		uint16_t aux=0;
		NiFpga_Status fpgaStatus =NiFpga_Status_Success;
		NiFpga_MergeStatus(&fpgaStatus,NiFpga_ReadU16(p_DrvPvt->session,p_DrvPvt->enumDMATtoGPUSamplingRate[n].value,&aux));
		if(NiFpga_IsError(fpgaStatus)){
			irio_mergeStatus(status,Read_NIRIO_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING FPGA Error reading %s%d. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_DMATTOGPUSAMPLINGRATE,n,fpgaStatus);
			local_status |= IRIO_warning;
		}else{
			*value=(int32_t)aux;
		}
	}else{
		irio_mergeStatus(status,Read_Resource_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING %s%d was not found.\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_DMATTOGPUSAMPLINGRATE,n);
		local_status |= IRIO_warning;
	}

	if(local_status<IRIO_error){
		return local_status;
	}else{
		return IRIO_error;
	}
}

int irio_setDMATtoGPUSamplingRate(irioDrv_t* p_DrvPvt,int n, int32_t value,TStatus* status){

	if(!p_DrvPvt->DMATtoGPUNo.found){
		irio_mergeStatus(status,Write_Resource_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING Can not use GPU DMAs. DMAs were not searched or found.\n",__func__,__LINE__,p_DrvPvt->appCallID);
		return IRIO_warning;
	}

	TIRIOStatusCode local_status = IRIO_success;
	if(n>=0 && n<p_DrvPvt->DMATtoGPUNo.value && p_DrvPvt->enumDMATtoGPUSamplingRate[n].found){
		NiFpga_Status fpgaStatus =NiFpga_Status_Success;
		int16_t aux = (int16_t)value;
		NiFpga_MergeStatus(&fpgaStatus,NiFpga_WriteU16(p_DrvPvt->session,p_DrvPvt->enumDMATtoGPUSamplingRate[n].value,aux));
		if(NiFpga_IsError(fpgaStatus)){
			irio_mergeStatus(status,Write_NIRIO_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING FPGA Error writing %s%d. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_DMATTOGPUSAMPLINGRATE,n,fpgaStatus);
			local_status |= IRIO_warning;
		}
	}else{
		irio_mergeStatus(status,Write_Resource_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING %s%d was not found.\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_DMATTOGPUSAMPLINGRATE,n);
		local_status |= IRIO_warning;
	}

	if(local_status<IRIO_error){
		return local_status;
	}else{
		return IRIO_error;
	}
}

int irio_getDMATtoGPUEnable(irioDrv_t* p_DrvPvt,int n,int32_t  *value, TStatus* status){

	if(!p_DrvPvt->DMATtoGPUNo.found){
		irio_mergeStatus(status,Read_Resource_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING Can not use GPU DMAs. DMAs were not searched or found.\n",__func__,__LINE__,p_DrvPvt->appCallID);
		return IRIO_warning;
	}

	TIRIOStatusCode local_status = IRIO_success;
	if(n>=0 && n<p_DrvPvt->DMATtoGPUNo.value && p_DrvPvt->enumDMATtoGPUEnable[n].found){
		NiFpga_Status fpgaStatus =NiFpga_Status_Success;
		NiFpga_Bool aux=NiFpga_True;
		NiFpga_MergeStatus(&fpgaStatus,NiFpga_ReadBool(p_DrvPvt->session,p_DrvPvt->enumDMATtoGPUEnable[n].value,&aux));
		if(NiFpga_IsError(fpgaStatus)){
			irio_mergeStatus(status,Read_NIRIO_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING FPGA Error reading %s%d. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_DMATTOGPUENABLE,n,fpgaStatus);
			local_status |= IRIO_warning;
		}else{
			*value=(int32_t)aux;
		}
	}else{
		irio_mergeStatus(status,Read_Resource_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING %s%d was not found.\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_DMATTOGPUENABLE,n);
		local_status |= IRIO_warning;
	}

	if(local_status<IRIO_error){
		return local_status;
	}else{
		return IRIO_error;
	}
}

int irio_setDMATtoGPUEnable(irioDrv_t* p_DrvPvt,int n,int32_t value, TStatus* status){

	if(!p_DrvPvt->DMATtoGPUNo.found){
		irio_mergeStatus(status,Write_Resource_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING Can not use GPU DMAs. DMAs were not searched or found.\n",__func__,__LINE__,p_DrvPvt->appCallID);
		return IRIO_warning;
	}

	TIRIOStatusCode local_status = IRIO_success;
	if(n>=0 && n<p_DrvPvt->DMATtoGPUNo.value && p_DrvPvt->enumDMATtoGPUEnable[n].found){
		NiFpga_Status fpgaStatus = NiFpga_Status_Success;
		if(value==0){
			NiFpga_MergeStatus(&fpgaStatus,NiFpga_WriteBool(p_DrvPvt->session,p_DrvPvt->enumDMATtoGPUEnable[n].value,NiFpga_False));
		}else{
			NiFpga_MergeStatus(&fpgaStatus,NiFpga_WriteBool(p_DrvPvt->session,p_DrvPvt->enumDMATtoGPUEnable[n].value,NiFpga_True));
		}
		if(NiFpga_IsError(fpgaStatus)){
			irio_mergeStatus(status,Write_NIRIO_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING FPGA Error writing %s%d. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_DMATTOGPUENABLE,n,fpgaStatus);
			local_status |= IRIO_warning;
		}
	}else{
		irio_mergeStatus(status,Write_Resource_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING %s%d was not found.\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_DMATTOGPUENABLE,n);
		local_status |= IRIO_warning;
	}

	if(local_status<IRIO_error){
		return local_status;
	}else{
		return IRIO_error;
	}
}

int irio_getDMATtoGPUData(irioDrv_t* p_DrvPvt, int Nelements, int n, uint64_t **data, int* elementsRead,TStatus* status){

	if(!p_DrvPvt->DMATtoGPUNo.found){
		irio_mergeStatus(status,Read_Resource_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING Can not use GPU DMAs. DMAs were not searched or found.\n",__func__,__LINE__,p_DrvPvt->appCallID);
		return IRIO_warning;
	}

	TIRIOStatusCode local_status = IRIO_success;
	if(n>=0 && n<p_DrvPvt->DMATtoGPUNo.value && p_DrvPvt->enumDMATtoGPU[n].found){


		NiFpga_Status fpgaStatus = NiFpga_Status_Success;
		size_t elementsToRead=0;
		size_t elementsRemaining=0;
		size_t elementsAcquired=0;
		uint64_t *gpu_ptr=NULL; // initialize always to null
		if(p_DrvPvt->DMATtoGPUFrameType[n]==0){//Format A
			elementsToRead=Nelements*p_DrvPvt->DMATtoGPUBlockNWords[n];
		}else{//Format B (with Timestamp and sampling info
			elementsToRead=Nelements*(p_DrvPvt->DMATtoGPUBlockNWords[n]+2);
		}
		*elementsRead=0;


		fpgaStatus = NiFpga_AcquireFifoReadElementsU64(p_DrvPvt->session,p_DrvPvt->enumDMATtoGPU[n].value,&gpu_ptr,0,0,&elementsAcquired,&elementsRemaining);
		if (elementsRemaining>=elementsToRead){
			NiFpga_MergeStatus(&fpgaStatus,NiFpga_AcquireFifoReadElementsU64(p_DrvPvt->session,p_DrvPvt->enumDMATtoGPU[n].value,&gpu_ptr,elementsToRead,FPGA_READ_BUFFER_TIMEOUT_1ms,&elementsAcquired,&elementsRemaining));
			*data=gpu_ptr;
			if(NiFpga_IsError(fpgaStatus)){
				irio_mergeStatus(status,Read_NIRIO_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING FPGA Error reading %s%d. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_DMATTOGPU,n,fpgaStatus);
				local_status |= IRIO_warning;
			}else{
				*elementsRead=Nelements;
			}
		}
	}else{
		irio_mergeStatus(status,Read_Resource_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING %s%d was not found.\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_DMATTOGPU,n);
		local_status |= IRIO_warning;
	}

	if(local_status<IRIO_error){
		return local_status;
	}else{
		return IRIO_error;
	}
}

int irio_getDMATtoGPUImage(irioDrv_t* p_DrvPvt, int imageSize, int n, uint64_t **data, int* elementsRead, TStatus* status){

        if(!p_DrvPvt->DMATtoGPUNo.found){
                irio_mergeStatus(status,Read_Resource_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING Can not use DMAs. DMAs were not searched or found.\n",__func__,__LINE__,p_DrvPvt->appCallID);
                return IRIO_warning;
        }

        TIRIOStatusCode local_status = IRIO_success;
        if(n>=0 && n<p_DrvPvt->DMATtoGPUNo.value && p_DrvPvt->enumDMATtoGPU[n].found){
                NiFpga_Status fpgaStatus = NiFpga_Status_Success;
                size_t elementsToRead=0;
                elementsToRead=imageSize*p_DrvPvt->DMATtoGPUSampleSize[n]/8;
                size_t elementsRemaining=0;
                size_t elementsAcquired=0;
                uint64_t *gpu_ptr=NULL; // initialize always to null

                *elementsRead=0;

                fpgaStatus = NiFpga_AcquireFifoReadElementsU64(p_DrvPvt->session,
                                                  p_DrvPvt->enumDMATtoGPU[n].value,
                                                  &gpu_ptr,
                                                  0,
                                                  0,
                                                  &elementsAcquired,
                                                  &elementsRemaining);

                //printf("elementsAcquired: %d", (int) elementsAcquired);
                //printf("elementsRemaining: %d", (int) elementsRemaining);




                if (elementsRemaining>=elementsToRead){

                        NiFpga_MergeStatus(&fpgaStatus, NiFpga_AcquireFifoReadElementsU64(p_DrvPvt->session,
                                                                                          p_DrvPvt->enumDMATtoGPU[n].value,
                                                                                          &gpu_ptr,
                                                                                          elementsToRead,
                                                                                          100,
                                                                                          &elementsAcquired,
                                                                                          &elementsRemaining));


                        *elementsRead = elementsAcquired*8;
                        *data=gpu_ptr;

                        if(NiFpga_IsError(fpgaStatus)){
                                irio_mergeStatus(status,Read_NIRIO_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING FPGA Error reading %s%d. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_DMATTOGPU,n,fpgaStatus);
                                local_status |= IRIO_warning;
                        }
//                        else{
//                                *elementsRead=imageSize;
//                                if(p_DrvPvt->DMATtoGPUFrameType[n]==0){//Format A Timestamp and frame counter embedded
//                                        memcpy(imgData,data,sizeof(imageData_t));
//                                        *imgDataValid=1;
//                                }else{
//                                        *imgDataValid=0;
//                                }

//                        }
                }
        }else{
                irio_mergeStatus(status,Read_Resource_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING %s%d was not found.\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_DMATTOGPU,n);
                local_status |= IRIO_warning;
        }

        if(local_status<IRIO_error){
                return local_status;
        }else{
                return IRIO_error;
        }

}


int irio_releaseDMATtoGPUImage(irioDrv_t* p_DrvPvt, int n,int  elementstofree, TStatus* status){

	TIRIOStatusCode local_status = IRIO_warning;
	NiFpga_Status fpgaStatus = NiFpga_Status_Success;
	NiFpga_MergeStatus(&fpgaStatus,
		  NiFpga_ReleaseFifoElements(p_DrvPvt->session,
                     p_DrvPvt->enumDMATtoGPU[n].value,
					 elementstofree));
	if(NiFpga_IsError(fpgaStatus)){
		irio_mergeStatus(status,Read_NIRIO_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING FPGA Error releasing %s%d elements. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_DMATTOGPU,n,fpgaStatus);
		local_status |= IRIO_warning;
	}

	return local_status;
}
