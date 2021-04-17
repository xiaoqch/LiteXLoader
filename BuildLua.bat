@echo off
cmake --no-warn-unused-cli -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -HLiteXLoader.Lua -BLiteXLoader.Lua/build -G "Visual Studio 16 2019" -T host=x64 -A x64 2>nul
cmake --no-warn-unused-cli -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -HLiteXLoader.Lua -BLiteXLoader.Lua/build -G "Visual Studio 16 2019" -T host=x64 -A x64
cmake --build LiteXLoader.Lua/build --config Release --target ALL_BUILD -- /maxcpucount:10
mkdir Release 2>nul
copy .\LiteXLoader.Lua\Output\Release\LiteXLoader.Lua.dll .\Release\LiteXLoader.Lua.dll /Y