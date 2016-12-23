/**************************************************************************//**
 * \file irioDataTypes.h
 * \authors Mariano Ruiz (Universidad Politécnica de Madrid, UPM)
 * \authors Diego Sanz (Universidad Politécnica de Madrid, UPM)
 * \authors Sergio Esquembri (Universidad Politécnica de Madrid, UPM)
 * \authors Enrique Bernal (Universidad Politécnica de Madrid, UPM)
 * \authors Alvaro Bustos (Universidad Politécnica de Madrid, UPM)
 * \brief Typedefs and Macro definitions for IRIO Driver
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

#ifndef IRIODATATYPES_H
#define IRIODATATYPES_H

#include "NiFpga.h"

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @name Size and timeout constants
 * Definitions of maximum size for channel read, dmas and max timeout in reads
 */
///@{
#define SIZE_HOST_DMAS 2048000
#define FPGA_READ_BUFFER_TIMEOUT_1ms 1
#define NUMBEROFU64TOREADPERCHANNEL	4096
///@}

/**@name FlexRIO Adapter Modules Identifiers
 *
 */
///@{
#define FlexRIO_Module_IO_NI5761 0x109374C6 // NI5761: 0x109374C6, supported by ITER in AC version not DC, 4 analog inputs
#define FlexRIO_Module_IO_NI5781 0x1093748A // NI5781: 0x1093748A, not supported by ITER, 2 AI and 2 AO
#define FlexRIO_Module_IO_NI6581 0x10937418 // NI6581: 0x10937418, supported by ITER, digital I/O module
#define FlexRIO_Module_IO_NI5734 0x10937595 // NI5734: 0x10937595, not supported by ITER, 4AI + digital I/Os
///@}


/** @name Maximum FlexRIO resources
 * Definitions of the maximum number of resources that can be instantiated for FlexRIO platform.
 */
///@{
#define FLEXRIO_MAX_ANALOGS_IN 4
#define FLEXRIO_MAX_AUXA_IN 16
#define FLEXRIO_MAX_ANALOGS_OUT 2
#define FLEXRIO_MAX_AUXA_OUT 16
#define FLEXRIO_MAX_DIGITALS 90
#define FLEXRIO_MAX_AUXDIGITALS 16
#define FLEXRIO_MAX_DMAS 16
#define FLEXRIO_MAX_SIGNALGENERATOR 2
///@}

/** @name Maximum cRIO resources
 * Definitions of the maximum number of resources that can be instantiated for cRIO platform.
 */
///@{
#define CRIO_MAX_ANALOGS_IN 256
#define CRIO_MAX_AUXA_IN 256
#define CRIO_MAX_ANALOGS_OUT 256
#define CRIO_MAX_AUXA_OUT 256
#define CRIO_MAX_DIGITALS 256
#define CRIO_MAX_AUXDIGITALS 256
#define CRIO_MAX_DMAS 3
#define CRIO_MAX_SIGNALGENERATOR 256
#define CRIO_MAX_MODULES 16
///@}


#define U16BIT 65535
#define SHORT_CHAR_STRING 10
#define LONG_CHAR_STRING 40

/**
 * Enum Type for IRIO supported platforms
 */
typedef enum {
	IRIO_FlexRIO = 0,//!< FlexRIO Devices
	IRIO_cRIO,       //!< CompactRIO Devices
	IRIO_RSeries     //!< RSeries Devices
}TIRIOPlatforms;

/**
 * Enum Type for driver operation state
 *
 * Specify the result of a method call
 */
typedef enum  {
    IRIO_success = 0,   		//!< success The driver is working without problems
	IRIO_warning,				//!< warning Some problem happened. Operation might not be completed, but no need to shutdown or restart
	IRIO_error					//!< error Critical error happened and IRIO should not be used
}TIRIOStatusCode;

#define NUM_ERRORS 18

