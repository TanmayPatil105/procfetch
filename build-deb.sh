#!/bin/bash

TREE='.'
BINARY=procfetch
VERSION=0.2.2
ARCHITECTURE=amd64


echo "Building the binary..."
make -C ${TREE}

if [ -d "debian-package" ]; then
  rm -rf debian-package
fi

echo "Creating 'debian-package' directory..."
mkdir -p ${TREE}/debian-package
mkdir -p ${TREE}/debian-package/DEBIAN
mkdir -p ${TREE}/debian-package/usr/bin
mkdir -p ${TREE}/debian-package/usr/share/procfetch

echo "Copying files to the package structure..."
cp ${TREE}/debian/control ${TREE}/debian-package/DEBIAN/control
cp ${TREE}/src/procfetch ${TREE}/debian-package/usr/bin/procfetch
cp -r ${TREE}/ascii ${TREE}/debian-package/usr/share/procfetch

echo "Creating Debian package..."
dpkg-deb --build ./debian-package ${TREE}/debian-package/${BINARY}_${VERSION}_${ARCHITECTURE}.deb

echo "DEB package built successfully"

