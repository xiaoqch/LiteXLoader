@echo off
mkdir Release 2>nul

:: Js
set NAME=ScriptX-Js-Build
set Release=ScriptX-Js
cmake --no-warn-unused-cli -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -H%NAME% -B%NAME%/build -G "Visual Studio 16 2019" -T host=x64 -A x64 2>nul
cmake --no-warn-unused-cli -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -H%NAME% -B%NAME%/build -G "Visual Studio 16 2019" -T host=x64 -A x64
cmake --build %NAME%/build --config MinSizeRel --target ALL_BUILD -j 8 --
copy .\%NAME%\build\MinSizeRel\ScriptX.lib .\Release\%Release%.lib /Y 2>nul

:: Lua
set NAME=ScriptX-Lua-Build
set Release=ScriptX-Lua
cmake --no-warn-unused-cli -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -H%NAME% -B%NAME%/build -G "Visual Studio 16 2019" -T host=x64 -A x64 2>nul
cmake --no-warn-unused-cli -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -H%NAME% -B%NAME%/build -G "Visual Studio 16 2019" -T host=x64 -A x64
cmake --build %NAME%/build --config MinSizeRel --target ALL_BUILD -j 8 --
copy .\%NAME%\build\MinSizeRel\ScriptX.lib .\Release\%Release%.lib /Y 2>nul