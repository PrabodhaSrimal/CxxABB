####################################################################################################
#                                                                                                  #
#                                                          _|        _|                            #
#                    _|_|_|  _|    _|  _|    _|    _|_|_|  _|_|_|    _|_|_|                        #
#                  _|          _|_|      _|_|    _|    _|  _|    _|  _|    _|                      #
#                  _|        _|    _|  _|    _|  _|    _|  _|    _|  _|    _|                      #
#                    _|_|_|  _|    _|  _|    _|    _|_|_|  _|_|_|    _|_|_|                        #
#                                                                                                  #
#                           CxxABB - C++ Application Building Blocks                               # 
#                                                                                                  #
#                  Copyright (C) 2017 Prabodha Srimal <prabodha007@gmail.com>                      #
#                                                                                                  #
####################################################################################################

# @version $Revision: 21 $:
# @author  $Author: prabodar $:
# @date    $Date: 2012-11-22 13:46:12 +0530 (Thu, 22 Nov 2012) $:
# @id      $Id: Makefile 21 2012-11-22 08:16:12Z prabodar $:

## Global Posix Makefile
## This makefile is used only for posix systems

#include user specific configs, if exist
sinclude User.mk

ifndef PRJ.ROOT
export PRJ.ROOT := $(dir $(CURDIR)/$(word $(words $(MAKEFILE_LIST)),$(MAKEFILE_LIST)))
$(info [INFO] Project Root is assumed as $(PRJ.ROOT) )
endif

# depend on PRJ.ROOT set paths
export BIN.PATH := $(PRJ.ROOT)bin
export LIB.PATH := $(PRJ.ROOT)lib
export BLD.PATH := $(PRJ.ROOT)build
export SRC.PATH := $(PRJ.ROOT)src

.PHONY: subdirs $(COMPONENTS)

subdirs: $(COMPONENTS)

$(COMPONENTS):
	+$(MAKE) -C $(SRC.PATH)/$@/ $(MAKECMDGOALS)

## Module wise dependancies
#ModuleA: CxxAbbCore
sinclude UserDep.mk

test: subdirs
runtest: subdirs

clean: subdirs
