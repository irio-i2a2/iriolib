/*
 * Generated with the FPGA Interface C API Generator 13.0.0
 * for NI-RIO 13.0.0 or later.
 */

#ifndef __NiFpga_FlexRIOMod5734_7966_h__
#define __NiFpga_FlexRIOMod5734_7966_h__

#ifndef NiFpga_Version
   #define NiFpga_Version 1300
#endif

#include "NiFpga.h"

/**
 * The filename of the FPGA bitfile.
 *
 * This is a #define to allow for string literal concatenation. For example:
 *
 *    static const char* const Bitfile = "C:\\" NiFpga_FlexRIOMod5734_7966_Bitfile;
 */
#define NiFpga_FlexRIOMod5734_7966_Bitfile "NiFpga_FlexRIOMod5734_7966.lvbitx"

/**
 * The signature of the FPGA bitfile.
 */
static const char* const NiFpga_FlexRIOMod5734_7966_Signature = "4CCA8F88F7DF78E25CC3A9AB579EA25C";

typedef enum
{
   NiFpga_FlexRIOMod5734_7966_IndicatorBool_InitDone = 0x10A,
   NiFpga_FlexRIOMod5734_7966_IndicatorBool_RIOAdapterCorrect = 0x11A,
   NiFpga_FlexRIOMod5734_7966_IndicatorBool_auxDI0 = 0x6E,
   NiFpga_FlexRIOMod5734_7966_IndicatorBool_auxDI1 = 0x6A,
   NiFpga_FlexRIOMod5734_7966_IndicatorBool_auxDI2 = 0x66,
   NiFpga_FlexRIOMod5734_7966_IndicatorBool_auxDI3 = 0x62,
   NiFpga_FlexRIOMod5734_7966_IndicatorBool_auxDI4 = 0x5E,
   NiFpga_FlexRIOMod5734_7966_IndicatorBool_auxDI5 = 0x5A,
   NiFpga_FlexRIOMod5734_7966_IndicatorBool_auxDI6 = 0xE2,
   NiFpga_FlexRIOMod5734_7966_IndicatorBool_auxDI7 = 0xD2,
} NiFpga_FlexRIOMod5734_7966_IndicatorBool;

typedef enum
{
   NiFpga_FlexRIOMod5734_7966_IndicatorU8_DevProfile = 0x10E,
   NiFpga_FlexRIOMod5734_7966_IndicatorU8_DevQualityStatus = 0x52,
   NiFpga_FlexRIOMod5734_7966_IndicatorU8_Platform = 0x11E,
   NiFpga_FlexRIOMod5734_7966_IndicatorU8_SGNo = 0xC2,
} NiFpga_FlexRIOMod5734_7966_IndicatorU8;

typedef enum
{
   NiFpga_FlexRIOMod5734_7966_IndicatorI16_DevTemp = 0x4E,
} NiFpga_FlexRIOMod5734_7966_IndicatorI16;

typedef enum
{
   NiFpga_FlexRIOMod5734_7966_IndicatorU16_DMATtoHOSTOverflows = 0x42,
   NiFpga_FlexRIOMod5734_7966_IndicatorU16_InputStateSPI = 0x126,
   NiFpga_FlexRIOMod5734_7966_IndicatorU16_OutputStateSPI = 0x122,
} NiFpga_FlexRIOMod5734_7966_IndicatorU16;

typedef enum
{
   NiFpga_FlexRIOMod5734_7966_IndicatorI32_auxAI0 = 0x9C,
   NiFpga_FlexRIOMod5734_7966_IndicatorI32_auxAI1 = 0x98,
   NiFpga_FlexRIOMod5734_7966_IndicatorI32_auxAI10 = 0xC8,
   NiFpga_FlexRIOMod5734_7966_IndicatorI32_auxAI2 = 0x94,
   NiFpga_FlexRIOMod5734_7966_IndicatorI32_auxAI3 = 0x90,
   NiFpga_FlexRIOMod5734_7966_IndicatorI32_auxAI4 = 0x8C,
   NiFpga_FlexRIOMod5734_7966_IndicatorI32_auxAI5 = 0x88,
   NiFpga_FlexRIOMod5734_7966_IndicatorI32_auxAI6 = 0xDC,
   NiFpga_FlexRIOMod5734_7966_IndicatorI32_auxAI7 = 0xE8,
   NiFpga_FlexRIOMod5734_7966_IndicatorI32_auxAI8 = 0xEC,
   NiFpga_FlexRIOMod5734_7966_IndicatorI32_auxAI9 = 0xC4,
} NiFpga_FlexRIOMod5734_7966_IndicatorI32;

typedef enum
{
   NiFpga_FlexRIOMod5734_7966_IndicatorU32_ExpectedIOModuleID = 0x110,
   NiFpga_FlexRIOMod5734_7966_IndicatorU32_Fref = 0x104,
   NiFpga_FlexRIOMod5734_7966_IndicatorU32_InsertedIOModuleID = 0x114,
   NiFpga_FlexRIOMod5734_7966_IndicatorU32_SGFref0 = 0xBC,
   NiFpga_FlexRIOMod5734_7966_IndicatorU32_SGFref1 = 0xB8,
} NiFpga_FlexRIOMod5734_7966_IndicatorU32;

