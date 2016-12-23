/*
 * Generated with the FPGA Interface C API Generator 13.0.0
 * for NI-RIO 13.0.0 or later.
 */

#ifndef __NiFpga_FlexRIO_GPUIMAQ_h__
#define __NiFpga_FlexRIO_GPUIMAQ_h__

#ifndef NiFpga_Version
   #define NiFpga_Version 1300
#endif

#include "NiFpga.h"

/**
 * The filename of the FPGA bitfile.
 *
 * This is a #define to allow for string literal concatenation. For example:
 *
 *    static const char* const Bitfile = "C:\\" NiFpga_FlexRIO_GPUIMAQ_Bitfile;
 */
#define NiFpga_FlexRIO_GPUIMAQ_Bitfile "NiFpga_FlexRIO_GPUIMAQ.lvbitx"

/**
 * The signature of the FPGA bitfile.
 */
static const char* const NiFpga_FlexRIO_GPUIMAQ_Signature = "2A7D9F4974835CA690487564EDF57074";

typedef enum
{
   NiFpga_FlexRIO_GPUIMAQ_IndicatorBool_DI0 = 0x806E,
   NiFpga_FlexRIO_GPUIMAQ_IndicatorBool_DI1 = 0x806A,
   NiFpga_FlexRIO_GPUIMAQ_IndicatorBool_InitDone = 0x80CA,
   NiFpga_FlexRIO_GPUIMAQ_IndicatorBool_PXIIN0 = 0x805A,
   NiFpga_FlexRIO_GPUIMAQ_IndicatorBool_PXIIN1 = 0x8052,
   NiFpga_FlexRIO_GPUIMAQ_IndicatorBool_RIOAdapterCorrect = 0x80C2,
   NiFpga_FlexRIO_GPUIMAQ_IndicatorBool_auxDI0 = 0x8066,
   NiFpga_FlexRIO_GPUIMAQ_IndicatorBool_auxDI1 = 0x8062,
   NiFpga_FlexRIO_GPUIMAQ_IndicatorBool_uartBreakIndicator = 0x800A,
   NiFpga_FlexRIO_GPUIMAQ_IndicatorBool_uartFramingError = 0x8006,
   NiFpga_FlexRIO_GPUIMAQ_IndicatorBool_uartOverrunError = 0x8002,
   NiFpga_FlexRIO_GPUIMAQ_IndicatorBool_uartRxReady = 0x8016,
   NiFpga_FlexRIO_GPUIMAQ_IndicatorBool_uartTxReady = 0x8012,
} NiFpga_FlexRIO_GPUIMAQ_IndicatorBool;

typedef enum
{
   NiFpga_FlexRIO_GPUIMAQ_IndicatorU8_DevProfile = 0x80B2,
   NiFpga_FlexRIO_GPUIMAQ_IndicatorU8_DevQualityStatus = 0x80BA,
   NiFpga_FlexRIO_GPUIMAQ_IndicatorU8_Platform = 0x80A6,
   NiFpga_FlexRIO_GPUIMAQ_IndicatorU8_uartRxByte = 0x800E,
} NiFpga_FlexRIO_GPUIMAQ_IndicatorU8;

typedef enum
{
   NiFpga_FlexRIO_GPUIMAQ_IndicatorI16_DevTemp = 0x80B6,
} NiFpga_FlexRIO_GPUIMAQ_IndicatorI16;

typedef enum
{
   NiFpga_FlexRIO_GPUIMAQ_IndicatorU16_DMATtoGPUOverflows = 0x8092,
} NiFpga_FlexRIO_GPUIMAQ_IndicatorU16;

typedef enum
{
   NiFpga_FlexRIO_GPUIMAQ_IndicatorI32_auxAI0 = 0x808C,
   NiFpga_FlexRIO_GPUIMAQ_IndicatorI32_auxAI1 = 0x8088,
} NiFpga_FlexRIO_GPUIMAQ_IndicatorI32;

typedef enum
{
   NiFpga_FlexRIO_GPUIMAQ_IndicatorU32_Fref = 0x80BC,
   NiFpga_FlexRIO_GPUIMAQ_IndicatorU32_InsertedIOModuleID = 0x80C4,
} NiFpga_FlexRIO_GPUIMAQ_IndicatorU32;

