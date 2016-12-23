/*
 * Generated with the FPGA Interface C API Generator 13.0.0
 * for NI-RIO 13.0.0 or later.
 */

#ifndef __NiFpga_PXI7966RNi1483TEST_h__
#define __NiFpga_PXI7966RNi1483TEST_h__

#ifndef NiFpga_Version
   #define NiFpga_Version 1300
#endif

#include "NiFpga.h"

/**
 * The filename of the FPGA bitfile.
 *
 * This is a #define to allow for string literal concatenation. For example:
 *
 *    static const char* const Bitfile = "C:\\" NiFpga_PXI7966RNi1483TEST_Bitfile;
 */
#define NiFpga_PXI7966RNi1483TEST_Bitfile "NiFpga_PXI7966RNi1483TEST.lvbitx"

/**
 * The signature of the FPGA bitfile.
 */
static const char* const NiFpga_PXI7966RNi1483TEST_Signature = "631C960D38ED247066AA6E73F3EE465C";

typedef enum
{
   NiFpga_PXI7966RNi1483TEST_IndicatorBool_AcqStatusAcqInProgress = 0x4E,
   NiFpga_PXI7966RNi1483TEST_IndicatorBool_DI0 = 0x16,
   NiFpga_PXI7966RNi1483TEST_IndicatorBool_DI1 = 0x12,
   NiFpga_PXI7966RNi1483TEST_IndicatorBool_DMAsOverflowB = 0x2A,
   NiFpga_PXI7966RNi1483TEST_IndicatorBool_InitDone = 0x52,
   NiFpga_PXI7966RNi1483TEST_IndicatorBool_RIOAdapterCorrect = 0x86,
   NiFpga_PXI7966RNi1483TEST_IndicatorBool_auxDO11 = 0x3E,
   NiFpga_PXI7966RNi1483TEST_IndicatorBool_uartBreakIndicator = 0xB2,
   NiFpga_PXI7966RNi1483TEST_IndicatorBool_uartFramingError = 0xAE,
   NiFpga_PXI7966RNi1483TEST_IndicatorBool_uartOverrunError = 0xAA,
   NiFpga_PXI7966RNi1483TEST_IndicatorBool_uartRxReady = 0xB6,
   NiFpga_PXI7966RNi1483TEST_IndicatorBool_uartTxReady = 0xBA,
} NiFpga_PXI7966RNi1483TEST_IndicatorBool;

typedef enum
{
   NiFpga_PXI7966RNi1483TEST_IndicatorU8_DevProfile = 0x82,
   NiFpga_PXI7966RNi1483TEST_IndicatorU8_DevQualityStatus = 0x72,
   NiFpga_PXI7966RNi1483TEST_IndicatorU8_NoOfWFGen = 0x9E,
   NiFpga_PXI7966RNi1483TEST_IndicatorU8_Platform = 0x7E,
} NiFpga_PXI7966RNi1483TEST_IndicatorU8;

typedef enum
{
   NiFpga_PXI7966RNi1483TEST_IndicatorI16_DeviceTemp = 0x36,
   NiFpga_PXI7966RNi1483TEST_IndicatorI16_auxAI0 = 0xE,
   NiFpga_PXI7966RNi1483TEST_IndicatorI16_auxAI1 = 0x6,
} NiFpga_PXI7966RNi1483TEST_IndicatorI16;

typedef enum
{
   NiFpga_PXI7966RNi1483TEST_IndicatorU16_DMAsOverflow = 0x22,
} NiFpga_PXI7966RNi1483TEST_IndicatorU16;

typedef enum
{
   NiFpga_PXI7966RNi1483TEST_IndicatorI32_uartRxData = 0xBC,
} NiFpga_PXI7966RNi1483TEST_IndicatorI32;

typedef enum
{
   NiFpga_PXI7966RNi1483TEST_IndicatorU32_AcqStatusClocksPerLine = 0x48,
   NiFpga_PXI7966RNi1483TEST_IndicatorU32_AcqStatusFramesAcqd = 0x40,
   NiFpga_PXI7966RNi1483TEST_IndicatorU32_AcqStatusLinesPerFrame = 0x44,
   NiFpga_PXI7966RNi1483TEST_IndicatorU32_ExpectedIOModuleID = 0x8C,
   NiFpga_PXI7966RNi1483TEST_IndicatorU32_Fref = 0xA0,
   NiFpga_PXI7966RNi1483TEST_IndicatorU32_InsertedIOModuleID = 0x88,
} NiFpga_PXI7966RNi1483TEST_IndicatorU32;

