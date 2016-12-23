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


# List of examples to build (source files)
EXAMPLES :=  cRIO_DAQDMA.c

LIBRARIES := m rt irioCore


# List of library dependencies (at least kernel module user library)


IRIOCORE_DIR := $(realpath ../../../../target/lib)

# ======================================================================
# ADDITIONAL SETTINGS --- AVOID USING THESE (FIX UNIT STRUCTURE INSTEAD)
# ======================================================================

# Additional include directories
INCLUDE_DIRS := $(CODAC_ROOT)/include 

ifndef CODAC_ROOT
INCLUDE_DIRS += ../support
CFLAGS += -DCLOSE_VERSION_NIRIO
else
CFLAGS += -DOPEN_VERSION_NIRIO
endif

# Additional library directories
LIBRARY_DIRS = $(IRIOCORE_DIR) $(CODAC_ROOT)/lib 
ifeq ($(CONFIG_NIRIO_DMA_TO_GPU),1)
LIBRARIES_DIRS += $(CODAC_ROOT)/lib
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


# ======================================================================
# GENERIC BUILD RULES --- DO NOT MODIFY
# ======================================================================

# Sources
sources := $(EXAMPLES)
include_dirs := . ../../../main/c/include/ $(INCLUDE_DIRS)

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