typedef enum
{
   NiFpga_FlexRIO_GPUIMAQ_ControlBool_ControlEnable = 0x8032,
   NiFpga_FlexRIO_GPUIMAQ_ControlBool_DAQStartStop = 0x80AE,
   NiFpga_FlexRIO_GPUIMAQ_ControlBool_DMATtoGPUEnable0 = 0x8096,
   NiFpga_FlexRIO_GPUIMAQ_ControlBool_DO0 = 0x807E,
   NiFpga_FlexRIO_GPUIMAQ_ControlBool_DO1 = 0x807A,
   NiFpga_FlexRIO_GPUIMAQ_ControlBool_DVALHigh = 0x8046,
   NiFpga_FlexRIO_GPUIMAQ_ControlBool_DebugMode = 0x80AA,
   NiFpga_FlexRIO_GPUIMAQ_ControlBool_FVALHigh = 0x8042,
   NiFpga_FlexRIO_GPUIMAQ_ControlBool_LVALHigh = 0x803E,
   NiFpga_FlexRIO_GPUIMAQ_ControlBool_LineScan = 0x8036,
   NiFpga_FlexRIO_GPUIMAQ_ControlBool_PXIOUT0 = 0x805E,
   NiFpga_FlexRIO_GPUIMAQ_ControlBool_PXIOUT1 = 0x8056,
   NiFpga_FlexRIO_GPUIMAQ_ControlBool_SpareHigh = 0x803A,
   NiFpga_FlexRIO_GPUIMAQ_ControlBool_auxDO0 = 0x8076,
   NiFpga_FlexRIO_GPUIMAQ_ControlBool_auxDO1 = 0x8072,
   NiFpga_FlexRIO_GPUIMAQ_ControlBool_uartByteMode = 0x801E,
   NiFpga_FlexRIO_GPUIMAQ_ControlBool_uartReceive = 0x802A,
   NiFpga_FlexRIO_GPUIMAQ_ControlBool_uartSetBaudRate = 0x8026,
   NiFpga_FlexRIO_GPUIMAQ_ControlBool_uartTransmit = 0x802E,
} NiFpga_FlexRIO_GPUIMAQ_ControlBool;

typedef enum
{
   NiFpga_FlexRIO_GPUIMAQ_ControlU8_Configuration = 0x804E,
   NiFpga_FlexRIO_GPUIMAQ_ControlU8_SignalMapping = 0x804A,
   NiFpga_FlexRIO_GPUIMAQ_ControlU8_uartBaudRate = 0x8022,
   NiFpga_FlexRIO_GPUIMAQ_ControlU8_uartTxByte = 0x801A,
} NiFpga_FlexRIO_GPUIMAQ_ControlU8;

typedef enum
{
   NiFpga_FlexRIO_GPUIMAQ_ControlI32_auxAO0 = 0x8084,
   NiFpga_FlexRIO_GPUIMAQ_ControlI32_auxAO1 = 0x8080,
} NiFpga_FlexRIO_GPUIMAQ_ControlI32;

typedef enum
{
   NiFpga_FlexRIO_GPUIMAQ_IndicatorArrayU8_DMATtoGPUFrameType = 0x809E,
   NiFpga_FlexRIO_GPUIMAQ_IndicatorArrayU8_DMATtoGPUSampleSize = 0x809A,
   NiFpga_FlexRIO_GPUIMAQ_IndicatorArrayU8_FPGAVIversion = 0x80CE,
} NiFpga_FlexRIO_GPUIMAQ_IndicatorArrayU8;

typedef enum
{
   NiFpga_FlexRIO_GPUIMAQ_IndicatorArrayU8Size_DMATtoGPUFrameType = 1,
   NiFpga_FlexRIO_GPUIMAQ_IndicatorArrayU8Size_DMATtoGPUSampleSize = 1,
   NiFpga_FlexRIO_GPUIMAQ_IndicatorArrayU8Size_FPGAVIversion = 2,
} NiFpga_FlexRIO_GPUIMAQ_IndicatorArrayU8Size;

typedef enum
{
   NiFpga_FlexRIO_GPUIMAQ_IndicatorArrayU16_DMATtoGPUNCh = 0x80A2,
} NiFpga_FlexRIO_GPUIMAQ_IndicatorArrayU16;

typedef enum
{
   NiFpga_FlexRIO_GPUIMAQ_IndicatorArrayU16Size_DMATtoGPUNCh = 1,
} NiFpga_FlexRIO_GPUIMAQ_IndicatorArrayU16Size;

typedef enum
{
   NiFpga_FlexRIO_GPUIMAQ_TargetToHostFifoU64_DMATtoGPU0 = 0,
} NiFpga_FlexRIO_GPUIMAQ_TargetToHostFifoU64;

#endif
