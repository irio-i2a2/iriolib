/*
 * Generated with the FPGA Interface C API Generator 13.0.0
 * for NI-RIO 13.0.0 or later.
 */

#ifndef __NiFpga_FlexRIO_GPUDAQ_h__
#define __NiFpga_FlexRIO_GPUDAQ_h__

#ifndef NiFpga_Version
   #define NiFpga_Version 1300
#endif

#include "NiFpga.h"

/**
 * The filename of the FPGA bitfile.
 *
 * This is a #define to allow for string literal concatenation. For example:
 *
 *    static const char* const Bitfile = "C:\\" NiFpga_FlexRIO_GPUDAQ_Bitfile;
 */
#define NiFpga_FlexRIO_GPUDAQ_Bitfile "NiFpga_FlexRIO_GPUDAQ.lvbitx"

/**
 * The signature of the FPGA bitfile.
 */
static const char* const NiFpga_FlexRIO_GPUDAQ_Signature = "C7EBBF4ADD3362B8A4251AF931A70ED2";

typedef enum
{
   NiFpga_FlexRIO_GPUDAQ_IndicatorBool_DI0 = 0x8056,
   NiFpga_FlexRIO_GPUDAQ_IndicatorBool_DI1 = 0x8052,
   NiFpga_FlexRIO_GPUDAQ_IndicatorBool_InitDone = 0x80D6,
   NiFpga_FlexRIO_GPUDAQ_IndicatorBool_PXIIN0 = 0x8042,
   NiFpga_FlexRIO_GPUDAQ_IndicatorBool_PXIIN1 = 0x803A,
   NiFpga_FlexRIO_GPUDAQ_IndicatorBool_RIOAdapterCorrect = 0x80CE,
   NiFpga_FlexRIO_GPUDAQ_IndicatorBool_auxDI0 = 0x804E,
   NiFpga_FlexRIO_GPUDAQ_IndicatorBool_auxDI1 = 0x804A,
} NiFpga_FlexRIO_GPUDAQ_IndicatorBool;

typedef enum
{
   NiFpga_FlexRIO_GPUDAQ_IndicatorU8_DevProfile = 0x80BE,
   NiFpga_FlexRIO_GPUDAQ_IndicatorU8_DevQualityStatus = 0x80C6,
   NiFpga_FlexRIO_GPUDAQ_IndicatorU8_Platform = 0x80B2,
   NiFpga_FlexRIO_GPUDAQ_IndicatorU8_SGNo = 0x8036,
} NiFpga_FlexRIO_GPUDAQ_IndicatorU8;

typedef enum
{
   NiFpga_FlexRIO_GPUDAQ_IndicatorI16_DevTemp = 0x80C2,
} NiFpga_FlexRIO_GPUDAQ_IndicatorI16;

typedef enum
{
   NiFpga_FlexRIO_GPUDAQ_IndicatorU16_DMATtoGPUOverflows = 0x809A,
} NiFpga_FlexRIO_GPUDAQ_IndicatorU16;

typedef enum
{
   NiFpga_FlexRIO_GPUDAQ_IndicatorI32_AI0 = 0x808C,
   NiFpga_FlexRIO_GPUDAQ_IndicatorI32_AI1 = 0x8088,
   NiFpga_FlexRIO_GPUDAQ_IndicatorI32_auxAI0 = 0x8084,
   NiFpga_FlexRIO_GPUDAQ_IndicatorI32_auxAI1 = 0x8080,
} NiFpga_FlexRIO_GPUDAQ_IndicatorI32;

typedef enum
{
   NiFpga_FlexRIO_GPUDAQ_IndicatorU32_Fref = 0x80C8,
   NiFpga_FlexRIO_GPUDAQ_IndicatorU32_InsertedIOModuleID = 0x80D0,
   NiFpga_FlexRIO_GPUDAQ_IndicatorU32_SGFref0 = 0x801C,
   NiFpga_FlexRIO_GPUDAQ_IndicatorU32_SGFref1 = 0x8004,
} NiFpga_FlexRIO_GPUDAQ_IndicatorU32;

typedef enum
{
   NiFpga_FlexRIO_GPUDAQ_ControlBool_AOEnable0 = 0x806E,
   NiFpga_FlexRIO_GPUDAQ_ControlBool_AOEnable1 = 0x806A,
   NiFpga_FlexRIO_GPUDAQ_ControlBool_DAQStartStop = 0x80BA,
   NiFpga_FlexRIO_GPUDAQ_ControlBool_DMATtoGPUEnable0 = 0x80A2,
   NiFpga_FlexRIO_GPUDAQ_ControlBool_DMATtoGPUEnable1 = 0x8092,
   NiFpga_FlexRIO_GPUDAQ_ControlBool_DO0 = 0x8066,
   NiFpga_FlexRIO_GPUDAQ_ControlBool_DO1 = 0x8062,
   NiFpga_FlexRIO_GPUDAQ_ControlBool_DebugMode = 0x80B6,
   NiFpga_FlexRIO_GPUDAQ_ControlBool_PXIOUT0 = 0x8046,
   NiFpga_FlexRIO_GPUDAQ_ControlBool_PXIOUT1 = 0x803E,
   NiFpga_FlexRIO_GPUDAQ_ControlBool_auxDO0 = 0x805E,
   NiFpga_FlexRIO_GPUDAQ_ControlBool_auxDO1 = 0x805A,
} NiFpga_FlexRIO_GPUDAQ_ControlBool;

