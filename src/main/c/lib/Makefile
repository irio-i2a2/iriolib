#+======================================================================
# $HeadURL: 
# $Id: 
#
# Project       : IRIO Driver
#
# Description   : IRIO Driver library api Makefile
#
# Authors       : Mariano Ruiz, Diego Sanz, Sergio Esquembri, Enrique Bernal, Alvaro Bustos
# Authors Affiliation    : Universidad Politécnica de Madrid (UPM)
#
# Copyright (C) : 2010-2015 Universidad Politécnica de Madrid (UPM)
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
# Colors
BOLD=\e[1m
NC=\e[0m

C=gcc
CPP=g++
CFLAGS = -c -Wall -fPIC -g #-Wextra check 
CFLAGS += -D_GNU_SOURCE #asprintf
#CFLAGS += -DIRIO_GPU

TARGET_LIB = irioCore

BIN_TARGET_PATH=../../../../target/bin
TARGET_PATH = ../../../../target
TARGET_LIB_PATH = ../../../../target/lib
OBJECT_DIR = $(TARGET_LIB_PATH)/obj

INCLUDE_DIRS =-I../include 

SRCS_LIB= irioDriver.c irioResourceFinder.c irioHandlerAnalog.c irioHandlerDigital.c irioHandlerDMA.c irioHandlerSG.c irioHandlerImage.c irioError.c
HDRS_LIB= irioDriver.h irioResourceFinder.h irioHandlerAnalog.h irioHandlerDigital.h irioHandlerDMA.h irioHandlerSG.h irioHandlerImage.h irioError.h

ifeq ($(CONFIG_NIRIO_DMA_TO_GPU),1)
IRIO_GPU = 1
CFLAGS += -DIRIO_GPU
endif

ifdef IRIO_GPU
SRCS_LIB+=irioHandlerDMAGPU.c 
HDRS_LIB+= irioHandlerDMAGPU.h
endif

OBJS= $(SRCS_LIB:%.c=$(OBJECT_DIR)/%.o)

ifdef CODAC_ROOT
LDFLAGS += -lNiFpga -lniflexrio  -L$(CODAC_ROOT)/lib --coverage -fprofile-arcs -ftest-coverage -lgcov
INCLUDE_DIRS += -I$(CODAC_ROOT)/include
CFLAGS += -DOPEN_VERSION_NIRIO
else 
SUPPORT_SRC += ../support/NiFpga.c
SUPPORT_OBJ = $(addprefix $(OBJECT_DIR)/, $(subst .c,.o,$(notdir $(SUPPORT_SRC))))
INCLUDE_DIRS += -I../support -I/usr/local/natinst/niflexrio/include
LDFLAGS += -ldl -lNiFpga -lniflexrio
CFLAGS += -DCLOSE_VERSION_NIRIO
endif

# Uncomment the following lines to use GCov
#LD_FLAGS += --coverage -fprofile-arcs -ftest-coverage -lgcov
#CFLAGS += -fprofile-arcs -ftest-coverage --coverage 
########################################



all:  staticlib dynamiclib
	@mkdir -p $(TARGET_PATH)/include
	@cp -r ../include $(TARGET_PATH) 

dynamiclib: $(OBJS) $(SUPPORT_OBJ)
	@mkdir -p $(TARGET_LIB_PATH)
	$(C) -shared $(LDFLAGS) -o $(TARGET_LIB_PATH)/libirioCore.so $^

staticlib: $(OBJS) $(SUPPORT_OBJ)
	@mkdir -p $(TARGET_LIB_PATH)
	$(AR) $(ARFLAGS) $(TARGET_LIB_PATH)/libirioCore.a $^

$(OBJS): $(SRCS_LIB) $(HDRS_LIB:%=../include/%)
	@mkdir -p $(BIN_TARGET_PATH) 
	@mkdir -p $(OBJECT_DIR)
	$(C) $(CFLAGS) $(INCLUDE_DIRS) -o $@ $(subst .o,.c,$(notdir $@))

$(SUPPORT_OBJ): $(SUPPORT_SRC)
	@mkdir -p $(BIN_TARGET_PATH) 
	@mkdir -p $(OBJECT_DIR)
	$(C) $(CFLAGS) $(INCLUDE_DIRS) -o $@ $(SUPPORT_SRC)
	mkdir -p $(TARGET_PATH)/include
	cp $(subst .c,.h,$(SUPPORT_SRC)) $(TARGET_PATH)/include
	
install:
		@echo -e "$(BOLD)Installing...$(NC)"
		@sudo mkdir -p $(IRIO_INSTALL_PATH)/include/iriocore
		@sudo cp -r $(TARGET_PATH)/include/. $(IRIO_INSTALL_PATH)/include/iriocore
		@sudo mkdir -p $(IRIO_INSTALL_PATH)/lib64/iriocore
		@find $(TARGET_PATH)/lib/. -type f ! -name '*.o' -exec cp {} $(IRIO_INSTALL_PATH)/lib64/iriocore/ \;
		
clean:
	@rm -rf $(TARGET_PATH) 
	@sudo rm -rf $(IRIO_INSTALL_PATH)/include/iriocore
	@sudo rm -rf $(IRIO_INSTALL_PATH)/src/iriocore
	@sudo rm -rf $(IRIO_INSTALL_PATH)/lib64/iriocore
	 

