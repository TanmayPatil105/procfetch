#!/bin/bash

# check args
if [  $# != 0 ]
then
    exit 1
fi

echo "Building procfetch ..."
make all || exit $?

echo "Removing old version ..."
sudo make uninstall || exit ?

sudo make install || exit $?
echo "Installed succesfully"
