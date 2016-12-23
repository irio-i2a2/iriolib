/*
 * Generated with the FPGA Interface C API Generator 13.0.0
 * for NI-RIO 13.0.0 or later.
 */

#ifndef __NiFpga_FPGA1483_8tap8GPUV1_0_h__
#define __NiFpga_FPGA1483_8tap8GPUV1_0_h__

#ifndef NiFpga_Version
   #define NiFpga_Version 1300
#endif

#include "NiFpga.h"

/**
 * The filename of the FPGA bitfile.
 *
 * This is a #define to allow for string literal concatenation. For example:
 *
 *    static const char* const Bitfile = "C:\\" NiFpga_FPGA1483_8tap8GPUV1_0_Bitfile;
 */
#define NiFpga_FPGA1483_8tap8GPUV1_0_Bitfile "NiFpga_FPGA1483_8tap8GPUV1_0.lvbitx"

/**
 * The signature of the FPGA bitfile.
 */
static const char* const NiFpga_FPGA1483_8tap8GPUV1_0_Signature = "9401FC64C063BA1F26D388E5AD25B963";

typedef enum
{
   NiFpga_FPGA1483_8tap8GPUV1_0_IndicatorBool_AcqStatusAcqInProgress = 0xE,
   NiFpga_FPGA1483_8tap8GPUV1_0_IndicatorBool_DI0 = 0x26,
   NiFpga_FPGA1483_8tap8GPUV1_0_IndicatorBool_DI1 = 0x2A,
   NiFpga_FPGA1483_8tap8GPUV1_0_IndicatorBool_InitDone = 0x4E,
   NiFpga_FPGA1483_8tap8GPUV1_0_IndicatorBool_RIOAdapterCorrect = 0xC6,
   NiFpga_FPGA1483_8tap8GPUV1_0_IndicatorBool_uartBreakIndicator = 0x9A,
   NiFpga_FPGA1483_8tap8GPUV1_0_IndicatorBool_uartFramingError = 0x9E,
   NiFpga_FPGA1483_8tap8GPUV1_0_IndicatorBool_uartOverrunError = 0xA2,
   NiFpga_FPGA1483_8tap8GPUV1_0_IndicatorBool_uartRxReady = 0x96,
   NiFpga_FPGA1483_8tap8GPUV1_0_IndicatorBool_uartTxReady = 0x92,
} NiFpga_FPGA1483_8tap8GPUV1_0_IndicatorBool;

typedef enum
{
   NiFpga_FPGA1483_8tap8GPUV1_0_IndicatorU8_DevProfile = 0xCA,
   NiFpga_FPGA1483_8tap8GPUV1_0_IndicatorU8_DevQualityStatus = 0xCE,
   NiFpga_FPGA1483_8tap8GPUV1_0_IndicatorU8_Platform = 0xD2,
   NiFpga_FPGA1483_8tap8GPUV1_0_IndicatorU8_SGNo = 0x62,
   NiFpga_FPGA1483_8tap8GPUV1_0_IndicatorU8_uartRxByte = 0x8E,
} NiFpga_FPGA1483_8tap8GPUV1_0_IndicatorU8;

typedef enum
{
   NiFpga_FPGA1483_8tap8GPUV1_0_IndicatorI16_DevTemp = 0x56,
} NiFpga_FPGA1483_8tap8GPUV1_0_IndicatorI16;

typedef enum
{
   NiFpga_FPGA1483_8tap8GPUV1_0_IndicatorU16_DMATtoGPUOverflows = 0x46,
} NiFpga_FPGA1483_8tap8GPUV1_0_IndicatorU16;

typedef enum
{
   NiFpga_FPGA1483_8tap8GPUV1_0_IndicatorI32_auxAI0 = 0x2C,
   NiFpga_FPGA1483_8tap8GPUV1_0_IndicatorI32_auxAI1 = 0x34,
} NiFpga_FPGA1483_8tap8GPUV1_0_IndicatorI32;

typedef enum
{
   NiFpga_FPGA1483_8tap8GPUV1_0_IndicatorU32_AcqStatusClocksPerLine = 0x10,
   NiFpga_FPGA1483_8tap8GPUV1_0_IndicatorU32_AcqStatusFramesAcqd = 0x18,
   NiFpga_FPGA1483_8tap8GPUV1_0_IndicatorU32_AcqStatusLinesPerFrame = 0x14,
   NiFpga_FPGA1483_8tap8GPUV1_0_IndicatorU32_Fref = 0xB8,
   NiFpga_FPGA1483_8tap8GPUV1_0_IndicatorU32_InsertedIOModuleID = 0xC0,
} NiFpga_FPGA1483_8tap8GPUV1_0_IndicatorU32;