typedef enum {
	Generic_Error=-NUM_ERRORS,			//!< To be used by upper layers for custom errors in irio_mergeStatus
	HardwareNotFound_Error, 			//!< Given serial number was not found
	BitfileDownload_Error,				//!< Error downloading bitfile
	InitDone_Error, 					//!< Could not configure adapter module
	IOModule_Error,						//!< IO module/s check error
	NIRIO_API_Error, 					//!< Any error given as result of a call to NIRIO
	ListRIODevicesCommand_Error,		//!< Call to lsrio failed
	ListRIODevicesParsing_Error,		//!< Parse lsrio output failed
	SignatureNotFound_Error,			//!< Bitfile signature not found in header file
	ResourceNotFound_Error, 			//!< Mandatory resource not found in header file
	SignatureValueNotValid_Error,		//!< Parse bitfile signature failed
	ResourceValueNotValid_Error,		//!< Parse resource value failed
	MemoryAllocation_Error,				//!< System call for memory allocation failed
	BitfileNotFound_Error,				//!< Project bitfile was not found
	HeaderNotFound_Error,				//!< Project header file was not found
	FileAccess_Error,					//!< Access to a file failed
	FileNotFound_Error,					//!< Search for a file failed
	FeatureNotImplemented_Error, 		//!< Try to use RSeries devices or undefined profiles
	Success = 0,						//!< Everything is OK
	TemporaryFileDelete_Warning,		//!< Could not delete file in tmp generated by lsrio
	ResourceNotFound_Warning,			//!< Part of optional resource not found (e.g. AO0 Present but AOEnable0 not present)
	FPGAAlreadyRunning_Warning,			//!< Tried to change FPGAStart after FPGA was started/FPGA was started (bitfile downloaded was already in the FPGA)
	Read_NIRIO_Warning,					//!< Resource read failed in NIRIO call
	Read_Resource_Warning,				//!< Tried to access a non-present resource
	Write_NIRIO_Warning,				//!< Resource write failed in NIRIO call
	Write_Resource_Warning,				//!< Tried to write in a non-present resource
	ConfigDMA_Warning,					//!< Attempt to Configure, Start or Stop DMA failed
	ConfigUART_Warning,					//!< Timeout setting baudrate
	ValueOOB_Warning,					//!< Value given in configuration is out of bounds
	Generic_Warning,					//!< To be used by upper layers for custom warnings in irio_mergeStatus
	DAQtimeout_Warning,					//!< Timeout when waiting for data in data acquisition
	CLSLinetimeout_Warning,				//!< Timeout in cameralink reception of serial line data
	ResourceRelease_Warning				//!< NIRIO error while releasing Fifo elements
}TErrorDetailCode;

typedef enum {
	IRIO_coupling_AC=0,
	IRIO_coupling_DC,
	IRIO_coupling_NULL
}TIRIOCouplingMode;

/**
 * IRIO status structure
 *
 * Warning and error messages are concatenated in msg in each IRIO API call.
 * code will show the maximum level of message stored (error>warning>success). Code should always contain a valid pointer to allocated memory or NULL.
 */
typedef struct TStatus{
	TIRIOStatusCode code; 	//!< Current level of error
	TErrorDetailCode detailCode; //!< Status code for a more detailed information of the status. To be used in \ref irio_getErrorString
	char* msg;				//!< Stored log of errors and warnings. Should always contain a valid pointer to allocated memory or NULL.
}TStatus;

/**
 * Type for FPGA Resources
 *
 * Stores whether the resource was found or not and its offset
 */
typedef struct TResourcePort{
	uint8_t found;
	uint32_t value;
}TResourcePort;

/**
 * Type for managing GPU buffers
 *
 * Data type used for managing GPU DMAs
 */
typedef struct TFifoGPUInfo
{
		unsigned long long bytes;
		unsigned long buff_ptr;
		size_t	gpu_buff_size;
		void *	gpu_buff_ptr;
		int sel_mem;
}TFifoGPUInfo;

