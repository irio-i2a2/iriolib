/******************************************************************************
 * $HeadURL:
 * $Id:
 *
 * Project		 : IRIO Driver
 *
 * Description	 : IRIO Driver Test Program for unit testing.
 *
 * Author		 : Universidad Politécnica de Madrid (UPM)
 *
 *
 * Copyright (c) : 2010-2015 Universidad Politécnica de Madrid (UPM)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 ******************************************************************************/

#ifndef irioCoreLIBTEST_HPP_
#define irioCoreLIBTEST_HPP_

#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <cxxtest/TestSuite.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>
#include <errno.h>

#include <irioDriver.h>
#include <irioDataTypes.h>
#include <irioHandlerAnalog.h>
#include <irioHandlerDigital.h>
#include <irioHandlerDMA.h>
#include <irioHandlerImage.h>
#include <irioHandlerSG.h>
#include <irioResourceFinder.h>


#define RESOURCE_NAME_DAQ "/dev/pxi6259"
#define NUMBER_OF_CARDS 1

#define MAX_BUFFER      50
#define MAX_DEVICES     50

class irioCoreLibTestSuite : public CxxTest::TestSuite
{
private:
        
public:
        int num_devices;

public:
        irioDrv_t p_DrvPvt;
        //TIRIOStatusCodes status;
        char *bitFilePath;
        void **fileContent;
        char *filePath;
        
         /* TEST CASES */
         void testInitialize() {
//        	 	status=IRIO_success;
//                printf("\n[L1] Testing Initialization");
//                status=(TIRIOStatusCodes)initializeLibrary(&p_DrvPvt);
//                TS_ASSERT_EQUALS(status, IRIO_success);

         }
         void testFinalize() {
//        	 	status=IRIO_success;
//        	 	printf("\n[L2] Testing Finalization");
//				status=(TIRIOStatusCodes)finalizeLibrary(&p_DrvPvt);
//				TS_ASSERT_EQUALS(status, IRIO_success);
		 }


         void testconfigureTarget() {
//				status=IRIO_success;
//				printf("\n[L2] Testing configureTarget");
//				bitFilePath=(char*)malloc(100);
//				strcpy(bitFilePath,"/home/mruiz/iriocore/target/test/c/lib/bitfiles/NiFpga_PXI7966RNi1483TEST.lvbitx");
//				//Device Model
//				strcpy(p_DrvPvt.PXIModel,"PXIe-7966R");
//				//Device Serial Number
//				strcpy(p_DrvPvt.DeviceSerialNumber,"177a2ad");
//				status=(TIRIOStatusCodes)configureTarget(&p_DrvPvt,bitFilePath);
//				TS_ASSERT_EQUALS(status, IRIO_success);
//				status=(TIRIOStatusCodes)finalizeLibrary(&p_DrvPvt);

		 }
         

};

#endif /* PXI6259LIBTEST_HPP_ */
