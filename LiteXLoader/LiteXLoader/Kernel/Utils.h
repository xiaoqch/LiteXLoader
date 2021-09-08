#pragma once
#include "Global.h"
#include <string>
#include <vector>
#include <fstream>
#include <optional>

std::vector<std::string> SplitCmdLine(const std::string& paras);
std::vector<std::string> SplitStrWithPattern(const std::string& str, const std::string& pattern);
bool StartsWith(const std::string& str, const std::string& start);
bool EndsWith(const std::string& str, const std::string& end);
void SplitHttpUrl(const std::string& url, std::string& host, std::string& path);

bool IsVersionLess(const std::string& v1, const std::string& v2);
bool IsVersionLess(int v1a, int v1b, int v1c, int v2a, int v2b, int v2c);

wchar_t* str2cwstr(std::string str);
std::string wstr2str(std::wstring wstr);

std::optional<std::string> ReadAllFile(const std::string& filePath, bool isBinary);

unsigned long long GetCurrentTimeStampMS();

FILE* GetFILEfromFstream(std::fstream& fs);
HANDLE GetHANDLEfromFstream(std::fstream& fs);