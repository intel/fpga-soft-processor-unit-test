 
# Enable second expansion
.SECONDEXPANSION:

# Clear all built in suffixes
.SUFFIXES:

THIS_MK_ABSPATH := $(abspath $(lastword $(MAKEFILE_LIST)))
THIS_MK_DIR := $(patsubst %/,%,$(dir $(THIS_MK_ABSPATH)))

##############################################################################
# Set default goal before any targets. The default goal here is "test"
##############################################################################
DEFAULT_TARGET := all

.DEFAULT_GOAL := default
.PHONY: default
default: $(DEFAULT_TARGET)

##############################################################################
# Project Definitions
##############################################################################
Q_PROJECT := top

NIOS_CODE_HEX_SRC := fw/code/mem_init/nios_code.hex
NIOS_CODE_HEX_DEST := nios_code.hex

##############################################################################
# Makefile starts here
##############################################################################
.PHONY: prep
prep: 
	quartus_ipgenerate top -c top
	$(MAKE) -C fw
	$(info Preparation completed)

.PHONY: all 
all: output_files/top.sof

.PHONY: clean
clean:
	$(info Cleaning Project)
	-rm -rf output_files db incremental_db reg.rout *.qws
	-rm -rf $(NIOS_CODE_HEX_DEST)
	quartus_ipgenerate --clean top -c top
	-rm -rf src/top_sys/top_sys
	-$(MAKE) -C fw clean

##############################################################################
# Internal targets 
##############################################################################

$(NIOS_CODE_HEX_SRC): prep
$(NIOS_CODE_HEX_DEST): $(NIOS_CODE_HEX_SRC)
	$(info Copying $(NIOS_CODE_HEX_SRC) to $(NIOS_CODE_HEX_DEST))
	cp $< $@

output_files/top.sof : $(NIOS_CODE_HEX_DEST)
	$(info Compiling design to $@)
	quartus_syn top -c top
	quartus_fit top -c top
	quartus_asm top -c top
	quartus_sta top -c top
	$(info Compilation completed successfully)

