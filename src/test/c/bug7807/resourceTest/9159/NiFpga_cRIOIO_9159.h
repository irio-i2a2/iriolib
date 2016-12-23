/*
 * Generated with the FPGA Interface C API Generator 13.0.0
 * for NI-RIO 13.0.0 or later.
 */

#ifndef __NiFpga_cRIOIO_9159_h__
#define __NiFpga_cRIOIO_9159_h__

#ifndef NiFpga_Version
   #define NiFpga_Version 1300
#endif

#include "NiFpga.h"

/**
 * The filename of the FPGA bitfile.
 *
 * This is a #define to allow for string literal concatenation. For example:
 *
 *    static const char* const Bitfile = "C:\\" NiFpga_cRIOIO_9159_Bitfile;
 */
#define NiFpga_cRIOIO_9159_Bitfile "NiFpga_cRIOIO_9159.lvbitx"

/**
 * The signature of the FPGA bitfile.
 */
static const char* const NiFpga_cRIOIO_9159_Signature = "0AAC99310A046DA5E746222A2B4D08BF";

typedef enum
{
   NiFpga_cRIOIO_9159_IndicatorBool_DI0 = 0x813A,
   NiFpga_cRIOIO_9159_IndicatorBool_DI1 = 0x813E,
   NiFpga_cRIOIO_9159_IndicatorBool_DI2 = 0x816E,
   NiFpga_cRIOIO_9159_IndicatorBool_InitDone = 0x8176,
   NiFpga_cRIOIO_9159_IndicatorBool_auxDI0 = 0x814A,
   NiFpga_cRIOIO_9159_IndicatorBool_auxDI1 = 0x814E,
   NiFpga_cRIOIO_9159_IndicatorBool_cRIOModulesOK = 0x8182,
} NiFpga_cRIOIO_9159_IndicatorBool;

typedef enum
{
   NiFpga_cRIOIO_9159_IndicatorU8_DevProfile = 0x810E,
   NiFpga_cRIOIO_9159_IndicatorU8_DevQualityStatus = 0x818A,
   NiFpga_cRIOIO_9159_IndicatorU8_Platform = 0x819A,
   NiFpga_cRIOIO_9159_IndicatorU8_SGNo = 0x8152,
} NiFpga_cRIOIO_9159_IndicatorU8;

typedef enum
{
   NiFpga_cRIOIO_9159_IndicatorI16_DevTemp = 0x817E,
} NiFpga_cRIOIO_9159_IndicatorI16;

typedef enum
{
   NiFpga_cRIOIO_9159_IndicatorU16_state = 0x81A2,
} NiFpga_cRIOIO_9159_IndicatorU16;

typedef enum
{
   NiFpga_cRIOIO_9159_IndicatorI32_AI0 = 0x8110,
   NiFpga_cRIOIO_9159_IndicatorI32_AI1 = 0x8114,
   NiFpga_cRIOIO_9159_IndicatorI32_AI2 = 0x8158,
   NiFpga_cRIOIO_9159_IndicatorI32_auxAI0 = 0x8118,
   NiFpga_cRIOIO_9159_IndicatorI32_auxAI1 = 0x811C,
} NiFpga_cRIOIO_9159_IndicatorI32;

typedef enum
{
   NiFpga_cRIOIO_9159_IndicatorU32_Fref = 0x8178,
} NiFpga_cRIOIO_9159_IndicatorU32;

typedef enum
{
   NiFpga_cRIOIO_9159_ControlBool_AOEnable0 = 0x815E,
   NiFpga_cRIOIO_9159_ControlBool_AOEnable1 = 0x8162,
   NiFpga_cRIOIO_9159_ControlBool_AOEnable2 = 0x8166,
   NiFpga_cRIOIO_9159_ControlBool_DAQStartStop = 0x8192,
   NiFpga_cRIOIO_9159_ControlBool_DO0 = 0x8132,
   NiFpga_cRIOIO_9159_ControlBool_DO1 = 0x8136,
   NiFpga_cRIOIO_9159_ControlBool_DO2 = 0x816A,
   NiFpga_cRIOIO_9159_ControlBool_DebugMode = 0x818E,
   NiFpga_cRIOIO_9159_ControlBool_auxDO0 = 0x8142,
   NiFpga_cRIOIO_9159_ControlBool_auxDO1 = 0x8146,
} NiFpga_cRIOIO_9159_ControlBool;

typedef enum
{
   NiFpga_cRIOIO_9159_ControlU16_SamplingRate0 = 0x8172,
} NiFpga_cRIOIO_9159_ControlU16;

typedef enum
{
   NiFpga_cRIOIO_9159_ControlI32_AO0 = 0x8120,
   NiFpga_cRIOIO_9159_ControlI32_AO1 = 0x8124,
   NiFpga_cRIOIO_9159_ControlI32_AO2 = 0x8154,
   NiFpga_cRIOIO_9159_ControlI32_auxAO0 = 0x8128,
   NiFpga_cRIOIO_9159_ControlI32_auxAO1 = 0x812C,
} NiFpga_cRIOIO_9159_ControlI32;

typedef enum
{
   NiFpga_cRIOIO_9159_ControlU32_LoopuSec = 0x81A4,
   NiFpga_cRIOIO_9159_ControlU32_TabControl = 0x819C,
} NiFpga_cRIOIO_9159_ControlU32;

typedef enum
{
   NiFpga_cRIOIO_9159_IndicatorArrayU8_FPGAVIversion = 0x8186,
} NiFpga_cRIOIO_9159_IndicatorArrayU8;

typedef enum
{
   NiFpga_cRIOIO_9159_IndicatorArrayU8Size_FPGAVIversion = 2,
} NiFpga_cRIOIO_9159_IndicatorArrayU8Size;

typedef enum
{
   NiFpga_cRIOIO_9159_IndicatorArrayU16_InsertedIOModulesID = 0x8194,
} NiFpga_cRIOIO_9159_IndicatorArrayU16;

typedef enum
{
   NiFpga_cRIOIO_9159_IndicatorArrayU16Size_InsertedIOModulesID = 16,
} NiFpga_cRIOIO_9159_IndicatorArrayU16Size;

#endif
