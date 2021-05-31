#include "APIhelp.h"
#include "LxlAPI.h"
using namespace script;

Local<Value> LxlGetVersion(const Arguments& args)
{
    return String::newString(LXL_VERSION);
}

Local<Value> LxlLoadPlugin(const Arguments& args)
{
    /////////////////// Add code here /////////////////
    return Local<Value>();
}