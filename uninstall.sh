#!/bin/bash

BIN="/bin/procfetch"
DIR="/usr/share/procfetch"

if [ -f "$BIN" ]
then
	sudo rm "$BIN"
fi
if [ -d "$DIR" ]
then
	sudo rm -rf "$DIR"
fi
echo "Uninstalled succesfully"
