#+======================================================================
# $HeadURL: 
# $Id: 
#
# Project       : IRIO Driver
#
# Description   : IRIO Driver library examples Makefile
#
# Authors       : Mariano Ruiz, Diego Sanz, Sergio Esquembri, Enrique Bernal, Alvaro Bustos
# Authors Affiliation    : Universidad Politécnica de Madrid (UPM) (Technical University of Madrid)
#
# Copyright (C) : 2010-2015 Universidad Politécnica de Madrid (UPM) (Technical University of Madrid)
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
#
#-======================================================================


# ======================================================================
# UNIT SPECIFIC SETTINGS
# ======================================================================

#ifndef CODAC_ROOT
#IRIO_INSTALL_PATH=/usr/local
#endif

# List of examples to build (source files)
EXAMPLES := cRIO_IO.c cRIO_DAQDMA.c
EXAMPLES += FlexRIO_noModule.c FlexRIO_onlyResources.c FlexRIO_mod5761.c FlexRIO_mod5761-perf.c FlexRIO_mod5734.c FlexRIO_mod6581.c FlexRIO_mod1483-Image-CPU.c FlexRIO_mod1483-uart.c	  

LIBRARIES := m rt irioCore

ifeq ($(CONFIG_NIRIO_DMA_TO_GPU),1)
IRIO_GPU = 1
CFLAGS += -DIRIO_GPU
EXAMPLES += FlexRIO_mod1483-7966-Image-GPU.c
LIBRARIES += iriocuda 
endif



# List of library dependencies (at least kernel module user library)


IRIOCORE_DIR := $(realpath ../../../../target/lib)
CUDA_DIR := /usr/local/cuda-6.5
 

# ======================================================================
# ADDITIONAL SETTINGS --- AVOID USING THESE (FIX UNIT STRUCTURE INSTEAD)
# ======================================================================

# Additional include directories
INCLUDE_DIRS := $(CODAC_ROOT)/include $(CUDA_DIR)/include 

# Additional library directories
LIBRARY_DIRS := $(IRIOCORE_DIR) $(CODAC_ROOT)/lib 

ifndef CODAC_ROOT
INCLUDE_DIRS += ../support /usr/local/natinst/niflexrio/include
LIBRARY_DIRS +=  /usr/local/natinst/niflexrio/lib
LIBRARIES += m rt niflexrio
CFLAGS += -DCLOSE_VERSION_NIRIO
else
CFLAGS += -DOPEN_VERSION_NIRIO
endif

ifeq ($(CONFIG_NIRIO_DMA_TO_GPU),1)
INCLUDE_DIRS += $(CODAC_ROOT)/include/iriocuda
endif


# ======================================================================
# UNIT SPECIFIC COMPILER AND LINKER FLAGS
# ======================================================================

# Optimized release builds (default)
CFLAGS_RELEASE := -fPIC -Wall -O3
LDFLAGS_RELEASE := -Wall -Wl,-rpath=$(IRIOCORE_DIR) 

# Debug builds (make debug)
CFLAGS_DEBUG := -fPIC -Wall -O0 -g -ggdb -DDEBUG 
LDFLAGS_DEBUG := -Wall -g  -Wl,-rpath=$(IRIOCORE_DIR) 


ifeq ($(CONFIG_NIRIO_DMA_TO_GPU),1)
LDFLAGS_RELEASE += -Wl,-rpath=$(CUDA_DIR)/lib64 
LDFLAGS_DEBUG += -Wl,-rpath=$(CUDA_DIR)/lib64 
endif
# Uncomment the following lines to use GCov
#CFLAGS_DEBUG += -fprofile-arcs -ftest-coverage
#LDFLAGS_DEBUG += -fprofile-arcs -ftest-coverage -lgcov
########################################


# ======================================================================
# GENERIC BUILD RULES --- DO NOT MODIFY
# ======================================================================

# Sources
sources := $(EXAMPLES)
include_dirs := . ../include $(INCLUDE_DIRS) 

# Binaries
alltargets := $(basename $(sources))

# Library dependencies
library_dirs := ../lib $(LIBRARY_DIRS)
libraries := pthread $(LIBRARIES)

# Compiler/linker flags
CFLAGS += $(include_dirs:%=-I"%") -D_GNU_SOURCE
LDFLAGS += $(library_dirs:%=-L"%") $(libraries:%=-l%) 
CC := $(if $(findstring .cpp, $(sources)), $(CXX), $(CC))

# Colors
BOLD=\e[1m
NC=\e[0m

# Rules
.PHONY: .depend all release debug clean

all: debug

release: CFLAGS += $(CFLAGS_RELEASE)
release: LDFLAGS += $(LDFLAGS_RELEASE)
release: .depend $(alltargets)

debug: CFLAGS += $(CFLAGS_DEBUG)
debug: LDFLAGS += $(LDFLAGS_DEBUG)
debug: .depend $(alltargets)

clean:
	@$(RM) -f $(alltargets) .depend *~
	@sudo rm -rf $(IRIO_INSTALL_PATH)/irio_examples
	@sudo rm -rf $(IRIO_INSTALL_PATH)/src/iriocore/
	
install:
	@mkdir -p ../../../../target/main
	@cp -r ../../../main/. ../../../../target/main
	@mkdir -p $(IRIO_INSTALL_PATH)/irio_examples
	@cp -r ../../../../target/main/c/examples/. $(IRIO_INSTALL_PATH)/irio_examples
	@sudo mkdir -p $(IRIO_INSTALL_PATH)/src/iriocore
	@find ../../../../target/main/c/lib/ -type f ! -name Makefile -exec cp -r {} $(IRIO_INSTALL_PATH)/src/iriocore/ \;
	
%: %.cpp
	@echo -e "$(BOLD)>>> Building '$@' example...$(NC)"
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $^

%: %.c
	@echo -e "$(BOLD)>>> Building '$@' example...$(NC)"
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $^

# Generate header file dependencies
.depend: $(sources)
	@$(CC) $(CFLAGS) -MM $^ > $@

-include .depend
