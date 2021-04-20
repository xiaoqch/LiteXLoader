#include "LogAPI.h"
#include <iostream>
#include "APIhelp.h"
using namespace script;

Local<Value> Print(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)

    for(int i = 0; i < args.size(); ++i)
        PrintValue(std::cout,args[i]);
    std::cout << std::endl;
    return Boolean::newBoolean(true);
}