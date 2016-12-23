#+======================================================================
# $HeadURL: 
# $Id: 
#
# Project       : IRIO Driver
#
# Description   : IRIO Driver library test Makefile
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

CFLAGS=-Wall -g
LDFLAGS=-lm -lrt -lNiFpga -lniflexrio

SOURCES=$(wildcard *.c)
TESTS=$(SOURCES:.c=)

.PHONY : all test clean runner runner.cpp

all: runner $(TESTS)

# A rule that runs the unit tests
test: runner
#	./runner
# A rule that cleans everithing
clean:
	rm -f runner.cpp runner $(TESTS)

# How to build the test runner
runner: runner.cpp
	g++ -Wall -g  -L../../../lib -Wl,-rpath=../../../lib -L$(CODAC_ROOT)/lib -Wl,-rpath=$(CODAC_ROOT)/lib -I ../../../main/c/ -I ../../../main/c/include -I$(CODAC_ROOT)/include -lirioCore -o $@ $^
	
# How to generate the test runner
runner.cpp: irioCorelib-test.hpp
	cxxtestgen.py -o $@ --error-printer $^

%: %.c
	gcc $(CFLAGS) $(LDFLAGS) -I ../../../main/c/include -I$(CODAC_ROOT)/include -L../../../lib -Wl,-rpath=../../../lib -L$(CODAC_ROOT)/lib -Wl,-rpath=$(CODAC_ROOT)/lib -lirioCore -lNiFpga -lniflexrio -o $@ $<

