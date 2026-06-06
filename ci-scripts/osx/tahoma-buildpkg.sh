#!/bin/bash
if [ -f Tahoma2D_plugins_osx.zip ]
then
   rm -f Tahoma2D_plugins_osx.zip
fi

rm -rf plugin_package
mkdir plugin_package
cd plugin_package

mkdir plugins

cp ../Dwango/lib/*.plugin plugins
cp ../Wolf/lib/*.plugin plugins

# Change library paths to look in Tahoma2D's framework
for X in `ls *.plugin`
do
  Y=`basename $X`
  echo ">>> Fixing lib paths for $X"
  install_name_tool -id @loader_path/$Y $X
    
  install_name_tool -delete_rpath opencv_calib3d $X
  install_name_tool -delete_rpath opencv_core $X
  install_name_tool -delete_rpath opencv_features2d $X
  install_name_tool -delete_rpath opencv_flann $X
  install_name_tool -delete_rpath opencv_gapi $X
  install_name_tool -delete_rpath opencv_highgui $X
  install_name_tool -delete_rpath opencv_imgcodecs $X
  install_name_tool -delete_rpath opencv_imgproc $X
  install_name_tool -delete_rpath opencv_ml $X
  install_name_tool -delete_rpath opencv_objdetect $X
  install_name_tool -delete_rpath opencv_photo $X
  install_name_tool -delete_rpath opencv_stitching $X
  install_name_tool -delete_rpath opencv_video $X
  install_name_tool -delete_rpath opencv_videoio $X
  
  for Z in `otool -L $X | grep libopencv | sed -e"s/	//" -e"s/ .*$//"`
  do
     W="@executable_path/../Frameworks/"$(basename $Z)
     echo "   Replacing $Z with $W"
     install_name_tool -change $Z $W $X
  done
done

cp ../LICENSE.txt .
cp "../ci-scripts/osx/Installation Instructions.txt" .

zip -ro ../Tahoma2D_plugins_osx.zip plugins *
