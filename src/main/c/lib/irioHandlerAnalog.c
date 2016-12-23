/**************************************************************************//**
 * \file irioHandlerAnalog.c
 * \authors Mariano Ruiz (Universidad Politécnica de Madrid, UPM)
 * \authors Diego Sanz (Universidad Politécnica de Madrid, UPM)
 * \authors Sergio Esquembri (Universidad Politécnica de Madrid, UPM)
 * \authors Enrique Bernal (Universidad Politécnica de Madrid, UPM)
 * \authors Alvaro Bustos (Universidad Politécnica de Madrid, UPM)
 * \brief Analog I/O handler methods for IRIO Driver
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

#include "irioDataTypes.h"
#include "irioHandlerAnalog.h"
#include "irioResourceFinder.h"
#include "irioError.h"

#include "NiFpga.h"

#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

/** @name Analog Resource Strings
 * Strings for analog FPGA Resources
 */
///@{
#define STRINGNAME_ANALOGINPUT "_IndicatorI32_AI"
#define STRINGNAME_ANALOGOUTPUT "_ControlI32_AO"
#define STRINGNAME_AOENABLE "_ControlBool_AOEnable"
#define STRINGNAME_AUXAI "_IndicatorI32_auxAI"
#define STRINGNAME_AUXAO "_ControlI32_auxAO"
///@}

int irio_findAnalogs(irioDrv_t* p_DrvPvt, TStatus* status){

	TIRIOStatusCode local_status = IRIO_success;
	local_status |= irio_findAnalogInputs(p_DrvPvt,status);

	local_status |= irio_findAnalogOutputs(p_DrvPvt,status);

	if(local_status<IRIO_error){
		return local_status;
	}else{
		return IRIO_error;
	}
}

int irio_findAuxAnalogs(irioDrv_t* p_DrvPvt, TStatus* status){

	TIRIOStatusCode local_status = IRIO_success;

	local_status |= irio_findAuxAnalogInputs(p_DrvPvt,status);

	local_status |= irio_findAuxAnalogOutputs(p_DrvPvt,status);

	if(local_status<IRIO_error){
		return local_status;
	}else{
		return IRIO_error;
	}
}

