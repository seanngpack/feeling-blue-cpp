#!/bin/bash

if [[ "$OSTYPE" == "darwin"* ]]; then
        echo "we're on mac"
elif [[ "$OSTYPE" == "windowsnt"* ]]; then
        echo "we're on windows"
else
        echo "this OS is not supported"
fi