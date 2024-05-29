##############################################################################
# SimpleSocket Library Makefile
###############################################################################
include $(MAKEFILEHOME)/Makefile.platform

TARG_SOURCE_DIR = $(CURDIR)
TARG_BUILD_MAJOR     = 2
TARG_BUILD_MINOR     = 0
TARG_BUILD_PATCH     = 0
TARG_BUILD_REVISION  = 0
TARGET_TYPE              = library
TARGET_NAME              = clsocket

TARG_OPTIONS           += -std=c++11

###############################################################################
# compilation flags
###############################################################################
# Includes
TARG_INCLUDES += -I $(CURDIR)/include

TARG_CPP_SOURCES += src/SimpleSocket.cpp
TARG_CPP_SOURCES += src/ActiveSocket.cpp
TARG_CPP_SOURCES += src/PassiveSocket.cpp

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

