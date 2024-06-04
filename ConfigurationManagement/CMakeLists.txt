cmake_minimum_required(VERSION 3.14)

project(ConfigurationManagement)

# Add jsoncpp library
include(FetchContent)

FetchContent_Declare(
    jsoncpp
    GIT_REPOSITORY https://github.com/open-source-parsers/jsoncpp.git
    GIT_TAG master
)
FetchContent_MakeAvailable(jsoncpp)

# Add tinyxml2 library
FetchContent_Declare(
    tinyxml2
    GIT_REPOSITORY https://github.com/leethomason/tinyxml2.git
    GIT_TAG master
)
FetchContent_MakeAvailable(tinyxml2)

# Specify the source files
file(GLOB SOURCES CONFIGURE_DEPENDS "src/ConcreteClasses/*.cpp")

# Add your main project sources
add_executable(ConfigurationManagement ${SOURCES} src/main.cpp)

# Include directories for third-party projects
target_include_directories(ConfigurationManagement PRIVATE include ${jsoncpp_SOURCE_DIR}/include ${tinyxml2_SOURCE_DIR})

# Link third-party libraries
target_link_libraries(ConfigurationManagement PRIVATE jsoncpp_lib tinyxml2)