typedef enum
{
   NiFpga_FPGA1483_8tap8GPUV1_0_ControlBool_AcqControlIgnoreDVAL = 0x2,
   NiFpga_FPGA1483_8tap8GPUV1_0_ControlBool_ControlEnable = 0xA6,
   NiFpga_FPGA1483_8tap8GPUV1_0_ControlBool_DAQStartStop = 0x52,
   NiFpga_FPGA1483_8tap8GPUV1_0_ControlBool_DMATtoGPUEnable0 = 0x42,
   NiFpga_FPGA1483_8tap8GPUV1_0_ControlBool_DO0 = 0x1E,
   NiFpga_FPGA1483_8tap8GPUV1_0_ControlBool_DO1 = 0x22,
   NiFpga_FPGA1483_8tap8GPUV1_0_ControlBool_DVALHigh = 0x5E,
   NiFpga_FPGA1483_8tap8GPUV1_0_ControlBool_DebugMode = 0x5A,
   NiFpga_FPGA1483_8tap8GPUV1_0_ControlBool_FVALHigh = 0x66,
   NiFpga_FPGA1483_8tap8GPUV1_0_ControlBool_LVALHigh = 0x6A,
   NiFpga_FPGA1483_8tap8GPUV1_0_ControlBool_LineScan = 0x3A,
   NiFpga_FPGA1483_8tap8GPUV1_0_ControlBool_SpareHigh = 0x6E,
   NiFpga_FPGA1483_8tap8GPUV1_0_ControlBool_uartByteMode = 0x82,
   NiFpga_FPGA1483_8tap8GPUV1_0_ControlBool_uartReceive = 0x7E,
   NiFpga_FPGA1483_8tap8GPUV1_0_ControlBool_uartSetBaudRate = 0x86,
   NiFpga_FPGA1483_8tap8GPUV1_0_ControlBool_uartTransmit = 0x7A,
} NiFpga_FPGA1483_8tap8GPUV1_0_ControlBool;

typedef enum
{
   NiFpga_FPGA1483_8tap8GPUV1_0_ControlU8_Configuration = 0x76,
   NiFpga_FPGA1483_8tap8GPUV1_0_ControlU8_SignalMapping = 0x72,
   NiFpga_FPGA1483_8tap8GPUV1_0_ControlU8_uartBaudRate = 0x8A,
   NiFpga_FPGA1483_8tap8GPUV1_0_ControlU8_uartTxByte = 0xAA,
} NiFpga_FPGA1483_8tap8GPUV1_0_ControlU8;

typedef enum
{
   NiFpga_FPGA1483_8tap8GPUV1_0_ControlU16_DMATtoGPUSamplingRate0 = 0x3E,
} NiFpga_FPGA1483_8tap8GPUV1_0_ControlU16;

typedef enum
{
   NiFpga_FPGA1483_8tap8GPUV1_0_ControlI32_auxAO0 = 0x30,
} NiFpga_FPGA1483_8tap8GPUV1_0_ControlI32;

typedef enum
{
   NiFpga_FPGA1483_8tap8GPUV1_0_ControlU32_AcqControlFramesToAcq = 0x4,
   NiFpga_FPGA1483_8tap8GPUV1_0_ControlU32_AcqControlLineScanHeight = 0x8,
   NiFpga_FPGA1483_8tap8GPUV1_0_ControlU32_TabControl = 0xD4,
} NiFpga_FPGA1483_8tap8GPUV1_0_ControlU32;

typedef enum
{
   NiFpga_FPGA1483_8tap8GPUV1_0_IndicatorArrayU8_DMATtoGPUFrameType = 0xB2,
   NiFpga_FPGA1483_8tap8GPUV1_0_IndicatorArrayU8_DMATtoGPUSampleSize = 0xB6,
   NiFpga_FPGA1483_8tap8GPUV1_0_IndicatorArrayU8_FPGAVIversion = 0xBE,
} NiFpga_FPGA1483_8tap8GPUV1_0_IndicatorArrayU8;

typedef enum
{
   NiFpga_FPGA1483_8tap8GPUV1_0_IndicatorArrayU8Size_DMATtoGPUFrameType = 1,
   NiFpga_FPGA1483_8tap8GPUV1_0_IndicatorArrayU8Size_DMATtoGPUSampleSize = 1,
   NiFpga_FPGA1483_8tap8GPUV1_0_IndicatorArrayU8Size_FPGAVIversion = 2,
} NiFpga_FPGA1483_8tap8GPUV1_0_IndicatorArrayU8Size;

typedef enum
{
   NiFpga_FPGA1483_8tap8GPUV1_0_IndicatorArrayU16_DMATtoGPUBlockNWords = 0x4A,
   NiFpga_FPGA1483_8tap8GPUV1_0_IndicatorArrayU16_DMATtoGPUNCh = 0xAE,
} NiFpga_FPGA1483_8tap8GPUV1_0_IndicatorArrayU16;

typedef enum
{
   NiFpga_FPGA1483_8tap8GPUV1_0_IndicatorArrayU16Size_DMATtoGPUBlockNWords = 1,
   NiFpga_FPGA1483_8tap8GPUV1_0_IndicatorArrayU16Size_DMATtoGPUNCh = 1,
} NiFpga_FPGA1483_8tap8GPUV1_0_IndicatorArrayU16Size;

typedef enum
{
   NiFpga_FPGA1483_8tap8GPUV1_0_TargetToHostFifoU64_DMATtoGPU0 = 0,
} NiFpga_FPGA1483_8tap8GPUV1_0_TargetToHostFifoU64;

#endif
