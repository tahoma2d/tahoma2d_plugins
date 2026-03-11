IF EXIST Tahoma2D_plugins_win.zip del Tahoma2D_plugins_win.zip

IF EXIST plugin_package rmdir /S /Q plugin_package
mkdir plugin_package
cd plugin_package

mkdir plugins

xcopy /Y /E /I ..\Dwango\bin\Release\*.plugin plugins
xcopy /Y /E /I ..\Wolf\bin\Release\*.plugin plugins

copy /y ..\LICENSE.txt .
copy /y "..\ci-scripts\windows\Installation Instructions.txt" .

7z a ..\Tahoma2D_plugins_win.zip *
