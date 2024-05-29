##############################################################################
# SimpleSocket Library Makefile
###############################################################################
#
# The following variables must be set for the makefile to work correctly
# (although they can be set to NULL):
#
# TARG_DIR               - Directory of sources
# TARG_TYPE              - library or executable
# TARG_NAME              - Name of final executable/library
# TARG_OPTIONS           - options to be used for compiling all version 
# TARG_DEBUG_OPTIONS     - target specific options to use when compiling
#                          a debug version (BUILD=Debug)
# TARG_OPTIMIZED_OPTIONS - target specific options to use when compiling
#                          a non-debug execution (BUILD=Release)
# TARG_SOURCES           - Additional files used by this target (.cpp .h)
# 
##############################################################################
include $(MAKEFILEHOME)/Makefile.platform

TARG_SOURCE_DIR = $(CURDIR)
TARG_BUILD_MAJOR     = 1
TARG_BUILD_MINOR     = 4
TARG_BUILD_PATCH     = 0
TARG_BUILD_REVISION  = 0
TARGET_TYPE              = library
TARGET_NAME              = clsocket

TARG_OPTIONS           += -std=c++11

TARG_INCLUDES += -I .

TARG_CPP_SOURCES += SimpleSocket.cpp
TARG_CPP_SOURCES += ActiveSocket.cpp
TARG_CPP_SOURCES += PassiveSocket.cpp

###############################################################################
# Installation section
###############################################################################

# package control file
PACKAGE_DESCRIPTION = "FederatedDesigns cross platform socket library"
PACKAGE_DEV_DESCRIPTION = "FederatedDesigns cross platform socket development library"
PACKAGE_MAINTAINER = "Mark Carrier \<mark@federateddesigns.com\>"

# package include files
PACKAGE_INCLUDE_FILES = $(CURDIR)/ActiveSocket.h
PACKAGE_INCLUDE_FILES += $(CURDIR)/Host.h
PACKAGE_INCLUDE_FILES += $(CURDIR)/HTTPActiveSocket.h
PACKAGE_INCLUDE_FILES += $(CURDIR)/PassiveSocket.h
PACKAGE_INCLUDE_FILES += $(CURDIR)/SimpleSocket.h
PACKAGE_INCLUDE_FILES += $(CURDIR)/StatTimer.h


###############################################################################
# Include a Makefile to configure the compiler, etc for the current platform
# (This is relative to the Makefile that included this Makefile!)
###############################################################################
include $(MAKEFILEHOME)/Makefile.macros
include $(MAKEFILEHOME)/Makefile.basic

