/*
 * Generated with the FPGA Interface C API Generator 13.0.0
 * for NI-RIO 13.0.0 or later.
 */

#ifndef __NiFpga_cRIODAQDMA_9159_h__
#define __NiFpga_cRIODAQDMA_9159_h__

#ifndef NiFpga_Version
   #define NiFpga_Version 1300
#endif

#include "NiFpga.h"

/**
 * The filename of the FPGA bitfile.
 *
 * This is a #define to allow for string literal concatenation. For example:
 *
 *    static const char* const Bitfile = "C:\\" NiFpga_cRIODAQDMA_9159_Bitfile;
 */
#define NiFpga_cRIODAQDMA_9159_Bitfile "NiFpga_cRIODAQDMA_9159.lvbitx"

/**
 * The signature of the FPGA bitfile.
 */
static const char* const NiFpga_cRIODAQDMA_9159_Signature = "34FC3BC59783A5916BE838B70093D8F5";

typedef enum
{
   NiFpga_cRIODAQDMA_9159_IndicatorBool_DI0 = 0x8156,
   NiFpga_cRIODAQDMA_9159_IndicatorBool_DI1 = 0x815A,
   NiFpga_cRIODAQDMA_9159_IndicatorBool_DI2 = 0x8172,
   NiFpga_cRIODAQDMA_9159_IndicatorBool_InitDone = 0x8192,
   NiFpga_cRIODAQDMA_9159_IndicatorBool_auxDI0 = 0x8166,
   NiFpga_cRIODAQDMA_9159_IndicatorBool_cRIOModulesOK = 0x81B6,
} NiFpga_cRIODAQDMA_9159_IndicatorBool;

typedef enum
{
   NiFpga_cRIODAQDMA_9159_IndicatorU8_DevProfile = 0x819A,
   NiFpga_cRIODAQDMA_9159_IndicatorU8_DevQualityStatus = 0x81A6,
   NiFpga_cRIODAQDMA_9159_IndicatorU8_Platform = 0x81BA,
   NiFpga_cRIODAQDMA_9159_IndicatorU8_SGNo = 0x810E,
} NiFpga_cRIODAQDMA_9159_IndicatorU8;

typedef enum
{
   NiFpga_cRIODAQDMA_9159_IndicatorI16_DevTemp = 0x819E,
} NiFpga_cRIODAQDMA_9159_IndicatorI16;

typedef enum
{
   NiFpga_cRIODAQDMA_9159_IndicatorU16_DMATtoHOSTOverflows = 0x818A,
   NiFpga_cRIODAQDMA_9159_IndicatorU16_state = 0x81C2,
} NiFpga_cRIODAQDMA_9159_IndicatorU16;

typedef enum
{
   NiFpga_cRIODAQDMA_9159_IndicatorI32_AI0 = 0x8138,
   NiFpga_cRIODAQDMA_9159_IndicatorI32_AI1 = 0x8134,
   NiFpga_cRIODAQDMA_9159_IndicatorI32_AI2 = 0x814C,
   NiFpga_cRIODAQDMA_9159_IndicatorI32_auxAI0 = 0x813C,
} NiFpga_cRIODAQDMA_9159_IndicatorI32;

typedef enum
{
   NiFpga_cRIODAQDMA_9159_IndicatorU32_Fref = 0x8194,
   NiFpga_cRIODAQDMA_9159_IndicatorU32_SGFref0 = 0x8150,
} NiFpga_cRIODAQDMA_9159_IndicatorU32;

typedef enum
{
   NiFpga_cRIODAQDMA_9159_ControlBool_AOEnable0 = 0x8116,
   NiFpga_cRIODAQDMA_9159_ControlBool_AOEnable1 = 0x8132,
   NiFpga_cRIODAQDMA_9159_ControlBool_AOEnable2 = 0x814A,
   NiFpga_cRIODAQDMA_9159_ControlBool_DAQStartStop = 0x81AE,
   NiFpga_cRIODAQDMA_9159_ControlBool_DMATtoHOSTEnable0 = 0x8186,
   NiFpga_cRIODAQDMA_9159_ControlBool_DO0 = 0x815E,
   NiFpga_cRIODAQDMA_9159_ControlBool_DO1 = 0x8162,
   NiFpga_cRIODAQDMA_9159_ControlBool_DO2 = 0x816E,
   NiFpga_cRIODAQDMA_9159_ControlBool_DebugMode = 0x81AA,
   NiFpga_cRIODAQDMA_9159_ControlBool_auxDO0 = 0x816A,
} NiFpga_cRIODAQDMA_9159_ControlBool;

