cmake_minimum_required(VERSION 3.0)
project(ThreadingProject)

# Include source code include directories
include_directories(../include)

# Define a variable for the executable name
set(EXECUTABLE_NAME "CLIENTCPP" CACHE STRING "Name of the executable")

# Specify the source files
file(GLOB SOURCES CONFIGURE_DEPENDS "../src/JsonModule/*" "../src/Adapters/*" "../src/Modules/*" "../src/Modules/TCPService/*" "../src/Modules/Logger/*" "../src/Modules/HTTPService/*")

add_executable(${EXECUTABLE_NAME} ${SOURCES} ../src/client_main.cpp)

# Include third-party library using ExternalProject_Add
include(ExternalProject)

ExternalProject_Add(
    jsoncpp
    PREFIX ${CMAKE_BINARY_DIR}/thirdparty/jsoncpp
    GIT_REPOSITORY https://github.com/open-source-parsers/jsoncpp.git
    GIT_TAG master  # Or specify the commit hash, branch, etc.
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
)
ExternalProject_Get_Property(jsoncpp SOURCE_DIR BINARY_DIR)

# Specify include directories 
target_include_directories(${EXECUTABLE_NAME} PUBLIC ${BINARY_DIR}/include ${CMAKE_SOURCE_DIR}/../lib/mysql-connector-cpp/include/mysql-cppconn-8/jdbc/cppconn ${CMAKE_SOURCE_DIR}/../lib/mysql-connector-cpp/include/mysql-cppconn-8/jdbc ${CMAKE_BINARY_DIR}/thirdparty/jsoncpp/include)

# Specify directories for linking for a specific target
target_link_directories(${EXECUTABLE_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/../lib/mysql-connector-cpp/lib/x86_64-linux-gnu ${CMAKE_BINARY_DIR}/../thirdparty/jsoncpp/lib)

# link against the library
target_link_libraries(${EXECUTABLE_NAME} PUBLIC ${BINARY_DIR}/lib/libjsoncpp.a PRIVATE mysqlcppconn-static resolv ssl crypto)

# Make sure my_executable depends on my_library
add_dependencies(${EXECUTABLE_NAME} jsoncpp)
