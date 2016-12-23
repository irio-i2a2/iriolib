/******************************************************************************
 * $HeadURL: 
 * $Id: 
 *
 * Project		 : CODAC Core System
 *
 * Description	 : irioCorelib Test Program
 *
 * Author		 : Mariano Ruiz (UPM)
 *
 * Co-Author	 : 
 *
 * Copyright (c) : 2010-2016 ITER Organization,
 *				   CS 90 046
 *				   13067 St. Paul-lez-Durance Cedex
 *				   France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/
 

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <signal.h>
#include <getopt.h>
#include <sched.h>



volatile sig_atomic_t terminate = 0;

void sighandler(int signal) {
    switch (signal) {
    case SIGTERM:
    case SIGINT:
        terminate = 1;
        break;
    default:
        break;
    }
}




void usage(char *name)
{
	printf("Usage: %s <device number>\n", name);
}


int main(int argc, char** argv)
{

	int result_ok = -1;
	
	
    

    // Catch CTRL-C for proper shutdown
    struct sigaction sigact = {
        .sa_handler = sighandler,
        .sa_flags   = 0,
    };
    sigemptyset(&sigact.sa_mask);
    sigaction(SIGINT, &sigact, NULL);


	if (argc < 2)
	{
		usage(argv[0]);
		exit(0);
	}



	if(result_ok >= 0)
	{
		if(result_ok)
			printf("Test result: OK\n");
		else
			printf("Test result: FAIL\n");
	}

    return 0;
}


