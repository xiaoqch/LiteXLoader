#include "APIhelp.h"
#include "FileSystemAPI.h"
#include <filesystem>
#include <fstream>
using namespace script;
using namespace std::filesystem;

Local<Value> DirCreate(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)

    try{
        create_directory(args[0].asString().toString());
        return Boolean::newBoolean(true);
    }
    catch(const filesystem_error& e)
    {
        DEBUG("Fail to Create Dir "+ args[0].asString().toString() +"!\n");
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
        DEBUG("Fail to Delete "+ args[0].asString().toString() +"!\n");
        return Boolean::newBoolean(false);
    }
    CATCH("Fail in DeletePath!")
}

Local<Value> PathExists(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)

    try{
        bool result = exists(args[0].asString().toString());
        return Boolean::newBoolean(result);
    }
    catch(const filesystem_error& e)
    {
        DEBUG("Fail to Delete "+ args[0].asString().toString() +"!\n");
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
        DEBUG("Fail to Copy "+ args[0].asString().toString() +"!\n");
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
        DEBUG("Fail to Rename "+ args[0].asString().toString() +"!\n");
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
        DEBUG("Fail to Move "+ args[0].asString().toString() +"!\n");
        return Boolean::newBoolean(false);
    }
    CATCH("Fail in MovePath!")
}

Local<Value> FileReadAll(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)
    
    try{
        std::ifstream fileRead(args[0].asString().toString());
        if(!fileRead)
            return Local<Value>(); //Null
        std::string texts((std::istreambuf_iterator<char>(fileRead)),
            std::istreambuf_iterator<char>());
        fileRead.close();
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
        std::ofstream fileWrite(args[0].asString().toString(),std::ios::out);
        if(!fileWrite)
            return Boolean::newBoolean(false);
        fileWrite << args[1].asString().toString();
        return Boolean::newBoolean(fileWrite.good());
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
    CHECK_ARGS_COUNT(args,1)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)

    try
    {
        ///////////////////////// Add code here /////////////////////////
        return Boolean::newBoolean(true);
    }
    CATCH("Fail in SystemCmd!")
}