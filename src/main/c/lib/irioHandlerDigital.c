/**************************************************************************//**
 * \file irioHandlerDigital.c
 * \authors Mariano Ruiz (Universidad Politécnica de Madrid, UPM)
 * \authors Diego Sanz (Universidad Politécnica de Madrid, UPM)
 * \authors Sergio Esquembri (Universidad Politécnica de Madrid, UPM)
 * \authors Enrique Bernal (Universidad Politécnica de Madrid, UPM)
 * \authors Alvaro Bustos (Universidad Politécnica de Madrid, UPM)
 * \brief Digital I/O handler methods for IRIO Driver
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
#include "irioHandlerDigital.h"
#include "irioResourceFinder.h"
#include "irioError.h"

#include "NiFpga.h"

#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

/** @name Digital Resource Strings
 * Strings for digital FPGA Resources
 */
///@{
#define STRINGNAME_DIGITALINPUT "_IndicatorBool_DI"
#define STRINGNAME_DIGITALOUTPUT "_ControlBool_DO"
#define STRINGNAME_AUXDI "_IndicatorBool_auxDI"
#define STRINGNAME_AUXDO "_ControlBool_auxDO"
///@}

int irio_findDigitals(irioDrv_t* p_DrvPvt, TStatus* status){

	TIRIOStatusCode local_status = IRIO_success;
	local_status |= irio_findDigitalInputs(p_DrvPvt,status);


	local_status |= irio_findDigitalOutputs(p_DrvPvt,status);

	if(local_status<IRIO_error){
		return local_status;
	}else{
		return IRIO_error;
	}
}

int irio_findAuxDigitals(irioDrv_t* p_DrvPvt, TStatus* status){

	TIRIOStatusCode local_status = IRIO_success;
	local_status |= irio_findAuxDigitalInputs(p_DrvPvt,status);


	local_status |= irio_findAuxDigitalOutputs(p_DrvPvt,status);

	if(local_status<IRIO_error){
		return local_status;
	}else{
		return IRIO_error;
	}
}

