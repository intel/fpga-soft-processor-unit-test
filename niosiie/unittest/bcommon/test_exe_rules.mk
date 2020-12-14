# Build module specific gtest
$(addprefix $($(MODULE_NAME)_WORK_DIR)/,$(GTEST_OBJ_LIST)) : MODULE_NAME := $(MODULE_NAME)
$(addprefix $($(MODULE_NAME)_WORK_DIR)/,$(GTEST_OBJ_LIST)) : TARGET_SPECIFIC_CPPFLAGS := $($(MODULE_NAME)_GTEST_CPPFLAGS)
$(addprefix $($(MODULE_NAME)_WORK_DIR)/,$(GTEST_OBJ_LIST)) : $($(MODULE_NAME)_WORK_DIR)/%.obj : $(WORK_DIR)/%.cc | $($(MODULE_NAME)_WORK_DIR)
	$(global_obj_from_cpp)

# Target to build all of the module
build-$(MODULE_NAME) :

##############################################################################
# Build module specific main
##############################################################################
TEST_EXE_NAMES += $(MODULE_NAME)_main
$(MODULE_NAME)_main : MODULE_NAME := $(MODULE_NAME)
$(MODULE_NAME)_main : $(addprefix $($(MODULE_NAME)_WORK_DIR)/,main.obj $(MAIN_SYSTEM_OBJS_LIST) $(MAIN_NIOS_OBJS_LIST) $(MAIN_UNITTEST_OBJS_LIST) $(GTEST_OBJ_LIST))
	$(global_exe_link)

build-$(MODULE_NAME) : $(MODULE_NAME)_main

ifeq ($(ENABLE_RUN_RULES),1)
test-main : run_$(MODULE_NAME)_main
test : run_$(MODULE_NAME)_main
endif
run_$(MODULE_NAME)_main : MODULE_NAME := $(MODULE_NAME)
run_$(MODULE_NAME)_main : $(MODULE_NAME)_main
	$(info Preparing to run $@)
	# Clear the LD_LIBRARY_PATH before running
	LD_LIBRARY_PATH=$(GCC_ROOTDIR)/lib64:$(BOOST_ROOT_DIR)/lib unbuffer ./$(MODULE_NAME)_main | tee $(MODULE_NAME)_main_out.txt

