#+======================================================================
# $HeadURL: 
# $Id: 
#
# Project       : IRIO Driver
#
# Description   : IRIO Driver module C code Makefile
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

#SUBDIRS=$(dir $(wildcard */Makefile))
SUBDIRS:= lib 
SUBDIRS += examples
BOLD=\e[1m
NC=\e[0m

all:
	@$(foreach dir, $(SUBDIRS), echo -e "$(BOLD)Building $(dir:/=)...$(NC)" && $(MAKE) -C $(dir) $@ &&) :
%:
	@$(foreach dir, $(SUBDIRS), $(MAKE) -C $(dir) $@ &&) :

