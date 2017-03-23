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
# @id      $Id: User.mk 21 2012-11-22 08:16:12Z prabodar $:

## CxxAbbCore Project - User customizations

## Project root path
## Can be hard-coded. Or else it will be guessed 
#PRJ.ROOT = 

## Compile type - {Release - R | Debug - D}
export COMPILE.TYPE = D

## Add modules and libraries sub directories to this list that needs to be built
export COMPONENTS = CxxAbbCore 

## CxxAbb library flags manually set
## CXXABB_OS_FAMILY
## CXXABB_ARCH
#export CXXABB.FLAGS = 
