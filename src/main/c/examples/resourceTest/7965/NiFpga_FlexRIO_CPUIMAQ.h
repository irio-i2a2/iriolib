/*
 * Generated with the FPGA Interface C API Generator 13.0.0
 * for NI-RIO 13.0.0 or later.
 */

#ifndef __NiFpga_FlexRIO_CPUIMAQ_h__
#define __NiFpga_FlexRIO_CPUIMAQ_h__

#ifndef NiFpga_Version
   #define NiFpga_Version 1300
#endif

#include "NiFpga.h"

/**
 * The filename of the FPGA bitfile.
 *
 * This is a #define to allow for string literal concatenation. For example:
 *
 *    static const char* const Bitfile = "C:\\" NiFpga_FlexRIO_CPUIMAQ_Bitfile;
 */
#define NiFpga_FlexRIO_CPUIMAQ_Bitfile "NiFpga_FlexRIO_CPUIMAQ.lvbitx"

/**
 * The signature of the FPGA bitfile.
 */
static const char* const NiFpga_FlexRIO_CPUIMAQ_Signature = "4DC5A06144362A10577C847978954F35";

typedef enum
{
   NiFpga_FlexRIO_CPUIMAQ_IndicatorBool_DI0 = 0x72,
   NiFpga_FlexRIO_CPUIMAQ_IndicatorBool_DI1 = 0x6E,
   NiFpga_FlexRIO_CPUIMAQ_IndicatorBool_InitDone = 0xCE,
   NiFpga_FlexRIO_CPUIMAQ_IndicatorBool_PXIIN0 = 0x5E,
   NiFpga_FlexRIO_CPUIMAQ_IndicatorBool_PXIIN1 = 0x56,
   NiFpga_FlexRIO_CPUIMAQ_IndicatorBool_RIOAdapterCorrect = 0xC6,
   NiFpga_FlexRIO_CPUIMAQ_IndicatorBool_auxDI0 = 0x6A,
   NiFpga_FlexRIO_CPUIMAQ_IndicatorBool_auxDI1 = 0x66,
   NiFpga_FlexRIO_CPUIMAQ_IndicatorBool_uartBreakIndicator = 0xE,
   NiFpga_FlexRIO_CPUIMAQ_IndicatorBool_uartFramingError = 0xA,
   NiFpga_FlexRIO_CPUIMAQ_IndicatorBool_uartOverrunError = 0x6,
   NiFpga_FlexRIO_CPUIMAQ_IndicatorBool_uartRxReady = 0x1A,
   NiFpga_FlexRIO_CPUIMAQ_IndicatorBool_uartTxReady = 0x16,
} NiFpga_FlexRIO_CPUIMAQ_IndicatorBool;

typedef enum
{
   NiFpga_FlexRIO_CPUIMAQ_IndicatorU8_DevProfile = 0xB6,
   NiFpga_FlexRIO_CPUIMAQ_IndicatorU8_DevQualityStatus = 0xBE,
   NiFpga_FlexRIO_CPUIMAQ_IndicatorU8_Platform = 0xAA,
   NiFpga_FlexRIO_CPUIMAQ_IndicatorU8_uartRxByte = 0x12,
} NiFpga_FlexRIO_CPUIMAQ_IndicatorU8;

typedef enum
{
   NiFpga_FlexRIO_CPUIMAQ_IndicatorI16_DevTemp = 0xBA,
} NiFpga_FlexRIO_CPUIMAQ_IndicatorI16;

typedef enum
{
   NiFpga_FlexRIO_CPUIMAQ_IndicatorU16_DMATtoHOSTOverflows = 0x96,
} NiFpga_FlexRIO_CPUIMAQ_IndicatorU16;

typedef enum
{
   NiFpga_FlexRIO_CPUIMAQ_IndicatorI32_auxAI0 = 0x90,
   NiFpga_FlexRIO_CPUIMAQ_IndicatorI32_auxAI1 = 0x8C,
} NiFpga_FlexRIO_CPUIMAQ_IndicatorI32;

typedef enum
{
   NiFpga_FlexRIO_CPUIMAQ_IndicatorU32_Fref = 0xC0,
   NiFpga_FlexRIO_CPUIMAQ_IndicatorU32_InsertedIOModuleID = 0xC8,
} NiFpga_FlexRIO_CPUIMAQ_IndicatorU32;

