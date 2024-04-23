#!/bin/bash

BUILD_DIRECTORY_NAME="build"
CMAKE_SERVER="$BUILD_DIRECTORY_NAME/server"
CMAKE_CLIENT="$BUILD_DIRECTORY_NAME/client"
CMAKE_SERVER_FILE="CMakeServer"
CMAKE_CLIENT_FILE="CMakeClient"
SERVER_APP_NAME="server"
CLIENT_APP_NAME="client"
SERVER_EXECUTABLE_PATH="$CMAKE_SERVER/$SERVER_APP_NAME"
CLIENT_EXECUTABLE_PATH="$CMAKE_CLIENT/$CLIENT_APP_NAME"
BINARY_PATH="bin"