typedef enum
{
   NiFpga_FlexRIO_GPUDAQ_ControlU8_SGSignalType0 = 0x8022,
   NiFpga_FlexRIO_GPUDAQ_ControlU8_SGSignalType1 = 0x800A,
} NiFpga_FlexRIO_GPUDAQ_ControlU8;

typedef enum
{
   NiFpga_FlexRIO_GPUDAQ_ControlU16_DMATtoGPUSamplingRate0 = 0x809E,
   NiFpga_FlexRIO_GPUDAQ_ControlU16_DMATtoGPUSamplingRate1 = 0x8096,
   NiFpga_FlexRIO_GPUDAQ_ControlU16_SGAmp0 = 0x8032,
   NiFpga_FlexRIO_GPUDAQ_ControlU16_SGAmp1 = 0x801A,
} NiFpga_FlexRIO_GPUDAQ_ControlU16;

typedef enum
{
   NiFpga_FlexRIO_GPUDAQ_ControlI32_AO0 = 0x807C,
   NiFpga_FlexRIO_GPUDAQ_ControlI32_AO1 = 0x8078,
   NiFpga_FlexRIO_GPUDAQ_ControlI32_auxAO0 = 0x8074,
   NiFpga_FlexRIO_GPUDAQ_ControlI32_auxAO1 = 0x8070,
} NiFpga_FlexRIO_GPUDAQ_ControlI32;

typedef enum
{
   NiFpga_FlexRIO_GPUDAQ_ControlU32_SGFreq0 = 0x802C,
   NiFpga_FlexRIO_GPUDAQ_ControlU32_SGFreq1 = 0x8014,
   NiFpga_FlexRIO_GPUDAQ_ControlU32_SGPhase0 = 0x8028,
   NiFpga_FlexRIO_GPUDAQ_ControlU32_SGPhase1 = 0x8010,
   NiFpga_FlexRIO_GPUDAQ_ControlU32_SGUpdateRate0 = 0x8024,
   NiFpga_FlexRIO_GPUDAQ_ControlU32_SGUpdateRate1 = 0x800C,
} NiFpga_FlexRIO_GPUDAQ_ControlU32;

typedef enum
{
   NiFpga_FlexRIO_GPUDAQ_IndicatorArrayU8_DMATtoGPUFrameType = 0x80AA,
   NiFpga_FlexRIO_GPUDAQ_IndicatorArrayU8_DMATtoGPUSampleSize = 0x80A6,
   NiFpga_FlexRIO_GPUDAQ_IndicatorArrayU8_FPGAVIversion = 0x80DA,
} NiFpga_FlexRIO_GPUDAQ_IndicatorArrayU8;

typedef enum
{
   NiFpga_FlexRIO_GPUDAQ_IndicatorArrayU8Size_DMATtoGPUFrameType = 2,
   NiFpga_FlexRIO_GPUDAQ_IndicatorArrayU8Size_DMATtoGPUSampleSize = 2,
   NiFpga_FlexRIO_GPUDAQ_IndicatorArrayU8Size_FPGAVIversion = 2,
} NiFpga_FlexRIO_GPUDAQ_IndicatorArrayU8Size;

typedef enum
{
   NiFpga_FlexRIO_GPUDAQ_IndicatorArrayU16_DMATtoGPUBlockNWords = 0x8000,
   NiFpga_FlexRIO_GPUDAQ_IndicatorArrayU16_DMATtoGPUNCh = 0x80AC,
} NiFpga_FlexRIO_GPUDAQ_IndicatorArrayU16;

typedef enum
{
   NiFpga_FlexRIO_GPUDAQ_IndicatorArrayU16Size_DMATtoGPUBlockNWords = 2,
   NiFpga_FlexRIO_GPUDAQ_IndicatorArrayU16Size_DMATtoGPUNCh = 2,
} NiFpga_FlexRIO_GPUDAQ_IndicatorArrayU16Size;

typedef enum
{
   NiFpga_FlexRIO_GPUDAQ_TargetToHostFifoU64_DMATtoGPU0 = 1,
   NiFpga_FlexRIO_GPUDAQ_TargetToHostFifoU64_DMATtoGPU1 = 0,
} NiFpga_FlexRIO_GPUDAQ_TargetToHostFifoU64;

#endif
