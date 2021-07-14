#include "Global.h"
#include <string>
#include <vector>
#include <functional>

//文件系统
bool Raw_DirCreate(const std::string &path);
bool Raw_PathExists(const std::string &path);
bool Raw_FileReadFrom(const std::string &path, std::string & result);
bool Raw_FileWriteTo(const std::string &path, const std::string &data);

//系统调用
bool Raw_SystemCmd(const std::string &cmd, std::function<void(int,std::string)> callback, int timeLimit);

//网络
bool Raw_HttpGet(const std::string &url, std::function<void(int,std::string)> callback);
bool Raw_HttpPost(const std::string& url, const std::string& data, const std::string& type, std::function<void(int, std::string)> callback);

//获取信息
std::string Raw_GetDateTimeStr();
std::string Raw_RandomGuid();

//辅助
unsigned int Raw_GetSystemThreadIdFromStdThread(std::thread::id id);
bool Raw_KillThread(DWORD id);