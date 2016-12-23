/*
 * Generated with the FPGA Interface C API Generator 13.0.0
 * for NI-RIO 13.0.0 or later.
 */

#ifndef __NiFpga_FlexRIOMod1483_7965_h__
#define __NiFpga_FlexRIOMod1483_7965_h__

#ifndef NiFpga_Version
   #define NiFpga_Version 1300
#endif

#include "NiFpga.h"

/**
 * The filename of the FPGA bitfile.
 *
 * This is a #define to allow for string literal concatenation. For example:
 *
 *    static const char* const Bitfile = "C:\\" NiFpga_FlexRIOMod1483_7965_Bitfile;
 */
#define NiFpga_FlexRIOMod1483_7965_Bitfile "NiFpga_FlexRIOMod1483_7965.lvbitx"

/**
 * The signature of the FPGA bitfile.
 */
static const char* const NiFpga_FlexRIOMod1483_7965_Signature = "939A10DEF9F060EBB92A2A924B5CA7B4";

typedef enum
{
   NiFpga_FlexRIOMod1483_7965_IndicatorBool_AcqStatusAcqInProgress = 0xE,
   NiFpga_FlexRIOMod1483_7965_IndicatorBool_DI0 = 0x26,
   NiFpga_FlexRIOMod1483_7965_IndicatorBool_DI1 = 0x2A,
   NiFpga_FlexRIOMod1483_7965_IndicatorBool_InitDone = 0x56,
   NiFpga_FlexRIOMod1483_7965_IndicatorBool_RIOAdapterCorrect = 0xC6,
   NiFpga_FlexRIOMod1483_7965_IndicatorBool_uartBreakIndicator = 0xA2,
   NiFpga_FlexRIOMod1483_7965_IndicatorBool_uartFramingError = 0xA6,
   NiFpga_FlexRIOMod1483_7965_IndicatorBool_uartOverrunError = 0xAA,
   NiFpga_FlexRIOMod1483_7965_IndicatorBool_uartRxReady = 0x9E,
   NiFpga_FlexRIOMod1483_7965_IndicatorBool_uartTxReady = 0x9A,
} NiFpga_FlexRIOMod1483_7965_IndicatorBool;

typedef enum
{
   NiFpga_FlexRIOMod1483_7965_IndicatorU8_DevProfile = 0xCA,
   NiFpga_FlexRIOMod1483_7965_IndicatorU8_DevQualityStatus = 0xCE,
   NiFpga_FlexRIOMod1483_7965_IndicatorU8_Platform = 0xE2,
   NiFpga_FlexRIOMod1483_7965_IndicatorU8_Platform2 = 0xD2,
   NiFpga_FlexRIOMod1483_7965_IndicatorU8_SGNo = 0x6A,
} NiFpga_FlexRIOMod1483_7965_IndicatorU8;

typedef enum
{
   NiFpga_FlexRIOMod1483_7965_IndicatorI16_DeviceTemp = 0x5E,
   NiFpga_FlexRIOMod1483_7965_IndicatorI16_auxAI0 = 0x2E,
   NiFpga_FlexRIOMod1483_7965_IndicatorI16_auxAI1 = 0x36,
} NiFpga_FlexRIOMod1483_7965_IndicatorI16;

typedef enum
{
   NiFpga_FlexRIOMod1483_7965_IndicatorU16_DMATtoGPUOverflow = 0x46,
} NiFpga_FlexRIOMod1483_7965_IndicatorU16;

typedef enum
{
   NiFpga_FlexRIOMod1483_7965_IndicatorI32_uartRxData = 0x94,
} NiFpga_FlexRIOMod1483_7965_IndicatorI32;

typedef enum
{
   NiFpga_FlexRIOMod1483_7965_IndicatorU32_AcqStatusClocksPerLine = 0x10,
   NiFpga_FlexRIOMod1483_7965_IndicatorU32_AcqStatusFramesAcqd = 0x18,
   NiFpga_FlexRIOMod1483_7965_IndicatorU32_AcqStatusLinesPerFrame = 0x14,
   NiFpga_FlexRIOMod1483_7965_IndicatorU32_Fref = 0xB8,
   NiFpga_FlexRIOMod1483_7965_IndicatorU32_InsertedIOModuleID = 0xC0,
} NiFpga_FlexRIOMod1483_7965_IndicatorU32;

