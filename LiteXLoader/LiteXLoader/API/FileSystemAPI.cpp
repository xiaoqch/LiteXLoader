#include "APIHelp.h"
#include "FileSystemAPI.h"
#include <Engine/TimeTaskSystem.h>
#include <Engine/LocalShareData.h>
#include <Kernel/System.h>
#include <Kernel/Utils.h>
#include <filesystem>
#include <fstream>
using namespace script;
using namespace std::filesystem;
using namespace std;

//////////////////// Class Definition ////////////////////

enum class FileOpenMode : int
{
    ReadMode, WriteMode, AppendMode
};

ClassDefine<FileClass> FileClassBuilder =
    defineClass<FileClass>("file")
        .constructor(nullptr)
        .instanceProperty("path", &FileClass::getPath)
        .instanceProperty("size", &FileClass::getSize)

        .instanceFunction("readSync", &FileClass::readSync)
        .instanceFunction("readLineSync", &FileClass::readLineSync)
        .instanceFunction("readAllSync", &FileClass::readAllSync)
        .instanceFunction("writeSync", &FileClass::writeSync)
        .instanceFunction("writeLineSync", &FileClass::writeLineSync)

        .instanceFunction("read", &FileClass::read)
        .instanceFunction("readLine", &FileClass::readLine)
        .instanceFunction("readAll", &FileClass::readAll)
        .instanceFunction("write", &FileClass::write)
        .instanceFunction("writeLine", &FileClass::writeLine)

        .instanceFunction("isEOF", &FileClass::isEOF)
        .instanceFunction("seekTo", &FileClass::seekTo)
        .instanceFunction("setSize", &FileClass::setSize)
        .instanceFunction("close", &FileClass::close)
        .instanceFunction("flush", &FileClass::flush)
        .instanceFunction("errorCode", &FileClass::errorCode)
        .instanceFunction("clear", &FileClass::clear)

        .property("readMode", [] { return Number::newNumber((int)FileOpenMode::ReadMode); })
        .property("writeMode", [] { return Number::newNumber((int)FileOpenMode::WriteMode); })
        .property("appendMode", [] { return Number::newNumber((int)FileOpenMode::AppendMode); })
    
        .function("open", &FileClass::open)
        .function("readFrom", &FileClass::readFromStatic)
        .function("writeTo", &FileClass::writeToStatic)
        .function("writeLine", &FileClass::writeLineStatic)

        .function("createDir", &FileClass::createDir)
        .function("mkdir", &FileClass::createDir)
        .function("copy", &FileClass::copy)
        .function("move", &FileClass::move)
        .function("rename", &FileClass::rename)
        .function("delete", &FileClass::del)
        .function("exists", &FileClass::exists)
        .function("checkIsDir", &FileClass::checkIsDir)
        .function("getFileSize", &FileClass::getFileSize)
        .function("getFilesList", &FileClass::getFilesList)
        .build();

//////////////////// Classes ////////////////////

//生成函数
Local<Object> FileClass::newFile(std::fstream&& f, const std::string& path, bool isBinary)
{
    auto newp = new FileClass(std::move(f), path, isBinary);
    return newp->getScriptObject();
}

//成员函数
void FileClass::set(std::fstream&& f, const std::string& path, bool isBinary)
{
    this->file = std::move(f);
    this->path = path;
    this->isBinary = isBinary;
}

Local<Value> FileClass::getPath()
{
    try {
        return String::newString(path);
    }
    CATCH("Fail in getPath!");
}

Local<Value> FileClass::getSize()
{
    try {
        size_t cur = file.tellg();
        file.seekg(0, file.end);
        size_t size = file.tellg();
        file.seekg(cur, file.beg);

        return Number::newNumber((long long)size);
    }
    CATCH("Fail in getPath!");
}

Local<Value> FileClass::readSync(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 1);
    CHECK_ARG_TYPE(args[0], ValueKind::kNumber);

    try {
        int cnt = args[0].toInt();
        char* buf = new char[cnt];
        file.read(buf, cnt);
        size_t bytes = file.gcount();

        Local<Value> res = isBinary ? ByteBuffer::newByteBuffer(buf, bytes).asValue() : String::newString(string_view(buf,bytes)).asValue();
        delete buf;
        return res;
    }
    CATCH("Fail in readSync!");
}

Local<Value> FileClass::readLineSync(const Arguments& args)
{
    try {
        string buf;
        getline(file, buf);
        return String::newString(buf);
    }
    CATCH("Fail in readLineSync!");
}

Local<Value> FileClass::readAllSync(const Arguments& args)
{
    try {
        return String::newString(std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()));
    }
    CATCH("Fail in readAllSync!");
}

