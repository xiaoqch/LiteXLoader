#pragma once
#include "Global.h"
#include <string>
#include <vector>
#include <fstream>

std::vector<std::string> SplitCmdLine(const std::string& paras);
std::vector<std::string> SplitStrWithPattern(const std::string& str, const std::string& pattern);
void SplitHttpUrl(const std::string& url, std::string& host, std::string& path);

wchar_t* str2cwstr(std::string str);
std::string wstr2str(std::wstring wstr);

unsigned long long GetCurrentTimeStampMS();

FILE* GetFILEfromFstream(std::fstream& fs);
HANDLE GetHANDLEfromFstream(std::fstream& fs);