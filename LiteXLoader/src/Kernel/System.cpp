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

bool Raw_DirCreate(const string &path)
{
    return filesystem::create_directories(path);
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

/////////////////// String Helper ///////////////////
wchar_t* str2wstr(string str)  
{  
    auto len = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), NULL, 0);  
    wchar_t* buffer = new wchar_t[len + 1];
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), buffer, len);  
    buffer[len] = '\0';

    return buffer;
}
string wstr2str(wstring wstr)  
{  
    auto len = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.size(), NULL, 0, NULL, NULL);  
    char* buffer = new char[len + 1];  
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.size(), buffer, len, NULL, NULL);  
    buffer[len] = '\0';  

    string result = string(buffer);  
    delete[] buffer;  
    return result;  
}
/////////////////// String Helper ///////////////////

bool Raw_SystemCmd(const std::string &cmd, std::function<void(int,std::string)> callback, int timeLimit)
{
    SECURITY_ATTRIBUTES sa;
	HANDLE hRead,hWrite;
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;
 
	if (!CreatePipe(&hRead,&hWrite,&sa,0))
		return false;
	STARTUPINFOW si = {0};
	PROCESS_INFORMATION pi;
 
	si.cb = sizeof(STARTUPINFO);
	GetStartupInfoW(&si); 
	si.hStdOutput = si.hStdError = hWrite;
	si.dwFlags = STARTF_USESTDHANDLES;
 
    auto wCmd = str2wstr(cmd);
	if (!CreateProcessW(NULL,wCmd,NULL,NULL,TRUE,NULL,NULL,NULL,&si,&pi))
	{
        delete [] wCmd;
		return false;
	}
	CloseHandle(hWrite);
    CloseHandle(pi.hThread);
 
    std::thread([hRead{std::move(hRead)},hProcess{std::move(pi.hProcess)},
        callback{std::move(callback)},timeLimit{std::move(timeLimit)}, wCmd{std::move(wCmd)}] ()
    {
        if(timeLimit == -1)
            WaitForSingleObject(hProcess,INFINITE);     //################## 句柄表！##################
        else
        {
            WaitForSingleObject(hProcess,timeLimit);
            TerminateProcess(hProcess,-1);
        }
        wchar_t buffer[4096] = {0};
        wstring strOutput;
        DWORD bytesRead,exitCode;

        delete [] wCmd;
        GetExitCodeProcess(hProcess,&exitCode);
        while (true)
        {
            if (!ReadFile(hRead,buffer,4096,&bytesRead,NULL))
                break;
            strOutput.append(buffer,bytesRead);
        }
        CloseHandle(hRead);
        CloseHandle(hProcess);

        callback((int)exitCode,wstr2str(strOutput));
    }).detach();

    return true;
}

std::pair<int,std::string> Raw_HttpRequestSync(const std::string &url,const std::string &method,const std::string &data)
{
    httplib::Client cli(url.c_str());       //############## 崩服 ##############
    auto response = (method == "POST" || method == "Post") ? cli.Post(data.c_str()) : cli.Get(data.c_str());
    return {response->status,response->body};
}

bool Raw_HttpRequestAsync(const string &url,const string &method,const string &data,function<void(int,std::string)> callback)
{
    httplib::Client *cli = new httplib::Client(url.c_str());    //############## 崩服 ##############
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

unsigned int Raw_GetSystemThreadIdFromStdThread(std::thread::id id)
{
    _Thrd_t t = *(_Thrd_t*)&id;
    return t._Id;
}

bool Raw_KillThread(DWORD id)
{
    HANDLE hThr = OpenThread(STANDARD_RIGHTS_REQUIRED,FALSE,(DWORD)id);
    if(hThr == NULL || hThr == INVALID_HANDLE_VALUE)
        return false;
    BOOL res = TerminateThread(hThr,-1);
    CloseHandle(hThr);
    return res;
}