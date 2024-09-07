Tahoma2D  and OpenToonz Plugins
=================

This repository contains example plugins for developers. 
It also contains plugins created by Dwango and Wolf in a Bowl.
They require OpenCV4 runtime. 

## Usage Requirements
- Windows:
  - The opencv_world451.dll exists in the Tahoma2D folder. (Already included in Tahoma2D v1.2 and later)
- macOS:
  - Use Tahoma2D 1.2 or later.
- Linux:
  - Use Tahoma2D 1.2 nightly 2021-05-29 14:59:02 UTC or later.
  - Install OpenCV 4.5.1 to /usr/local/lib via Linux package system or via these steps:
    ```
    $ cd /path-to-tahoma2d-installation/Tahoma2D
    $ ./Tahoma2D.Appimage --appimage-extract
    $ cd squashfs-root/usr/lib
    $ sudo cp libopen* /usr/local/lib
    ```

## How to use prebuilt plugins

1. Download the .ZIP file for your system.
2. Uncompress the .ZIP file,
3. Copy all the files to folder `/path-to-tahoma2d-installation/Tahoma2d/tahomastuff/plugins/`.
4. Restart Tahoma2D,

Plugin manual is [here](./doc/sample_plugins_manual.md).
 
