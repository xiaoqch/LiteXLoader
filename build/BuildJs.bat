@echo off
cd ..
cmake --no-warn-unused-cli -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -HLiteXLoader.Js -BLiteXLoader.Js/build -G "Visual Studio 16 2019" -T host=x64 -A x64 2>nul
cmake --no-warn-unused-cli -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -HLiteXLoader.Js -BLiteXLoader.Js/build -G "Visual Studio 16 2019" -T host=x64 -A x64
cmake --build LiteXLoader.Js/build --config Release --target ALL_BUILD -- /maxcpucount:10
mkdir Release 2>nul
copy .\LiteXLoader.Js\Output\Release\LiteXLoader.Js.dll .\Release\LiteXLoader.Js.dll /Y >nul
IF "%1%" == "" ( pause )
cd build