typedef enum
{
   NiFpga_FlexRIOMod5734_7966_ControlBool_AOEnable0 = 0x32,
   NiFpga_FlexRIOMod5734_7966_ControlBool_AOEnable1 = 0x16,
   NiFpga_FlexRIOMod5734_7966_ControlBool_DAQStartStop = 0x46,
   NiFpga_FlexRIOMod5734_7966_ControlBool_DMATtoHOSTEnable0 = 0x3A,
   NiFpga_FlexRIOMod5734_7966_ControlBool_DebugMode = 0x4A,
   NiFpga_FlexRIOMod5734_7966_ControlBool_auxDO0 = 0x86,
   NiFpga_FlexRIOMod5734_7966_ControlBool_auxDO1 = 0x82,
   NiFpga_FlexRIOMod5734_7966_ControlBool_auxDO2 = 0x7E,
   NiFpga_FlexRIOMod5734_7966_ControlBool_auxDO3 = 0x72,
   NiFpga_FlexRIOMod5734_7966_ControlBool_auxDO4 = 0x76,
   NiFpga_FlexRIOMod5734_7966_ControlBool_auxDO5 = 0x7A,
   NiFpga_FlexRIOMod5734_7966_ControlBool_auxDO6 = 0x56,
} NiFpga_FlexRIOMod5734_7966_ControlBool;

typedef enum
{
   NiFpga_FlexRIOMod5734_7966_ControlU8_SGSignalType0 = 0x36,
   NiFpga_FlexRIOMod5734_7966_ControlU8_SGSignalType1 = 0x1E,
} NiFpga_FlexRIOMod5734_7966_ControlU8;

typedef enum
{
   NiFpga_FlexRIOMod5734_7966_ControlU16_DMATtoHOSTSamplingRate0 = 0x3E,
   NiFpga_FlexRIOMod5734_7966_ControlU16_SGAmp0 = 0x12,
   NiFpga_FlexRIOMod5734_7966_ControlU16_SGAmp1 = 0x2E,
} NiFpga_FlexRIOMod5734_7966_ControlU16;

typedef enum
{
   NiFpga_FlexRIOMod5734_7966_ControlI32_AO0 = 0x4,
   NiFpga_FlexRIOMod5734_7966_ControlI32_AO1 = 0x20,
   NiFpga_FlexRIOMod5734_7966_ControlI32_auxAO0 = 0xB4,
   NiFpga_FlexRIOMod5734_7966_ControlI32_auxAO1 = 0xB0,
   NiFpga_FlexRIOMod5734_7966_ControlI32_auxAO2 = 0xAC,
   NiFpga_FlexRIOMod5734_7966_ControlI32_auxAO3 = 0xA8,
   NiFpga_FlexRIOMod5734_7966_ControlI32_auxAO4 = 0xA4,
   NiFpga_FlexRIOMod5734_7966_ControlI32_auxAO5 = 0xA0,
   NiFpga_FlexRIOMod5734_7966_ControlI32_auxAO6 = 0xE4,
   NiFpga_FlexRIOMod5734_7966_ControlI32_auxAO7 = 0xD8,
   NiFpga_FlexRIOMod5734_7966_ControlI32_auxAO8 = 0xD4,
   NiFpga_FlexRIOMod5734_7966_ControlI32_auxAO9 = 0xCC,
} NiFpga_FlexRIOMod5734_7966_ControlI32;

typedef enum
{
   NiFpga_FlexRIOMod5734_7966_ControlU32_SGFreq0 = 0xC,
   NiFpga_FlexRIOMod5734_7966_ControlU32_SGFreq1 = 0x28,
   NiFpga_FlexRIOMod5734_7966_ControlU32_SGPhase0 = 0x8,
   NiFpga_FlexRIOMod5734_7966_ControlU32_SGPhase1 = 0x24,
   NiFpga_FlexRIOMod5734_7966_ControlU32_SGUpdateRate0 = 0x0,
   NiFpga_FlexRIOMod5734_7966_ControlU32_SGUpdateRate1 = 0x18,
   NiFpga_FlexRIOMod5734_7966_ControlU32_TabControl = 0x128,
} NiFpga_FlexRIOMod5734_7966_ControlU32;

typedef enum
{
   NiFpga_FlexRIOMod5734_7966_IndicatorArrayU8_DMATtoHOSTFrameType = 0xF6,
   NiFpga_FlexRIOMod5734_7966_IndicatorArrayU8_DMATtoHOSTSampleSize = 0xFA,
   NiFpga_FlexRIOMod5734_7966_IndicatorArrayU8_FPGAVIversion = 0x102,
} NiFpga_FlexRIOMod5734_7966_IndicatorArrayU8;

typedef enum
{
   NiFpga_FlexRIOMod5734_7966_IndicatorArrayU8Size_DMATtoHOSTFrameType = 1,
   NiFpga_FlexRIOMod5734_7966_IndicatorArrayU8Size_DMATtoHOSTSampleSize = 1,
   NiFpga_FlexRIOMod5734_7966_IndicatorArrayU8Size_FPGAVIversion = 2,
} NiFpga_FlexRIOMod5734_7966_IndicatorArrayU8Size;

typedef enum
{
   NiFpga_FlexRIOMod5734_7966_IndicatorArrayU16_DMATtoHOSTBlockNWords = 0xFE,
   NiFpga_FlexRIOMod5734_7966_IndicatorArrayU16_DMATtoHOSTNCh = 0xF2,
} NiFpga_FlexRIOMod5734_7966_IndicatorArrayU16;

typedef enum
{
   NiFpga_FlexRIOMod5734_7966_IndicatorArrayU16Size_DMATtoHOSTBlockNWords = 1,
   NiFpga_FlexRIOMod5734_7966_IndicatorArrayU16Size_DMATtoHOSTNCh = 1,
} NiFpga_FlexRIOMod5734_7966_IndicatorArrayU16Size;

typedef enum
{
   NiFpga_FlexRIOMod5734_7966_TargetToHostFifoU64_DMATtoHOST0 = 0,
} NiFpga_FlexRIOMod5734_7966_TargetToHostFifoU64;

#endif
