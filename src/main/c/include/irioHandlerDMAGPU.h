/**************************************************************************//**
 * \file irioHandlerDMAGPU.h
 * \authors Mariano Ruiz (Universidad Politécnica de Madrid, UPM)
 * \authors Julian Nieto (Universidad Politécnica de Madrid, UPM)
 * \authors Paul Guillen (Universidad Politécnica de Madrid, UPM)
 * \authors Diego Sanz (Universidad Politécnica de Madrid, UPM)
 * \authors Sergio Esquembri (Universidad Politécnica de Madrid, UPM)
 * \authors Enrique Bernal (Universidad Politécnica de Madrid, UPM)
 * \authors Alvaro Bustos (Universidad Politécnica de Madrid, UPM)
 * \brief GPU DMA handler methods for IRIO driver
 * \date July 2015 (Last Review Sep 2015)
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

#ifndef IRIOHANDLERDMAGPU_H
#define IRIOHANDLERDMAGPU_H

#include "irioDataTypes.h"

#include <stdint.h>

#define MAX_DMA_FLEXRIO 16 //Maximun number of DMA used in FlexRIO technology

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Search and map data GPU DMA resources
 *
 * Searches for data GPU DMAs implemented for the current target.
 * Map ports found in the driver struct.
 * Errors will be prompted if:
 * 	\n Any of the mandatory ports (DMATtoGPUNCh,
 * DMATtoGPUOverflows,DMATtoGPUFrameType, DMATtoGPUSampleSize, and
 * DMATtoGPUBlockNWords) is not found.
 * 	\n Errors occur while reading from any of the mandatory ports.
 *  \n If any of the resources associated with a DMA
 *  (DMATtoGPU, DMATtoGPUEnable, and DMATtoGPUSamplingRate) is not found.
 *
 *  NOTE: The size of the array DMATtoGPUNCh will determine the number of DMAs to search.
 *  DMAs should be numbered consecutively starting from 0.
 *
 * DMAs should be configured before start acquisition using irio_setUpDMAsTtoGPU().
 *
 * DMA information and ports found will be saved in:
 * 	\n irioDrv_t::enumGPUTtoGPUNCh
 * 	\n irioDrv_t::DMATtoGPUNCh
 *  \n irioDrv_t::DMATtoGPUNo
 * 	\n irioDrv_t::enumDMATtoGPUOverflows
 *  \n irioDrv_t::enumDMATtoGPUFrameType
 *  \n irioDrv_t::DMATtoGPUFrameType
 *  \n irioDrv_t::enumDMATtoGPUSampleSize
 *  \n irioDrv_t::DMATtoGPUSampleSize
 *  \n irioDrv_t::enumDMATtoGPUBlockNWords
 *  \n irioDrv_t::DMATtoGPUBlockNWords
 *  \n irioDrv_t::enumDMATtoGPU
 *  \n irioDrv_t::enumDMATtoGPUSamplingRate
 *  \n irioDrv_t::enumDMATtoGPUEnable
 *  \n irioDrv_t::DMATtoGPUChIndex
 *
 * @param[in,out] p_DrvPvt 	Pointer to the driver session structure
 * @param[out] status	Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_findDAQDMAsGPU(irioDrv_t* p_DrvPvt,TStatus* status);

/**
 * Search and map an IMAQ DMA
 *
 * Search for NCHperDMATtoGPUSize array and DMAsOverflow ports. Then, search as many DMAs as
 * the size of the array. The DMAs must appear numbered from 0 to size-1.
 *
 * DMAs should be configured before start acquisition. @see {setUpDMAs}
 *
 * 	This method fills:
 *		p_DrvPvt->NumberOfDMAs
 *		p_DrvPvt->enumDMAsOverflow
 *		p_DrvPvt->arrayIdDMAtoGPU
 *		p_DrvPvt->enumSamplingRate
 *		p_DrvPvt->enumDMAsActvToGPU
 *		p_DrvPvt->arrayIdDMAtoGPU[i].IdDMA
 *		p_DrvPvt->arrayIdDMAtoGPU[i].NChDMA
 *		p_DrvPvt->arrayIdDMAtoGPU[i].NwordU64
 *		p_DrvPvt->arrayIdDMAtoGPU[i].index
 *
 * @param[in,out] p_DrvPvt 	Pointer to the driver session structure
 * @param[out] status	Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_findIMAQDMAsGPU(irioDrv_t* p_DrvPvt,TStatus* status);

/**
 * Set up DMAs
 *
 * Configure GPU memory and FPGA registers for DMA transfer. First, allocate GPU memory
 * for DMA transfer and clean up of possible trash data. Then set.
 * The DMA channel should be activated
 * by the user once the initialization is finished,
 *
 * This method calls calcADCValue() to configure AD/DA conversion values among other fields.
 * @see{calcADCValue}
 *
 * @param[in,out] p_DrvPvt 	Pointer to the driver session structure
 * @param[out] status	Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_setUpDMAsTtoGPU(irioDrv_t* p_DrvPvt,int depth, TStatus* status);

/**
 * Stops DMAs
 *
 * Stops all DMAs found, freeing the allocated memory for the DMA transfer.
 *
 * @param p_DrvPvt Pointer to the nirioDriver structure
 * @return Operation status. @see {TStatusCodes}
 */
int irio_closeDMAsTtoGPU(irioDrv_t* p_DrvPvt,TStatus* status);

