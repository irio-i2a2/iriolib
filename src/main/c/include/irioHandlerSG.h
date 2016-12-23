/**************************************************************************//**
 * \file irioHandlerSG.h
 * \authors Mariano Ruiz (Universidad Politécnica de Madrid, UPM)
 * \authors Diego Sanz (Universidad Politécnica de Madrid, UPM)
 * \authors Sergio Esquembri (Universidad Politécnica de Madrid, UPM)
 * \authors Enrique Bernal (Universidad Politécnica de Madrid, UPM)
 * \authors Alvaro Bustos (Universidad Politécnica de Madrid, UPM)
 * \brief Waveform generator handler methods for IRIO driver
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

#ifndef IRIOHANDLERSG_H
#define IRIOHANDLERSG_H

#include "irioDataTypes.h"

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Search and map Signal Generators
 *
 * Searches for signal generators implemented for the current target.
 * Map ports found and signal generators information in:
 * 	\n irioDrv_t::enumSGNo
 * 	\n irioDrv_t::NoOfSG
 * 	\n irioDrv_t::enumSGFreq
 * 	\n irioDrv_t::enumSGAmp
 * 	\n irioDrv_t::enumSGPhase
 * 	\n irioDrv_t::enumSGSignalType
 * 	\n irioDrv_t::enumSGUpdateRate
 * 	\n irioDrv_t::enumSGFref
 *
 * Errors will be prompted if:
 * 	\n SGNo port is not found or while reading from the port
 *  \n If any of the resources associated with a signal generator
 *  (SGFreq, SGAmp, SGPhase, SGSignalType, SGUpdateRate, SGFref) were not found.
 *  \n Error occur while reading SGFref
 *  \n Not enough signal generators were found.
 *
 * NOTE: The value read from SGNo will determine the number of signal generators to search.
 * Signal Generators should be numbered consecutively starting from 0.
 *
 * @param[in,out] p_DrvPvt 	Pointer to the driver session structure
 * @param[out] status	Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_findSGs(irioDrv_t* p_DrvPvt,TStatus* status);

/**
 * Read signal generator signal type
 *
 * Reads the value from a signal generator signal type port.
 * Errors may occur if the port was not found or while reading from the port.
 *
 * @param[in] p_DrvPvt 	Pointer to the driver session structure
 * @param[in[ n Number of the waveform generator which signal type will be read (SGSignalTypen)
 * @param[out] value  Current value of signal generator signal type
 * @param[out] status	Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_getSGSignalType(irioDrv_t* p_DrvPvt,int n,int32_t* value,TStatus* status);

/**
 * Write signal generator signal type
 *
 * Writes the given value to a signal generator signal type port.
 * Errors may occur if the port was not found or while writing to the port.
 *
 * @param[in] p_DrvPvt 	Pointer to the driver session structure
 * @param[in] n Number of the waveform generator which signal type will be written(SGSignalTypen)
 * @param[in] value Signal type to set
 * @param[out] status	Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_setSGSignalType(irioDrv_t* p_DrvPvt,int n,int32_t value,TStatus* status);

/**
 * Read signal generator frequency
 *
 * Reads the value from a signal generator frequency port.
 * Errors may occur if the port was not found or while reading from the port.
 *
 * @param[in] p_DrvPvt 	Pointer to the driver session structure
 * @param[in[ n Number of the waveform generator which frequency will be read (SGFreq)
 * @param[out] value  Current value of signal generator frequency
 * @param[out] status	Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_getSGFreq(irioDrv_t* p_DrvPvt,int n,int32_t* value,TStatus* status);

/**
 * Write signal generator frequency
 *
 * Writes the given value to a signal generator frequency port.
 * Errors may occur if the port was not found or while writing to the port.
 *
 * @param[in] p_DrvPvt 	Pointer to the driver session structure
 * @param[in] n Number of the waveform generator which frequency will be written(SGFreq)
 * @param[in] value Frequency to set
 * @param[out] status	Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_setSGFreq(irioDrv_t* p_DrvPvt,int n,int32_t value,TStatus* status);

/**
 * Read signal generator phase shift
 *
 * Reads the value from a signal generator phase shift port.
 * Errors may occur if the port was not found or while reading from the port.
 *
 * @param[in] p_DrvPvt 	Pointer to the driver session structure
 * @param[in[ n Number of the waveform generator which phase shift will be read (SGPhaseShift)
 * @param[out] value  Current value of signal generator phase shift
 * @param[out] status	Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_getSGPhase(irioDrv_t* p_DrvPvt,int n,int32_t* value,TStatus* status);

/**
 * Write signal generator phase shift
 *
 * Writes the given value to a signal generator phase shift port.
 * Errors may occur if the port was not found or while writing to the port.
 *
 * @param[in] p_DrvPvt 	Pointer to the driver session structure
 * @param[in] n Number of the waveform generator which phase shift will be written(SGPhaseShift)
 * @param[in] value Phase shift to set
 * @param[out] status	Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_setSGPhase(irioDrv_t* p_DrvPvt,int n,int32_t value,TStatus* status);

/**
 * Read signal generator amplitude
 *
 * Reads the value from a signal generator amplitude port.
 * Errors may occur if the port was not found or while reading from the port.
 *
 * @param[in] p_DrvPvt 	Pointer to the driver session structure
 * @param[in[ n Number of the waveform generator which amplitude will be read (SGAmp)
 * @param[out] value  Current value of signal generator amplitude
 * @param[out] status	Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_getSGAmp(irioDrv_t* p_DrvPvt,int n,int32_t* value,TStatus* status);

/**
 * Write signal generator amplitude
 *
 * Writes the given value to a signal generator amplitude port.
 * Errors may occur if the port was not found or while writing to the port.
 *
 * @param[in] p_DrvPvt 	Pointer to the driver session structure
 * @param[in] n Number of the waveform generator which amplitude will be written(SGAmplitude)
 * @param[in] value Amplitude to set
 * @param[out] status	Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_setSGAmp(irioDrv_t* p_DrvPvt,int n,int32_t value,TStatus* status);

/**
 * Read signal generator update rate
 *
 * Reads the value from a signal generator update rate port.
 * Errors may occur if the port was not found or while reading from the port.
 *
 * @param[in] p_DrvPvt 	Pointer to the driver session structure
 * @param[in[ n Number of the waveform generator which update rate will be read (SGUpdateRate)
 * @param[out] value  Current value of signal generator update rate
 * @param[out] status	Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_getSGUpdateRate(irioDrv_t* p_DrvPvt,int n,int32_t* value,TStatus* status);

/**
 * Write signal generator update rate
 *
 * Writes the given value to a signal generator update rate port.
 * Errors may occur if the port was not found or while writing to the port.
 *
 * @param[in] p_DrvPvt 	Pointer to the driver session structure
 * @param[in] n Number of the waveform generator which update rate will be written(SGUpdateRate)
 * @param[in] value Update rate to set
 * @param[out] status	Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_setSGUpdateRate(irioDrv_t* p_DrvPvt,int n,int32_t value,TStatus* status);

#ifdef __cplusplus
}
#endif

#endif
