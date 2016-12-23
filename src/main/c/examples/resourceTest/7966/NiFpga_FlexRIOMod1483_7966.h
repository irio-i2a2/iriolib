/*
 * Generated with the FPGA Interface C API Generator 13.0.0
 * for NI-RIO 13.0.0 or later.
 */

#ifndef __NiFpga_FlexRIOMod1483_7966_h__
#define __NiFpga_FlexRIOMod1483_7966_h__

#ifndef NiFpga_Version
   #define NiFpga_Version 1300
#endif

#include "NiFpga.h"

/**
 * The filename of the FPGA bitfile.
 *
 * This is a #define to allow for string literal concatenation. For example:
 *
 *    static const char* const Bitfile = "C:\\" NiFpga_FlexRIOMod1483_7966_Bitfile;
 */
#define NiFpga_FlexRIOMod1483_7966_Bitfile "NiFpga_FlexRIOMod1483_7966.lvbitx"

/**
 * The signature of the FPGA bitfile.
 */
static const char* const NiFpga_FlexRIOMod1483_7966_Signature = "97EAA658696E09D943D33B1319DD3579";

typedef enum
{
   NiFpga_FlexRIOMod1483_7966_IndicatorBool_AcqStatusAcqInProgress = 0xE,
   NiFpga_FlexRIOMod1483_7966_IndicatorBool_DI0 = 0x26,
   NiFpga_FlexRIOMod1483_7966_IndicatorBool_DI1 = 0x2A,
   NiFpga_FlexRIOMod1483_7966_IndicatorBool_InitDone = 0x46,
   NiFpga_FlexRIOMod1483_7966_IndicatorBool_RIOAdapterCorrect = 0xBE,
   NiFpga_FlexRIOMod1483_7966_IndicatorBool_uartBreakIndicator = 0x92,
   NiFpga_FlexRIOMod1483_7966_IndicatorBool_uartFramingError = 0x96,
   NiFpga_FlexRIOMod1483_7966_IndicatorBool_uartOverrunError = 0x9A,
   NiFpga_FlexRIOMod1483_7966_IndicatorBool_uartRxReady = 0x8E,
   NiFpga_FlexRIOMod1483_7966_IndicatorBool_uartTxReady = 0x8A,
} NiFpga_FlexRIOMod1483_7966_IndicatorBool;

typedef enum
{
   NiFpga_FlexRIOMod1483_7966_IndicatorU8_DevProfile = 0xC2,
   NiFpga_FlexRIOMod1483_7966_IndicatorU8_DevQualityStatus = 0xC6,
   NiFpga_FlexRIOMod1483_7966_IndicatorU8_Platform = 0xCA,
   NiFpga_FlexRIOMod1483_7966_IndicatorU8_SGNo = 0x5A,
   NiFpga_FlexRIOMod1483_7966_IndicatorU8_uartRxByte = 0x86,
} NiFpga_FlexRIOMod1483_7966_IndicatorU8;

typedef enum
{
   NiFpga_FlexRIOMod1483_7966_IndicatorI16_DevTemp = 0x4E,
   NiFpga_FlexRIOMod1483_7966_IndicatorI16_auxAI0 = 0x2E,
   NiFpga_FlexRIOMod1483_7966_IndicatorI16_auxAI1 = 0x36,
} NiFpga_FlexRIOMod1483_7966_IndicatorI16;

typedef enum
{
   NiFpga_FlexRIOMod1483_7966_IndicatorU16_DMATtoHOSTOverflows = 0x42,
} NiFpga_FlexRIOMod1483_7966_IndicatorU16;

typedef enum
{
   NiFpga_FlexRIOMod1483_7966_IndicatorU32_AcqStatusClocksPerLine = 0x10,
   NiFpga_FlexRIOMod1483_7966_IndicatorU32_AcqStatusFramesAcqd = 0x18,
   NiFpga_FlexRIOMod1483_7966_IndicatorU32_AcqStatusLinesPerFrame = 0x14,
   NiFpga_FlexRIOMod1483_7966_IndicatorU32_Fref = 0xB0,
   NiFpga_FlexRIOMod1483_7966_IndicatorU32_InsertedIOModuleID = 0xB8,
} NiFpga_FlexRIOMod1483_7966_IndicatorU32;

