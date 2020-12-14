# How does this makefile work:
#
# This makefile uses a modular definition to the rules to allow the building
# of the same test files but with different build options and compilers. This
# is enabled by first specifing the module specific options, then souring
# the cpp_rules and test_exe_rules Makefiles. The module options are used 
# to create new rules based on the MODULE_NAME variable

# Enable second expansion
.SECONDEXPANSION:

# Clear all built in suffixes
.SUFFIXES:

# Ensure that command failures from tee are returned
SHELL = /bin/bash -o pipefail

# Include the common files
THIS_MAKEFILE_DIR := $(patsubst %/,%,$(dir $(lastword $(MAKEFILE_LIST))))
TEST_BCOMMON_DIR := $(THIS_MAKEFILE_DIR)/bcommon
include $(TEST_BCOMMON_DIR)/common.mk

##############################################################################
# Google Test settings
##############################################################################
GTEST_ROOT = ../extlibs/googletest

# googlemock
GTEST_GMOCK_ROOT = $(GTEST_ROOT)/googlemock
GTEST_GMOCK_INC_ROOT = $(GTEST_GMOCK_ROOT)/include
GTEST_GMOCK_SRC_ROOT = $(GTEST_GMOCK_ROOT)/src
GTEST_GMOCK_OBJ_LIST = \
	gmock-all.obj

# googletest
GTEST_GTEST_ROOT = $(GTEST_ROOT)/googletest
GTEST_GTEST_INC_ROOT = $(GTEST_GTEST_ROOT)/include
GTEST_GTEST_SRC_ROOT = $(GTEST_GTEST_ROOT)/src
GTEST_GTEST_OBJ_LIST = \
	gtest-all.obj

GTEST_OBJ_LIST = $(GTEST_GMOCK_OBJ_LIST) $(GTEST_GTEST_OBJ_LIST)

##############################################################################
# Boost settings
##############################################################################
BOOST_ROOT_DIR = 

##############################################################################
# Directories
##############################################################################
# This is where we store the firmware models
SYSTEM_DIR := system

# This is where we store the unittests
UNITTEST_DIR := test

# This is where the NIOS code files are 
NIOS_CODE_DIR := ../design/fw/code
NIOS_OBJS_DIR := nios_code

# Code Coverage
LCOV_ROOT_DIR := .
LCOV_OUTPUT_FILE := test_coverage.lcov
LCOV_REPORT_DIR := lcov_coverage_report

##############################################################################
# Global CPP and LD flags affecting all targets
##############################################################################
# Add Gtest to the include
GLOBAL_CPPFLAGS += -I$(GTEST_GMOCK_ROOT) -I$(GTEST_GMOCK_INC_ROOT) -I$(GTEST_GTEST_ROOT) -I$(GTEST_GTEST_INC_ROOT)

GLOBAL_CPPFLAGS += -I$(BOOST_ROOT_DIR)/include -DBOOST_STACKTRACE_USE_ADDR2LINE
GLOBAL_LDFLAGS += -L$(BOOST_ROOT_DIR)/lib

# Add SYSTEM_MOCK 
GLOBAL_CPPFLAGS += -I. -I$(SYSTEM_DIR)

# Include BSP directoroes
BSP_DIR := ../design/fw/bsp/bsp
GLOBAL_CPPFLAGS += -I$(BSP_DIR) -I$(BSP_DIR)/HAL/inc -I$(BSP_DIR)/drivers/inc

# Include NIOS code files 
GLOBAL_CPPFLAGS += -I$(NIOS_CODE_DIR)/inc

# To build NIOS in unittest mode
GLOBAL_CPPFLAGS += -DUNITTEST_MODE_ON

# Add system libs
GLOBAL_LDFLAGS += -ldl

