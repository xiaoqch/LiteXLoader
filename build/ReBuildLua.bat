@echo off
rmdir /s/q "../LiteXLoader.Lua/build"
call ./BuildLua.bat batch
IF "%1%" == "" ( pause )