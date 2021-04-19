#pragma once
#include "../pch.h"

#define PREFIX "[LiteXLoader." ## LXL_SCRIPT_LANG_TYPE ## "]"
#define DEBUG(t)  std::cout << PREFIX ## "[Debug] " << t << std::endl
#define INFO(t)  std::cout << PREFIX ## "[Info] " << t << std::endl
#define WARN(t)  std::cout << PREFIX ## "[Warning] " << t << std::endl
#define ERROR(t)  std::cerr << PREFIX ## "[Error] " << t << std::endl
#define FATAL(t)  std::cerr << PREFIX ## "[FATAL] " << t << std::endl

#if 0
class Logger
{
private:
    std::ofstream logout;
    bool enableLog;
public:
    Logger(const std::string &logFile)
    {
        enableLog = conf::getBool("Main","EnableLog");
        logout.open(logFile);
    }

    void Log(const std::string &log)
    {
        if(enableLog)
        {
            cout << 
        }
    }
    void Log(const char *log, ...)
    {

    }
};
#endif