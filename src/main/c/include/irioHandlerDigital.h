/**************************************************************************//**
 * \file irioHandlerDigital.h
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

#ifndef IRIOHANDLERDIGITAL_H
#define IRIOHANDLERDIGITAL_H

#include "irioDataTypes.h"

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Search and map digital ports
 *
 * Searches for digital I/O ports implemented for the current target.
 * Maps ports found in the driver struct.
 * This method just call to irio_findDigitalInputs() and irio_findDigitalOutputs().
 *
 * @param[in,out] p_DrvPvt 	Pointer to the driver session structure
 * @param[out] status	Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_findDigitals(irioDrv_t* p_DrvPvt, TStatus* status);

/**
 * Search and map auxiliary digital ports
 *
 * Searches for auxiliary digital I/O ports implemented for the current target.
 * Maps ports found in the driver struct.
 * This method just call to irio_findAuxDigitalInputs() and irio_findAuxDigitalOutputs().
 *
 * @param[in,out] p_DrvPvt 	Pointer to the driver session structure
 * @param[out] status	Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_findAuxDigitals(irioDrv_t* p_DrvPvt, TStatus* status);

/**
 * Search and map digital input ports
 *
 * Searches for digital input ports implemented for the current target.
 * Maps ports found in irioDrv_t::enumDigitalInput.
 * No error will be prompted for missing digital input ports.
 *
 * @param[in,out] p_DrvPvt 	Pointer to the driver session structure
 * @param[out] status	Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_findDigitalInputs(irioDrv_t* p_DrvPvt, TStatus* status);

/**
 * Search and map auxiliary digital input ports
 *
 * Searches for auxiliary digital input ports implemented for the current target.
 * Maps ports found in irioDrv_t::enumauxDI.
 * No error will be prompted for missing auxiliary digital input ports.
 *
 * @param[in,out] p_DrvPvt 	Pointer to the driver session structure
 * @param[out] status	Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_findAuxDigitalInputs(irioDrv_t* p_DrvPvt, TStatus* status);


/**
 * Search and map digital output ports
 *
 * Searches for digital output ports implemented for the current target.
 * Maps ports found in irioDrv_t::enumDigitalOutput.
 * No error will be prompted for missing digital output ports.
 *
 * @param[in,out] p_DrvPvt 	Pointer to the driver session structure
 * @param[out] status	Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_findDigitalOutputs(irioDrv_t* p_DrvPvt, TStatus* status);

/**
 * Search and map auxiliary digital output ports
 *
 * Searches for auxiliary digital output ports implemented for the current target.
 * Maps ports found in irioDrv_t::enumauxDO.
 * No error will be prompted for missing auxiliary digital output ports.
 *
 * @param[in,out] p_DrvPvt 	Pointer to the driver session structure
 * @param[out] status	Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_findAuxDigitalOutputs(irioDrv_t* p_DrvPvt, TStatus* status);

/**
 * Read a Digital input
 *
 * Reads the value from a digital input port.
 * Errors may occur if the port was not found or while reading from the port.
 *
 * @param[in] p_DrvPvt 	Pointer to the driver session structure
 * @param[in] n Number of the digital input port to read (DIn)
 * @param[out] value Current value of the digital input
 * @param[out] status	Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_getDI(irioDrv_t* p_DrvPvt,int n,int32_t* value, TStatus* status);

/**
 * Read an auxiliary digital input
 *
 * Reads the value from an auxiliary digital input port.
 * Errors may occur if the port was not found or while reading from the port.
 *
 * @param[in] p_DrvPvt 	Pointer to the driver session structure
 * @param[in] n Number of the auxiliary digital input port to read (auxDIn)
 * @param[out] value  Current value of the auxiliary digital input
 * @param[out] status	Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_getAuxDI(irioDrv_t* p_DrvPvt,int n,int32_t* value, TStatus* status);

/**
 * Reads a digital output
 *
 * Reads the value from a digital output port.
 * Errors may occur if the port was not found or while reading from the port.
 *
 * @param[in] p_DrvPvt 	Pointer to the driver session structure
 * @param[in] n Number of the digital output port to read (DOn)
 * @param[out] value  Current value of the digital output
 * @param[out] status	Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_getDO(irioDrv_t* p_DrvPvt,int n,int32_t* value, TStatus* status);

/**
 * Write a digital output
 *
 * Writes the given value in a digital output port.
 * Errors may occur if the port was not found or while writing to the port.
 *
 * @param[in] p_DrvPvt 	Pointer to the driver session structure
 * @param[in] n Number of the digital output to write (DOn)
 * @param[in] value Value to set. Will be converted to NiFpga_Bool (0=NiFpga_False, other=NiFpga_True)
 * @param[out] status Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_setDO(irioDrv_t* p_DrvPvt,int n,int32_t value, TStatus* status);

/**
 * Read an auxiliary digital output
 *
 * Reads the value from the auxiliary digital output port.
 * Errors may occur if the port was not found or while reading from the port.
 *
 * @param[in] p_DrvPvt 	Pointer to the driver session structure
 * @param[in] n Number of the auxiliary digital output port to read (auxDOn)
 * @param[out] value  Current value of the auxiliary digital output
 * @param[out] status	Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_getAuxDO(irioDrv_t* p_DrvPvt,int n,int32_t* value, TStatus* status);

/**
 * Write an auxiliary digital output
 *
 * Writes the given value in an auxiliary digital output port.
 * Errors may occur if the port was not found or while writing to the port.
 *
 * @param[in] p_DrvPvt 	Pointer to the driver session structure
 * @param[in] n Number of the auxiliary digital output to write (auxDOn)
 * @param[in] value Value to set. Will be converted to NiFpga_Bool (0=NiFpga_False, other=NiFpga_True)
 * @param[out] status Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_setAuxDO(irioDrv_t* p_DrvPvt,int n,int32_t value, TStatus* status);

#ifdef __cplusplus
}
#endif

#endif
