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
# Makefile starts here
##############################################################################
 
.PHONY: all
all:
	$(MAKE) -C bsp
	$(MAKE) -C code
 
# Need to clean up code first; otherwise, code/Makefile will complain about missing BSP directory
.PHONY: clean
clean:
	$(MAKE) -C code clean
	$(MAKE) -C bsp clean

