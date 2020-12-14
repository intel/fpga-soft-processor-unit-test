# Nios II Software Unit Test Example Design
## Overview
This example design shows how to unit-test Nios II software by overwritting Nios II intrinsic functions. This technique requires little change to the source code, but can achieve a high degree of code coverage easily. 

### FPGA Design 
Under [design/](design/), there is an Intel Arria 10-based FPGA example design, which instantiates a Nios IIe soft processor. The idea behinds this example design is that an external device may request the FPGA to do some complex math operations on a block of data. In this design, an on-chip dual-port RAM is instanstiated, with one side connected to the Nios processor and the other side exported. There is also general purpose IO connectivity with the Nios processor and device pins. External device can push data into the RAM and request Nios processor to act on the data via GPIOs. For simplicity, a square root module is chosen over more complex math operations such as hashing. In the Qsys system, ALTSQRT IP is instantiated and connected to Nios processor on the Avalon Memory Map fabric. 

### Unit Test
Under [unittest/](unittest/), there are example unit test framework and test suites. The unit test framework is named System Mock. System Mock is a simplified software representation of a hardware system, where the soft processor resides. For FPGA designs with Nios system, Quartus generates a custom Board Support Package (BSP). BSP contains header files that support Nios software C-code to run on the soft processor and interact with components on the Avalon bus. System Mock creates mock modules for IP components with the configuration (e.g. base address) from the BSP. Furthermore, System Mock provides custom implementation for Nios II built-in load and store instructions. This allows System Mock to re-direct Nios Read/Write calls to the appropriate IP mocks and inject side effects or events. Since System Mock re-defines the Nios II built-in types and functions, Nios software code can be unit-tested as is. 

#### Unit Test Build Prerequisite
##### BSP Build
The BSP of the Nios system must be generated before compiling the unit tests. Go to [design/](design/), and run the following.
``` bash
$ make prep
```
##### Third-party library
Run the appropriate git command to populate the submodule directory [extlibs/googletest](extlibs/googletest) and provide the path to the Boost library in [unittest/Makefile](unittest/Makefile).

See Requirements section for more details on these third-party libraries. 

#### Unit Test Build Option
The unit test build is designed to support many different compilation options. There are options to compile with gcc or clang and with a variety of sanitizers, such as address and undefined behavior sanitizers. See [unittest/Makefile](unittest/Makefile) for more details about these configurations.

The followings are the `Make` options, under [unittest/](unittest/).

This command builds the default googletest executable and runs it. 
``` bash
$ make
```

This command builds all variants of the googletest executables and runs them all. 
``` bash
$ make test-all
```
#### Unit Test Build Option - Code Coverage
This command builds the non-optimized googletest executable for collecting gcov coverage data. After compilation, it runs `lcov` to analyze the coverage data and `genhtml` to create a report in HTML format.  
``` bash
$ make test-coverage
```

## Requirements
### Quartus Prime software
Intel Quartus Prime Pro Edition software is required to compile the Arria 10 FPGA design. 

### Third-party library
#### Googletest
This unit test framework is built on top of the [googletest](https://github.com/google/googletest) framework.

The googletest Git project (with release-1.10.0 tag) has been added as submodule under [extlibs/googletest](extlibs/googletest). 

#### Boost
This unit test framework uses the Boost C++ libraries. To install Boost, follow the guide from www.boost.org.

In [unittest/Makefile](unittest/Makefile), define this variable to point to your installation directory. This variable is appended to *LD_LIBRARY_PATH* environment variable, when executing googletest executables. 
``` Make
BOOST_ROOT_DIR = 
```

## Code Documentation
The in-code comments are written in compliance with Doxygen format. 
Easy-to-view code documentation can be generated via Doxygen. Run
``` bash
$ doxygen
```
Doxygen uses the configuration in Doxyfile and produces documentation in HTML format. Open the index.html file with broswer of your choice. 

Doxyfile was generated with
``` bash
$ doxygen -g
```

