#include "APIhelp.h"
#include "FileSystemAPI.h"
#include <Kernel/System.h>
#include <filesystem>
#include <fstream>
using namespace script;
using namespace std::filesystem;

Local<Value> DirCreate(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)

    try{
        Raw_DirCreate(args[0].asString().toString());
        return Boolean::newBoolean(true);
    }
    catch(const filesystem_error& e)
    {
        ERROR("Fail to Create Dir "+ args[0].asString().toString() +"!\n");
        return Boolean::newBoolean(false);
    }
    CATCH("Fail in CreateDir!")
}

Local<Value> PathDelete(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)

    try{
        remove_all(args[0].asString().toString());
        return Boolean::newBoolean(true);
    }
    catch(const filesystem_error& e)
    {
        ERROR("Fail to Delete "+ args[0].asString().toString() +"!\n");
        return Boolean::newBoolean(false);
    }
    CATCH("Fail in DeletePath!")
}

Local<Value> PathExists(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)

    try{
        return Boolean::newBoolean(Raw_PathExists(args[0].asString().toString()));
    }
    catch(const filesystem_error& e)
    {
        ERROR("Fail to Check "+ args[0].asString().toString() +"!\n");
        return Boolean::newBoolean(false);
    }
    CATCH("Fail in ExistsPath!")
}

Local<Value> PathCopy(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)
    CHECK_ARG_TYPE(args[1],ValueKind::kString)

    try{
        copy(args[0].asString().toString(),args[1].asString().toString());
        return Boolean::newBoolean(true);
    }
    catch(const filesystem_error& e)
    {
        ERROR("Fail to Copy "+ args[0].asString().toString() +"!\n");
        return Boolean::newBoolean(false);
    }
    CATCH("Fail in CopyPath!")
}

Local<Value> PathRename(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)
    CHECK_ARG_TYPE(args[1],ValueKind::kString)

    try{
        rename(args[0].asString().toString(),args[1].asString().toString());
        return Boolean::newBoolean(true);
    }
    catch(const filesystem_error& e)
    {
        ERROR("Fail to Rename "+ args[0].asString().toString() +"!\n");
        return Boolean::newBoolean(false);
    }
    CATCH("Fail in RenamePath!")
}

Local<Value> PathMove(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)
    CHECK_ARG_TYPE(args[1],ValueKind::kString)

    try{
        copy(args[0].asString().toString(),args[1].asString().toString());
        remove_all(args[0].asString().toString());
        return Boolean::newBoolean(true);
    }
    catch(const filesystem_error& e)
    {
        ERROR("Fail to Move "+ args[0].asString().toString() +"!\n");
        return Boolean::newBoolean(false);
    }
    CATCH("Fail in MovePath!")
}

Local<Value> FileReadAll(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)
    
    try{
        string texts;
        if(!Raw_FileReadAll(args[0].asString().toString(),texts))
            return Local<Value>();  //Null
        return String::newString(texts);
    }
    CATCH("Fail in FileReadAll!")
}

Local<Value> FileWriteAll(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,2)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)
    CHECK_ARG_TYPE(args[1],ValueKind::kString)
    
    try{
        return Boolean::newBoolean(Raw_FileWriteAll(args[0].asString().toString(),args[1].asString().toString()));
    }
    CATCH("Fail in FileWriteAll!")
}

Local<Value> FileWriteLine(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,2)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)
    CHECK_ARG_TYPE(args[1],ValueKind::kString)
    
    try{
        std::ofstream fileWrite(args[0].asString().toString(),std::ios::app);
        if(!fileWrite)
            return Boolean::newBoolean(false);
        fileWrite << args[1].asString().toString() << std::endl;
        return Boolean::newBoolean(fileWrite.good());
    }
    CATCH("Fail in FileWriteLine!")
}

Local<Value> SystemCmd(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,2)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)
    CHECK_ARG_TYPE(args[1],ValueKind::kFunction)
    if(args.size() >= 3)
        CHECK_ARG_TYPE(args[2],ValueKind::kNumber)

    try{
        Global<Function> callbackFunc{args[1].asFunction()};
        
        return Boolean::newBoolean(Raw_SystemCmd(args[0].toStr(),
            [callback{std::move(callbackFunc)},engine{EngineScope::currentEngine()}]
                (int exitCode,string output)
        {
            EngineScope scope(engine);
            callback.get().call({},Number::newNumber(exitCode),String::newString(output));
        }
        ,args.size() >= 3 ? args[2].toInt() : -1));
    }
    CATCH("Fail in SystemCmd")
}