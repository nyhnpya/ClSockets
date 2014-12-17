##############################################################################
# Test Suite makefile
###############################################################################
#
# Author : Mark Carrier (mark@carrierlabs.com)                               
# Copyright (c) 2006 Mark Carrier.  All rights reserved.
#
################################################################################
#
# The following variables must be set for the makefile to work correctly
# (although they can be set to NULL):
#
# TARG_DIR               - Directory of sources
# TARG_TYPE              - library or executable
# TARG_NAME              - Name of final executable/library
# TARG_OPTIONS           - options to be used for compiling all version 
# TARG_OPTIMIZED_OPTIONS - target specific options to use when compiling
#                          for non-debug execution on Unix 
# TARG_DEBUG_OPTIONS     - target specific options to use when compiling
#                          to debug the program execution on Unix (debug)
# TARG_SOURCES           - Additional files used by this target (.cpp .h)
# 
##############################################################################
include ./Makefiles/Makefile.platform

TARG_ROOT              = .
TARG_DIR               = $(TARG_ROOT)
TARG_TYPE              = executable
TARG_NAME              = SocketUnitTest
TARG_INSTALL           = ./

TARG_OPTIONS           = 
TARG_DEBUG_OPTIONS     = 
TARG_OPTIMIZED_OPTIONS = 
TARG_CFLAGS            = -Wall
TARG_LDFLAGS           = -lcppunit -ldl -lclsocket -lpthread
TARG_SOURCES           = UnitTestMain.cpp         \
			 SocketTestSuite.cpp

###############################################################################
# Include a Makefile to configure the compiler, etc for the current platform
# (This is relative to the Makefile that included this Makefile!)
###############################################################################
include ./Makefiles/Makefile.macros
include ./Makefiles/Makefile.basic
