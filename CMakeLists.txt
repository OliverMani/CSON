cmake_minimum_required(VERSION 3.0.0)
project(jsonc)

# Add all C source files in the src directory to the project
file(GLOB SOURCES "src/*.c")
add_executable(myexecutable ${SOURCES})

# Add include directories to the project
include_directories(include)

# To make "make docs" work, it should run a python script to generate README.md
add_custom_target(docs
    COMMAND python3 "make_docs.py" #"${CMAKE_SOURCE_DIR}/script2"
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/scripts"
    COMMENT "Generating documentation"
)