/*
 * Generated with the FPGA Interface C API Generator 13.0.0
 * for NI-RIO 13.0.0 or later.
 */

#ifndef __NiFpga_FlexRIO_perf_7966_h__
#define __NiFpga_FlexRIO_perf_7966_h__

#ifndef NiFpga_Version
   #define NiFpga_Version 1300
#endif

#include "NiFpga.h"

/**
 * The filename of the FPGA bitfile.
 *
 * This is a #define to allow for string literal concatenation. For example:
 *
 *    static const char* const Bitfile = "C:\\" NiFpga_FlexRIO_perf_7966_Bitfile;
 */
#define NiFpga_FlexRIO_perf_7966_Bitfile "NiFpga_FlexRIO_perf_7966.lvbitx"

/**
 * The signature of the FPGA bitfile.
 */
static const char* const NiFpga_FlexRIO_perf_7966_Signature = "4D9157A8A22A905F48BF1A55ECDC29C4";

typedef enum
{
   NiFpga_FlexRIO_perf_7966_IndicatorBool_InitDone = 0xEA,
   NiFpga_FlexRIO_perf_7966_IndicatorBool_RIOAdapterCorrect = 0xEE,
   NiFpga_FlexRIO_perf_7966_IndicatorBool_auxDI0 = 0x7A,
   NiFpga_FlexRIO_perf_7966_IndicatorBool_auxDI1 = 0x76,
   NiFpga_FlexRIO_perf_7966_IndicatorBool_auxDI2 = 0x72,
   NiFpga_FlexRIO_perf_7966_IndicatorBool_auxDI3 = 0x6E,
   NiFpga_FlexRIO_perf_7966_IndicatorBool_auxDI4 = 0x6A,
   NiFpga_FlexRIO_perf_7966_IndicatorBool_auxDI5 = 0x66,
   NiFpga_FlexRIO_perf_7966_IndicatorBool_auxDI6 = 0x42,
} NiFpga_FlexRIO_perf_7966_IndicatorBool;

typedef enum
{
   NiFpga_FlexRIO_perf_7966_IndicatorU8_DevProfile = 0xDE,
   NiFpga_FlexRIO_perf_7966_IndicatorU8_DevQualityStatus = 0x5E,
   NiFpga_FlexRIO_perf_7966_IndicatorU8_Platform = 0xFA,
   NiFpga_FlexRIO_perf_7966_IndicatorU8_SGNo = 0x62,
} NiFpga_FlexRIO_perf_7966_IndicatorU8;

typedef enum
{
   NiFpga_FlexRIO_perf_7966_IndicatorI16_DevTemp = 0x5A,
} NiFpga_FlexRIO_perf_7966_IndicatorI16;

typedef enum
{
   NiFpga_FlexRIO_perf_7966_IndicatorU16_DMATtoHOSTOverflows = 0x4E,
} NiFpga_FlexRIO_perf_7966_IndicatorU16;

typedef enum
{
   NiFpga_FlexRIO_perf_7966_IndicatorI32_auxAI0 = 0xA8,
   NiFpga_FlexRIO_perf_7966_IndicatorI32_auxAI1 = 0xA4,
   NiFpga_FlexRIO_perf_7966_IndicatorI32_auxAI10 = 0x0,
   NiFpga_FlexRIO_perf_7966_IndicatorI32_auxAI2 = 0xA0,
   NiFpga_FlexRIO_perf_7966_IndicatorI32_auxAI3 = 0x9C,
   NiFpga_FlexRIO_perf_7966_IndicatorI32_auxAI4 = 0x98,
   NiFpga_FlexRIO_perf_7966_IndicatorI32_auxAI5 = 0x94,
   NiFpga_FlexRIO_perf_7966_IndicatorI32_auxAI9 = 0x18,
} NiFpga_FlexRIO_perf_7966_IndicatorI32;

typedef enum
{
   NiFpga_FlexRIO_perf_7966_IndicatorU32_ExpectedIOModuleID = 0xF4,
   NiFpga_FlexRIO_perf_7966_IndicatorU32_Fref = 0xE4,
   NiFpga_FlexRIO_perf_7966_IndicatorU32_InsertedIOModuleID = 0xF0,
   NiFpga_FlexRIO_perf_7966_IndicatorU32_SGFref0 = 0xC4,
   NiFpga_FlexRIO_perf_7966_IndicatorU32_SGFref1 = 0xC8,
} NiFpga_FlexRIO_perf_7966_IndicatorU32;

