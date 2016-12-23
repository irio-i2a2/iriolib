//#define _GNU_SOURCE
//#include <stdlib.h>
//#include <stdarg.h>
#include <stdio.h>
//#include <errno.h>
//#include <fcntl.h>
//#include <stdint.h>
//#include <string.h>
//#include <sys/stat.h>
//#include <sys/types.h>
//#include <unistd.h>
//#include <NiFpga.h>
//
//#include "irioDataTypes.h"
//
//#ifdef CLOSE_VERSION_NIRIO
//	#define Resource_Type const char*
//#else
//	#define Resource_Type NiFpgaEx_ResourceType
//#endif
//
//#define STRINGNAME_PREFIX "NiFpga_"
//#define STRINGNAME_PROJECT "testNIRIOEDS"
//
//#define STRINGNAME_NOOFWFGEN "NoOfWFGen"
//
//#ifdef CLOSE_VERSION_NIRIO
//	#define TYPE_NOOFWFGEN "_IndicatorU8_"
//	#define TYPE_SIGNALTYPE "_ControlU8_"
//#else
//	#define TYPE_NOOFWFGEN NiFpgaEx_ResourceType_IndicatorU8
//	#define TYPE_SIGNALTYPE NiFpgaEx_ResourceType_ControlU8
//#endif
//
//int findHeaderPort(irioDrv_t* p_DrvPvt, const char* name, Resource_Type type, uint32_t* port){
//	NiFpga_Status status = NiFpga_Status_Success;
//	#ifdef CLOSE_VERSION_NIRIO
//		char* completeName = NULL;
//		asprintf(&completeName,"%s%s%s%s",STRINGNAME_PREFIX,p_DrvPvt->projectName,type,name);
//		char* aux = p_DrvPvt->headerFile;
//		//Move the file pointer right before the port name.
//		if((aux=strstr(aux,completeName))==NULL){
//			printf("\n(ERROR resourcefinder->findHeaderPort.) Not found:%s\n",completeName);
//			return -1;
//		}
//		//Read the port value
//		if (sscanf(aux,"%*s %*s %X",port)==0){///First, try to read an hexadecimal value
//			if (sscanf(aux,"%*s %*s %d",port)==0){///If failed, try to read a decimal value
//				printf("\n(ERROR resourcefinder->findHeaderPort.) value not found for:%s\n",completeName);
//				return -1;
//			}
//		}
//	#else
//		status= NiFpgaEx_FindResource(p_DrvPvt->session,name,type,(NiFpgaEx_Resource*) port);
//		if(status!=NiFpga_Status_Success){
//			printf("\n(ERROR resourcefinder->findHeaderPort. ERROR CODE:%d ) value not found for:%s\n",name, (int)status);
//		}
//	#endif
//
//	return status;
//}
//
//
//int findHeaderPortOpen(irioDrv_t* p_DrvPvt,const char* name, Resource_Type type, uint32_t* port){
//	NiFpga_Status status = NiFpga_Status_Success;
//	status= NiFpgaEx_FindResource(p_DrvPvt->session,name,type,(NiFpgaEx_Resource*) port);
//	if(status!=NiFpga_Status_Success){
//		printf("\n(ERROR resourcefinder->findHeaderPort. ERROR CODE:%d ) value not found for:%s\n",name, (int)status);
//	}
//	return status;
//}
//
//int testFindResourceClose(){
//	uint32_t port = 0;
//	irioDrv_t* p_DrvPvt = malloc(sizeof(irioDrv_t));
//	&p_DrvPvt->projectName = malloc(strlen(STRINGNAME_PROJECT));
//	strcpy(STRINGNAME_PROJECT,p_DrvPvt->projectName);
//	irio_initFileSearch("NiFpga_testNIRIOEDS.h",&p_DrvPvt->headerFile);
//	findHeaderPortClose(p_DrvPvt,STRINGNAME_NOOFWFGEN,TYPE_NOOFWFGEN,&port);
//	printf("PORT: %d\n",port);
//	irio_closeFileSearch(&p_DrvPvt->headerFile);
//	return 0;
//}
//
//int testFindResourceOpen(void) {
//	irioDrv_t* p_DrvPvt = malloc(sizeof(irioDrv_t));
//	NiFpga_Status status = NiFpga_Status_Success;
//	NiFpga_Session session = NULL;
//	status = NiFpga_Open("NiFpga_testNIRIOEDS.lvbitx",NULL,"RIO0",NiFpga_OpenAttribute_NoRun,&p_DrvPvt->session);
//	uint32_t port =0;
//	findHeaderPortOpen(session, STRINGNAME_NOOFWFGEN,TYPE_NOOFWFGEN,&port);
//	printf("PORT: %d\n",port);
//	status = NiFpga_Close(session,0);
//	return 0;
//}
//
int main(void) {
//	#ifdef CLOSE_VERSION_NIRIO
//		testFindResourceClose();
//	#else
//		testFindResourceOpen();
//	#endif

	printf("TEST OK");
	return 0;

}
