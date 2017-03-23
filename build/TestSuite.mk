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

# @version $Revision: 2321 $
# @author  $Author: prabodar $
# @date    $Date: 2012-11-16 12:04:57 +0530 (Fri, 16 Nov 2012) $
# @id      $Id: TestSuite.mk 2321 2012-11-16 06:34:57Z prabodar $

## Makefile for TestSuite - GTest and GMock



GTEST.DIR = $(SRC.PATH)/TestSuite
GTEST.HEADERS = $(GTEST.DIR)/include/gtest/*.h $(GTEST.DIR)/include/gtest/internal/*.h
GTEST.SRC = $(GTEST.DIR)/src/*.cc $(GTEST.DIR)/src/*.h $(GTEST.HEADERS)

GTEST.INCLUDES = -I$(GTEST.DIR)/include

#Don't create dependencies when we're cleaning, for instance
ifeq (0, $(words $(findstring $(MAKECMDGOALS), $(NODEPS))))

#Chances are, these files don't exist.  GMake will create them and
#clean up automatically afterwards
-include $(TEST.DEPFILES)

endif

#$(info [INFO] $(TEST.SOURCES))
#$(info [INFO] $(TEST.OBJECTS))

test: $(TARGET) $(TEST.TARGET)

runtest: test
	@$(ECHO) "[INFO] Running test cases..."
	@$(TEST.TARGET)

$(TEST.TARGET): $(TEST.OBJECTS) $(OBJ.DIR)/$(TST.DIR)/gtest_main.a
	$(CXX) $(CXXFLAGS) $(TEST.LINKPATH) $^ -o $@ $(TEST.LIBS) -lpthread 
	

$(TEST.SOURCES): $(TEST.HEADERS) $(GTEST_HEADERS)
	

#$(TEST.OBJECTS): $(TEST.SOURCES)
#	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(GTEST.INCLUDES) -c $< -o $@

$(OBJ.DIR)/$(TST.DIR)/%.o: $(TST.DIR)/%.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(GTEST.INCLUDES) -o $@ -c $<

# For simplicity and to avoid depending on Google Test's
# implementation details, the dependencies specified below are
# conservative and not optimized.  This is fine as Google Test
# compiles fast and for ordinary users its source rarely changes.

$(OBJ.DIR)/$(TST.DIR)/gtest-all.o : $(GTEST.SRC)
	$(CXX) $(CPPFLAGS) $(GTEST.INCLUDES) -I$(GTEST.DIR) $(CXXFLAGS) -c $(GTEST.DIR)/src/gtest-all.cc \
	-o $(OBJ.DIR)/$(TST.DIR)/gtest-all.o

$(OBJ.DIR)/$(TST.DIR)/gtest_main.o : $(GTEST.SRC)
	$(CXX) $(CPPFLAGS) $(GTEST.INCLUDES) -I$(GTEST.DIR) $(CXXFLAGS) -c $(GTEST.DIR)/src/gtest_main.cc \
	-o $(OBJ.DIR)/$(TST.DIR)/gtest_main.o

$(OBJ.DIR)/$(TST.DIR)/gtest.a : $(OBJ.DIR)/$(TST.DIR)/gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

$(OBJ.DIR)/$(TST.DIR)/gtest_main.a : $(OBJ.DIR)/$(TST.DIR)/gtest-all.o $(OBJ.DIR)/$(TST.DIR)/gtest_main.o
	$(AR) $(ARFLAGS) $@ $^

test_clean:
	-@$(RM) -f $(TEST.TARGET) $(TEST.OBJECTS) $(OBJ.DIR)/$(TST.DIR)/gtest-all.o \
	$(OBJ.DIR)/$(TST.DIR)/gtest_main.o $(OBJ.DIR)/$(TST.DIR)/gtest.a \
	$(OBJ.DIR)/$(TST.DIR)/gtest_main.a 
	
