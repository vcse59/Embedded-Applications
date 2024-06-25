cmake_minimum_required(VERSION 3.0)
project(ThreadingProject)

# Include source code include directories
include_directories(../include ../LoggerServer/include)

# Define a variable for the executable name
set(EXECUTABLE_NAME "loggerServer" CACHE STRING "Name of the executable")

# Specify the source files
file(GLOB SOURCES CONFIGURE_DEPENDS "../LoggerServer/src/*")

add_executable(${EXECUTABLE_NAME} ${SOURCES})

# Specify include directories 
target_include_directories(${EXECUTABLE_NAME} PUBLIC ${BINARY_DIR}/include )