Local<Value> FileClass::writeSync(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 1);

    try {
        string buf;
        if (args[0].isString())
        {
            file << args[0].toStr();
        }
        else if (args[0].isByteBuffer())
        {
            file.write((char*)args[0].asByteBuffer().getRawBytes(), args[0].asByteBuffer().byteLength());
        }
        else
        {
            ERROR("Wrong type of argument in writeSync!");
            return Local<Value>();
        }
        return Boolean::newBoolean(!file.fail() && !file.bad());
    }
    CATCH("Fail in writeSync!");
}

Local<Value> FileClass::writeLineSync(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 1);
    CHECK_ARG_TYPE(args[0], ValueKind::kString);

    try {
        file << args[0].toStr() << "\n";
        return Boolean::newBoolean(!file.fail() && !file.bad());
    }
    CATCH("Fail in writeLineSync!");
}

Local<Value> FileClass::read(const Arguments& args)
{
    return Local<Value>();
}

Local<Value> FileClass::readLine(const Arguments& args)
{
    return Local<Value>();
}

Local<Value> FileClass::readAll(const Arguments& args)
{
    return Local<Value>();
}

Local<Value> FileClass::write(const Arguments& args)
{
    return Local<Value>();
}

Local<Value> FileClass::writeLine(const Arguments& args)
{
    return Local<Value>();
}

Local<Value> FileClass::seekTo(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 2);
    CHECK_ARG_TYPE(args[0], ValueKind::kNumber);
    CHECK_ARG_TYPE(args[1], ValueKind::kBoolean);

    try {
        int pos = args[0].toInt();
        if (args[1].asBoolean().value())
        {
            //relative
            ios_base::seekdir way = ios_base::cur;
            file.seekg(pos, way);
            file.seekp(pos, way);
        }
        else
        {
            //absolute
            if (pos >= 0)
            {
                ios_base::seekdir way = ios_base::beg;
                file.seekg(pos, way);
                file.seekp(pos, way);
            }
            else
            {
                ios_base::seekdir way = ios_base::end;
                file.seekg(0, way);
                file.seekp(0, way);
            }
        }
        return Boolean::newBoolean(!file.fail() && !file.bad());
    }
    CATCH("Fail in seekTo!");
}

Local<Value> FileClass::setSize(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 1);
    CHECK_ARG_TYPE(args[0], ValueKind::kNumber);

    try {
        return Boolean::newBoolean(_chsize_s((int)GetHANDLEfromFstream(file), args[0].asNumber().toInt64()) == 0);
    }
    CATCH("Fail in setSize!");
}

Local<Value> FileClass::close(const Arguments& args)
{
    try {
        file.close();
        return Boolean::newBoolean(true);
    }
    CATCH("Fail in flush!");
}

Local<Value> FileClass::isEOF(const Arguments& args)
{
    try {
        return Boolean::newBoolean(file.eof());
    }
    CATCH("Fail in isEOF!");
}

Local<Value> FileClass::flush(const Arguments& args)
{
    try {
        file.flush();
        return Boolean::newBoolean(true);
    }
    CATCH("Fail in flush!");
}

Local<Value> FileClass::errorCode(const Arguments& args)
{
    try {
        file.flush();
        return Number::newNumber(errno);
    }
    CATCH("Fail in flush!");
}

Local<Value> FileClass::clear(const Arguments& args)
{
    try {
        file.clear();
        return Boolean::newBoolean(true);
    }
    CATCH("Fail in flush!");
}


//////////////////// APIs ////////////////////

Local<Value> OpenFile(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 2);
    CHECK_ARG_TYPE(args[0], ValueKind::kString);
    CHECK_ARG_TYPE(args[1], ValueKind::kNumber);
    if(args.size() >= 3)
        CHECK_ARG_TYPE(args[2], ValueKind::kBoolean);

    try{
        string path = args[0].toStr();
        FileOpenMode fMode = (FileOpenMode)(args[1].toInt());
        ios_base::openmode mode = ios_base::in | ios_base::out;
        if (fMode == FileOpenMode::AppendMode)
            mode |= ios_base::app;

        bool isBinary = false;
        if (args.size() >= 3 && args[2].asBoolean().value())
        {
            isBinary = true;
            mode |= ios_base::binary;
        }

        fstream fs(path, mode);
        if (!fs.is_open())
        {
            ERROR("Fail to Open File " + path + "!\n");
            return Local<Value>();
        }
        return FileClass::newFile(std::move(fs), path, isBinary);
    }
    catch(const filesystem_error& e)
    {
        ERROR("Fail to Open File "+ args[0].asString().toString() +"!\n");
        return Local<Value>();
    }
    CATCH("Fail in CreateDir!");
}

Local<Value> DirCreate(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 1);
    CHECK_ARG_TYPE(args[0], ValueKind::kString);

    try{
        Raw_DirCreate(args[0].asString().toString());
        return Boolean::newBoolean(true);
    }
    catch(const filesystem_error& e)
    {
        ERROR("Fail to Create Dir "+ args[0].asString().toString() +"!\n");
        return Boolean::newBoolean(false);
    }
    CATCH("Fail in CreateDir!");
}

