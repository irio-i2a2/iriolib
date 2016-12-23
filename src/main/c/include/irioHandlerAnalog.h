/**************************************************************************//**
 * \file irioHandlerAnalog.h
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

#ifndef IRIOHANDLERANALOG_H
#define IRIOHANDLERANALOG_H

#include "irioDataTypes.h"

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Search and map analog ports
 *
 * Searches for analog I/O ports implemented for the current target.
 * Maps ports found in the driver struct.
 * This method just call to irio_findAnalogInputs() and irio_findAnalogOutputs().
 *
 * @param[in,out] p_DrvPvt 	Pointer to the driver session structure
 * @param[out] status	Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_findAnalogs(irioDrv_t* p_DrvPvt,TStatus* status);

/**
 * Search and map auxiliary analog I/O ports
 *
 * Searches for auxiliary analog I/O ports implemented for the current target.
 * Maps ports found in the driver struct.
 * This method just call to irio_findAuxAnalogInputs() and irio_findAuxAnalogOutputs().
 *
 * @param[in,out] p_DrvPvt 	Pointer to the driver session structure
 * @param[out] status	Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_findAuxAnalogs(irioDrv_t* p_DrvPvt,TStatus* status);

/**
 * Search and map analog inputs ports
 *
 * Searches for analog input ports implemented for the current target.
 * Maps ports found in irioDrv_t::enumAnalogInput.
 * No error will be prompted for missing analog input ports.
 *
 * @param[in,out] p_DrvPvt 	Pointer to the driver session structure
 * @param[out] status	Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_findAnalogInputs(irioDrv_t* p_DrvPvt,TStatus* status);

/**
 * Search and map auxiliary analog inputs ports
 *
 * Searches for auxiliary analog input ports implemented for the current target.
 * Maps ports found in irioDrv_t::enumauxAI.
 * No error will be prompted for missing auxiliary analog input ports.
 *
 * @param[in,out] p_DrvPvt 	Pointer to the driver session structure
 * @param[out] status	Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_findAuxAnalogInputs(irioDrv_t* p_DrvPvt,TStatus* status);

/**
 * Search and map analog output ports
 *
 * Searches for auxiliary analog input ports implemented for the current target.
 * Maps ports found in irioDrv_t::enumAnalogOutput and irioDrv_t::enumAOEnable.
 * Analog output resources uses two ports (AO and AOEnable). No error will be prompted for
 * missing analog output resources but if only one of the two ports were found.
 *
 * @param[in,out] p_DrvPvt 	Pointer to the driver session structure
 * @param[out] status	Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_findAnalogOutputs(irioDrv_t* p_DrvPvt,TStatus* status);

/**
 * Search and map auxiliary analog output ports
 *
 * Searches for auxiliary analog output ports implemented for the current target.
 * Maps ports found in irioDrv_t::enumauxAO.
 * No error will be prompted for missing auxiliary analog output ports.
 *
 * @param[in,out] p_DrvPvt 	Pointer to the driver session structure
 * @param[out] status	Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_findAuxAnalogOutputs(irioDrv_t* p_DrvPvt,TStatus* status);

/**
 * Read an analog input
 *
 * Reads the value from an analog input port.
 * Errors may occur if the port was not found or while reading from the port.
 *
 * @param[in] p_DrvPvt 	Pointer to the driver session structure
 * @param[in] n Number of the analog input to read (AIn)
 * @param[out] value  Current value of the analog input
 * @param[out] status	Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_getAI(irioDrv_t* p_DrvPvt,int n,int32_t* value,TStatus* status);

/**
 * Read an auxiliary analog input
 *
 * Reads the value from an auxiliary analog input port.
 * Errors may occur if the port was not found or while reading from the port.
 *
 * @param[in] p_DrvPvt 	Pointer to the driver session structure
 * @param[in] n Number of the auxiliary analog input to read (AIn)
 * @param[out] value  Current value of the auxiliary analog input
 * @param[out] status	Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_getAuxAI(irioDrv_t* p_DrvPvt,int n,int32_t* value,TStatus* status);

/**
 * Read an analog output
 *
 * Reads the value from an analog output port.
 * Errors may occur if the port was not found or while reading from the port.
 *
 * @param[in] p_DrvPvt 	Pointer to the driver session structure
 * @param[in] n Number of the analog output to read (AOn)
 * @param[out] value  Current value of the analog output
 * @param[out] status	Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_getAO(irioDrv_t* p_DrvPvt,int n,int32_t* value,TStatus* status);

/**
 * Write an analog output
 *
 * Writes the given value in an analog output port.
 * Errors may occur if the port was not found or while writing to the port.
 *
 * @param[in] p_DrvPvt 	Pointer to the driver session structure
 * @param[in] n Number of the analog output to write (AOn)
 * @param[in] value Value to write
 * @param[out] status	Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_setAO(irioDrv_t* p_DrvPvt,int n,int32_t value,TStatus* status);

/**
 * Read an Analog Output Enable
 *
 * Reads from the FPGA whether a analog output port is enabled or not.
 * Errors may occur if the port was not found or while reading from the port
 *
 * @param[in] p_DrvPvt 	Pointer to the driver session structure
 * @param[in] n Number of the analog output port which enable signal will be read (AOEnablen)
 * @param[out] value   Current value of analog output enable
 * @param[out] status	Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_getAOEnable(irioDrv_t* p_DrvPvt,int n,int32_t* value,TStatus* status);

/**
 * Enable or disable an analog output
 *
 * Writes in a FPGA register to enable or disable the output for a analog output port.
 * Errors may occur if the port was not found or while writing to the port
 *
 * @param[in] p_DrvPvt 	Pointer to the driver session structure
 * @param[in] n Number of the analog output to enable/disable (AOEnablen)
 * @param[in] value 0=Disable Analog Output, 1=Enable Analog Output
 * @param[out] status	Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_setAOEnable(irioDrv_t* p_DrvPvt,int n,int32_t value,TStatus* status);

/**
 * Read an auxiliary analog output
 *
 * Reads the value from an auxiliary analog output port.
 * Errors may occur if the port was not found or while reading from the port
 *
 * @param[in] p_DrvPvt 	Pointer to the driver session structure
 * @param[in] n Number of the auxiliary analog output to read (auxAOn)
 * @param[out] value  Current value of the auxiliary analog output
 * @param[out] status	Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_getAuxAO(irioDrv_t* p_DrvPvt,int n,int32_t* value,TStatus* status);

/**
 * Write an auxiliary analog output
 *
 * Writes the given value in an auxiliary analog output port.
 * Errors may occur if the port was not found or while writing to the port.
 *
 * @param[in] p_DrvPvt 	Pointer to the driver session structure
 * @param[in] n Number of the auxiliary analog output to write (auxAOn)
 * @param[in] value Value to write
 * @param[out] status	Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_setAuxAO(irioDrv_t* p_DrvPvt,int n,int32_t value,TStatus* status);

#ifdef __cplusplus
}
#endif

#endif