##############################################################################
# Source file OBJs
##############################################################################
MAIN_NIOS_OBJS_LIST = \
	$(NIOS_OBJS_DIR)/gpio_utils.obj \
	$(NIOS_OBJS_DIR)/sqrt.obj \
	$(NIOS_OBJS_DIR)/utils.obj \
	$(NIOS_OBJS_DIR)/nios_main.obj \

MAIN_SYSTEM_OBJS_LIST = \
	$(SYSTEM_DIR)/altsqrt_mock.obj \
	$(SYSTEM_DIR)/onchip_mem_mock.obj \
	$(SYSTEM_DIR)/system_mock.obj \

MAIN_UNITTEST_OBJS_LIST = \
	$(UNITTEST_DIR)/test_sanity.obj \
	$(UNITTEST_DIR)/test_system_mock.obj \
	$(UNITTEST_DIR)/test_main_flow.obj \

##############################################################################
# Test executable targets
##############################################################################
TEST_EXE_NAMES := 

##############################################################################
# Set default goal before any targets. The default goal here is "all"
##############################################################################
GTEST_COMPILE_ONLY ?= 0
ALL_TARGET_PREREQS := run_asan_ubsan_main

ifeq ($(GTEST_COMPILE_ONLY), 1)
ALL_TARGET_PREREQS := $(patsubst run_%,%,$(ALL_TARGET_PREREQS))
endif

.PHONY: all
all: $(ALL_TARGET_PREREQS) 

##############################################################################
# Global clean target
##############################################################################
.PHONY: clean
clean :
	$(RMDIR) $(WORK_DIR)
	$(RM) $(TEST_EXE_NAMES)
	$(RM) *_out.txt
	$(RMDIR) $(LCOV_REPORT_DIR) $(LCOV_OUTPUT_FILE) 

##############################################################################
# Generic Targets
##############################################################################
# Copy google test source into work
$(addprefix $(WORK_DIR)/,$(patsubst %.obj,%.cc,$(GTEST_GMOCK_OBJ_LIST))) : $(WORK_DIR)/%.cc : $(GTEST_GMOCK_SRC_ROOT)/%.cc | $(WORK_DIR)
	$(COPY) $< $@

$(addprefix $(WORK_DIR)/,$(patsubst %.obj,%.cc,$(GTEST_GTEST_OBJ_LIST))) : $(WORK_DIR)/%.cc : $(GTEST_GTEST_SRC_ROOT)/%.cc | $(WORK_DIR)
	$(COPY) $< $@

# This target is automatically incremented based on the rules generated
.PHONY: test
test :

# Same as test but only runs the main exes
.PHONY: test-main
test-main :

##############################################################################
# Define the GCC with asan and ubsan target
##############################################################################
# Clear any existing variables and initalize them
include $(TEST_BCOMMON_DIR)/init_module.mk

# Compilation variables for this module
COMPILER := gcc
MODULE_NAME := asan_ubsan

# Suppress unused function and unused parameter warnings
WARNING_EXCEPTIONS := -Wno-unused-function -Wno-unused-parameter

# Use max optimizations
CPP_OPT := -O3

# Always enable address sanitizer to catch array out of bounds and other such violations
# Set the initial link library to asan to ensure it is first in the link list. This is
# required by asan
CPPFLAGS += -fsanitize=address
INITIAL_LINK_LIBRARY := -lasan 

# Option to enable Undefined Behaviour sanitizer
ifeq ($(USE_UBSAN),1)
	CPPFLAGS += -fsanitize=undefined
	LDFLAGS += -lubsan
endif

# By default cause the test exes to exit if a sanitizer error is found
ifneq ($(SANITIZER_RECOVERY),1)
	CPPFLAGS += -fno-sanitize-recover=all
endif


ifeq ($(USE_UBSAN),1)
	CPPFLAGS += -fsanitize=undefined
	LDFLAGS += -lubsan
endif

# Source the file to define the cpp rules
include $(TEST_BCOMMON_DIR)/cpp_rules.mk