typedef enum
{
   NiFpga_FlexRIO_perf_7966_ControlBool_AOEnable0 = 0x3A,
   NiFpga_FlexRIO_perf_7966_ControlBool_AOEnable1 = 0x1E,
   NiFpga_FlexRIO_perf_7966_ControlBool_DAQStartStop = 0x52,
   NiFpga_FlexRIO_perf_7966_ControlBool_DMATtoHOSTEnable0 = 0x46,
   NiFpga_FlexRIO_perf_7966_ControlBool_DebugMode = 0x56,
   NiFpga_FlexRIO_perf_7966_ControlBool_auxDO0 = 0x92,
   NiFpga_FlexRIO_perf_7966_ControlBool_auxDO1 = 0x8E,
   NiFpga_FlexRIO_perf_7966_ControlBool_auxDO2 = 0x8A,
   NiFpga_FlexRIO_perf_7966_ControlBool_auxDO3 = 0x7E,
   NiFpga_FlexRIO_perf_7966_ControlBool_auxDO4 = 0x82,
   NiFpga_FlexRIO_perf_7966_ControlBool_auxDO5 = 0x86,
} NiFpga_FlexRIO_perf_7966_ControlBool;

typedef enum
{
   NiFpga_FlexRIO_perf_7966_ControlU8_SGSignalType0 = 0x3E,
   NiFpga_FlexRIO_perf_7966_ControlU8_SGSignalType1 = 0x26,
} NiFpga_FlexRIO_perf_7966_ControlU8;

typedef enum
{
   NiFpga_FlexRIO_perf_7966_ControlU16_DMATtoHOSTSamplingRate0 = 0x4A,
   NiFpga_FlexRIO_perf_7966_ControlU16_SGAmp0 = 0x16,
   NiFpga_FlexRIO_perf_7966_ControlU16_SGAmp1 = 0x36,
} NiFpga_FlexRIO_perf_7966_ControlU16;

typedef enum
{
   NiFpga_FlexRIO_perf_7966_ControlI32_AO0 = 0x8,
   NiFpga_FlexRIO_perf_7966_ControlI32_AO1 = 0x28,
   NiFpga_FlexRIO_perf_7966_ControlI32_auxAO0 = 0xC0,
   NiFpga_FlexRIO_perf_7966_ControlI32_auxAO1 = 0xBC,
   NiFpga_FlexRIO_perf_7966_ControlI32_auxAO2 = 0xB8,
   NiFpga_FlexRIO_perf_7966_ControlI32_auxAO3 = 0xB4,
   NiFpga_FlexRIO_perf_7966_ControlI32_auxAO4 = 0xB0,
   NiFpga_FlexRIO_perf_7966_ControlI32_auxAO5 = 0xAC,
} NiFpga_FlexRIO_perf_7966_ControlI32;

typedef enum
{
   NiFpga_FlexRIO_perf_7966_ControlU32_SGFreq0 = 0x10,
   NiFpga_FlexRIO_perf_7966_ControlU32_SGFreq1 = 0x30,
   NiFpga_FlexRIO_perf_7966_ControlU32_SGPhase0 = 0xC,
   NiFpga_FlexRIO_perf_7966_ControlU32_SGPhase1 = 0x2C,
   NiFpga_FlexRIO_perf_7966_ControlU32_SGUpdateRate0 = 0x4,
   NiFpga_FlexRIO_perf_7966_ControlU32_SGUpdateRate1 = 0x20,
   NiFpga_FlexRIO_perf_7966_ControlU32_TabControl = 0xFC,
} NiFpga_FlexRIO_perf_7966_ControlU32;

typedef enum
{
   NiFpga_FlexRIO_perf_7966_IndicatorArrayU8_DMATtoHOSTFrameType = 0xD2,
   NiFpga_FlexRIO_perf_7966_IndicatorArrayU8_DMATtoHOSTSampleSize = 0xD6,
   NiFpga_FlexRIO_perf_7966_IndicatorArrayU8_FPGAVIversion = 0xE2,
} NiFpga_FlexRIO_perf_7966_IndicatorArrayU8;

typedef enum
{
   NiFpga_FlexRIO_perf_7966_IndicatorArrayU8Size_DMATtoHOSTFrameType = 1,
   NiFpga_FlexRIO_perf_7966_IndicatorArrayU8Size_DMATtoHOSTSampleSize = 1,
   NiFpga_FlexRIO_perf_7966_IndicatorArrayU8Size_FPGAVIversion = 2,
} NiFpga_FlexRIO_perf_7966_IndicatorArrayU8Size;

typedef enum
{
   NiFpga_FlexRIO_perf_7966_IndicatorArrayU16_DMATtoHOSTBlockNWords = 0xDA,
   NiFpga_FlexRIO_perf_7966_IndicatorArrayU16_DMATtoHOSTNCh = 0xCE,
} NiFpga_FlexRIO_perf_7966_IndicatorArrayU16;

typedef enum
{
   NiFpga_FlexRIO_perf_7966_IndicatorArrayU16Size_DMATtoHOSTBlockNWords = 1,
   NiFpga_FlexRIO_perf_7966_IndicatorArrayU16Size_DMATtoHOSTNCh = 1,
} NiFpga_FlexRIO_perf_7966_IndicatorArrayU16Size;

typedef enum
{
   NiFpga_FlexRIO_perf_7966_TargetToHostFifoU64_DMATtoHOST0 = 0,
} NiFpga_FlexRIO_perf_7966_TargetToHostFifoU64;

#endif
