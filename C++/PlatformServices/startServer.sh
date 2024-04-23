#!/bin/bash

source CONFIG.sh

if [ -f ./"$BINARY_PATH"/"$SERVER_APP_NAME" ]; then
    ./"$BINARY_PATH"/"$SERVER_APP_NAME"
else
    echo "Server executable doesn't exists"
fi
