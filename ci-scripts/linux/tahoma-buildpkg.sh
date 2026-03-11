#!/bin/bash
if [ -f Tahoma2D_plugins_linux.zip ]
then
   rm -f Tahoma2D_plugins_linux.zip
fi

rm -rf plugin_package
mkdir plugin_package
cd plugin_package

mkdir plugins

cp ../Dwango/lib/*.plugin plugins
cp ../Wolf/lib/*.plugin plugins

cp ../LICENSE.txt .
cp "../ci-scripts/linux/Installation Instructions.txt" .

zip -ro ../Tahoma2D_plugins_linux.zip *

cd ..
