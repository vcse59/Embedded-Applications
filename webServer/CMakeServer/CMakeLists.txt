cmake_minimum_required(VERSION 3.0)
project(ThreadingProject)

# Add a compiler flag option
option(USE_DATABASE_MODULE "Enable DB feature" OFF)

# Set the build type to Release if not set
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Read the contents of /etc/issue to determine the distribution
file(READ "/etc/issue" ETC_ISSUE)
string(REGEX MATCH "Ubuntu" DIST ${ETC_ISSUE})

# Include directories for header files
include_directories(${CMAKE_SOURCE_DIR}/../include)

# Define the executable name
set(EXECUTABLE_NAME "SERVERCPP" CACHE STRING "Name of the executable")

message(STATUS "USE_DATABASE_MODULE: ${USE_DATABASE_MODULE}")

# Conditionally set the list of sources
file(GLOB SOURCES CONFIGURE_DEPENDS
    "../src/Interfaces/*.cpp"
    "../src/JsonModule/*.cpp"
    "../src/Adapters/*.cpp"
    "../src/Modules/*.cpp"
    "../src/Modules/TCPService/TCPServer.cpp"
    "../src/Modules/Logger/*.cpp"
    "../src/Modules/HTTPService/*.cpp"
)

if(USE_DATABASE_MODULE)
    file(GLOB DATABASE_SOURCES CONFIGURE_DEPENDS "../src/Modules/Database/MySQL/*.cpp")
    list(APPEND SOURCES ${DATABASE_SOURCES})
    add_compile_definitions(_ENABLE_DB_)
endif()

# Add the executable target
add_executable(${EXECUTABLE_NAME} ${SOURCES} ../src/server_main.cpp)

# ExternalProject_Add example (jsoncpp)
include(ExternalProject)
ExternalProject_Add(
    jsoncpp
    PREFIX ${CMAKE_BINARY_DIR}/thirdparty/jsoncpp
    GIT_REPOSITORY https://github.com/open-source-parsers/jsoncpp.git
    GIT_TAG master  # Or specify the commit hash, branch, etc.
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR> -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
)
ExternalProject_Get_Property(jsoncpp SOURCE_DIR BINARY_DIR)

# Set include directories based on the distribution
if(DIST STREQUAL "Ubuntu")
    target_include_directories(${EXECUTABLE_NAME} PUBLIC
        ${BINARY_DIR}/include
        ${CMAKE_SOURCE_DIR}/../lib/mysql-lib/Ubuntu/mysql-connector-cpp/include/mysql-cppconn-8/jdbc/cppconn
        ${CMAKE_SOURCE_DIR}/../lib/mysql-lib/Ubuntu/mysql-connector-cpp/include/mysql-cppconn-8/jdbc
        ${CMAKE_BINARY_DIR}/thirdparty/jsoncpp/include
    )
else()
    message(STATUS ">>>> Found unknown distribution <<<<")
endif()

# Set link directories based on the distribution
if(DIST STREQUAL "Ubuntu")
    target_link_directories(${EXECUTABLE_NAME} PRIVATE
        ${CMAKE_BINARY_DIR}/thirdparty/jsoncpp/lib
    )
    if(USE_DATABASE_MODULE)
        target_link_directories(${EXECUTABLE_NAME} PRIVATE
            ${CMAKE_SOURCE_DIR}/../lib/mysql-lib/Ubuntu/mysql-connector-cpp/lib/x86_64-linux-gnu
        )
    endif()
else()
    message(STATUS ">>>> Found unknown distribution <<<<")
endif()


# Optionally, link additional libraries based on the flag
if(USE_DATABASE_MODULE)
    target_link_libraries(${EXECUTABLE_NAME} PUBLIC
        ${CMAKE_SOURCE_DIR}/../lib/mysql-lib/Ubuntu/mysql-connector-cpp/lib/x86_64-linux-gnu/libmysqlcppconn-static.a
        )
endif()

# Link against the jsoncpp library
target_link_libraries(${EXECUTABLE_NAME} PUBLIC
    ${BINARY_DIR}/lib/libjsoncpp.a PRIVATE resolv ssl crypto
)

# Optionally, set compile definitions for the target
if(USE_DATABASE_MODULE)
    target_compile_definitions(${EXECUTABLE_NAME} PRIVATE _ENABLE_DB_)
endif()

# Make sure my_executable depends on jsoncpp
add_dependencies(${EXECUTABLE_NAME} jsoncpp)
