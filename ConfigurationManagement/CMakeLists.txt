cmake_minimum_required(VERSION 3.0.0)
project(ConfigurationManagement VERSION 0.1.0 LANGUAGES C CXX)

# Include source code include directories
include_directories(include)

# Specify the source files
file(GLOB SOURCES CONFIGURE_DEPENDS "src/ConcreteClasses/*")

add_executable(ConfigurationManagement ${SOURCES} src/main.cpp)

