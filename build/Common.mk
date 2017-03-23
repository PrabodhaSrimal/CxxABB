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

# @version $Revision: 2317 $:
# @author  $Author: prabodar $:
# @date    $Date: 2012-11-16 10:04:43 +0530 (Fri, 16 Nov 2012) $:
# @id      $Id: Common.mk 2317 2012-11-16 04:34:43Z prabodar $:

## Common Makefile of build system

# check pre-requisites
ifndef PRJ.ROOT
$(error [ERROR] Project Root not defined !!! )
endif

ifndef TARGET.NAME
$(error [ERROR] TARGET.NAME not defined !!! );
endif

ifndef BLD.PATH
$(error [ERROR] BLD.PATH not defined !!! )
endif

ifndef BIN.PATH
$(error [ERROR] BIN.PATH not defined !!! )
endif

ifndef LIB.PATH
$(error [ERROR] LIB.PATH not defined !!! )
endif


# module specific paths. Relative to module base
export OBJ.DIR := obj
export SRC.DIR := src
export INC.DIR := include
export TST.DIR := test
export DEP.DIR := obj

#compile type should be defined in User.mk - Project specific
ifndef COMPILE_TYPE
	COMPILE_TYPE = D
endif

# target type should be defined in Module make file
ifndef TARGET.TYPE
	TARGET.TYPE = LIB
endif

#default output path
OUT.PATH = $(strip $(BIN.PATH))

## Guess SystemOS and Architecture
ifndef ARCH
ARCH := $(firstword $(shell uname -m))
endif

ifndef SYS
SYS := $(firstword $(shell uname -s))

ifeq ($(findstring CYGWIN,$(SYS)),CYGWIN)
SYS = CYGWIN
endif

ifeq ($(findstring MINGW,$(SYS)),MINGW)
SYS = MinGW
endif

endif ## Guess System and Architecture end

## Variable corrections
COMPILE.TYPE := $(strip $(COMPILE.TYPE))
TARGET.TYPE := $(strip $(TARGET.TYPE))
TARGET.NAME := $(strip $(TARGET.NAME))

include $(BLD.PATH)/config/${SYS}.${ARCH}.mk
include $(BLD.PATH)/Functions.mk

TARGET.PREFIX := $(strip $(TARGET.PREFIX))
TARGET.SUFFIX := $(strip $(TARGET.SUFFIX))


## TARGET.TYPE can be one of STLIB DYLIB EXE

ifeq ($(TARGET.TYPE), DYLIB)
OUT.PATH = $(LIB.PATH)
endif

ifeq ($(TARGET.TYPE), STLIB)
OUT.PATH = $(LIB.PATH)
endif

ifeq ($(TARGET.TYPE), EXE)
OUT.PATH = $(BIN.PATH)
endif

TARGET := $(OUT.PATH)/$(TARGET.PREFIX)$(TARGET.NAME)$(TARGET.ARCH.SUFFIX)$(TARGET.SUFFIX)
$(info [INFO] Target "$(TARGET.NAME)" of type "$(TARGET.TYPE)" output to "$(TARGET)" )

HEADERS := $(addprefix $(INC.DIR)/, $(HEADER))
SOURCES := $(addprefix $(SRC.DIR)/, $(SOURCE))
#OBJ = $(addprefix $(OBJ.DIR)/, $(SOURCE))
OBJ = $(addprefix $(OBJ.DIR)/, $(SOURCES))
#OBJ := $(addprefix $(SRC.DIR)/, $(SOURCE))
OBJECTS := $(addsuffix .o, $(basename $(OBJ)))

TEST.HEADERS := $(TEST.HEADER)
TEST.SOURCES := $(addprefix $(TST.DIR)/, $(TEST.SOURCE))
TEST.OBJ := $(addprefix $(OBJ.DIR)/, $(TEST.SOURCES))
TEST.OBJECTS := $(addsuffix .o, $(basename $(TEST.OBJ)))
TEST.TARGET := $(BIN.PATH)/Test$(TARGET.NAME)
TEST.LIBS := $(addsuffix $(TARGET.ARCH.SUFFIX), $(TEST.LIBS))
TEST.LINKPATH += -L$(LIB.PATH)

#$(info [INFO] Sources $(SOURCES))
#$(info [INFO] Objects $(OBJECTS))


# Add .d to Make's recognized suffixes.
#.SUFFIXES: .cpp
SUFFIXES += .d

#We don't need to clean up when we're making these targets
NODEPS := clean 

#These are the dependency files, which make will clean up after it creates them
DEPFILES := $(addprefix $(DEP.DIR)/, $(patsubst %.cpp,%.d,$(SOURCES)))
TEST.DEPFILES := $(addprefix $(DEP.DIR)/, $(patsubst %.cpp,%.d,$(TEST.SOURCES)))

#$(info [INFO] Deps $(DEPFILES))
#$(info [INFO] Deps $(TEST.DEPFILES))

## targets

.PHONY: all

all: $(TARGET)
	@$(ECHO) Called Target : $(MAKECMDGOALS)
	@$(ECHO) "Target  : "$(TARGET)

$(HEADERS):

$(TARGET): $(OBJECTS) $(HEADERS)
ifeq ($(TARGET.TYPE), STLIB)
	$(LIB) $@ $(OBJECTS)
else
	$(CXX) -o$@ $(OBJECTS) $(LINKFLAGS) $(LIBS)
endif

#ifeq ($(findstring test,$(MAKECMDGOALS)),test)
-include $(BLD.PATH)/TestSuite.mk
#endif

#Don't create dependencies when we're cleaning, for instance
ifeq (0, $(words $(findstring $(MAKECMDGOALS), $(NODEPS))))

#Chances are, these files don't exist.  GMake will create them and
#clean up automatically afterwards
-include $(DEPFILES)

endif


#This is the rule for creating the dependency files
$(DEP.DIR)/%.d: %.cpp
	$(TESTDIR) $(dir $@) || $(MKDIR) $(dir $@)
	$(MAKE.DEP)

$(OBJ.DIR)/$(SRC.DIR)/%.o: $(SRC.DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<


#.cpp.o:
#	$(CXX) $(CXXFLAGS) -o $@ -c $<
#
#.cpp:
#	$(CXX) $(CXXFLAGS) $< -o $@ $(LINKPATH) $(LIBS)

clean: test_clean
	-@$(RM) -f $(OBJECTS) $(DEPFILES) $(TEST.DEPFILES) $(TARGET)
	