# Source the file to define the test exe rules
include $(TEST_BCOMMON_DIR)/test_exe_rules.mk

##############################################################################
# Define the GCC with thread sanitizer
##############################################################################
# Clear any existing variables and initalize them
include $(TEST_BCOMMON_DIR)/init_module.mk

# Compilation variables for this module
COMPILER := gcc
MODULE_NAME := tsan

# Suppress unused function and unused parameter warnings
WARNING_EXCEPTIONS := -Wno-unused-function -Wno-unused-parameter

# Use max optimizations
CPP_OPT := -O3

# Enable the thread sanitizer
CPPFLAGS += -fsanitize=thread
LDFLAGS += -ltsan

# By default cause the test exes to exit if a sanitizer error is found
ifneq ($(SANITIZER_RECOVERY),1)
	CPPFLAGS += -fno-sanitize-recover=all
endif

# Source the file to define the cpp rules
include $(TEST_BCOMMON_DIR)/cpp_rules.mk

# Source the file to define the test exe rules
include $(TEST_BCOMMON_DIR)/test_exe_rules.mk

##############################################################################
# Define the GCC with asan and ubsan target with optimization 0 for coverage
##############################################################################
# Clear any existing variables and initalize them
include $(TEST_BCOMMON_DIR)/init_module.mk

# Compilation variables for this module
COMPILER := gcc
MODULE_NAME := zero_opt

# Suppress unused function and unused parameter warnings
WARNING_EXCEPTIONS := -Wno-unused-function -Wno-unused-parameter

# Use no optimizzations
CPP_OPT := -O0

# Add flags to generate Gcov reports
CPPFLAGS += -fprofile-arcs -ftest-coverage
LDFLAGS += -fprofile-arcs

# Always enable address sanitizer to catch array out of bounds and other such violations
# Set the initial link library to asan to ensure it is first in the link list. This is
# required by asan
CPPFLAGS += -fsanitize=address
INITIAL_LINK_LIBRARY := -lasan 

# Option to enable Undefined Behaviour sanitizer
ifeq ($(USE_UBSAN),1)
	CPPFLAGS += -fsanitize=undefined
	LDFLAGS += -lubsan
endif

# By default cause the test exes to exit if a sanitizer error is found
ifneq ($(SANITIZER_RECOVERY),1)
	CPPFLAGS += -fno-sanitize-recover=all
endif


ifeq ($(USE_UBSAN),1)
	CPPFLAGS += -fsanitize=undefined
	LDFLAGS += -lubsan
endif

# Disable the autorun rule generation. This is because we don't want to
# execute the no optimization versions of the unit test by default
ENABLE_RUN_RULES := 0

# Source the file to define the cpp rules
include $(TEST_BCOMMON_DIR)/cpp_rules.mk

# Source the file to define the test exe rules
include $(TEST_BCOMMON_DIR)/test_exe_rules.mk

# Initialize coverage data
.PHONY: init-coverage
init-coverage : $(MODULE_NAME)_main
	lcov --initial --directory $(LCOV_ROOT_DIR) --capture

run-coverage-exes : init-coverage
	LD_LIBRARY_PATH=$(GCC_ROOTDIR)/lib64:$(BOOST_ROOT_DIR)/lib ./zero_opt_main

.PHONY: test-coverage
test-coverage : run-coverage-exes
	lcov --directory $(LCOV_ROOT_DIR) --capture --output-file $(LCOV_OUTPUT_FILE)
# Exclude coverage on gtest libs and standard libs
	lcov -r $(LCOV_OUTPUT_FILE) '*googletest*' '*/boost/*' '*/usr/*' '*/gcc/*' '*/unittest/*' -o $(LCOV_OUTPUT_FILE)
	genhtml --ignore-errors source --highlight --legend $(LCOV_OUTPUT_FILE) -o $(LCOV_REPORT_DIR)
	@echo "[SUCCESS] Generated a coverage report: $(LCOV_REPORT_DIR)"
	@echo "[SUCCESS] Use your favourite browser to view the report." 

