#include "Utils.h"
#include <string>
#include <vector>
using namespace std;

vector<string> SplitCmdLine(const string& paras)
{
    if (paras.empty())
        return vector<string>();

    vector<string> res;
    string now, strInQuote = "";
    istringstream strIn(paras);
    while (strIn >> now)
    {
        if (!strInQuote.empty())
        {
            strInQuote = strInQuote + " " + now;
            if (now.back() == '\"')
            {
                strInQuote.pop_back();
                res.push_back(strInQuote.erase(0, 1));
                strInQuote = "";
            }
        }
        else
        {
            if (now.front() == '\"')
            {
                if (now.back() == '\"')
                {
                    now = now.substr(1, now.size() - 2);
                    res.push_back(now);
                }
                else
                    strInQuote = now;
            }
            else
                res.push_back(now);
        }
    }
    if (!strInQuote.empty())
    {
        istringstream leftIn(strInQuote);
        while (leftIn >> now)
            res.push_back(now);
    }
    return res;
}

std::vector<std::string> SplitStrWithPattern(const std::string& str, const std::string& pattern)
{
    std::vector<std::string> resVec;

    if (str.empty())
        return resVec;
    
    std::string strs = str + pattern;

    size_t pos = strs.find(pattern);
    size_t size = strs.size();

    while (pos != std::string::npos)
    {
        std::string x = strs.substr(0, pos);
        resVec.push_back(x);
        strs = strs.substr(pos + 1, size);
        pos = strs.find(pattern);
    }

    return resVec;
}

void SplitHttpUrl(const std::string& url, string& host, string& path)
{
    host = url;

    bool foundProcotol = host.find('//') != string::npos;

    auto splitPos = host.find('/', foundProcotol ? host.find('//') + 2 : 0);    //查找协议后的第一个/分割host与路径
    if (splitPos == string::npos)
    {
        path = "/";
    }
    else
    {
        path = host.substr(splitPos);
        host = host.substr(0, splitPos);
    }
}

wchar_t* str2cwstr(string str)
{
    auto len = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
    wchar_t* buffer = new wchar_t[len + 1];
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, buffer, len + 1);
    buffer[len] = L'\0';

    return buffer;
}

string wstr2str(wstring wstr)
{
    auto len = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
    char* buffer = new char[len + 1];
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, buffer, len + 1, NULL, NULL);
    buffer[len] = '\0';

    string result = string(buffer);
    delete[] buffer;
    return result;
}