typedef enum
{
   NiFpga_PXI7966RNi1483TEST_ControlBool_AcqControlIgnoreDVAL = 0x66,
   NiFpga_PXI7966RNi1483TEST_ControlBool_ControlEnable = 0xA6,
   NiFpga_PXI7966RNi1483TEST_ControlBool_DAQStartStop = 0x3A,
   NiFpga_PXI7966RNi1483TEST_ControlBool_DO0 = 0x1E,
   NiFpga_PXI7966RNi1483TEST_ControlBool_DO1 = 0x1A,
   NiFpga_PXI7966RNi1483TEST_ControlBool_DVALHigh = 0x6E,
   NiFpga_PXI7966RNi1483TEST_ControlBool_DebugMode = 0x2E,
   NiFpga_PXI7966RNi1483TEST_ControlBool_DigitalEnable = 0x2,
   NiFpga_PXI7966RNi1483TEST_ControlBool_FVALHigh = 0xE6,
   NiFpga_PXI7966RNi1483TEST_ControlBool_GroupEnable0 = 0x26,
   NiFpga_PXI7966RNi1483TEST_ControlBool_LVALHigh = 0xE2,
   NiFpga_PXI7966RNi1483TEST_ControlBool_LineScan = 0x6A,
   NiFpga_PXI7966RNi1483TEST_ControlBool_SpareHigh = 0xDE,
   NiFpga_PXI7966RNi1483TEST_ControlBool_StartDAQ = 0x62,
   NiFpga_PXI7966RNi1483TEST_ControlBool_StopDAQ = 0x56,
   NiFpga_PXI7966RNi1483TEST_ControlBool_uartByteMode = 0xCA,
   NiFpga_PXI7966RNi1483TEST_ControlBool_uartReceive = 0xCE,
   NiFpga_PXI7966RNi1483TEST_ControlBool_uartSetBaudRate = 0xC6,
   NiFpga_PXI7966RNi1483TEST_ControlBool_uartTransmit = 0xD2,
} NiFpga_PXI7966RNi1483TEST_ControlBool;

typedef enum
{
   NiFpga_PXI7966RNi1483TEST_ControlU8_Configuration = 0xD6,
   NiFpga_PXI7966RNi1483TEST_ControlU8_SignalMapping = 0xDA,
} NiFpga_PXI7966RNi1483TEST_ControlU8;

typedef enum
{
   NiFpga_PXI7966RNi1483TEST_ControlI16_auxAO0 = 0xA,
} NiFpga_PXI7966RNi1483TEST_ControlI16;

typedef enum
{
   NiFpga_PXI7966RNi1483TEST_ControlU16_SamplingRate0 = 0x32,
} NiFpga_PXI7966RNi1483TEST_ControlU16;

typedef enum
{
   NiFpga_PXI7966RNi1483TEST_ControlI32_DataBytetoTx = 0x90,
   NiFpga_PXI7966RNi1483TEST_ControlI32_uartBaudRate = 0xC0,
} NiFpga_PXI7966RNi1483TEST_ControlI32;

typedef enum
{
   NiFpga_PXI7966RNi1483TEST_ControlU32_AcqControlFramesToAcq = 0x5C,
   NiFpga_PXI7966RNi1483TEST_ControlU32_AcqControlLineScanHeight = 0x58,
} NiFpga_PXI7966RNi1483TEST_ControlU32;

typedef enum
{
   NiFpga_PXI7966RNi1483TEST_IndicatorArrayU8_BytesperSampleChannelDMATtoHOST = 0x76,
   NiFpga_PXI7966RNi1483TEST_IndicatorArrayU8_FPGAVIversion = 0x9A,
   NiFpga_PXI7966RNi1483TEST_IndicatorArrayU8_FrameTypeDMATtoHOST = 0x7A,
   NiFpga_PXI7966RNi1483TEST_IndicatorArrayU8_NCHperDMATtoHOST = 0x96,
} NiFpga_PXI7966RNi1483TEST_IndicatorArrayU8;

typedef enum
{
   NiFpga_PXI7966RNi1483TEST_IndicatorArrayU8Size_BytesperSampleChannelDMATtoHOST = 1,
   NiFpga_PXI7966RNi1483TEST_IndicatorArrayU8Size_FPGAVIversion = 2,
   NiFpga_PXI7966RNi1483TEST_IndicatorArrayU8Size_FrameTypeDMATtoHOST = 1,
   NiFpga_PXI7966RNi1483TEST_IndicatorArrayU8Size_NCHperDMATtoHOST = 1,
} NiFpga_PXI7966RNi1483TEST_IndicatorArrayU8Size;

typedef enum
{
   NiFpga_PXI7966RNi1483TEST_TargetToHostFifoU64_DMATtoHOST0 = 0,
} NiFpga_PXI7966RNi1483TEST_TargetToHostFifoU64;

#endif
