IF EXIST Tahoma2D-portable-win.zip del Tahoma2D-portable-win.zip

IF NOT EXIST build mkdir build
cd build

REM Setup for local builds
set MSVCVERSION="Visual Studio 17 2022"
set OPENCV_DIR=C:\opencv\4110\build

REM These are effective when running from Actions
IF EXIST C:\tools\opencv set OPENCV_DIR=C:\tools\opencv\build

cmake .. -G %MSVCVERSION%  -Ax64 -DOpenCV_DIR=%OPENCV_DIR%


IF EXIST C:\ProgramData\chocolatey\bin\cl.exe (
 	msbuild /p:CLToolPath=C:\ProgramData\chocolatey\bin /p:UseMultiToolTask=true /p:Configuration=Release /m /verbosity:minimal ALL_BUILD.vcxproj
) ELSE (
	msbuild /p:Configuration=Release /m /verbosity:minimal ALL_BUILD.vcxproj
)

cd ..

IF EXIST plugin rmdir /S /Q plugin
mkdir plugins

xcopy /Y /E /I Dwango\bin\Release\*.plugin plugins
xcopy /Y /E /I Wolf\bin\Release\*.plugin plugins

7z a Tahoma2D_plugins_win.zip plugins