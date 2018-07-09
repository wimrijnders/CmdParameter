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
  CXX_FLAGS += -DDEBUG -g -O0
  OBJ_DIR := $(OBJ_DIR)-debug
else
  CXX_FLAGS += -DNDEBUG -O3
endif

# Object files
LIB_OBJ := \
	Lib/Strings.o \
	Lib/CmdParameter.o

LIB_OBJ := $(patsubst %,$(OBJ_DIR)/%,$(LIB_OBJ))
#$(info LIB_OBJ: $(LIB_OBJ))


# Dependencies from list of object files
DEPS := $(LIB_OBJ:.o=.d)
#$(info $(DEPS))
-include $(DEPS)

# Top-level targets

.PHONY: help clean all lib test

# Following prevents deletion of object files after linking
# Otherwise, deletion happens for targets of the form '%.o'
.PRECIOUS: $(OBJ_DIR)/%.o

TARGET=$(OBJ_DIR)/libCmdParameter.a
$(info TARGET: $(TARGET))


all: $(OBJ_DIR) $(TARGET)

clean:
	rm -rf obj obj-debug


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

# sudo required for QPU-mode on Pi
ifeq ($(QPU), 1)
	RUN_TESTS := sudo $(RUN_TESTS)
endif


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


# init directories
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)/bin
	@mkdir -p $(OBJ_DIR)/Lib
