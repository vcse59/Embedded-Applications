#!/bin/bash

source CONFIG.sh

if [ -f ./"$BINARY_PATH"/"$CLIENT_APP_NAME" ]; then
    ./"$BINARY_PATH"/"$CLIENT_APP_NAME"
else
    echo "Client executable doesn't exists"
fi
