# Specify the target system as Windows
set(CMAKE_SYSTEM_NAME Windows)

# Set the MinGW compiler paths
set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)
set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)

# Optionally, set other compiler flags or options
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
