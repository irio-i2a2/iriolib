/*
 * Generated with the FPGA Interface C API Generator 13.0.0
 * for NI-RIO 13.0.0 or later.
 */

#ifndef __NiFpga_FlexRIO_GPUDAQ_h__
#define __NiFpga_FlexRIO_GPUDAQ_h__

#ifndef NiFpga_Version
   #define NiFpga_Version 1300
#endif

#include "NiFpga.h"

/**
 * The filename of the FPGA bitfile.
 *
 * This is a #define to allow for string literal concatenation. For example:
 *
 *    static const char* const Bitfile = "C:\\" NiFpga_FlexRIO_GPUDAQ_Bitfile;
 */
#define NiFpga_FlexRIO_GPUDAQ_Bitfile "NiFpga_FlexRIO_GPUDAQ.lvbitx"

/**
 * The signature of the FPGA bitfile.
 */
static const char* const NiFpga_FlexRIO_GPUDAQ_Signature = "BCE3AED4A1805F2ACE0FBA57A0060388";

typedef enum
{
   NiFpga_FlexRIO_GPUDAQ_IndicatorBool_DI0 = 0x5A,
   NiFpga_FlexRIO_GPUDAQ_IndicatorBool_DI1 = 0x56,
   NiFpga_FlexRIO_GPUDAQ_IndicatorBool_InitDone = 0xDA,
   NiFpga_FlexRIO_GPUDAQ_IndicatorBool_PXIIN0 = 0x46,
   NiFpga_FlexRIO_GPUDAQ_IndicatorBool_PXIIN1 = 0x3E,
   NiFpga_FlexRIO_GPUDAQ_IndicatorBool_RIOAdapterCorrect = 0xD2,
   NiFpga_FlexRIO_GPUDAQ_IndicatorBool_auxDI0 = 0x52,
   NiFpga_FlexRIO_GPUDAQ_IndicatorBool_auxDI1 = 0x4E,
} NiFpga_FlexRIO_GPUDAQ_IndicatorBool;

typedef enum
{
   NiFpga_FlexRIO_GPUDAQ_IndicatorU8_DevProfile = 0xC2,
   NiFpga_FlexRIO_GPUDAQ_IndicatorU8_DevQualityStatus = 0xCA,
   NiFpga_FlexRIO_GPUDAQ_IndicatorU8_Platform = 0xB6,
   NiFpga_FlexRIO_GPUDAQ_IndicatorU8_SGNo = 0x3A,
} NiFpga_FlexRIO_GPUDAQ_IndicatorU8;

typedef enum
{
   NiFpga_FlexRIO_GPUDAQ_IndicatorI16_DevTemp = 0xC6,
} NiFpga_FlexRIO_GPUDAQ_IndicatorI16;

typedef enum
{
   NiFpga_FlexRIO_GPUDAQ_IndicatorU16_DMATtoGPUOverflows = 0x9E,
} NiFpga_FlexRIO_GPUDAQ_IndicatorU16;

typedef enum
{
   NiFpga_FlexRIO_GPUDAQ_IndicatorI32_AI0 = 0x90,
   NiFpga_FlexRIO_GPUDAQ_IndicatorI32_AI1 = 0x8C,
   NiFpga_FlexRIO_GPUDAQ_IndicatorI32_auxAI0 = 0x88,
   NiFpga_FlexRIO_GPUDAQ_IndicatorI32_auxAI1 = 0x84,
} NiFpga_FlexRIO_GPUDAQ_IndicatorI32;

typedef enum
{
   NiFpga_FlexRIO_GPUDAQ_IndicatorU32_Fref = 0xCC,
   NiFpga_FlexRIO_GPUDAQ_IndicatorU32_InsertedIOModuleID = 0xD4,
   NiFpga_FlexRIO_GPUDAQ_IndicatorU32_SGFref0 = 0x20,
   NiFpga_FlexRIO_GPUDAQ_IndicatorU32_SGFref1 = 0x8,
} NiFpga_FlexRIO_GPUDAQ_IndicatorU32;

