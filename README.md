### Documentation comes soon hopefully!
### This documentation is just brief, until the library will work and get ready to use, but here are some basics (CMakeLsists.txt will now create Makefiles that builds up a basic JSON test from main.c, will be fixed):

## CSON
#### How to set up (the test)

1. Download the source code
2. Make sure you have CMake and Make installed
3. Open up terminal and change directory to the same directory as you find CMakeLists.txt
4. Run `cmake .` to generate Makefile
5. Run `make` to create the executable
6. Now, you can run the small generated test (which is main.c), by running `./myexecutable`
7. And finally, do whatever you want with this code/library I coded :)
#### CSON library
When using the library, you should be able to include the files with `#include <cson/[file.h]>`. The files you should be able to import are:
- `#include <cson/cson.h>` - The core library
- `#include <cson/io.h>` - IO, read and write files (!!!NOT READY!!!)
- `#include <cson/structures.h>` - Structs that the libary uses, not included unless you want to create extensions for CSON
- `#include <cson/validator.h>` - Can check if JSON is valid or not
