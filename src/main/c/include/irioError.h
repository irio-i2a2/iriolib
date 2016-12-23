/**************************************************************************//**
 * \file irioError.h
 * \authors Mariano Ruiz (Universidad Politécnica de Madrid, UPM)
 * \authors Diego Sanz (Universidad Politécnica de Madrid, UPM)
 * \authors Sergio Esquembri (Universidad Politécnica de Madrid, UPM)
 * \authors Enrique Bernal (Universidad Politécnica de Madrid, UPM)
 * \brief Functions for error management
 * \date Nov., 2015
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

#ifndef IRIOERROR_H_
#define IRIOERROR_H_

#include "irioDataTypes.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initializes status struct
 *
 * Initializes status code to success and msg pointer to NULL.
 * @param status status to be initialized
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_initStatus(TStatus* status);

/**
 * Resets status struct
 *
 * Free msg and set it to null. Reset status codes to success.
 * @param status status to be reseted
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_resetStatus(TStatus* status);

/**
 * Merge current and previous status
 *
 * This method concatenates the given formated message in the previous state,
 * updating status code if necessary.
 * Errors may occur while managing memory for the messages.
 *
 * @param[in,out] status Previous status
 * @param[in] code Detail error code of the new status
 * @param[in] printMsg Print the new message
 * @param[in] format Format of the status message
 * @param[in] ... Status message (according to format)
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_mergeStatus(TStatus* status,TErrorDetailCode code, int printMsg, const char *format, ...);

/**
 * Get description of an error code
 *
 * This method allocates memory in str and fills it with the description of the error code given in error.
 *
 * @param[in] error Error
 * @param[out] str Place were to put the error description. Memory will be allocated inside this method and should be free by the caller.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_getErrorString(TErrorDetailCode error, char** str);

#ifdef __cplusplus
}
#endif

#endif /* IRIOUTIL_H_ */
