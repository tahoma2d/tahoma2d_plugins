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
  
  install_name_tool -change /usr/local/lib/libopencv_calib3d.4.5.dylib @executable_path/../Frameworks/libopencv_calib3d.4.5.dylib $X
  install_name_tool -change /usr/local/lib/libopencv_core.4.5.dylib @executable_path/../Frameworks/libopencv_core.4.5.dylib $X
  install_name_tool -change /usr/local/lib/libopencv_features2d.4.5.dylib @executable_path/../Frameworks/libopencv_features2d.4.5.dylib $X
  install_name_tool -change /usr/local/lib/libopencv_flann.4.5.dylib @executable_path/../Frameworks/libopencv_flann.4.5.dylib $X
  install_name_tool -change /usr/local/lib/libopencv_gapi.4.5.dylib @executable_path/../Frameworks/libopencv_gapi.4.5.dylib $X
  install_name_tool -change /usr/local/lib/libopencv_highgui.4.5.dylib @executable_path/../Frameworks/libopencv_highgui.4.5.dylib $X
  install_name_tool -change /usr/local/lib/libopencv_imgcodecs.4.5.dylib @executable_path/../Frameworks/libopencv_imgcodecs.4.5.dylib $X
  install_name_tool -change /usr/local/lib/libopencv_imgproc.4.5.dylib @executable_path/../Frameworks/libopencv_imgproc.4.5.dylib $X
  install_name_tool -change /usr/local/lib/libopencv_ml.4.5.dylib @executable_path/../Frameworks/libopencv_ml.4.5.dylib $X
  install_name_tool -change /usr/local/lib/libopencv_objdetect.4.5.dylib @executable_path/../Frameworks/libopencv_objdetect.4.5.dylib $X
  install_name_tool -change /usr/local/lib/libopencv_photo.4.5.dylib @executable_path/../Frameworks/libopencv_photo.4.5.dylib $X
  install_name_tool -change /usr/local/lib/libopencv_stitching.4.5.dylib @executable_path/../Frameworks/libopencv_stitching.4.5.dylib $X
  install_name_tool -change /usr/local/lib/libopencv_video.4.5.dylib @executable_path/../Frameworks/libopencv_video.4.5.dylib $X
  install_name_tool -change /usr/local/lib/libopencv_videoio.4.5.dylib @executable_path/../Frameworks/libopencv_videoio.4.5.dylib $X
done

cp ../LICENSE.txt .
cp "../ci-scripts/osx/Installation Instructions.txt" .

zip -ro ../Tahoma2D_plugins_osx.zip plugins *