typedef enum
{
   NiFpga_FlexRIO_CPUIMAQ_ControlBool_ControlEnable = 0x36,
   NiFpga_FlexRIO_CPUIMAQ_ControlBool_DAQStartStop = 0xB2,
   NiFpga_FlexRIO_CPUIMAQ_ControlBool_DMATtoHOSTEnable0 = 0x9A,
   NiFpga_FlexRIO_CPUIMAQ_ControlBool_DO0 = 0x82,
   NiFpga_FlexRIO_CPUIMAQ_ControlBool_DO1 = 0x7E,
   NiFpga_FlexRIO_CPUIMAQ_ControlBool_DVALHigh = 0x4A,
   NiFpga_FlexRIO_CPUIMAQ_ControlBool_DebugMode = 0xAE,
   NiFpga_FlexRIO_CPUIMAQ_ControlBool_FVALHigh = 0x46,
   NiFpga_FlexRIO_CPUIMAQ_ControlBool_LVALHigh = 0x42,
   NiFpga_FlexRIO_CPUIMAQ_ControlBool_LineScan = 0x3A,
   NiFpga_FlexRIO_CPUIMAQ_ControlBool_PXIOUT0 = 0x62,
   NiFpga_FlexRIO_CPUIMAQ_ControlBool_PXIOUT1 = 0x5A,
   NiFpga_FlexRIO_CPUIMAQ_ControlBool_SpareHigh = 0x3E,
   NiFpga_FlexRIO_CPUIMAQ_ControlBool_auxDO0 = 0x7A,
   NiFpga_FlexRIO_CPUIMAQ_ControlBool_auxDO1 = 0x76,
   NiFpga_FlexRIO_CPUIMAQ_ControlBool_uartByteMode = 0x22,
   NiFpga_FlexRIO_CPUIMAQ_ControlBool_uartReceive = 0x2E,
   NiFpga_FlexRIO_CPUIMAQ_ControlBool_uartSetBaudRate = 0x2A,
   NiFpga_FlexRIO_CPUIMAQ_ControlBool_uartTransmit = 0x32,
} NiFpga_FlexRIO_CPUIMAQ_ControlBool;

typedef enum
{
   NiFpga_FlexRIO_CPUIMAQ_ControlU8_Configuration = 0x52,
   NiFpga_FlexRIO_CPUIMAQ_ControlU8_SignalMapping = 0x4E,
   NiFpga_FlexRIO_CPUIMAQ_ControlU8_uartBaudRate = 0x26,
   NiFpga_FlexRIO_CPUIMAQ_ControlU8_uartTxByte = 0x1E,
} NiFpga_FlexRIO_CPUIMAQ_ControlU8;

typedef enum
{
   NiFpga_FlexRIO_CPUIMAQ_ControlI32_auxAO0 = 0x88,
   NiFpga_FlexRIO_CPUIMAQ_ControlI32_auxAO1 = 0x84,
} NiFpga_FlexRIO_CPUIMAQ_ControlI32;

typedef enum
{
   NiFpga_FlexRIO_CPUIMAQ_IndicatorArrayU8_DMATtoHOSTFrameType = 0xA2,
   NiFpga_FlexRIO_CPUIMAQ_IndicatorArrayU8_DMATtoHOSTSampleSize = 0x9E,
   NiFpga_FlexRIO_CPUIMAQ_IndicatorArrayU8_FPGAVIversion = 0x2,
} NiFpga_FlexRIO_CPUIMAQ_IndicatorArrayU8;

typedef enum
{
   NiFpga_FlexRIO_CPUIMAQ_IndicatorArrayU8Size_DMATtoHOSTFrameType = 1,
   NiFpga_FlexRIO_CPUIMAQ_IndicatorArrayU8Size_DMATtoHOSTSampleSize = 1,
   NiFpga_FlexRIO_CPUIMAQ_IndicatorArrayU8Size_FPGAVIversion = 2,
} NiFpga_FlexRIO_CPUIMAQ_IndicatorArrayU8Size;

typedef enum
{
   NiFpga_FlexRIO_CPUIMAQ_IndicatorArrayU16_DMATtoHOSTNCh = 0xA6,
} NiFpga_FlexRIO_CPUIMAQ_IndicatorArrayU16;

typedef enum
{
   NiFpga_FlexRIO_CPUIMAQ_IndicatorArrayU16Size_DMATtoHOSTNCh = 1,
} NiFpga_FlexRIO_CPUIMAQ_IndicatorArrayU16Size;

#endif
