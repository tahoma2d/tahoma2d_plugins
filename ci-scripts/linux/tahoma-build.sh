#!/bin/bash
if [ -f Tahoma2D_plugins_linux.zip ]
then
   rm -f Tahoma2D_plugins_linux.zip
fi

if [ ! -d build ]
then
   mkdir build
fi

cd build

cmake ..

make

cd ..

rm -rf plugins
mkdir plugins
cd plugins

cp ../Dwango/lib/*.plugin .
cp ../Wolf/lib/*.plugin .

cd ..

zip -ro Tahoma2D_plugins_linux.zip plugins
