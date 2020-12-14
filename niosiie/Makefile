# Enable second expansion
.SECONDEXPANSION:
 
# Clear all built in suffixes
.SUFFIXES:
 
##############################################################################
# Set default goal before any targets. The default goal here is "all"
##############################################################################
DEFAULT_TARGET := all
 
.DEFAULT_GOAL := default
.PHONY: default
default: $(DEFAULT_TARGET)
 
##############################################################################
# Sub-directories
##############################################################################
 
DESIGN_DIR = design
TEST_DIR = unittest
 
##############################################################################
# Makefile starts here
##############################################################################

# Steps:
#   1. Compile BSP and NIOS code
#   2. Run unittest against the NIOS code
#   3. Compile the full design
.PHONY: all
all:
	$(MAKE) -C $(DESIGN_DIR) prep
	$(MAKE) -C $(TEST_DIR)
	$(MAKE) -C $(DESIGN_DIR)
 
.PHONY: clean
clean:
	$(MAKE) -C $(DESIGN_DIR) clean
	$(MAKE) -C $(TEST_DIR) clean
