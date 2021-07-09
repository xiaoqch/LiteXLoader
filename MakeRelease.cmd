@echo off
echo Making Release ...
copy /Y .\LiteXLoader\Release\*.dll .\RELEASE >nul
copy /Y .\LiteXLoader\BaseLib\BaseLib.js .\RELEASE\lib >nul
copy /Y .\LiteXLoader\BaseLib\BaseLib.lua .\RELEASE\lib >nul
echo Release Finished.
explorer.exe .\RELEASE
timeout /T 3 >nul