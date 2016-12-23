/*
 * Generated with the FPGA Interface C API Generator 13.0.0
 * for NI-RIO 13.0.0 or later.
 */

#ifndef __NiFpga_FlexRIO_GPUDAQ_7966_h__
#define __NiFpga_FlexRIO_GPUDAQ_7966_h__

#ifndef NiFpga_Version
   #define NiFpga_Version 1300
#endif

#include "NiFpga.h"

/**
 * The filename of the FPGA bitfile.
 *
 * This is a #define to allow for string literal concatenation. For example:
 *
 *    static const char* const Bitfile = "C:\\" NiFpga_FlexRIO_GPUDAQ_7966_Bitfile;
 */
#define NiFpga_FlexRIO_GPUDAQ_7966_Bitfile "NiFpga_FlexRIO_GPUDAQ_7966.lvbitx"

/**
 * The signature of the FPGA bitfile.
 */
static const char* const NiFpga_FlexRIO_GPUDAQ_7966_Signature = "D3E40A5B955D8BF539EC86ED87F7ADC1";

typedef enum
{
   NiFpga_FlexRIO_GPUDAQ_7966_IndicatorBool_DI0 = 0x56,
   NiFpga_FlexRIO_GPUDAQ_7966_IndicatorBool_DI1 = 0x52,
   NiFpga_FlexRIO_GPUDAQ_7966_IndicatorBool_InitDone = 0xD6,
   NiFpga_FlexRIO_GPUDAQ_7966_IndicatorBool_PXIIN0 = 0x42,
   NiFpga_FlexRIO_GPUDAQ_7966_IndicatorBool_PXIIN1 = 0x3A,
   NiFpga_FlexRIO_GPUDAQ_7966_IndicatorBool_RIOAdapterCorrect = 0xCE,
   NiFpga_FlexRIO_GPUDAQ_7966_IndicatorBool_auxDI0 = 0x4E,
   NiFpga_FlexRIO_GPUDAQ_7966_IndicatorBool_auxDI1 = 0x4A,
} NiFpga_FlexRIO_GPUDAQ_7966_IndicatorBool;

typedef enum
{
   NiFpga_FlexRIO_GPUDAQ_7966_IndicatorU8_DevProfile = 0xBE,
   NiFpga_FlexRIO_GPUDAQ_7966_IndicatorU8_DevQualityStatus = 0xC6,
   NiFpga_FlexRIO_GPUDAQ_7966_IndicatorU8_Platform = 0xB2,
   NiFpga_FlexRIO_GPUDAQ_7966_IndicatorU8_SGNo = 0x36,
} NiFpga_FlexRIO_GPUDAQ_7966_IndicatorU8;

typedef enum
{
   NiFpga_FlexRIO_GPUDAQ_7966_IndicatorI16_DevTemp = 0xC2,
} NiFpga_FlexRIO_GPUDAQ_7966_IndicatorI16;

typedef enum
{
   NiFpga_FlexRIO_GPUDAQ_7966_IndicatorU16_DMATtoGPUOverflows = 0x9A,
} NiFpga_FlexRIO_GPUDAQ_7966_IndicatorU16;

typedef enum
{
   NiFpga_FlexRIO_GPUDAQ_7966_IndicatorI32_AI0 = 0x8C,
   NiFpga_FlexRIO_GPUDAQ_7966_IndicatorI32_AI1 = 0x88,
   NiFpga_FlexRIO_GPUDAQ_7966_IndicatorI32_auxAI0 = 0x84,
   NiFpga_FlexRIO_GPUDAQ_7966_IndicatorI32_auxAI1 = 0x80,
} NiFpga_FlexRIO_GPUDAQ_7966_IndicatorI32;

typedef enum
{
   NiFpga_FlexRIO_GPUDAQ_7966_IndicatorU32_Fref = 0xC8,
   NiFpga_FlexRIO_GPUDAQ_7966_IndicatorU32_InsertedIOModuleID = 0xD0,
   NiFpga_FlexRIO_GPUDAQ_7966_IndicatorU32_SGFref0 = 0x1C,
   NiFpga_FlexRIO_GPUDAQ_7966_IndicatorU32_SGFref1 = 0x4,
} NiFpga_FlexRIO_GPUDAQ_7966_IndicatorU32;

typedef enum
{
   NiFpga_FlexRIO_GPUDAQ_7966_ControlBool_AOEnable0 = 0x6E,
   NiFpga_FlexRIO_GPUDAQ_7966_ControlBool_AOEnable1 = 0x6A,
   NiFpga_FlexRIO_GPUDAQ_7966_ControlBool_DAQStartStop = 0xBA,
   NiFpga_FlexRIO_GPUDAQ_7966_ControlBool_DMATtoGPUEnable0 = 0xA2,
   NiFpga_FlexRIO_GPUDAQ_7966_ControlBool_DMATtoGPUEnable1 = 0x92,
   NiFpga_FlexRIO_GPUDAQ_7966_ControlBool_DO0 = 0x66,
   NiFpga_FlexRIO_GPUDAQ_7966_ControlBool_DO1 = 0x62,
   NiFpga_FlexRIO_GPUDAQ_7966_ControlBool_DebugMode = 0xB6,
   NiFpga_FlexRIO_GPUDAQ_7966_ControlBool_PXIOUT0 = 0x46,
   NiFpga_FlexRIO_GPUDAQ_7966_ControlBool_PXIOUT1 = 0x3E,
   NiFpga_FlexRIO_GPUDAQ_7966_ControlBool_auxDO0 = 0x5E,
   NiFpga_FlexRIO_GPUDAQ_7966_ControlBool_auxDO1 = 0x5A,
} NiFpga_FlexRIO_GPUDAQ_7966_ControlBool;

