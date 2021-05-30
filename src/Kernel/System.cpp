#include "System.h"
#include <string>
#include <ctime>
#include <cstdio>
#include <fstream>
#include <filesystem>
#include <thread>
#include <functional>
#include <objbase.h>
#include <windows.h>
using namespace std;

string Raw_GetDateTimeStr()
{
    time_t t = time(NULL);
    tm ts;
    localtime_s(&ts,&t);
    char buf[24]= {0};
    strftime(buf, 24, "%Y-%m-%d %H:%M:%S", &ts);
    return string(buf); 
}

string Raw_RandomGuid()
{
    GUID guid;
    CoCreateGuid(&guid);

    char dst[36] = {0};
    snprintf(dst, 36,
        "%08x%04x%04x%02x%02x%02x%02x%02x%02x%02x%02x",
        guid.Data1, guid.Data2, guid.Data3,
        guid.Data4[0], guid.Data4[1], guid.Data4[2],
        guid.Data4[3], guid.Data4[4], guid.Data4[5],
        guid.Data4[6], guid.Data4[7]);
    return string(dst);
}

bool Raw_PathExists(const string &path)
{
    return filesystem::exists(path);
}

bool Raw_FileReadAll(const string &path, string & result)
{
    ifstream fRead(path);
    if(!fRead)
        return false;
    string data((std::istreambuf_iterator<char>(fRead)),
        std::istreambuf_iterator<char>());
    fRead.close();
    result = data;
    return true;
}

bool Raw_FileWriteAll(const std::string &path, const std::string &data)
{
    std::ofstream fileWrite(path,std::ios::out);
    if(!fileWrite)
        return false;
    fileWrite << data;
    return fileWrite.good();
}

bool Raw_SystemCmd(const std::string &cmd, std::function<void(int,std::string)> callback, int timeLimit)
{
    return true;
}

std::pair<int,std::string> Raw_HttpRequestSync(const std::string &url,const std::string &method,const std::string &data)
{
    httplib::Client cli(url.c_str());
    auto response = (method == "POST" || method == "Post") ? cli.Post(data.c_str()) : cli.Get(data.c_str());
    return {response->status,response->body};
}

bool Raw_HttpRequestAsync(const string &url,const string &method,const string &data,function<void(int,std::string)> callback)
{
    httplib::Client *cli = new httplib::Client(url.c_str());
    if(!cli->is_valid())
    {
        delete cli;
        return false;
    }
    std::thread([cli,method{std::move(method)},data{std::move(data)},callback{std::move(callback)}]() {
        auto response = (method == "POST" || method == "Post") ? cli->Post(data.c_str()) : cli->Get(data.c_str());
        delete cli;
        callback(response->status,response->body);
    }).detach();

    return true;
}