/**
 * Main struct of irioCore
 *
 * Stores all ports, the current session and the status.
 */
typedef struct irioDrv_t
{
	//Candidates for commonNirioStruct
	char* headerFile;				//!< Pointer to header file content
	char *appCallID; 				//!< EPICS device port
	char fpgaRIO[15];				//!< NiFpga driver port (RIO0,RIO1,...)
	char RIODeviceModel[20];		//!< Board Model (PXI-7965R,...)
	char DeviceSerialNumber[20]; 	//!< Board serial number
	int fpgaStarted;				//!< Indicates whether or not the FPGA has been started
	NiFpga_Session session; 		//!< Session obtained by C API to manage a FPGA
	char* projectName;				//!< Part of the name of the bitfile downloaded into the FPGA.
	int NiFpgaDriverType; 			//!< 0 For privative driver, 1 for open source driver
	int verbosity;					//!< Indicates whether or not print trace messages in IRIO API methods

	//Mandatory resources
	TResourcePort enumPlatform;						//!< Port for FPGA platform
	uint8_t platform;								//!< Indicates the type of platform been handled (cRIO, FlexRIO, ...)
	TResourcePort enumFPGAVIversion;				//!< Port for FPGA VI Version
	char FPGAVIversion[SHORT_CHAR_STRING];			//!< This Array Take the values from the FPGA array register: FPGAVIversion
	char FPGAVIStringversion[SHORT_CHAR_STRING]; 	//!< This Array is used to keep the number version to publish through its getter

	TResourcePort enumInitDone;						//!< Port for InitDone indicator
	NiFpga_Bool initDone;							//!< Indicates whether or not the FPGA was initialized properly
	uint32_t moduleValue; 							//!< Identifier of the module connected to the FPGA
		//FlexRIO Mandatory resources
	TResourcePort enumInsertedIoModuleID;			//!< Port for FlexRIO InsertedIOModuleID indicator
	TResourcePort enumRIOAdapterCorrect;			//!< Port for FlexRIO RIOAdapterCorrect indicator
	NiFpga_Bool RIOAdapterCorrect;					//!< Indicates if the FlexRIO adapter module connected is the expected one and if it was properly initialized
		//cRIO Mandatory resources
	TResourcePort enumInsertedModulesID;			//!< Port for cRIO InsertedModulesID indicator
	uint16_t *InsertedModulesID;					//!< Values of cRIO modules inserted
	TResourcePort enumcRIOModulesOk;				//!< Port for cRIOModulesOk indicator
	uint8_t  cRIOModulesOK;							//!< Indicates if all cRIO modules connected are the expected ones and if they were properly initialized
	TResourcePort *enumSamplingRate;				//!< Port for Point-By-Point sampling rates

	TResourcePort enumFref;					//!< Port for Fref
	uint32_t Fref; 							//!< Clock reference of the FPGA for signal generation
	TResourcePort enumDevQualityStatus;		//!< Port for DevQualityStatus
	TResourcePort enumDevTemp;				//!< Port for DevTemp
	TResourcePort enumDevProfile;			//!< Port for DevProfile
	uint8_t devProfile;						//!< Indicates the profile of the target (DAQ, IMAQ, PBP,...)
	TResourcePort enumDAQStartStop;			//!< Port for DAQStartStop
	TResourcePort enumDebugMode;			//!< Port for DebugMode

	//DAQ profile
	TResourcePort enumDMATtoHOSTNCh;			//!< Port for DMATtoHOSTNCh
	TResourcePort DMATtoHOSTNo;  				//!< Number of DMAs that has the FPGA for acquiring data in the host, this is the size of DMATtoHOSTNCh array
	uint16_t *DMATtoHOSTNCh; 					//!< Array that contains the number of Channels per DMA
	uint16_t *DMATtoHOSTChIndex; 				//!< Array containing the first channel indexed in each DMA
	TResourcePort enumDMATtoHOSTOverflows;		//!< Port for DMATtoHOSTOverflows
	TResourcePort *enumDMATtoHOST;				//!< Ports for DMATtoHOSTs
	TResourcePort *enumDMATtoHOSTEnable;		//!< Ports for DMATtoHOSTEnables
	TResourcePort *enumDMATtoHOSTSamplingRate;	//!< Ports for DMATtoHOSTSamplingRates
	TResourcePort enumDMATtoHOSTFrameType;		//!< Port for DMATtoHOSTFrameType
	uint8_t *DMATtoHOSTFrameType; 				//!< Array that contains the frame type used by the different DMAs
	TResourcePort enumDMATtoHOSTSampleSize;		//!< Port for DMATtoHOSTSampleSize
	uint8_t *DMATtoHOSTSampleSize; 				//!< Array that contains the sample size used by the different DMAs
	TResourcePort enumDMATtoHOSTBlockNWords;	//!< Port for DMATtoHOSTBlockNWords
	uint16_t *DMATtoHOSTBlockNWords;			//!< Array that contains size of DMA data blocks in terms of DMA words.
	uint16_t max_dmas;							//!< Maximum number of DMAs to be instantiated.


	//DAQGPU profile
	TResourcePort enumDMATtoGPUNCh;				//!< Port for enumDMATtoGPUNCh
	TResourcePort DMATtoGPUNo;      			//!< Number of DMAs that has the FPGA for acquiring data in the GPU, this is the size of DMATtoGPUNCh array
	uint16_t *DMATtoGPUNCh;         			//!< Array that contains the number of Channels per GPU_DMA
	uint16_t *DMATtoGPUChIndex;    				//!< Array containing the first channel indexed in each GPU_DMA
	TResourcePort enumDMATtoGPUOverflows;		//!< Port for DMATtoGPUOverflows
	TResourcePort *enumDMATtoGPU;				//!< Ports for DMATtoGPUs
	TResourcePort *enumDMATtoGPUEnable;			//!< Ports for DMATtoGPUEnables
	TResourcePort *enumDMATtoGPUSamplingRate;	//!< Ports for DMATtoGPUSamplingRates
	TResourcePort enumDMATtoGPUFrameType;		//!< Port for DMATtoGPUFrameType
	uint8_t *DMATtoGPUFrameType; 				//!< Array that contains the frame type used by the different GPU_DMAs
	TResourcePort enumDMATtoGPUSampleSize;		//!< Port for DMATtoGPUSampleSize
	uint8_t *DMATtoGPUSampleSize; 				//!< Array that contains the sample size used by the different GPU_DMAs
	TResourcePort enumDMATtoGPUBlockNWords;		//!< Port for DMATtoGPUBlockNWords
	uint16_t *DMATtoGPUBlockNWords;				//!< Array that contains size of GPU DMA data blocks in terms of DMA words.
	uint16_t max_dmas_gpu;						//!< Maximum number of GPU DMAs to be instantiated
	uint64_t *gpu_buffer		;					//!< GPU DMA buffer information

    //optional resources digital I/O
	TResourcePort *enumAnalogInput;		//!< Ports for AnalogInputs
	uint16_t max_analoginputs;			//!< Maximum number of analog inputs to be instantiated
	double CVADC; 						//!< Conversion to Volts of analog inputs
	TIRIOCouplingMode couplingMode;					//!< Coupling mode
	TResourcePort *enumAnalogOutput;	//!< Ports for AnalogOutputs
	TResourcePort *enumAOEnable;		//!< Ports for AOEnables
	uint16_t max_analogoutputs;			//!< Maximum number of analog outputs to be instantiated
	double CVDAC; 						//!< Conversion from Volts for analog outputs
	float maxAnalogOut; 				//!< Maximum value to be written in an analog output
	float minAnalogOut;					//!< Minimum value to be written in an analog output

	TResourcePort *enumDigitalInput;	//!< Ports for DigitalInputs
	uint16_t max_digitalsinputs;		//!< Maximum number of digital inputs to be instantiated

	TResourcePort *enumDigitalOutput;	//!< Ports for DigitalOutputs
	uint16_t max_digitalsoutputs;		//!< Maximum number of digital outputs to be instantiated

	TResourcePort *enumauxAI;			//!< Ports for auxAI
	uint16_t max_auxanaloginputs;		//!< Maximum number of auxiliary analog inputs to be instantiated

	TResourcePort *enumauxAO;			//!< Ports for auxAO
	uint16_t max_auxanalogoutputs;		//!< Maximum number of auxiliary analog outputs to be instantiated

	TResourcePort *enumauxDI;			//!< Ports for auxDI
	uint16_t max_auxdigitalsinputs;		//!< Maximum number of auxiliary digital inputs to be instantiated

	TResourcePort *enumauxDO;			//!< Ports for auxDO
	uint16_t max_auxdigitalsoutputs;	//!< Maximum number of auxiliary digital outputs to be instantiated

	//optional resources Signal generators,
	TResourcePort enumSGNo;				//!< Port for SGNo
	uint8_t NoOfSG;						//!< Number of signal generators instantiated
	uint16_t max_numberofSG;			//!< Maximum number of signal generators to be instantiated
	TResourcePort *enumSGFreq;			//!< Ports for SGFreq
	TResourcePort *enumSGAmp;			//!< Ports for SGAmp
	TResourcePort *enumSGPhase;			//!< Ports for SGPhase
	TResourcePort *enumSGSignalType;	//!< Ports for SGSignalType
	TResourcePort *enumSGUpdateRate;	//!< Ports for SGUpdateRate
	TResourcePort *enumSGFref;			//!< Ports for SGFref
	uint32_t *SGfref; 					//!< Reference frequency used for signal generation using DDS technique


	//CL Enums
	TResourcePort enumuartByteMode; 		//!< Port for uartByteMode
	TResourcePort enumuartSetBaudRate; 		//!< Port for uartSetBaudRate
	TResourcePort enumuartTransmit;			//!< Port for uartTransmit
	TResourcePort enumuartReceive;			//!< Port for uartReceive
	TResourcePort enumuartBaudRate;			//!< Port for uartBaudRate
	TResourcePort enumuartTxByte;			//!< Port for uartTxByte
	TResourcePort enumFVALHigh;				//!< Port for FVALHigh
	TResourcePort enumLVALHigh;				//!< Port for LVALHigh
	TResourcePort enumDVALHigh;				//!< Port for DVALHigh
	TResourcePort enumSpareHigh;			//!< Port for SpareHigh
	TResourcePort enumControlEnable;		//!< Port for ControlEnable
	TResourcePort enumLineScan;				//!< Port for LineScan
	TResourcePort enumSignalMapping;		//!< Port for SignalMapping
	TResourcePort enumConfiguration;		//!< Port for Configuration
	TResourcePort enumuartTxReady;			//!< Port for uartTxReady
	TResourcePort enumuartRxReady;			//!< Port for uartRxReady
	TResourcePort enumuartOverrunError;		//!< Port for uartOverrunError
	TResourcePort enumuartFramingError;		//!< Port for uartFrammingError
	TResourcePort enumuartBreakIndicator;	//!< Port for uartBreakIndicator
	TResourcePort enumuartRxByte;			//!< Port for uartRxByte

	float minSamplingRate;
	float maxSamplingRate;
}irioDrv_t;

/**
 * Type for cRIO modules
 *
 * Relates cRIO module names and Ids
 */
typedef struct cRIOmodule
{
	uint16_t id;
	char cmodule[7];
} cRIOmodule;

/**
 * Type for image time stamp
 *
 * Used to read image embedded time stamp and frame counter
 */
typedef struct imageData_t{
	uint16_t fc :16;
	uint64_t tsec :48;
	uint32_t tnsec :32;
}imageData_t;

#ifdef __cplusplus
}
#endif

#endif