typedef enum
{
   NiFpga_FlexRIO_GPUDAQ_7966_ControlU8_SGSignalType0 = 0x22,
   NiFpga_FlexRIO_GPUDAQ_7966_ControlU8_SGSignalType1 = 0xA,
} NiFpga_FlexRIO_GPUDAQ_7966_ControlU8;

typedef enum
{
   NiFpga_FlexRIO_GPUDAQ_7966_ControlU16_DMATtoGPUSamplingRate0 = 0x9E,
   NiFpga_FlexRIO_GPUDAQ_7966_ControlU16_DMATtoGPUSamplingRate1 = 0x96,
   NiFpga_FlexRIO_GPUDAQ_7966_ControlU16_SGAmp0 = 0x32,
   NiFpga_FlexRIO_GPUDAQ_7966_ControlU16_SGAmp1 = 0x1A,
} NiFpga_FlexRIO_GPUDAQ_7966_ControlU16;

typedef enum
{
   NiFpga_FlexRIO_GPUDAQ_7966_ControlI32_AO0 = 0x7C,
   NiFpga_FlexRIO_GPUDAQ_7966_ControlI32_AO1 = 0x78,
   NiFpga_FlexRIO_GPUDAQ_7966_ControlI32_auxAO0 = 0x74,
   NiFpga_FlexRIO_GPUDAQ_7966_ControlI32_auxAO1 = 0x70,
} NiFpga_FlexRIO_GPUDAQ_7966_ControlI32;

typedef enum
{
   NiFpga_FlexRIO_GPUDAQ_7966_ControlU32_SGFreq0 = 0x2C,
   NiFpga_FlexRIO_GPUDAQ_7966_ControlU32_SGFreq1 = 0x14,
   NiFpga_FlexRIO_GPUDAQ_7966_ControlU32_SGPhase0 = 0x28,
   NiFpga_FlexRIO_GPUDAQ_7966_ControlU32_SGPhase1 = 0x10,
   NiFpga_FlexRIO_GPUDAQ_7966_ControlU32_SGUpdateRate0 = 0x24,
   NiFpga_FlexRIO_GPUDAQ_7966_ControlU32_SGUpdateRate1 = 0xC,
} NiFpga_FlexRIO_GPUDAQ_7966_ControlU32;

typedef enum
{
   NiFpga_FlexRIO_GPUDAQ_7966_IndicatorArrayU8_DMATtoGPUFrameType = 0xAA,
   NiFpga_FlexRIO_GPUDAQ_7966_IndicatorArrayU8_DMATtoGPUSampleSize = 0xA6,
   NiFpga_FlexRIO_GPUDAQ_7966_IndicatorArrayU8_FPGAVIversion = 0xDA,
} NiFpga_FlexRIO_GPUDAQ_7966_IndicatorArrayU8;

typedef enum
{
   NiFpga_FlexRIO_GPUDAQ_7966_IndicatorArrayU8Size_DMATtoGPUFrameType = 2,
   NiFpga_FlexRIO_GPUDAQ_7966_IndicatorArrayU8Size_DMATtoGPUSampleSize = 2,
   NiFpga_FlexRIO_GPUDAQ_7966_IndicatorArrayU8Size_FPGAVIversion = 2,
} NiFpga_FlexRIO_GPUDAQ_7966_IndicatorArrayU8Size;

typedef enum
{
   NiFpga_FlexRIO_GPUDAQ_7966_IndicatorArrayU16_DMATtoGPUBlockNWords = 0x0,
   NiFpga_FlexRIO_GPUDAQ_7966_IndicatorArrayU16_DMATtoGPUNCh = 0xAC,
} NiFpga_FlexRIO_GPUDAQ_7966_IndicatorArrayU16;

typedef enum
{
   NiFpga_FlexRIO_GPUDAQ_7966_IndicatorArrayU16Size_DMATtoGPUBlockNWords = 2,
   NiFpga_FlexRIO_GPUDAQ_7966_IndicatorArrayU16Size_DMATtoGPUNCh = 2,
} NiFpga_FlexRIO_GPUDAQ_7966_IndicatorArrayU16Size;

typedef enum
{
   NiFpga_FlexRIO_GPUDAQ_7966_TargetToHostFifoU64_DMATtoGPU0 = 1,
   NiFpga_FlexRIO_GPUDAQ_7966_TargetToHostFifoU64_DMATtoGPU1 = 0,
} NiFpga_FlexRIO_GPUDAQ_7966_TargetToHostFifoU64;

#endif
