#!/bin/bash
sudo apt-get update
sudo apt-get install -y cmake liblzo2-dev liblz4-dev libpng-dev libegl1-mesa-dev libgles2-mesa-dev libglew-dev freeglut3-dev libsuperlu-dev wget libboost-all-dev liblzma-dev libjson-c-dev libjpeg8-dev libjpeg-turbo8-dev libturbojpeg0-dev libglib2.0-dev

sudo apt-get install -y nasm yasm libgnutls28-dev libunistring-dev libass-dev libbluray-dev libmp3lame-dev libopus-dev libsnappy-dev libtheora-dev libvorbis-dev libvpx-dev libwebp-dev libxml2-dev libfontconfig1-dev libopencore-amrnb-dev libopencore-amrwb-dev libspeex-dev libsoxr-dev libopenjp2-7-dev
sudo apt-get install -y python2 python3-pip
sudo apt-get install -y build-essential libgirepository1.0-dev autotools-dev intltool gettext libtool patchelf autopoint libusb-1.0-0 libusb-1.0-0-dev
sudo apt-get install -y libdeflate-dev
sudo apt-get install -y libfuse2
sudo apt-get install -y protobuf-compiler libprotobuf-dev

pip3 install --upgrade pip
pip3 install numpy