typedef enum
{
   NiFpga_cRIODAQDMA_9159_ControlU8_SGSignalType0 = 0x811A,
} NiFpga_cRIODAQDMA_9159_ControlU8;

typedef enum
{
   NiFpga_cRIODAQDMA_9159_ControlU16_DMATtoHOSTSamplingRate0 = 0x8182,
   NiFpga_cRIODAQDMA_9159_ControlU16_SGAmp0 = 0x812A,
} NiFpga_cRIODAQDMA_9159_ControlU16;

typedef enum
{
   NiFpga_cRIODAQDMA_9159_ControlI32_AO0 = 0x811C,
   NiFpga_cRIODAQDMA_9159_ControlI32_AO1 = 0x8110,
   NiFpga_cRIODAQDMA_9159_ControlI32_AO2 = 0x8144,
   NiFpga_cRIODAQDMA_9159_ControlI32_auxAO0 = 0x8140,
} NiFpga_cRIODAQDMA_9159_ControlI32;

typedef enum
{
   NiFpga_cRIODAQDMA_9159_ControlU32_LoopuSec = 0x81C4,
   NiFpga_cRIODAQDMA_9159_ControlU32_SGFreq0 = 0x8124,
   NiFpga_cRIODAQDMA_9159_ControlU32_SGPhase0 = 0x8120,
   NiFpga_cRIODAQDMA_9159_ControlU32_SGUpdateRate0 = 0x812C,
   NiFpga_cRIODAQDMA_9159_ControlU32_TabControl = 0x81BC,
} NiFpga_cRIODAQDMA_9159_ControlU32;

typedef enum
{
   NiFpga_cRIODAQDMA_9159_IndicatorArrayU8_DMATtoHOSTFrameType = 0x817A,
   NiFpga_cRIODAQDMA_9159_IndicatorArrayU8_DMATtoHOSTSampleSize = 0x817E,
   NiFpga_cRIODAQDMA_9159_IndicatorArrayU8_FPGAVIversion = 0x81A2,
} NiFpga_cRIODAQDMA_9159_IndicatorArrayU8;

typedef enum
{
   NiFpga_cRIODAQDMA_9159_IndicatorArrayU8Size_DMATtoHOSTFrameType = 1,
   NiFpga_cRIODAQDMA_9159_IndicatorArrayU8Size_DMATtoHOSTSampleSize = 1,
   NiFpga_cRIODAQDMA_9159_IndicatorArrayU8Size_FPGAVIversion = 2,
} NiFpga_cRIODAQDMA_9159_IndicatorArrayU8Size;

typedef enum
{
   NiFpga_cRIODAQDMA_9159_IndicatorArrayU16_DMATtoHOSTBlockNWords = 0x818E,
   NiFpga_cRIODAQDMA_9159_IndicatorArrayU16_DMATtoHOSTNCh = 0x8176,
   NiFpga_cRIODAQDMA_9159_IndicatorArrayU16_InsertedIOModulesID = 0x81B0,
} NiFpga_cRIODAQDMA_9159_IndicatorArrayU16;

typedef enum
{
   NiFpga_cRIODAQDMA_9159_IndicatorArrayU16Size_DMATtoHOSTBlockNWords = 1,
   NiFpga_cRIODAQDMA_9159_IndicatorArrayU16Size_DMATtoHOSTNCh = 1,
   NiFpga_cRIODAQDMA_9159_IndicatorArrayU16Size_InsertedIOModulesID = 16,
} NiFpga_cRIODAQDMA_9159_IndicatorArrayU16Size;

typedef enum
{
   NiFpga_cRIODAQDMA_9159_TargetToHostFifoU64_DMATtoHOST0 = 0,
} NiFpga_cRIODAQDMA_9159_TargetToHostFifoU64;

#endif