typedef enum
{
   NiFpga_FlexRIOMod1483_7966_ControlBool_AcqControlIgnoreDVAL = 0x2,
   NiFpga_FlexRIOMod1483_7966_ControlBool_ControlEnable = 0x9E,
   NiFpga_FlexRIOMod1483_7966_ControlBool_DAQStartStop = 0x4A,
   NiFpga_FlexRIOMod1483_7966_ControlBool_DMATtoHOSTEnable0 = 0x3E,
   NiFpga_FlexRIOMod1483_7966_ControlBool_DO0 = 0x1E,
   NiFpga_FlexRIOMod1483_7966_ControlBool_DO1 = 0x22,
   NiFpga_FlexRIOMod1483_7966_ControlBool_DVALHigh = 0x56,
   NiFpga_FlexRIOMod1483_7966_ControlBool_DebugMode = 0x52,
   NiFpga_FlexRIOMod1483_7966_ControlBool_FVALHigh = 0x5E,
   NiFpga_FlexRIOMod1483_7966_ControlBool_LVALHigh = 0x62,
   NiFpga_FlexRIOMod1483_7966_ControlBool_LineScan = 0x3A,
   NiFpga_FlexRIOMod1483_7966_ControlBool_SpareHigh = 0x66,
   NiFpga_FlexRIOMod1483_7966_ControlBool_uartByteMode = 0x7A,
   NiFpga_FlexRIOMod1483_7966_ControlBool_uartReceive = 0x76,
   NiFpga_FlexRIOMod1483_7966_ControlBool_uartSetBaudRate = 0x7E,
   NiFpga_FlexRIOMod1483_7966_ControlBool_uartTransmit = 0x72,
} NiFpga_FlexRIOMod1483_7966_ControlBool;

typedef enum
{
   NiFpga_FlexRIOMod1483_7966_ControlU8_Configuration = 0x6E,
   NiFpga_FlexRIOMod1483_7966_ControlU8_SignalMapping = 0x6A,
   NiFpga_FlexRIOMod1483_7966_ControlU8_uartBaudRate = 0x82,
   NiFpga_FlexRIOMod1483_7966_ControlU8_uartTxByte = 0xA2,
} NiFpga_FlexRIOMod1483_7966_ControlU8;

typedef enum
{
   NiFpga_FlexRIOMod1483_7966_ControlI16_auxAO0 = 0x32,
} NiFpga_FlexRIOMod1483_7966_ControlI16;

typedef enum
{
   NiFpga_FlexRIOMod1483_7966_ControlU32_AcqControlFramesToAcq = 0x4,
   NiFpga_FlexRIOMod1483_7966_ControlU32_AcqControlLineScanHeight = 0x8,
   NiFpga_FlexRIOMod1483_7966_ControlU32_TabControl = 0xCC,
} NiFpga_FlexRIOMod1483_7966_ControlU32;

typedef enum
{
   NiFpga_FlexRIOMod1483_7966_IndicatorArrayU8_DMATtoHOSTFrameType = 0xAA,
   NiFpga_FlexRIOMod1483_7966_IndicatorArrayU8_DMATtoHOSTSampleSize = 0xAE,
   NiFpga_FlexRIOMod1483_7966_IndicatorArrayU8_FPGAVIversion = 0xB6,
} NiFpga_FlexRIOMod1483_7966_IndicatorArrayU8;

typedef enum
{
   NiFpga_FlexRIOMod1483_7966_IndicatorArrayU8Size_DMATtoHOSTFrameType = 1,
   NiFpga_FlexRIOMod1483_7966_IndicatorArrayU8Size_DMATtoHOSTSampleSize = 1,
   NiFpga_FlexRIOMod1483_7966_IndicatorArrayU8Size_FPGAVIversion = 2,
} NiFpga_FlexRIOMod1483_7966_IndicatorArrayU8Size;

typedef enum
{
   NiFpga_FlexRIOMod1483_7966_IndicatorArrayU16_DMATtoHOSTNCh = 0xA6,
} NiFpga_FlexRIOMod1483_7966_IndicatorArrayU16;

typedef enum
{
   NiFpga_FlexRIOMod1483_7966_IndicatorArrayU16Size_DMATtoHOSTNCh = 1,
} NiFpga_FlexRIOMod1483_7966_IndicatorArrayU16Size;

typedef enum
{
   NiFpga_FlexRIOMod1483_7966_TargetToHostFifoU64_DMATtoHOST0 = 0,
} NiFpga_FlexRIOMod1483_7966_TargetToHostFifoU64;

#endif
