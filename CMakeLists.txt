cmake_minimum_required(VERSION 3.0.0)
project(jsonc)

# Add all C source files in the src directory to the project
file(GLOB SOURCES "src/*.c")
add_executable(myexecutable ${SOURCES})

# Add include directories to the project
include_directories(include)
