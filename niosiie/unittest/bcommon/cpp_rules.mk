# Ensure the following variables are set using immediate assignments prior to souring this script
# MODULE_NAME := mymod
# COMPILER := gcc
# CPP_OPT := -O3
# INITIAL_LINK_LIBRARY := -lasan
# LDFLAGS :=
# CPP_FLAGS :=
# WARNING_EXCEPTIONS := 

##############################################################################
# Checks
##############################################################################
ifeq ($(MODULE_NAME),)
	$(error No module name specified with $$MODULE_NAME)
endif

##############################################################################
# Global compilation variables
##############################################################################
ifeq ($(COMPILER),gcc)
	$(MODULE_NAME)_CXX := g++
	$(MODULE_NAME)_LINKER := g++
	$(MODULE_NAME)_INITIAL_LINK_LIBRARY := $(INITIAL_LINK_LIBRARY)
	$(MODULE_NAME)_LDFLAGS := -fPIC -pthread $(GLOBAL_LDFLAGS) $(LDFLAGS)
	$(MODULE_NAME)_CPP_OPT := $(CPP_OPT)
	$(MODULE_NAME)_CPP_DEBUG := -ggdb3
	$(MODULE_NAME)_WARNING_EXCEPTIONS := $(WARNING_EXCEPTIONS)
	$(MODULE_NAME)_CPPFLAGS := -m64 -std=c++17 -pthread -MMD -MP -Wall -Wextra -Werror $($(MODULE_NAME)_WARNING_EXCEPTIONS) -fPIC -fno-omit-frame-pointer $($(MODULE_NAME)_CPP_OPT) $($(MODULE_NAME)_CPP_DEBUG) $(GLOBAL_CPPFLAGS) $(CPPFLAGS)
	$(MODULE_NAME)_GTEST_CPPFLAGS := "-Wno-deprecated-copy"
else ifeq ($(COMPILER),clang)
	$(MODULE_NAME)_CXX := clang++
	$(MODULE_NAME)_LINKER := clang++
	$(MODULE_NAME)_INITIAL_LINK_LIBRARY := $(INITIAL_LINK_LIBRARY)
	$(MODULE_NAME)_LDFLAGS := -fPIC -pthread $(GLOBAL_LDFLAGS) $(LDFLAGS)
	$(MODULE_NAME)_CPP_OPT := $(CPP_OPT)
	$(MODULE_NAME)_CPP_DEBUG := -g
	$(MODULE_NAME)_WARNING_EXCEPTIONS := $(WARNING_EXCEPTIONS)
	$(MODULE_NAME)_CPPFLAGS := -m64 -std=c++17 -x c++ -pthread -MMD -MP -Wall -Wextra -Werror $($(MODULE_NAME)_WARNING_EXCEPTIONS) -fPIC -fno-omit-frame-pointer $($(MODULE_NAME)_CPP_OPT) $($(MODULE_NAME)_CPP_DEBUG) $(GLOBAL_CPPFLAGS) $(CPPFLAGS)
	$(MODULE_NAME)_GTEST_CPPFLAGS :=
else
	$(error No compiler specified with $$COMPILER)
endif

# Define the module work_dir
$(MODULE_NAME)_WORK_DIR := $(WORK_DIR)/$(MODULE_NAME)

###############################################################################
# Define OBJ lists for use in building targets
###############################################################################

