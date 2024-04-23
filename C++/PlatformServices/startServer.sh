#!/bin/bash

source CONFIG.sh

if [ -f "$SERVER_EXECUTABLE_PATH" ]; then
    ./"$SERVER_EXECUTABLE_PATH"
else
    echo "Server executable doesn't exists"
fi