typedef enum
{
   NiFpga_FlexRIOMod1483_7965_ControlBool_AcqControlIgnoreDVAL = 0x2,
   NiFpga_FlexRIOMod1483_7965_ControlBool_ControlEnable = 0xAE,
   NiFpga_FlexRIOMod1483_7965_ControlBool_DAQStartStop = 0x5A,
   NiFpga_FlexRIOMod1483_7965_ControlBool_DMATtoGPUEnable0 = 0x42,
   NiFpga_FlexRIOMod1483_7965_ControlBool_DO0 = 0x1E,
   NiFpga_FlexRIOMod1483_7965_ControlBool_DO1 = 0x22,
   NiFpga_FlexRIOMod1483_7965_ControlBool_DVALHigh = 0x66,
   NiFpga_FlexRIOMod1483_7965_ControlBool_DebugMode = 0x62,
   NiFpga_FlexRIOMod1483_7965_ControlBool_FVALHigh = 0x6E,
   NiFpga_FlexRIOMod1483_7965_ControlBool_LVALHigh = 0x72,
   NiFpga_FlexRIOMod1483_7965_ControlBool_LineScan = 0x3A,
   NiFpga_FlexRIOMod1483_7965_ControlBool_SpareHigh = 0x76,
   NiFpga_FlexRIOMod1483_7965_ControlBool_uartByteMode = 0x8A,
   NiFpga_FlexRIOMod1483_7965_ControlBool_uartReceive = 0x86,
   NiFpga_FlexRIOMod1483_7965_ControlBool_uartSetBaudRate = 0x8E,
   NiFpga_FlexRIOMod1483_7965_ControlBool_uartTransmit = 0x82,
} NiFpga_FlexRIOMod1483_7965_ControlBool;

typedef enum
{
   NiFpga_FlexRIOMod1483_7965_ControlU8_Configuration = 0x7E,
   NiFpga_FlexRIOMod1483_7965_ControlU8_SignalMapping = 0x7A,
} NiFpga_FlexRIOMod1483_7965_ControlU8;

typedef enum
{
   NiFpga_FlexRIOMod1483_7965_ControlI16_auxAO0 = 0x32,
} NiFpga_FlexRIOMod1483_7965_ControlI16;

typedef enum
{
   NiFpga_FlexRIOMod1483_7965_ControlU16_DMATtoGPUSamplingRate0 = 0x3E,
} NiFpga_FlexRIOMod1483_7965_ControlU16;

typedef enum
{
   NiFpga_FlexRIOMod1483_7965_ControlI32_DataBytetoTx = 0xB0,
   NiFpga_FlexRIOMod1483_7965_ControlI32_uartBaudRate = 0x90,
} NiFpga_FlexRIOMod1483_7965_ControlI32;

typedef enum
{
   NiFpga_FlexRIOMod1483_7965_ControlU32_AcqControlFramesToAcq = 0x4,
   NiFpga_FlexRIOMod1483_7965_ControlU32_AcqControlLineScanHeight = 0x8,
   NiFpga_FlexRIOMod1483_7965_ControlU32_TabControl = 0xD4,
} NiFpga_FlexRIOMod1483_7965_ControlU32;

typedef enum
{
   NiFpga_FlexRIOMod1483_7965_IndicatorArrayU8_BytesperSampleChannelDMATtoHOST = 0xDA,
   NiFpga_FlexRIOMod1483_7965_IndicatorArrayU8_DMATtoGPUFrameType = 0x4E,
   NiFpga_FlexRIOMod1483_7965_IndicatorArrayU8_DMATtoGPUNCh = 0xB6,
   NiFpga_FlexRIOMod1483_7965_IndicatorArrayU8_DMATtoGPUSampleSize = 0x4A,
   NiFpga_FlexRIOMod1483_7965_IndicatorArrayU8_FPGAVIversion = 0xBE,
   NiFpga_FlexRIOMod1483_7965_IndicatorArrayU8_FrameTypeDMATtoHOST = 0xDE,
} NiFpga_FlexRIOMod1483_7965_IndicatorArrayU8;

typedef enum
{
   NiFpga_FlexRIOMod1483_7965_IndicatorArrayU8Size_BytesperSampleChannelDMATtoHOST = 1,
   NiFpga_FlexRIOMod1483_7965_IndicatorArrayU8Size_DMATtoGPUFrameType = 1,
   NiFpga_FlexRIOMod1483_7965_IndicatorArrayU8Size_DMATtoGPUNCh = 1,
   NiFpga_FlexRIOMod1483_7965_IndicatorArrayU8Size_DMATtoGPUSampleSize = 1,
   NiFpga_FlexRIOMod1483_7965_IndicatorArrayU8Size_FPGAVIversion = 2,
   NiFpga_FlexRIOMod1483_7965_IndicatorArrayU8Size_FrameTypeDMATtoHOST = 1,
} NiFpga_FlexRIOMod1483_7965_IndicatorArrayU8Size;

typedef enum
{
   NiFpga_FlexRIOMod1483_7965_IndicatorArrayU16_DMATtoGPUBlockNWords = 0x52,
} NiFpga_FlexRIOMod1483_7965_IndicatorArrayU16;

typedef enum
{
   NiFpga_FlexRIOMod1483_7965_IndicatorArrayU16Size_DMATtoGPUBlockNWords = 1,
} NiFpga_FlexRIOMod1483_7965_IndicatorArrayU16Size;

typedef enum
{
   NiFpga_FlexRIOMod1483_7965_TargetToHostFifoU64_DMATtoHOST0 = 0,
} NiFpga_FlexRIOMod1483_7965_TargetToHostFifoU64;

#endif