Local<Value> PathDelete(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 1);
    CHECK_ARG_TYPE(args[0], ValueKind::kString);

    try{
        remove_all(args[0].asString().toString());
        return Boolean::newBoolean(true);
    }
    catch(const filesystem_error& e)
    {
        ERROR("Fail to Delete "+ args[0].asString().toString() +"!\n");
        return Boolean::newBoolean(false);
    }
    CATCH("Fail in DeletePath!");
}

Local<Value> PathExists(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 1);
    CHECK_ARG_TYPE(args[0], ValueKind::kString);

    try{
        return Boolean::newBoolean(Raw_PathExists(args[0].asString().toString()));
    }
    catch(const filesystem_error& e)
    {
        ERROR("Fail to Check "+ args[0].asString().toString() +"!\n");
        return Boolean::newBoolean(false);
    }
    CATCH("Fail in ExistsPath!");
}

Local<Value> PathCopy(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 2);
    CHECK_ARG_TYPE(args[0], ValueKind::kString);
    CHECK_ARG_TYPE(args[1], ValueKind::kString);

    try{
        copy(args[0].asString().toString(),args[1].asString().toString());
        return Boolean::newBoolean(true);
    }
    catch(const filesystem_error& e)
    {
        ERROR("Fail to Copy "+ args[0].asString().toString() +"!\n");
        return Boolean::newBoolean(false);
    }
    CATCH("Fail in CopyPath!");
}

Local<Value> PathRename(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 2);
    CHECK_ARG_TYPE(args[0], ValueKind::kString);
    CHECK_ARG_TYPE(args[1], ValueKind::kString);

    try{
        rename(args[0].asString().toString(),args[1].asString().toString());
        return Boolean::newBoolean(true);
    }
    catch(const filesystem_error& e)
    {
        ERROR("Fail to Rename "+ args[0].asString().toString() +"!\n");
        return Boolean::newBoolean(false);
    }
    CATCH("Fail in RenamePath!");
}

Local<Value> PathMove(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 2);
    CHECK_ARG_TYPE(args[0], ValueKind::kString);
    CHECK_ARG_TYPE(args[1], ValueKind::kString);

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
    CATCH("Fail in MovePath!");
}

Local<Value> CheckIsDir(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 1);
    CHECK_ARG_TYPE(args[0], ValueKind::kString);

    try {
        path p(args[0].toStr());
        if (!exists(p))
            return Boolean::newBoolean(false);

        return Boolean::newBoolean(directory_entry(p).is_directory());
    }
    catch (const filesystem_error& e)
    {
        ERROR("Fail to Get Type of " + args[0].asString().toString() + "!\n");
        return Local<Value>();
    }
    CATCH("Fail in GetFilesList!");
}

Local<Value> GetFileSize(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 1);
    CHECK_ARG_TYPE(args[0], ValueKind::kString);

    try {
        path p(args[0].toStr());
        if (!exists(p))
            return Number::newNumber(0);
        if(directory_entry(p).is_directory())
            return Number::newNumber(0);

        auto sz = file_size(p);
        return Number::newNumber((int64_t)sz);
    }
    catch (const filesystem_error& e)
    {
        ERROR("Fail to Get Size of " + args[0].asString().toString() + "!\n");
        return Local<Value>();
    }
    CATCH("Fail in GetFilesList!");
}

Local<Value> GetFilesList(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 1);
    CHECK_ARG_TYPE(args[0], ValueKind::kString);

    try {
        auto fileList = Raw_GetFilesList(args[0].toStr());

        Local<Array> arr = Array::newArray();
        for (auto& file : fileList)
            arr.add(String::newString(file));
        return arr;
    }
    CATCH("Fail in GetFilesList!");
}

Local<Value> FileReadFrom(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 1);
    CHECK_ARG_TYPE(args[0], ValueKind::kString);
    
    try{
        string texts;
        if(!Raw_FileReadFrom(args[0].asString().toString(),texts))
            return Local<Value>();  //Null
        return String::newString(texts);
    }
    CATCH("Fail in FileReadAll!");
}

Local<Value> FileWriteTo(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 2);
    CHECK_ARG_TYPE(args[0], ValueKind::kString);
    CHECK_ARG_TYPE(args[1], ValueKind::kString);
    
    try{
        return Boolean::newBoolean(Raw_FileWriteTo(args[0].asString().toString(),args[1].asString().toString()));
    }
    CATCH("Fail in FileWriteAll!");
}

Local<Value> FileWriteLine(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 2);
    CHECK_ARG_TYPE(args[0], ValueKind::kString);
    CHECK_ARG_TYPE(args[1], ValueKind::kString);
    
    try{
        std::ofstream fileWrite(args[0].asString().toString(),std::ios::app);
        if(!fileWrite)
            return Boolean::newBoolean(false);
        fileWrite << args[1].asString().toString() << std::endl;
        return Boolean::newBoolean(fileWrite.good());
    }
    CATCH("Fail in FileWriteLine!");
}