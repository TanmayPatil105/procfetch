#!/bin/bash

BIN="/bin/procfetch"
DIR="/usr/share/procfetch"

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
if [ -d "$DIR" ]
then
	sudo rm -rf "$DIR"
fi
echo "Creating /bin/procfetch ..."
sudo cp ./src/procfetch /bin
echo "Creating /usr/share/procfetch ..."
sudo mkdir /usr/share/procfetch
sudo cp -r ./ascii /usr/share/procfetch
echo "Installed succesfully"