##############################################################################
# Define the clang with address sanitizer, ubsan  target
##############################################################################
# Clear mny existing variables and initalize them
include $(TEST_BCOMMON_DIR)/init_module.mk

# Compilation variables for this module
COMPILER := clang
MODULE_NAME := clang_asan_ubsan

# Suppress unused function and unused parameter warnings
WARNING_EXCEPTIONS := -Wno-unused-function -Wno-unused-parameter -Wno-null-pointer-arithmetic -Wno-logical-not-parentheses

# Use max optimizations
CPP_OPT := -O3

# Enable the address sanitizer and UB sanitizer
ifeq ($(USE_UBSAN),1)
	CPPFLAGS += -fsanitize=address,undefined
	LDFLAGS += -fsanitize=address,undefined
else
	CPPFLAGS += -fsanitize=address
	LDFLAGS += -fsanitize=address
endif

# By default cause the test exes to exit if a sanitizer error is found
ifneq ($(SANITIZER_RECOVERY),1)
	CPPFLAGS += -fno-sanitize-recover=all
endif

# Source the file to define the cpp rules
include $(TEST_BCOMMON_DIR)/cpp_rules.mk

# Source the file to define the test exe rules
include $(TEST_BCOMMON_DIR)/test_exe_rules.mk

##############################################################################
# Define the clang with control flow sanitizer target
##############################################################################
ifeq ($(ENABLE_CLANG_CFSAN),1)

# Clear any existing variables and initalize them
include $(TEST_BCOMMON_DIR)/init_module.mk

# Compilation variables for this module
COMPILER := clang
MODULE_NAME := clang_cfsan

# Suppress unused function and unused parameter warnings
WARNING_EXCEPTIONS := -Wno-unused-function -Wno-unused-parameter -Wno-null-pointer-arithmetic -Wno-logical-not-parentheses

# Use max optimizations
CPP_OPT := -O3

# Enable the control flow sanitizer. This also requires setting visability to hidden and link time optimization (LTO)
CPPFLAGS += -fsanitize=cfi -fvisibility=hidden -flto
LDFLAGS += -fsanitize=cfi -flto

# By default cause the test exes to exit if a sanitizer error is found
ifneq ($(SANITIZER_RECOVERY),1)
	CPPFLAGS += -fno-sanitize-recover=all
endif

# Source the file to define the cpp rules
include $(TEST_BCOMMON_DIR)/cpp_rules.mk

# Source the file to define the test exe rules
include $(TEST_BCOMMON_DIR)/test_exe_rules.mk

endif #ENABLE_CLANG_CFSAN

##############################################################################
# Define the clang with safe stack sanitizer target
##############################################################################

# Clear any existing variables and initalize them
include $(TEST_BCOMMON_DIR)/init_module.mk

# Compilation variables for this module
COMPILER := clang
MODULE_NAME := clang_stacksan

# Suppress unused function and unused parameter warnings
WARNING_EXCEPTIONS := -Wno-unused-function -Wno-unused-parameter -Wno-null-pointer-arithmetic -Wno-logical-not-parentheses

# Use max optimizations
CPP_OPT := -O3

# Enable the safe stack sanitizer
CPPFLAGS += -fsanitize=safe-stack
LDFLAGS += -fsanitize=safe-stack

# By default cause the test exes to exit if a sanitizer error is found
ifneq ($(SANITIZER_RECOVERY),1)
	CPPFLAGS += -fno-sanitize-recover=all
endif

# Source the file to define the cpp rules
include $(TEST_BCOMMON_DIR)/cpp_rules.mk

# Source the file to define the test exe rules
include $(TEST_BCOMMON_DIR)/test_exe_rules.mk

##############################################################################
# Define the clang with thread sanitizer target
##############################################################################
ifeq ($(ENABLE_CLANG_TSAN),1)

