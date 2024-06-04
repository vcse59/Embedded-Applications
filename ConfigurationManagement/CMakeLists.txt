cmake_minimum_required(VERSION 3.14)

project(ConfigurationManagement)

# Add jsoncpp library
include(FetchContent)

FetchContent_Declare(
    jsoncpp
    GIT_REPOSITORY https://github.com/open-source-parsers/jsoncpp.git
    GIT_TAG master
)
set(JSONCPP_WITH_TESTS OFF)  # Disable building tests for jsoncpp
FetchContent_MakeAvailable(jsoncpp)

# Add tinyxml2 library
FetchContent_Declare(
    tinyxml2
    GIT_REPOSITORY https://github.com/leethomason/tinyxml2.git
    GIT_TAG master
)
FetchContent_MakeAvailable(tinyxml2)

# Specify the source files
file(GLOB SOURCES CONFIGURE_DEPENDS "src/ConcreteClasses/*")

# Add your main project sources
add_executable(ConfigurationManagement ${SOURCES} src/main.cpp)

# Include directories for third-party projects
target_include_directories(ConfigurationManagement PRIVATE include ${jsoncpp_SOURCE_DIR}/include ${tinyxml2_SOURCE_DIR}/include)

# Link third-party libraries
target_link_libraries(ConfigurationManagement PRIVATE jsoncpp_lib tinyxml2)

# Determine the appropriate library file extensions and paths
if(WIN32)
    set(JSONCPP_LIB_FILE "libjsoncpp.dll")
    set(TINYXML2_LIB_FILE "libtinyxml2.dll")
else()
    set(JSONCPP_LIB_FILE "libjsoncpp.so")
    set(TINYXML2_LIB_FILE "libtinyxml2.so")
endif()

# Set the paths to the libraries
set(JSONCPP_LIB_PATH "${jsoncpp_BINARY_DIR}/src/lib_json/${JSONCPP_LIB_FILE}")
set(TINYXML2_LIB_PATH "${tinyxml2_BINARY_DIR}/${TINYXML2_LIB_FILE}")

# Add custom command to copy the jsoncpp library to the output directory
add_custom_command(TARGET ConfigurationManagement POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${JSONCPP_LIB_PATH}"
    ${CMAKE_BINARY_DIR}/)

# Add custom command to copy the tinyxml2 library to the output directory
add_custom_command(TARGET ConfigurationManagement POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${TINYXML2_LIB_PATH}"
    ${CMAKE_BINARY_DIR}/)