int irio_findAnalogInputs(irioDrv_t* p_DrvPvt, TStatus* status){
	//Search for analog inputs
	TStatus local_status;
	irio_initStatus(&local_status);
	int max_analoginputs=p_DrvPvt->max_analoginputs;
	int i;
	int count=0;

	if(p_DrvPvt->verbosity){
		printf("[%s,%d]-(%s) TRACE Analog Inputs found:",__func__,__LINE__,p_DrvPvt->appCallID);
	}

	//Analog Inputs are optional. If something fails, clean the error.
	for(i=0;i<max_analoginputs;i++){
		if(irio_findResourceEnum(p_DrvPvt,STRINGNAME_ANALOGINPUT,i,&p_DrvPvt->enumAnalogInput[i],&local_status,0)==IRIO_success){
			if(p_DrvPvt->verbosity){
				printf("AI%d, ",i);
			}
			count++;
		}
		irio_resetStatus(&local_status);
	}

	if(p_DrvPvt->verbosity){
		printf("\n");
		printf("[%s,%d]-(%s) TRACE TOTAL Analog Inputs found: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,count);
	}

	return IRIO_success;
}

int irio_findAuxAnalogInputs(irioDrv_t* p_DrvPvt,TStatus* status){
	//Search for aux analog inputs
	TStatus local_status;
	irio_initStatus(&local_status);
	int max_auxanaloginputs=p_DrvPvt->max_auxanaloginputs;
	int i;
	int count=0;

	if(p_DrvPvt->verbosity){
		printf("[%s,%d]-(%s) TRACE Auxiliary Analog Inputs found:",__func__,__LINE__,p_DrvPvt->appCallID);
	}

	//Aux Analog Inputs are optional. If something fails, clean the error.
	for(i=0;i<max_auxanaloginputs;i++){
		if(irio_findResourceEnum(p_DrvPvt,STRINGNAME_AUXAI,i,&p_DrvPvt->enumauxAI[i],&local_status,0)==IRIO_success){
			if(p_DrvPvt->verbosity){
				printf("auxAI%d, ",i);
			}
			count++;
		}
		irio_resetStatus(&local_status);
	}

	if(p_DrvPvt->verbosity){
		printf("\n");
		printf("[%s,%d]-(%s) TRACE TOTAL Auxiliary Analog Inputs found: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,count);
	}

	return IRIO_success;
}

int irio_findAnalogOutputs(irioDrv_t* p_DrvPvt, TStatus* status){
	//Search for analog outputs
	TStatus local_status,accum_status;
	irio_initStatus(&local_status);
	irio_initStatus(&accum_status);
	TIRIOStatusCode local_code = IRIO_success;
	TIRIOStatusCode local_code1 = IRIO_success;
	TIRIOStatusCode local_code2 = IRIO_success;
	int max_analogoutputs=p_DrvPvt->max_analogoutputs;
	int i;
	int count=0;

	if(p_DrvPvt->verbosity){
		printf("[%s,%d]-(%s) TRACE Analog Outputs found:",__func__,__LINE__,p_DrvPvt->appCallID);
	}

	//Analog Outputs are optional.
	//There is no fail for not finding and AO,
	//but if there is one must have AOEnable
	for(i=0;i<max_analogoutputs;i++){
		local_code1 = irio_findResourceEnum(p_DrvPvt,STRINGNAME_ANALOGOUTPUT,i,&p_DrvPvt->enumAnalogOutput[i],&local_status,0);
			//AO found, search for Enable and UpdateRate
		local_code2 = irio_findResourceEnum(p_DrvPvt,STRINGNAME_AOENABLE,i,&p_DrvPvt->enumAOEnable[i],&local_status,0);

		if(local_code1==local_code2){ //No error, found or not found
			if(local_code1==IRIO_success){
				if(p_DrvPvt->verbosity){
					printf("AO%d, ",i);
				}
				count++;
			}
		}else{ //Error only one register found
			irio_mergeStatus(&accum_status,ResourceNotFound_Warning,0,"%s[%s,%d]-(%s) WARNING AO%d resource will not be used.\n",local_status.msg,__func__,__LINE__,p_DrvPvt->appCallID,i);
			local_code |= IRIO_warning;
			p_DrvPvt->enumAnalogOutput[i].found=0;
			p_DrvPvt->enumAOEnable[i].found=0;
		}
		irio_resetStatus(&local_status);
	}

	if(accum_status.code!=IRIO_success){
		irio_mergeStatus(status,accum_status.detailCode,p_DrvPvt->verbosity,"%s",accum_status.msg);
		irio_resetStatus(&accum_status);
	}

	if(p_DrvPvt->verbosity){
		printf("\n");
		printf("[%s,%d]-(%s) TRACE TOTAL Analog Outputs found: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,count);
	}

	if(local_code<IRIO_error){
		return local_code;
	}else{
		return IRIO_error;
	}
}

int irio_findAuxAnalogOutputs(irioDrv_t* p_DrvPvt, TStatus* status){

	//Search for aux analog outputs
	TStatus local_status;
	irio_initStatus(&local_status);
	int i;
	int max_auxanalogoutputs=p_DrvPvt->max_auxanalogoutputs;
	int count=0;

	if(p_DrvPvt->verbosity){
		printf("[%s,%d]-(%s) TRACE Auxiliary Analog Outputs found:",__func__,__LINE__,p_DrvPvt->appCallID);
	}

	//Aux Analog Outputs are optional. If something fails, clean the error.
	for(i=0;i<max_auxanalogoutputs;i++){
		if(irio_findResourceEnum(p_DrvPvt,STRINGNAME_AUXAO,i,&p_DrvPvt->enumauxAO[i],&local_status,0)==IRIO_success){
			if(p_DrvPvt->verbosity){
				printf("auxAO%d, ",i);
			}
			count++;
		}
		irio_resetStatus(&local_status);
	}

	if(p_DrvPvt->verbosity){
		printf("\n");
		printf("[%s,%d]-(%s) TRACE TOTAL Auxiliary Analog Outputs found: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,count);
	}
	return IRIO_success;
}

int irio_getAI(irioDrv_t* p_DrvPvt,int n,int32_t* value,TStatus* status){
	TIRIOStatusCode local_status = IRIO_success;

	if(n>=0 && n<(p_DrvPvt->max_analoginputs) && p_DrvPvt->enumAnalogInput[n].found){
		NiFpga_Status fpgaStatus = NiFpga_Status_Success;
		int32_t aux;
		fpgaStatus=NiFpga_ReadI32(p_DrvPvt->session,p_DrvPvt->enumAnalogInput[n].value,&aux);
		if(NiFpga_IsError(fpgaStatus)){
			irio_mergeStatus(status,Read_NIRIO_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING FPGA Error reading %s%d. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_ANALOGINPUT,n,fpgaStatus);
			local_status |= IRIO_warning;
		}else{
			*value=(int32_t)aux;
		}
	}else{
		irio_mergeStatus(status,Read_Resource_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING %s%d was not found.\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_ANALOGINPUT,n);
		local_status |= IRIO_warning;
	}

	if(local_status<IRIO_error){
		return local_status;
	}else{
		return IRIO_error;
	}
}

int irio_getAuxAI(irioDrv_t* p_DrvPvt,int n,int32_t* value, TStatus* status){
	TIRIOStatusCode local_status = IRIO_success;
	if(n>=0 && n<(p_DrvPvt->max_auxanaloginputs) && p_DrvPvt->enumauxAI[n].found){
		NiFpga_Status fpgaStatus = NiFpga_Status_Success;
		fpgaStatus=NiFpga_ReadI32(p_DrvPvt->session,p_DrvPvt->enumauxAI[n].value,value);
		if(NiFpga_IsError(fpgaStatus)){
			irio_mergeStatus(status,Read_NIRIO_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING FPGA Error reading %s%d. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_AUXAI,n,fpgaStatus);
			local_status |= IRIO_warning;
		}
	}else{
		irio_mergeStatus(status,Read_Resource_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING %s%d was not found.\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_AUXAI,n);
		local_status |= IRIO_warning;
	}

	if(local_status<IRIO_error){
		return local_status;
	}else{
		return IRIO_error;
	}
}

int irio_getAO(irioDrv_t* p_DrvPvt,int n,int32_t* value, TStatus* status){
	TIRIOStatusCode local_status = IRIO_success;
	if(n>=0 && n<(p_DrvPvt->max_analogoutputs) && p_DrvPvt->enumAnalogOutput[n].found){
		NiFpga_Status fpgaStatus = NiFpga_Status_Success;
		int32_t aux=0;
		fpgaStatus=NiFpga_ReadI32(p_DrvPvt->session,p_DrvPvt->enumAnalogOutput[n].value,&aux);
		if(NiFpga_IsError(fpgaStatus)){
			irio_mergeStatus(status,Read_NIRIO_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING FPGA Error reading %s%d. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_ANALOGOUTPUT,n,fpgaStatus);
			local_status |= IRIO_warning;
		}else{
			*value=(int32_t)aux;
		}
	}else{
		irio_mergeStatus(status,Read_Resource_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING %s%d was not found.\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_ANALOGOUTPUT,n);
		local_status |= IRIO_warning;
	}

	if(local_status<IRIO_error){
		return local_status;
	}else{
		return IRIO_error;
	}
}

int irio_setAO(irioDrv_t* p_DrvPvt,int n,int32_t value,TStatus* status){
	TIRIOStatusCode local_status = IRIO_success;
	if(n>=0 && n<(p_DrvPvt->max_analogoutputs) && p_DrvPvt->enumAnalogOutput[n].found){
		NiFpga_Status fpgaStatus = NiFpga_Status_Success;
		int32_t aux = value;

		fpgaStatus=NiFpga_WriteI32(p_DrvPvt->session,p_DrvPvt->enumAnalogOutput[n].value,aux);
		if(NiFpga_IsError(fpgaStatus)){
			irio_mergeStatus(status,Write_NIRIO_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING FPGA Error writing %s%d. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_ANALOGOUTPUT,n,fpgaStatus);
			local_status |= IRIO_warning;
		}
	}else{
		irio_mergeStatus(status,Write_Resource_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING %s%d was not found.\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_ANALOGOUTPUT,n);
		local_status |= IRIO_warning;
	}

	if(local_status<IRIO_error){
		return local_status;
	}else{
		return IRIO_error;
	}
}

int irio_getAOEnable(irioDrv_t* p_DrvPvt,int n,int32_t* value, TStatus* status){
	TIRIOStatusCode local_status = IRIO_success;
	if(n>=0 && n<(p_DrvPvt->max_numberofSG) && p_DrvPvt->enumAOEnable[n].found){
		NiFpga_Status fpgaStatus = NiFpga_Status_Success;
		NiFpga_Bool aux = NiFpga_True;
		fpgaStatus=NiFpga_ReadBool(p_DrvPvt->session,p_DrvPvt->enumAOEnable[n].value,&aux);
		if(NiFpga_IsError(fpgaStatus)){
			irio_mergeStatus(status,Read_NIRIO_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING FPGA Error reading %s%d. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_AOENABLE,n,fpgaStatus);
			local_status |= IRIO_warning;
		}else{
			*value=(int32_t)aux;
		}
	}else{
		irio_mergeStatus(status,Read_Resource_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING %s%d was not found.\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_AOENABLE,n);
		local_status |= IRIO_warning;
	}

	if(local_status<IRIO_error){
		return local_status;
	}else{
		return IRIO_error;
	}
}

int irio_setAOEnable(irioDrv_t* p_DrvPvt,int n,int32_t value, TStatus* status){
	TIRIOStatusCode local_status = IRIO_success;
	if(n>=0 && n<(p_DrvPvt->max_numberofSG) && p_DrvPvt->enumAOEnable[n].found){
		NiFpga_Status fpgaStatus = NiFpga_Status_Success;
		if(value==0){
			fpgaStatus=NiFpga_WriteBool(p_DrvPvt->session,p_DrvPvt->enumAOEnable[n].value,NiFpga_False);
		}else{
			fpgaStatus=NiFpga_WriteBool(p_DrvPvt->session,p_DrvPvt->enumAOEnable[n].value,NiFpga_True);
		}
		if(NiFpga_IsError(fpgaStatus)){
			irio_mergeStatus(status,Write_NIRIO_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING FPGA Error writing %s%d. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_AOENABLE,n,fpgaStatus);
			local_status |= IRIO_warning;
		}
	}else{
		irio_mergeStatus(status,Write_Resource_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING %s%d was not found.\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_AOENABLE,n);
		local_status |= IRIO_warning;
	}

	if(local_status<IRIO_error){
		return local_status;
	}else{
		return IRIO_error;
	}
}

int irio_getAuxAO(irioDrv_t* p_DrvPvt,int n,int32_t* value, TStatus* status){
	TIRIOStatusCode local_status = IRIO_success;
	if(n>=0 && n<(p_DrvPvt->max_auxanalogoutputs) && p_DrvPvt->enumauxAO[n].found){
		NiFpga_Status fpgaStatus = NiFpga_Status_Success;
		fpgaStatus=NiFpga_ReadI32(p_DrvPvt->session,p_DrvPvt->enumauxAO[n].value,value);
		if(NiFpga_IsError(fpgaStatus)){
			irio_mergeStatus(status,Read_NIRIO_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING FPGA Error reading %s%d. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_AUXAO,n,fpgaStatus);
			local_status |= IRIO_warning;
		}
	}else{
		irio_mergeStatus(status,Read_Resource_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING %s%d was not found.\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_AUXAO,n);
		local_status |= IRIO_warning;
	}

	if(local_status<IRIO_error){
		return local_status;
	}else{
		return IRIO_error;
	}
}

int irio_setAuxAO(irioDrv_t* p_DrvPvt,int n,int32_t value, TStatus* status){
	TIRIOStatusCode local_status = IRIO_success;
	if(n>=0 && n<(p_DrvPvt->max_auxanalogoutputs) && p_DrvPvt->enumauxAO[n].found){
		NiFpga_Status fpgaStatus = NiFpga_Status_Success;
		fpgaStatus=NiFpga_WriteI32(p_DrvPvt->session,p_DrvPvt->enumauxAO[n].value,value);
		if(NiFpga_IsError(fpgaStatus)){
			irio_mergeStatus(status,Write_NIRIO_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING FPGA Error writing %s%d. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_AUXAO,n,fpgaStatus);
			local_status |= IRIO_warning;
		}
	}else{
		irio_mergeStatus(status,Write_Resource_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING %s%d was not found.\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_AUXAO,n);
		local_status |= IRIO_warning;
	}

	if(local_status<IRIO_error){
		return local_status;
	}else{
		return IRIO_error;
	}
}