# Clear any existing variables and initalize them
include $(TEST_BCOMMON_DIR)/init_module.mk

# Compilation variables for this module
COMPILER := clang
MODULE_NAME := clang_tsan

# Suppress unused function and unused parameter warnings
WARNING_EXCEPTIONS := -Wno-unused-function -Wno-unused-parameter -Wno-null-pointer-arithmetic -Wno-logical-not-parentheses

# Use max optimizations
CPP_OPT := -O3

# Enable the thread sanitizer
CPPFLAGS += -fsanitize=thread
LDFLAGS += -fsanitize=thread

# By default cause the test exes to exit if a sanitizer error is found
ifneq ($(SANITIZER_RECOVERY),1)
	CPPFLAGS += -fno-sanitize-recover=all
endif

# Source the file to define the cpp rules
include $(TEST_BCOMMON_DIR)/cpp_rules.mk

# Source the file to define the test exe rules
include $(TEST_BCOMMON_DIR)/test_exe_rules.mk

endif #ENABLE_CLANG_TSAN

##############################################################################
# Define the clang with memory sanitizer target
##############################################################################
ifeq ($(ENABLE_CLANG_MSAN),1)
# Clear any existing variables and initalize them
include $(TEST_BCOMMON_DIR)/init_module.mk

# Compilation variables for this module
COMPILER := clang
MODULE_NAME := clang_msan

# Suppress unused function and unused parameter warnings
WARNING_EXCEPTIONS := -Wno-unused-function -Wno-unused-parameter -Wno-null-pointer-arithmetic -Wno-logical-not-parentheses

# Use max optimizations
CPP_OPT := -O3

# Enable the memory sanitizer
CPPFLAGS += -fsanitize=memory
LDFLAGS += -fsanitize=memory

# By default cause the test exes to exit if a sanitizer error is found
ifneq ($(SANITIZER_RECOVERY),1)
	CPPFLAGS += -fno-sanitize-recover=all
endif

# Source the file to define the cpp rules
include $(TEST_BCOMMON_DIR)/cpp_rules.mk

# Source the file to define the test exe rules
include $(TEST_BCOMMON_DIR)/test_exe_rules.mk

endif #ENABLE_CLANG_MSAN

##############################################################################
# Define the clang with libfuzzer targets
# Based on https://github.com/google/fuzzer-test-suite/blob/master/tutorial/libFuzzerTutorial.md
##############################################################################
ifeq ($(ENABLE_FUZZER),1)
# Clear any existing variables and initalize them
include $(TEST_BCOMMON_DIR)/init_module.mk

# Compilation variables for this module
COMPILER := clang
MODULE_NAME := libfuzz

# Suppress unused function and unused parameter warnings
WARNING_EXCEPTIONS := -Wno-unused-function -Wno-unused-parameter -Wno-null-pointer-arithmetic -Wno-logical-not-parentheses

# Use max optimizations
CPP_OPT := -O3

# Enable the safe stack sanitizer
CPPFLAGS += -fsanitize=address,fuzzer
LDFLAGS += -fsanitize=address,fuzzer

# By default cause the test exes to exit if a sanitizer error is found
ifneq ($(SANITIZER_RECOVERY),1)
	CPPFLAGS += -fno-sanitize-recover=all
endif

# Source the file to define the cpp rules
include $(TEST_BCOMMON_DIR)/cpp_rules.mk

# Create all the targets for fuzzing. The parameter passed is the 
# name of the file and the target. Each target is compiled with all
# $(MAIN_SYSTEM_OBJS_LIST)
$(eval $(call create_fuzz_target,fuzz_main))

endif #ENABLE_FUZZER


##############################################################################
# Final targets. Must appear at end of file
##############################################################################
.PHONY: build-all
build-all : $(TEST_EXE_NAMES)

.PHONY: test-all
test-all: $(addprefix run_, $(TEST_EXE_NAMES)) 

