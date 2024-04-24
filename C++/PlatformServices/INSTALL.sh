#!/bin/bash

source CONFIG.sh

# Remove the directory if it already exists
if [ -d "$CMAKE_SERVER" ]; then
    echo "Directory 'CMAKE_SERVER' already exists..Removing it."
    rm -rf "$CMAKE_SERVER"
fi

# Create server cmake directory
mkdir -p "$CMAKE_SERVER"

# Create the directory
if [ $? -eq 0 ]; then
    echo "Directory '$CMAKE_SERVER' created successfully."
fi


# Remove the directory if it already exists
if [ -d "$CMAKE_CLIENT" ]; then
    echo "Directory 'CMAKE_CLIENT' already exists..Removing it."
    rm -rf "$CMAKE_CLIENT"
fi

# Create client cmake directory
mkdir -p "$CMAKE_CLIENT"

# Create the directory
if [ $? -eq 0 ]; then
    echo "Directory '$CMAKE_CLIENT' created successfully."
fi

# Change to server directory and run cmake command
echo "Building server......"
cd "$CMAKE_SERVER"
cmake -D EXECUTABLE_NAME="$SERVER_APP_NAME" "../../$CMAKE_SERVER_FILE"
make

# Change to client directory and run cmake command
cd ../../
echo "Building client......"
cd "$CMAKE_CLIENT"
cmake -D EXECUTABLE_NAME="$CLIENT_APP_NAME" "../../$CMAKE_CLIENT_FILE"
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
