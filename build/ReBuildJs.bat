@echo off
rmdir /s/q "../LiteXLoader.Js/build"
call ./BuildJs.bat batch
IF "%1%" == "" ( pause )