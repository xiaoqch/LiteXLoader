# LiteXRunner
>A multi-language script plugin runner for BDS LiteLoader, based on ScriptX

## Compiling
1. Install **Microsoft Visual Studio** with standard C++ building suite
2. Install the newest version of Windows SDK
3. Install CMake and add it to `PATH`
4. Run the building batch below

---

### Build & Link
- `BuildJs.bat`&nbsp;&nbsp;&nbsp;-&nbsp;&nbsp;&nbsp;Build LiteXRunner for Js
- `BuildLua.bat`&nbsp;&nbsp;&nbsp;-&nbsp;&nbsp;&nbsp;Build LiteLuaRunner for Lua
- `BuildAll.bat`&nbsp;&nbsp;&nbsp;-&nbsp;&nbsp;&nbsp;Build All targets above

### Clean everything generated & Rebuild & Link
- `ReBuildJs.bat`&nbsp;&nbsp;&nbsp;-&nbsp;&nbsp;&nbsp;ReBuild LiteXRunner for Js
- `ReBuildLua.bat`&nbsp;&nbsp;&nbsp;-&nbsp;&nbsp;&nbsp;ReBuild LiteXRunner for Lua
- `ReBuildAll.bat`&nbsp;&nbsp;&nbsp;-&nbsp;&nbsp;&nbsp;ReBuild All targets above

#### After building, DLLs will generate at `Release` directory.