# Determine all the source files in the module dir, then create the OBJ list
# Test files are written in C++
__MODULE_CPP_FILES := $(wildcard *.cpp) $(wildcard $(SYSTEM_DIR)/*.cpp) $(wildcard $(UNITTEST_DIR)/*.cpp)

$(MODULE_NAME)_CPP_OBJ_TARGETS = $(addprefix $($(MODULE_NAME)_WORK_DIR)/,$(patsubst %.cpp,%.obj,$(__MODULE_CPP_FILES)))
$(MODULE_NAME)_CPP_DEP = $(addprefix $($(MODULE_NAME)_WORK_DIR)/,$(patsubst %.cpp,%.d,$(__MODULE_CPP_FILES)))
$(MODULE_NAME)_CPP_I_TARGETS = $(addprefix $($(MODULE_NAME)_WORK_DIR)/,$(patsubst %.cpp,%.i,$(__MODULE_CPP_FILES)))

# NIOS code is written in C
__MODULE_C_FILES := $(wildcard $(NIOS_CODE_DIR)/src/*.c)
$(MODULE_NAME)_NIOS_WORK_DIR := $($(MODULE_NAME)_WORK_DIR)/nios_code

$(MODULE_NAME)_C_OBJ_TARGETS = $(addprefix $($(MODULE_NAME)_NIOS_WORK_DIR)/,$(patsubst %.c,%.obj,$(notdir $(__MODULE_C_FILES))))
$(MODULE_NAME)_C_DEP = $(addprefix $($(MODULE_NAME)_NIOS_WORK_DIR)/,$(patsubst %.c,%.d,$(notdir $(__MODULE_C_FILES))))
$(MODULE_NAME)_C_I_TARGETS = $(addprefix $($(MODULE_NAME)_NIOS_WORK_DIR)/,$(patsubst %.c,%.i,$(notdir $(__MODULE_C_FILES))))

###############################################################################
# Rules
###############################################################################
# Global cpp to obj rule recipe
define global_obj_from_cpp
$(info Building module $@ from $^ using CPPFLAGS = $($(MODULE_NAME)_CPPFLAGS) $(TARGET_SPECIFIC_CPPFLAGS))
$(info Compiling...$<)
$(RM) $@
$($(MODULE_NAME)_CXX) $($(MODULE_NAME)_CPPFLAGS) $(TARGET_SPECIFIC_CPPFLAGS) -c -o $@ $<
endef

define global_i_from_cpp
$(info Building module $@ from $^ using CPPFLAGS = $($(MODULE_NAME)_CPPFLAGS) $(TARGET_SPECIFIC_CPPFLAGS))
$(info Building I...$<)
$(RM) $@
$($(MODULE_NAME)_CXX) $($(MODULE_NAME)_CPPFLAGS) $(TARGET_SPECIFIC_CPPFLAGS) -E -o $@ $<
endef

# Global exe link rule recipe
define global_exe_link
$(info Building module $@ from $^ using LDFLAGS = $($(MODULE_NAME)_INITIAL_LINK_LIBRARY) $($(MODULE_NAME)_LDFLAGS) $(TARGET_SPECIFIC_LDFLAGS))
$(info Linking...$<)
$(RM) $@
$($(MODULE_NAME)_LINKER) $($(MODULE_NAME)_INITIAL_LINK_LIBRARY) $($(MODULE_NAME)_LDFLAGS) $(TARGET_SPECIFIC_LDFLAGS) -o $@ $(filter %.obj,$^) $(TARGET_SPECIFIC_STATIC_LIBS)
endef

# Global shared lib link rule recipe
define global_lib_link
$(info Building module $@ from $^ using LDFLAGS = $($(MODULE_NAME)_INITIAL_LINK_LIBRARY) $($(MODULE_NAME)_LDFLAGS) $(TARGET_SPECIFIC_LDFLAGS))
$(info Linking...$<)
$(RM) $@
$($(MODULE_NAME)_LINKER) -shared $($(MODULE_NAME)_INITIAL_LINK_LIBRARY) $($(MODULE_NAME)_LDFLAGS) $(TARGET_SPECIFIC_LDFLAGS) -o $@ $<
endef

###############################################################################
# CPP rules
###############################################################################

# CPP to OBJ rules
$($(MODULE_NAME)_CPP_OBJ_TARGETS) : MODULE_NAME := $(MODULE_NAME)
$($(MODULE_NAME)_CPP_OBJ_TARGETS) : $($(MODULE_NAME)_WORK_DIR)/%.obj : %.cpp | $(sort $(dir $($(MODULE_NAME)_CPP_OBJ_TARGETS)))
	$(global_obj_from_cpp)

# Include autogenerated dependencies
-include $($(MODULE_NAME)_CPP_DEP)

# Preprocessor output
$($(MODULE_NAME)_CPP_I_TARGETS) : MODULE_NAME := $(MODULE_NAME)
$($(MODULE_NAME)_CPP_I_TARGETS) : $($(MODULE_NAME)_WORK_DIR)/%.i : %.cpp | $(sort $(dir $($(MODULE_NAME)_CPP_I_TARGETS)))
	$(global_i_from_cpp)

$(sort $(dir $($(MODULE_NAME)_CPP_OBJ_TARGETS))) :
	$(MKDIR) $@

###############################################################################
# C rules
###############################################################################

# C to OBJ rules
$($(MODULE_NAME)_C_OBJ_TARGETS) : MODULE_NAME := $(MODULE_NAME)
$($(MODULE_NAME)_C_OBJ_TARGETS) : $($(MODULE_NAME)_NIOS_WORK_DIR)/%.obj : $(NIOS_CODE_DIR)/src/%.c | $(sort $(dir $($(MODULE_NAME)_C_OBJ_TARGETS)))
	$(global_obj_from_cpp)

# Include autogenerated dependencies
-include $($(MODULE_NAME)_C_DEP)

# Preprocessor output
$($(MODULE_NAME)_C_I_TARGETS) : MODULE_NAME := $(MODULE_NAME)
$($(MODULE_NAME)_C_I_TARGETS) : $($(MODULE_NAME)_NIOS_WORK_DIR)/%.i : $(NIOS_CODE_DIR)/src/%.c | $(sort $(dir $($(MODULE_NAME)_C_I_TARGETS)))
	$(global_i_from_cpp)

$(sort $(dir $($(MODULE_NAME)_C_OBJ_TARGETS))) :
	$(MKDIR) $@

###############################################################################
# Rules
###############################################################################

define create_fuzz_target
$(info creating fuzz target for $(MODULE_NAME))
TEST_EXE_NAMES += $(MODULE_NAME)_$(1)
$(MODULE_NAME)_$(1) : MODULE_NAME := $(MODULE_NAME)
$(MODULE_NAME)_$(1) : $(addprefix $($(MODULE_NAME)_WORK_DIR)/,$(1).obj $(MAIN_SYSTEM_OBJS_LIST))
	$$(global_exe_link)

build-$(MODULE_NAME) : $(MODULE_NAME)_$(1)

test : run_$(MODULE_NAME)_$(1)
run_$(MODULE_NAME)_$(1) : MODULE_NAME := $(MODULE_NAME)
run_$(MODULE_NAME)_$(1) : $(MODULE_NAME)_$(1)
	$(info Preparing to run $@)
	# Clear the LD_LIBRARY_PATH before running
	LD_LIBRARY_PATH=$(GCC_ROOTDIR)/lib64:$(BOOST_ROOT_DIR)/lib ./$(MODULE_NAME)_$(1)
endef

define create_exe_target
$(info creating exe target for $(MODULE_NAME))
TEST_EXE_NAMES += $(MODULE_NAME)
$(MODULE_NAME) : MODULE_NAME := $(MODULE_NAME)
$(MODULE_NAME) : $(addprefix $($(MODULE_NAME)_WORK_DIR)/,$(1).obj $(MAIN_SYSTEM_OBJS_LIST))
	$$(global_exe_link)

build-$(MODULE_NAME) : $(MODULE_NAME)_$(1)
endef

