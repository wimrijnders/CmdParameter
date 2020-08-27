#
# There are two builds possible, with output directories:
#
#   obj              - using emulator
#   obj-debug        - output debug info, using emulator
#
###########################################################

EXAMPLES = \
	Simple \
	Actions

EXAMPLE_TARGETS = $(patsubst %,$(OBJ_DIR)/bin/%,$(EXAMPLES))


help:
	@echo 'Usage:'
	@echo
	@echo '    make [DEBUG=1] [target]*'
	@echo
	@echo 'Where target:'
	@echo
	@echo '    help          - Show this text (default)'
	@echo '    all           - Build all test programs'
	@echo '    clean         - Delete all interim and target files'
	@echo '    test          - Run the unit tests'
	@echo '    lite          - Generate the light version of the library code'
	@echo
	@echo '    Any of the examples: $(EXAMPLES)'
	@echo
	@echo 'Flags:'
	@echo
	@echo '    DEBUG=1       - If specified, the source code and target code is shown on stdout when running a test'
	@echo


# Root directory of library
ROOT = Lib

# Compiler and default flags
CXX = g++

# Flag `-Wno-psabi` is to surpress a superfluous warning when compiling with GCC 6.3.0
CXX_FLAGS = \
 -fPIC \
 -Wconversion \
 -std=c++11 \
 -Wno-psabi \
 -I $(ROOT) \
 -MMD -MP -MF"$(@:%.o=%.d)"

# Object directory
OBJ_DIR = obj

# Debug mode
ifeq ($(DEBUG), 1)
  CXX_FLAGS += -Wall -DDEBUG -g -O0
  OBJ_DIR := $(OBJ_DIR)-debug
else
  CXX_FLAGS += -DNDEBUG -O3
endif

#
# Object files
#

#
# Following are included directly in the compile units;
# they should not be compiled to object files.
#
# List kept here as reference
#
# HIDDEN_OBJ := \
# 	Support/Strings

EXPORT_OBJ := \
	TypedParameter \
	Types/Types \
	Types/NoneParameter \
	Types/IntParameter \
	Types/OptionParameter \
	Types/StringParameter \
	Types/UnnamedParameter \
	Types/UnsignedIntParameter \
	Types/PositiveIntParameter \
	Types/PositiveFloatParameter \
	DefAction \
	CmdParameters \
	DefParameter \
	CmdValidation

LIB_OBJ := $(patsubst %,$(OBJ_DIR)/Lib/%.o,$(EXPORT_OBJ))
#$(info LIB_OBJ: $(LIB_OBJ))

# Dependencies from list of object files
DEPS := $(LIB_OBJ:.o=.d)
#$(info $(DEPS))
-include $(DEPS)

# Top-level targets

.PHONY: help clean all lib test lite $(EXAMPLES)

# Following prevents deletion of object files after linking
# Otherwise, deletion happens for targets of the form '%.o'
.PRECIOUS: $(OBJ_DIR)/%.o

HIDDEN_TARGET=$(OBJ_DIR)/libHidden.a
EXPORT_TARGET=$(OBJ_DIR)/libExport.a
TARGET=$(OBJ_DIR)/libCmdParameter.a
#$(info TARGET: $(TARGET))


all: $(OBJ_DIR) $(TARGET) $(EXAMPLE_TARGETS)

clean:
	rm -rf obj obj-debug generated

#
# Targets for the static library
#

$(TARGET): $(LIB_OBJ)
	@echo Creating $@
	@mkdir -p $(@D)
	@ar qcsT $@ $(LIB_OBJ)  # T - 'thin archives', allows merging of archives


# General compilation of cpp files
# The % will take into account subdirectories under OBJ_DIR.
$(OBJ_DIR)/%.o: %.cpp
	@echo Compiling $<
	@mkdir -p $(@D)
	@$(CXX) -c $(CXX_FLAGS) -o $@ $<


#
# Targets for Unit Tests
#
UNIT_TESTS := $(OBJ_DIR)/bin/runTests

# Source files for unit tests to include in compilation
UNITTEST_FILES = \
	Tests/testMain.cpp                \
	Tests/testCmdLine.cpp                \
	Tests/testParams.cpp              \
	Tests/testActions.cpp             \
	Tests/testOptions.cpp             \
	Tests/Support/cout_redirect.cpp   \
	Tests/TestData/TestParameters.cpp \
	Tests/TestData/TestActions.cpp

TESTS_OBJ = $(patsubst %cpp,$(OBJ_DIR)/%o,$(UNITTEST_FILES))
#$(info $(TESTS_OBJ))


$(UNIT_TESTS): $(TESTS_OBJ) $(TARGET)
	@echo Linking unit tests
	@mkdir -p $(@D)
	@$(CXX) $(CXX_FLAGS) -DDEBUG $(TESTS_OBJ) -L$(OBJ_DIR) -lCmdParameter -o $@

make_test: $(UNIT_TESTS)

#
# NOTE: tests are failing for the case of compiling without debug info,
#       notably for the 'Simple' tool. I don't care, I'm want to be rid
#       of the 'Simple' compilation anyway (in its current form).
#
test : $(UNIT_TESTS) | $(OBJ_DIR)/bin/Simple
	@echo Running unit tests with '$(UNIT_TESTS)'
	@$(UNIT_TESTS)

#
# Targets for Examples
#

$(OBJ_DIR)/bin/%: $(OBJ_DIR)/Examples/%.o $(TARGET)
	@echo Linking $@...
	@$(CXX) $(CXX_FLAGS) $^ $(LIBS) -o $@

$(EXAMPLES) :% :$(OBJ_DIR)/bin/%
