#!/bin/bash

source CONFIG.sh

if [ -f "$CLIENT_EXECUTABLE_PATH" ]; then
    ./"$CLIENT_EXECUTABLE_PATH"
else
    echo "Client executable doesn't exists"
fi