#!/bin/bash

source CONFIG.sh

buildType="Release";

if [ -n "$1" ]; then
    buildType=$1
fi

if [ ! -d "$CMAKE_SERVER" ]; then
	# Create server cmake directory
	mkdir -p "$CMAKE_SERVER"

	# Create the directory
	if [ $? -eq 0 ]; then
    		echo "Directory '$CMAKE_SERVER' created successfully."
	fi
fi


if [ ! -d "$CMAKE_CLIENT" ]; then
	# Create server cmake directory
	mkdir -p "$CMAKE_CLIENT"

	# Create the directory
	if [ $? -eq 0 ]; then
    		echo "Directory '$CMAKE_CLIENT' created successfully."
	fi
fi

if [ ! -d "$CMAKE_LOGGER" ]; then
	# Create server cmake directory
	mkdir -p "$CMAKE_LOGGER"

	# Create the directory
	if [ $? -eq 0 ]; then
    		echo "Directory '$CMAKE_LOGGER' created successfully."
	fi
fi

# Change to server directory and run cmake command
echo "Building $buildType server......"
cd "$CMAKE_SERVER"
if [ "$buildType" = "Debug" ]; then
    cmake -DCMAKE_BUILD_TYPE=Debug -D EXECUTABLE_NAME="$SERVER_APP_NAME" "../../$CMAKE_SERVER_FILE"
else
    cmake -DCMAKE_BUILD_TYPE=Release -D EXECUTABLE_NAME="$SERVER_APP_NAME" "../../$CMAKE_SERVER_FILE"
fi
make

# Change to client directory and run cmake command
cd ../../
echo "Building $buildType client......"
cd "$CMAKE_CLIENT"
if [ "$buildType" = "Debug" ]; then
    cmake -DCMAKE_BUILD_TYPE=Debug -D EXECUTABLE_NAME="$CLIENT_APP_NAME" "../../$CMAKE_CLIENT_FILE"
else
    cmake -DCMAKE_BUILD_TYPE=Release -D EXECUTABLE_NAME="$CLIENT_APP_NAME" "../../$CMAKE_CLIENT_FILE"
fi
make

# Change to client directory and run cmake command
cd ../../
echo "Building $buildType loggerServer......"
cd "$CMAKE_LOGGER"
if [ "$buildType" = "Debug" ]; then
    cmake -DCMAKE_BUILD_TYPE=Debug -D EXECUTABLE_NAME="$LOGGER_SERVER_APP_NAME" "../../$CMAKE_LOGGER_SERVER_FILE"
else
    cmake -DCMAKE_BUILD_TYPE=Release -D EXECUTABLE_NAME="$LOGGER_SERVER_APP_NAME" "../../$CMAKE_LOGGER_SERVER_FILE"
fi
make

# Copy the binaries to bin directory
cd ../../

# Remove the directory if it already exists
if [ -d "$BINARY_PATH" ]; then
    echo "Directory 'BINARY_PATH' already exists..Removing it."
    rm -rf "$BINARY_PATH"
fi

# Create bin directory
mkdir -p "$BINARY_PATH"

cp "$SERVER_EXECUTABLE_PATH" "$BINARY_PATH"/
cp "$CLIENT_EXECUTABLE_PATH" "$BINARY_PATH"/
cp "$LOGGER_SERVER_EXECUTABLE_PATH" "$BINARY_PATH"/
cp -r "$WEB_FILE_PATH" "$BINARY_PATH"/
