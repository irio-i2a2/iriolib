/*******************************************************//**
 * \file irioResourceFinder.h
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

#ifndef IRIORESOURCEFINDER_H
#define IRIORESOURCEFINDER_H

#include "irioDataTypes.h"

#include <stdint.h>

/** @name Base Strings
 * Base Strings for FPGA Resource names
 */
///@{
#define STRINGNAME_PREFIX "NiFpga_"
#define STRINGNAME_BITFILEEXT ".lvbitx"
///@}

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Find the RIO device target
 *
 * Search the RIO device target identified by its serial number.
 * Map the device driver target port in irioDrv_t::DeviceSerialNumber
 * Errors may be prompted if:
 * \n Call to device driver fails
 * \n Can't create or delete a temporary file
 * \n No device with the given serial number is found
 *
 * @param[in,out] p_DrvPvt 	Pointer to the driver session structure
 * @param[out] status	Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_findRIO(irioDrv_t *p_DrvPvt,TStatus* status);

/**
 * Initialize file search resources
 *
 * Read the content of the given file into the pointer passed as parameter.
 * Memory for the file content will be allocated inside the method.
 * Errors may occur while managing the file or the memory.
 * Resources should be free using irio_closeFileSearch().
 *
 * @param[in] p_DrvPvt 	Pointer to the driver session structure
 * @param[in] filePath Path to the file
 * @param[out] fileContent Content of the file
 * @param[out] status	Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_initFileSearch(irioDrv_t *p_DrvPvt, char* filePath, void** fileContent,TStatus* status);

/**
 * Free file search resources
 *
 * Free the resources allocated in irio_initFileSearch().
 *
 * @param[in] p_DrvPvt 	Pointer to the driver session structure
 * @param[in] fileContent Pointer to the resources for file search
 * @param[out] status	Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_closeFileSearch(irioDrv_t *p_DrvPvt, void** fileContent,TStatus* status);

/**
 * Search resource port
 *
 * Search a resource port identified by its name (and number if necessary).
 * Map resource port in the given struct.
 * Errors will be prompted if the resource was not found or the port information could not be read.
 *
 * @param[in] p_DrvPvt 	Pointer to the driver session structure
 * @param[in] resourceName Name of the port to search
 * @param[in] index Number of the resource. Concatenated to resourceName if greater than -1.
 * @param[out] port Resource port
 * @param[out] status	Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_findResourceEnum(irioDrv_t *p_DrvPvt, const char* resourceName, int32_t index, TResourcePort* port,TStatus* status,int printErrMsg);

/**
 * Search string value in header file
 *
 * Searches the macro that defines a value for the given string in the file content.
 * Return macro value in given pointer. The memory for the value will be allocated inside the method.
 *
 * @param[in] p_DrvPvt 	Pointer to the driver session structure
 * @param[in] fileContent String where the macro will be searched
 * @param[in] toSearch Name of the string define to search
 * @param[out] str Place were to put the search results
 * @param[out] status	Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_findHeaderString(irioDrv_t *p_DrvPvt, char* fileContent, const char* toSearch, char** str,TStatus* status);

#ifdef __cplusplus
}
#endif

#endif
