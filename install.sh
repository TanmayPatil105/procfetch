#!/bin/bash

BIN="/bin/procfetch"
DATADIR="/usr/share/procfetch"

# check args
if [  $# != 0 ]
then
    exit 1
fi

echo "Building procfetch ..."
make all || exit $?

if [ -f "$BIN" ]
then
    echo "Removed old version"
    sudo rm "$BIN"
fi
if [ -d "$DATADIR" ]
then
	sudo rm -rf "$DATADIR"
fi

sudo make install || exit $?
echo "Installed succesfully"
