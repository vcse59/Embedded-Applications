cmake_minimum_required(VERSION 3.0)
project(ThreadingProject)

file(READ "/etc/issue" ETC_ISSUE)
string(REGEX MATCH "Ubuntu" DIST ${ETC_ISSUE})

# Include source code include directories
include_directories(../include)

# Define a variable for the executable name
set(EXECUTABLE_NAME "SERVERCPP" CACHE STRING "Name of the executable")

# Specify the source files
file(GLOB SOURCES CONFIGURE_DEPENDS "../src/Interfaces/*" "../src/JsonModule/*" "../src/Adapters/*" "../src/Modules/*" "../src/Modules/Database/MySQL/*" "../src/Modules/TCPService/TCPServer.cpp" "../src/Modules/Logger/*" "../src/Modules/HTTPService/*")

add_executable(${EXECUTABLE_NAME} ${SOURCES} ../src/server_main.cpp)

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

if(DIST STREQUAL "Ubuntu")
    # Specify include directories 
    target_include_directories(${EXECUTABLE_NAME} PUBLIC ${BINARY_DIR}/include
    ${CMAKE_SOURCE_DIR}/../lib/mysql-lib/Ubuntu/mysql-connector-cpp/include/mysql-cppconn-8/jdbc/cppconn
    ${CMAKE_SOURCE_DIR}/../lib/mysql-lib/Ubuntu/mysql-connector-cpp/include/mysql-cppconn-8/jdbc ${CMAKE_BINARY_DIR}/thirdparty/jsoncpp/include)
else()
    message(STATUS ">>>> Found unknown distribution <<<<")
endif()

# Specify directories for linking for a specific target
if(DIST STREQUAL "Ubuntu")
  target_link_directories(${EXECUTABLE_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/../lib/mysql-lib/Ubuntu/mysql-connector-cpp/lib/x86_64-linux-gnu ${CMAKE_BINARY_DIR}/../thirdparty/jsoncpp/lib)
else()
    message(STATUS ">>>> Found unknown distribution <<<<")
endif()

# link against the library
target_link_libraries(${EXECUTABLE_NAME} PUBLIC ${BINARY_DIR}/lib/libjsoncpp.a ${CMAKE_SOURCE_DIR}/../lib/mysql-lib/Ubuntu/mysql-connector-cpp/lib/x86_64-linux-gnu/libmysqlcppconn-static.a PRIVATE resolv ssl crypto)

# Make sure my_executable depends on my_library
add_dependencies(${EXECUTABLE_NAME} jsoncpp)
