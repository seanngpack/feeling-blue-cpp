#!/bin/bash

if [ $TRAVIS_OS_NAME = 'osx' ];
then

    # Install some custom requirements on macOS
    # e.g. brew install pyenv-virtualenv

    echo "we're on macos"

else
    # Install some custom requirements on Linux
fi