typedef enum
{
   NiFpga_FlexRIO_GPUDAQ_ControlBool_AOEnable0 = 0x72,
   NiFpga_FlexRIO_GPUDAQ_ControlBool_AOEnable1 = 0x6E,
   NiFpga_FlexRIO_GPUDAQ_ControlBool_DAQStartStop = 0xBE,
   NiFpga_FlexRIO_GPUDAQ_ControlBool_DMATtoGPUEnable0 = 0xA6,
   NiFpga_FlexRIO_GPUDAQ_ControlBool_DMATtoGPUEnable1 = 0x96,
   NiFpga_FlexRIO_GPUDAQ_ControlBool_DO0 = 0x6A,
   NiFpga_FlexRIO_GPUDAQ_ControlBool_DO1 = 0x66,
   NiFpga_FlexRIO_GPUDAQ_ControlBool_DebugMode = 0xBA,
   NiFpga_FlexRIO_GPUDAQ_ControlBool_PXIOUT0 = 0x4A,
   NiFpga_FlexRIO_GPUDAQ_ControlBool_PXIOUT1 = 0x42,
   NiFpga_FlexRIO_GPUDAQ_ControlBool_auxDO0 = 0x62,
   NiFpga_FlexRIO_GPUDAQ_ControlBool_auxDO1 = 0x5E,
} NiFpga_FlexRIO_GPUDAQ_ControlBool;

typedef enum
{
   NiFpga_FlexRIO_GPUDAQ_ControlU8_SGSignalType0 = 0x26,
   NiFpga_FlexRIO_GPUDAQ_ControlU8_SGSignalType1 = 0xE,
} NiFpga_FlexRIO_GPUDAQ_ControlU8;

typedef enum
{
   NiFpga_FlexRIO_GPUDAQ_ControlU16_DMATtoGPUSamplingRate0 = 0xA2,
   NiFpga_FlexRIO_GPUDAQ_ControlU16_DMATtoGPUSamplingRate1 = 0x9A,
   NiFpga_FlexRIO_GPUDAQ_ControlU16_SGAmp0 = 0x36,
   NiFpga_FlexRIO_GPUDAQ_ControlU16_SGAmp1 = 0x1E,
} NiFpga_FlexRIO_GPUDAQ_ControlU16;

typedef enum
{
   NiFpga_FlexRIO_GPUDAQ_ControlI32_AO0 = 0x80,
   NiFpga_FlexRIO_GPUDAQ_ControlI32_AO1 = 0x7C,
   NiFpga_FlexRIO_GPUDAQ_ControlI32_auxAO0 = 0x78,
   NiFpga_FlexRIO_GPUDAQ_ControlI32_auxAO1 = 0x74,
} NiFpga_FlexRIO_GPUDAQ_ControlI32;

typedef enum
{
   NiFpga_FlexRIO_GPUDAQ_ControlU32_SGFreq0 = 0x30,
   NiFpga_FlexRIO_GPUDAQ_ControlU32_SGFreq1 = 0x18,
   NiFpga_FlexRIO_GPUDAQ_ControlU32_SGPhase0 = 0x2C,
   NiFpga_FlexRIO_GPUDAQ_ControlU32_SGPhase1 = 0x14,
   NiFpga_FlexRIO_GPUDAQ_ControlU32_SGUpdateRate0 = 0x28,
   NiFpga_FlexRIO_GPUDAQ_ControlU32_SGUpdateRate1 = 0x10,
} NiFpga_FlexRIO_GPUDAQ_ControlU32;

typedef enum
{
   NiFpga_FlexRIO_GPUDAQ_IndicatorArrayU8_DMATtoGPUFrameType = 0xAE,
   NiFpga_FlexRIO_GPUDAQ_IndicatorArrayU8_DMATtoGPUSampleSize = 0xAA,
   NiFpga_FlexRIO_GPUDAQ_IndicatorArrayU8_FPGAVIversion = 0x2,
} NiFpga_FlexRIO_GPUDAQ_IndicatorArrayU8;

typedef enum
{
   NiFpga_FlexRIO_GPUDAQ_IndicatorArrayU8Size_DMATtoGPUFrameType = 2,
   NiFpga_FlexRIO_GPUDAQ_IndicatorArrayU8Size_DMATtoGPUSampleSize = 2,
   NiFpga_FlexRIO_GPUDAQ_IndicatorArrayU8Size_FPGAVIversion = 2,
} NiFpga_FlexRIO_GPUDAQ_IndicatorArrayU8Size;

typedef enum
{
   NiFpga_FlexRIO_GPUDAQ_IndicatorArrayU16_DMATtoGPUBlockNWords = 0x4,
   NiFpga_FlexRIO_GPUDAQ_IndicatorArrayU16_DMATtoGPUNCh = 0xB0,
} NiFpga_FlexRIO_GPUDAQ_IndicatorArrayU16;

typedef enum
{
   NiFpga_FlexRIO_GPUDAQ_IndicatorArrayU16Size_DMATtoGPUBlockNWords = 2,
   NiFpga_FlexRIO_GPUDAQ_IndicatorArrayU16Size_DMATtoGPUNCh = 2,
} NiFpga_FlexRIO_GPUDAQ_IndicatorArrayU16Size;

#endif