/**
 * Clean DMA FIFOs
 *
 * Read from all DMA FIFOs every second until it get emptied
 *
 * @param[in,out] p_DrvPvt 	Pointer to the driver session structure
 * @param[out] status	Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_cleanDMAsTtoGPU(irioDrv_t* p_DrvPvt,TStatus* status);

/**
 * Clean DMA FIFO
 *
 * Read from DMA FIFOs every second until it get emptied. Reads are done over the
 * given buffer.
 *
 * @param[in] p_DrvPvt 	Pointer to the driver session structure
 * @param[in] n Number of the DMA to clean
 * @param[in] cleanbuffer buffer to be used for DMA reads.
 * @param[in]  buffersize size of the clean buffer.
 * @param[out] status	Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_cleanDMATtoGPU(irioDrv_t* p_DrvPvt, int n,uint64_t** cleanbuffer, size_t buffersize,TStatus* status);

/**
 * Get DMA Overflow
 *
 * Reads the value of the FPGA DMA Overflow register.
 *
 * Errors may occur if the port was not found or while writing the value in the device.
 *
 * @param[in] p_DrvPvt 	Pointer to the driver session structure
 * @param[in] value Value of the overflow bit from DMAsOverflow register. Non-zero value means overflow.
 * @param[out] status	Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_getDMATtoGPUOverflow(irioDrv_t* p_DrvPvt,int32_t* value,TStatus* status);

/**
 * Read the sampling rate of a DMA
 *
 * Read the sampling rate value of a DMA from the FPGA.
 * Errors may occur if the port was not found or while reading its value from the device.
 *
 * @param[in] p_DrvPvt 	Pointer to the driver session structure
 * @param[in] n Number of the DMA which sampling rate will be read (SamplingRaten)
 * @param[out] value Pointer where result will be stored
 * @param[out] status	Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_getDMATtoGPUSamplingRate(irioDrv_t* p_DrvPvt, int n,int32_t *value,TStatus* status);

/**
 * Set the sampling rate of a DMA
 *
 * Write the given value a DMA Sampling Rate port.
 * No range check or conversion is performed on the given value.
 * Errors may occur if the port was not found or while writing the value in the device.
 *
 * @param[in] p_DrvPvt 	Pointer to the driver session structure
 * @param[in] n Number of the DMA which sampling rate will be read (SamplingRaten)
 * @param[in] Value to be written in the samplingrate terminal
 * @param[out] status	Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 *
 */
int irio_setDMATtoGPUSamplingRate(irioDrv_t* p_DrvPvt, int n,int32_t value,TStatus* status);

/**
 * Get DMA group enable
 *
 * Reads from the FPGA whether a DMA channel group is enabled or not.
 * Errors may occur if the port was not found or while reading its value from the device.
 *
 * @param[in] p_DrvPvt 	Pointer to the driver session structure
 * @param[in] n Number of the DMA which group enable will be read (GroupEnablen)
 * @param[out] status	Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_getDMATtoGPUEnable(irioDrv_t* p_DrvPvt, int n,int32_t* value,TStatus* status);

/**
 * Enables or disables a DMA channel group
 *
 * Writes in a FPGA register to enable or disable a DMA channel group.
 * Errors may occur if the port was not found or while writing the value in the device.
 *
 * @param[in] p_DrvPvt 	Pointer to the driver session structure
 * @param[in] n Number of the DMA which group to enable/disable (GroupEnablen)
 * @param value Value to set. Will be converted to NiFpga_Bool (0=NiFpga_False, other=NiFpga_True)
 * @param[out] status	Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_setDMATtoGPUEnable(irioDrv_t* p_DrvPvt, int n,int32_t value,TStatus* status);

/*
 * Reads data from the DMA
 *
 * Reads Nelements from the DMA if they are available. If there are not enough
 * elements, nothing is read. The size in DMA words of an element must be
 * set before (NWordU64). Elements read will be written in the given buffer.
 *
 * @param p_DrvPvt Pointer to the nirioDriver structure
 * @param Nelements number of elements to read. Size of an element is given by NWordU64 of DMAStruct. @see {TdataDMA}
 * @param n Number of Number of the DMA where data should be read
 * @param data buffer where data should be stored
 * @param elementsRead number of elements read. Can be 0 or Nelements
 * @return Operation status. @see {TStatusCodes}
 */
/**
 * Not implemented yet
 */
int irio_getDMATtoGPUData(irioDrv_t* p_DrvPvt, int Nelements, int n, uint64_t **data, int* elementsRead,TStatus* status);

/*
 * This function allow to get a pointer point to GPU Memory where the data are available. This pointer points (double pointer)
 * to GPU memory. Therefore, the access to this data using the CPU the user has to copy them to CPU memory using the appropriate function.
 *
 * @param[in] p_DrvPvt Pointer to the nirioDriver structure
 * @param[in] imageSize number of elements to get.
 * @param[in] n Identifier of the DMA Buffer (DMA FIFO)
 * @param[out] data double pointer to GPU memory buffer
 * @param[out] elementsRead number of elements available in the buffer
 * @param[out] status	Warning and error messages produced during the execution of this call will be added here.
 * @return return \ref TIRIOStatusCode
 */

int irio_getDMATtoGPUImage(irioDrv_t* p_DrvPvt, int imageSize, int n, uint64_t **data, int* elementsRead, TStatus* status);

/*
 * Release the memory in the buffer
 *
 * @param[in] p_DrvPvt Pointer to the nirioDriver structure
 * @param[in] n Identifier of the DMA Buffer (DMA FIFO)
 * @param[in] elementstofree number of elements realeased in the buffer
 * @param[out] status	Warning and error messages produced during the execution of this call will be added here.
 * @return return \ref TIRIOStatusCode
 */

int irio_releaseDMATtoGPUImage(irioDrv_t* p_DrvPvt, int n,int  elementstofree, TStatus* status);

#ifdef __cplusplus
}
#endif

#endif