int irio_findDigitalInputs(irioDrv_t* p_DrvPvt, TStatus* status){
	//Search for digital inputs
	TStatus local_status;
	irio_initStatus(&local_status);
	int max_digitalsinputs=p_DrvPvt->max_digitalsinputs;
	int i;
	int count=0;

	if(p_DrvPvt->verbosity){
		printf("[%s,%d]-(%s) TRACE Digital Inputs found:",__func__,__LINE__,p_DrvPvt->appCallID);
	}

	//Digital Inputs are optional. If something fails, clean the error.
	for(i=0;i<max_digitalsinputs;i++){
		if(irio_findResourceEnum(p_DrvPvt,STRINGNAME_DIGITALINPUT,i,&p_DrvPvt->enumDigitalInput[i],&local_status,0)==IRIO_success){
			if(p_DrvPvt->verbosity){
				printf("DI%d, ",i);
			}
			count++;
		}
		irio_resetStatus(&local_status);
	}

	if(p_DrvPvt->verbosity){
		printf("\n");
		printf("[%s,%d]-(%s) TRACE TOTAL Digital Inputs found: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,count);
	}

	return IRIO_success;
}

int irio_findAuxDigitalInputs(irioDrv_t* p_DrvPvt, TStatus* status){
	//Search for aux digital inputs
	TStatus local_status;
	irio_initStatus(&local_status);
	int max_auxdigitalsinputs=p_DrvPvt->max_auxdigitalsinputs;
	int i;
	int count=0;

	if(p_DrvPvt->verbosity){
		printf("[%s,%d]-(%s) TRACE Auxiliary Digital Inputs found:",__func__,__LINE__,p_DrvPvt->appCallID);
	}

	//Aux Digital Inputs are optional. If something fails, clean the error.
	for(i=0;i<max_auxdigitalsinputs;i++){
		if(irio_findResourceEnum(p_DrvPvt,STRINGNAME_AUXDI,i,&p_DrvPvt->enumauxDI[i],&local_status,0)==IRIO_success){
			if(p_DrvPvt->verbosity){
				printf("auxDI%d, ",i);
			}
			count++;
		}
		irio_resetStatus(&local_status);
	}

	if(p_DrvPvt->verbosity){
		printf("\n");
		printf("[%s,%d]-(%s) TRACE TOTAL Auxiliary Digital Inputs found: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,count);
	}

	return IRIO_success;
}

int irio_findDigitalOutputs(irioDrv_t* p_DrvPvt, TStatus* status){
	//Search for digital outputs
	TStatus local_status;
	irio_initStatus(&local_status);
	int max_digitalsoutputs=p_DrvPvt->max_digitalsoutputs;
	int i;
	int count=0;

	if(p_DrvPvt->verbosity){
		printf("[%s,%d]-(%s) TRACE Digital Outputs found:",__func__,__LINE__,p_DrvPvt->appCallID);
	}

	//Digital Outputs are optional.
	//There is no fail for not finding and DO,
	for(i=0;i<max_digitalsoutputs;i++){
		if(irio_findResourceEnum(p_DrvPvt,STRINGNAME_DIGITALOUTPUT,i,&p_DrvPvt->enumDigitalOutput[i],&local_status,0)==IRIO_success){
			if(p_DrvPvt->verbosity){
				printf("DO%d, ",i);
			}
			count++;
		}
		irio_resetStatus(&local_status);
	}

	if(p_DrvPvt->verbosity){
		printf("\n");
		printf("[%s,%d]-(%s) TRACE TOTAL Digital Outputs found: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,count);
	}

	return IRIO_success;
}

int irio_findAuxDigitalOutputs(irioDrv_t* p_DrvPvt, TStatus* status){
	//Search for aux digital outputs
	TStatus local_status;
	irio_initStatus(&local_status);
	int max_auxdigitalsoutputs=p_DrvPvt->max_auxdigitalsoutputs;
	int i;
	int count=0;


	if(p_DrvPvt->verbosity){
		printf("[%s,%d]-(%s) TRACE Auxiliary Digital Outputs found:",__func__,__LINE__,p_DrvPvt->appCallID);
	}

	//Aux Digital Outputs are optional. If something fails, clean the error.
	for(i=0;i<max_auxdigitalsoutputs;i++){
		if(irio_findResourceEnum(p_DrvPvt,STRINGNAME_AUXDO,i,&p_DrvPvt->enumauxDO[i],&local_status,0)==IRIO_success){
			if(p_DrvPvt->verbosity){
				printf("auxDO%d, ",i);
			}
			count++;
		}
		irio_resetStatus(&local_status);
	}

	if(p_DrvPvt->verbosity){
		printf("\n");
		printf("[%s,%d]-(%s) TRACE TOTAL Auxiliary Digital Outputs found: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,count);
	}

	return IRIO_success;
}

int irio_getDI(irioDrv_t* p_DrvPvt,int n,int32_t* value, TStatus* status){
	TIRIOStatusCode local_status = IRIO_success;

	if(n>=0 && n<(p_DrvPvt->max_digitalsinputs) && p_DrvPvt->enumDigitalInput[n].found){
		NiFpga_Status fpgaStatus = NiFpga_Status_Success;
		NiFpga_Bool aux=NiFpga_True;
		NiFpga_MergeStatus(&fpgaStatus,NiFpga_ReadBool(p_DrvPvt->session,p_DrvPvt->enumDigitalInput[n].value,&aux));
		if(NiFpga_IsError(fpgaStatus)){
			irio_mergeStatus(status,Read_NIRIO_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING FPGA Error reading %s%d. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_DIGITALINPUT,n,fpgaStatus);
			local_status |= IRIO_warning;
		}else{
			*value=(int32_t)aux;
		}
	}else{
		irio_mergeStatus(status,Read_Resource_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING %s%d was not found.\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_DIGITALINPUT,n);
		local_status |= IRIO_warning;
	}

	if(local_status<IRIO_error){
		return local_status;
	}else{
		return IRIO_error;
	}
}

int irio_getAuxDI(irioDrv_t* p_DrvPvt,int n,int32_t* value, TStatus* status){
	TIRIOStatusCode local_status = IRIO_success;
	if(n>=0 && n<(p_DrvPvt->max_auxdigitalsinputs) && p_DrvPvt->enumauxDI[n].found){
		NiFpga_Status fpgaStatus = NiFpga_Status_Success;
		NiFpga_Bool aux=NiFpga_True;
		NiFpga_MergeStatus(&fpgaStatus,NiFpga_ReadBool(p_DrvPvt->session,p_DrvPvt->enumauxDI[n].value,&aux));
		if(NiFpga_IsError(fpgaStatus)){
			irio_mergeStatus(status,Read_NIRIO_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING FPGA Error reading %s%d. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_AUXDI,n,fpgaStatus);
			local_status |= IRIO_warning;
		}else{
			*value=(int32_t)aux;
		}
	}else{
		irio_mergeStatus(status,Read_Resource_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING %s%d was not found.\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_AUXDI,n);
		local_status |= IRIO_warning;
	}

	if(local_status<IRIO_error){
		return local_status;
	}else{
		return IRIO_error;
	}
}

int irio_getDO(irioDrv_t* p_DrvPvt,int n,int32_t* value, TStatus* status){
	TIRIOStatusCode local_status = IRIO_success;

	if(n>=0 && n<(p_DrvPvt->max_digitalsoutputs) && p_DrvPvt->enumDigitalOutput[n].found){
		NiFpga_Status fpgaStatus = NiFpga_Status_Success;
		NiFpga_Bool aux=NiFpga_True;
		NiFpga_MergeStatus(&fpgaStatus,NiFpga_ReadBool(p_DrvPvt->session,p_DrvPvt->enumDigitalOutput[n].value,&aux));
		if(NiFpga_IsError(fpgaStatus)){
			irio_mergeStatus(status,Read_NIRIO_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING FPGA Error reading %s%d. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_DIGITALOUTPUT,n,fpgaStatus);
			local_status |= IRIO_warning;
		}else{
			*value=(int32_t)aux;
		}
	}else{
		irio_mergeStatus(status,Read_Resource_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING %s%d was not found.\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_DIGITALOUTPUT,n);
		local_status |= IRIO_warning;
	}

	if(local_status<IRIO_error){
		return local_status;
	}else{
		return IRIO_error;
	}
}

int irio_setDO(irioDrv_t* p_DrvPvt,int n,int32_t value, TStatus* status){
	TIRIOStatusCode local_status = IRIO_success;
	if(n>=0 && n<(p_DrvPvt->max_digitalsoutputs) && p_DrvPvt->enumDigitalOutput[n].found){
		NiFpga_Status fpgaStatus = NiFpga_Status_Success;
		if(value==0){
			NiFpga_MergeStatus(&fpgaStatus,NiFpga_WriteBool(p_DrvPvt->session,p_DrvPvt->enumDigitalOutput[n].value,NiFpga_False));
		}else{
			NiFpga_MergeStatus(&fpgaStatus,NiFpga_WriteBool(p_DrvPvt->session,p_DrvPvt->enumDigitalOutput[n].value,NiFpga_True));
		}
		if(NiFpga_IsError(fpgaStatus)){
			irio_mergeStatus(status,Write_NIRIO_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING FPGA Error writing %s%d. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_DIGITALOUTPUT,n,fpgaStatus);
			local_status |= IRIO_warning;
		}
	}else{
		irio_mergeStatus(status,Write_Resource_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING %s%d was not found.\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_DIGITALOUTPUT,n);
		local_status |= IRIO_warning;
	}

	if(local_status<IRIO_error){
		return local_status;
	}else{
		return IRIO_error;
	}
}

int irio_getAuxDO(irioDrv_t* p_DrvPvt,int n,int32_t* value, TStatus* status){
	TIRIOStatusCode local_status = IRIO_success;
	if(n>=0 && n<(p_DrvPvt->max_auxdigitalsoutputs) && p_DrvPvt->enumauxDO[n].found){
		NiFpga_Status fpgaStatus = NiFpga_Status_Success;
		NiFpga_Bool aux=NiFpga_True;
		NiFpga_MergeStatus(&fpgaStatus,NiFpga_ReadBool(p_DrvPvt->session,p_DrvPvt->enumauxDO[n].value,&aux));
		if(NiFpga_IsError(fpgaStatus)){
			irio_mergeStatus(status,Read_NIRIO_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING FPGA Error reading %s%d. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_AUXDO,n,fpgaStatus);
			local_status |= IRIO_warning;
		}else{
			*value=(int32_t)aux;
		}
	}else{
		irio_mergeStatus(status,Read_Resource_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING %s%d was not found.\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_AUXDO,n);
		local_status |= IRIO_warning;
	}

	if(local_status<IRIO_error){
		return local_status;
	}else{
		return IRIO_error;
	}}

int irio_setAuxDO(irioDrv_t* p_DrvPvt,int n,int32_t value, TStatus* status){
	TIRIOStatusCode local_status = IRIO_success;
	if(n>=0 && n<(p_DrvPvt->max_auxdigitalsoutputs) && p_DrvPvt->enumauxDO[n].found){
		NiFpga_Status fpgaStatus = NiFpga_Status_Success;
		if(value==0){
			NiFpga_MergeStatus(&fpgaStatus,NiFpga_WriteBool(p_DrvPvt->session,p_DrvPvt->enumauxDO[n].value,NiFpga_False));
		}else{
			NiFpga_MergeStatus(&fpgaStatus,NiFpga_WriteBool(p_DrvPvt->session,p_DrvPvt->enumauxDO[n].value,NiFpga_True));
		}
		if(NiFpga_IsError(fpgaStatus)){
			irio_mergeStatus(status,Write_NIRIO_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING FPGA Error writing %s%d. Error Code: %d\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_AUXDO,n,fpgaStatus);
			local_status |= IRIO_warning;
		}
	}else{
		irio_mergeStatus(status,Write_Resource_Warning,p_DrvPvt->verbosity,"[%s,%d]-(%s) WARNING %s%d was not found.\n",__func__,__LINE__,p_DrvPvt->appCallID,STRINGNAME_AUXDO,n);
		local_status |= IRIO_warning;
	}

	if(local_status<IRIO_error){
		return local_status;
	}else{
		return IRIO_error;
	}
}
