#
# There are two builds possible, with output directories:
#
#   obj              - using emulator
#   obj-debug        - output debug info, using emulator
#
###########################################################

help:
	@echo 'Usage:'
	@echo
	@echo '    make [DEBUG=1] [target]*'
	@echo
	@echo 'Where target:'
	@echo
	@echo '    help          - Show this text'
	@echo '    all           - Build all test programs'
	@echo '    clean         - Delete all interim and target files'
	@echo '    test          - Run the unit tests'
	@echo
	@echo 'Flags:'
	@echo
	@echo '    DEBUG=1       - If specified, the source code and target code is shown on stdout when running a test'
	@echo


# Root directory of library
ROOT = Lib

# Compiler and default flags
CXX = g++
CXX_FLAGS = -Wconversion -std=c++11 -I $(ROOT) -MMD -MP -MF"$(@:%.o=%.d)"

# Object directory
OBJ_DIR = obj

# Debug mode
ifeq ($(DEBUG), 1)
  CXX_FLAGS += -Wall -DDEBUG -g -O0
  OBJ_DIR := $(OBJ_DIR)-debug
else
  CXX_FLAGS += -DNDEBUG -O3
endif

# Object files
LIB_OBJ := \
	Lib/Support/Strings.o \
	Lib/Types/Types.o \
	Lib/Types/NoneParameter.o \
	Lib/Types/IntParameter.o \
	Lib/Types/StringParameter.o \
	Lib/Types/UnnamedParameter.o \
	Lib/Types/UnsignedIntParameter.o \
	Lib/Types/PositiveIntParameter.o \
	Lib/Types/PositiveFloatParameter.o \
	Lib/DefParameter.o \
	Lib/CmdParameter.o

LIB_OBJ := $(patsubst %,$(OBJ_DIR)/%,$(LIB_OBJ))
#$(info LIB_OBJ: $(LIB_OBJ))


# Dependencies from list of object files
DEPS := $(LIB_OBJ:.o=.d)
#$(info $(DEPS))
-include $(DEPS)

# Top-level targets

.PHONY: help clean all lib test lite

# Following prevents deletion of object files after linking
# Otherwise, deletion happens for targets of the form '%.o'
.PRECIOUS: $(OBJ_DIR)/%.o

TARGET=$(OBJ_DIR)/libCmdParameter.a
#$(info TARGET: $(TARGET))


all: $(OBJ_DIR) $(TARGET)

clean:
	rm -rf obj obj-debug generated

# init directories
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)/bin
	@mkdir -p $(OBJ_DIR)/Lib/Types
	@mkdir -p $(OBJ_DIR)/Lib/Support


#
# Targets for the static library
#

$(TARGET): $(LIB_OBJ)
	@echo Creating $@
	@ar rcs $@ $^

$(OBJ_DIR)/Lib/%.o: $(ROOT)/%.cpp | $(OBJ_DIR)
	@echo Compiling $<
	@$(CXX) -c -o $@ $< $(CXX_FLAGS)


#
# Targets for Unit Tests
#

RUN_TESTS := $(OBJ_DIR)/bin/runTests

# Source files for unit tests to include in compilation
UNIT_TESTS = \
	Tests/testMain.cpp                \
	Tests/testParams.cpp              \
	Tests/Support/cout_redirect.cpp   \
	Tests/TestData/TestParameters.cpp

#
# For some reason, doing an interim step to .o results in linkage errors (undefined references).
# So this target compiles the source files directly to the executable.
#
# Flag `-Wno-psabi` is to surpress a superfluous warning when compiling with GCC 6.3.0
#
$(OBJ_DIR)/bin/runTests: $(UNIT_TESTS) $(TARGET)
	@echo Compiling unit tests
	@$(CXX) $(CXX_FLAGS) -Wno-psabi $^ -L$(OBJ_DIR) -lCmdParameter -o $@

make_test: $(OBJ_DIR)/bin/runTests
	@echo building unit tests

test : $(OBJ_DIR)/bin/runTests
	@echo Running unit tests with '$(RUN_TESTS)'
	@$(RUN_TESTS)


#
# Targets for Lite Output
#

# WTF interim cpp files deleted
.PRECIOUS: generated/Lib/%.h generated/Lib/%.cpp

LITE_OBJ := \
	Lib/Support/Strings.o \
	Lib/Types/Types.o \
	Lib/Types/NoneParameter.o \
	Lib/Types/IntParameter.o \
	Lib/DefParameter.o \
	Lib/CmdParameter.o

LITE_DEPS := $(LITE_OBJ:.o=.cpp)

LITE_INC := $(LITE_OBJ:.o=.h)
LITE_INC += Lib/Types/ParamType.h

LITE_GEN := $(patsubst %,generated/%,$(LITE_DEPS))
LITE_INC_GEN := $(patsubst %,generated/%,$(LITE_INC))

init_lite:
	@mkdir -p generated/Lib/Types
	@mkdir -p generated/Lib/Support
	@mkdir -p generated/obj/Lib/Types
	@mkdir -p generated/obj/Lib/Support

generated/Lib/%.cpp: $(ROOT)/%.cpp | init_lite
	@echo Converting to lite $<
	Lite/gen_lite.rb $@ $<

generated/Lib/%.h: $(ROOT)/%.h | init_lite
	@echo Converting include to lite $<
	Lite/gen_lite.rb $@ $<

#generated/Lib/Types/ParamType.h: $(ROOT)/Lib/Types/ParamType.h | init_lite
#	@echo Converting include to lite $<
#	Lite/gen_lite.rb $@ $<

generated/obj/Lib/%.o: generated/Lib/%.cpp | $(LITE_GEN) $(LITE_INC_GEN)
	@echo Compiling lite $<
	@$(CXX) -c -o $@ $< $(CXX_FLAGS)

GEN_TARGETS := $(patsubst %,generated/obj/%,$(LITE_OBJ))

generated/obj/libLite.a: $(GEN_TARGETS)
	@echo Creating $@
	@ar rcs $@ $^


lite: generated/obj/libLite.a


