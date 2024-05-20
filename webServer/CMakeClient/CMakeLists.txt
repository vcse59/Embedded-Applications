cmake_minimum_required(VERSION 3.0)
project(ThreadingProject)

# Include source code include directories
include_directories(../include)

# Define a variable for the executable name
set(EXECUTABLE_NAME "CLIENTCPP" CACHE STRING "Name of the executable")

# Specify the source files
file(GLOB SOURCES CONFIGURE_DEPENDS "../src/Modules/TCPService/TCPClient.cpp")

add_executable(${EXECUTABLE_NAME} ${SOURCES} ../src/client_main.cpp)

# Specify include directories 
target_include_directories(${EXECUTABLE_NAME} PUBLIC ${BINARY_DIR}/include)